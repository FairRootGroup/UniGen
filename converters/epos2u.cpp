// ------------------------------------------------------------------
// -----                        epos2u                          -----
// -----            Created in March 2013 by M. Szuba           -----
// ------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <sstream>

#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>

#include "URun.h"
#include "UEvent.h"
#include "UParticle.h"
#include "UPIDConverter.h"

using namespace std;


Bool_t ReadOptnsFile(const char *fileName,
                     string &model,
                     Int_t &aProj, Int_t &zProj,
                     Int_t &aTarg, Int_t &zTarg,
                     Double32_t &pProj, Double32_t &bMin, Double32_t &bMax,
                     Double32_t &phiMin, Double32_t &phiMax,
                     Int_t &evReqFromModel,
                     UPIDConverter::EConvention &pidConvention);
Bool_t ReadEventHeaderFormat(istringstream &recordStream);
Bool_t ReadParticleFormat(istringstream &recordStream);


int main(int argc, char* argv[])
{
  string model;
  Int_t aProj;
  Int_t zProj;
  Int_t aTarg;
  Int_t zTarg;
  Double32_t pProj;
  Double32_t pTarg;
  Double32_t bMin;
  Double32_t bMax;
  Double32_t phiMin;
  Double32_t phiMax;
  Int_t evReqFromModel;
  UPIDConverter::EConvention pidConvention;

  if ((argc < 4) || (argc > 5)) {
    cout << "Usage:\n"
        << argv[0] << "optnsFileName inputFileName outputFileName [maxEvents]"
        << endl;
    return 1;
  }
  const char *optnsFileName = argv[1];
  const char *inputFileName = argv[2];
  const char *outputFileName = argv[3];
  const Int_t maxEvents = (argc > 4) ? atoi(argv[4]) : -1;

  /* Open input files (and check whether we support the options used)
     before doing anything else. */
  pTarg = 0.;
  // FIXME: support centre-of-mass mode
  if (ReadOptnsFile(optnsFileName, model, aProj, zProj, aTarg, zTarg, pProj,
                    bMin, bMax, phiMin, phiMax, evReqFromModel,
                    pidConvention) == kFALSE)
    return 2;
  ifstream fin(inputFileName);
  if (!fin) {
    Error("epos2u", "Failed to open input file %s", inputFileName);
    return 3;
  }

  /* Now prepare the output. */
  TFile *outFile = TFile::Open(outputFileName, "RECREATE");
  if ((outFile == NULL) || (outFile->IsZombie())) {
    Error("epos2u", "Output file %s couldn't be opened", outputFileName);
    return 4;
  }
  UEvent *event = new UEvent();
  TTree *tree = new TTree("events", "Event Data");
  tree->Branch("event", "UEvent", &event);

  /* Reduce the number of events to process if requested and necessary. */
  const UInt_t nEvents = (evReqFromModel > maxEvents) ?
      maxEvents : evReqFromModel;

  /* We declare this outside the loop because in our present
     implementation, we do not actually parse children - thus never
     actually changing the contents of this array. */
  Int_t child[] = {0, 0};

  string line;
  for (UInt_t iEvent = 0; iEvent < nEvents; ++iEvent) {
    UInt_t eventNo = 0;
    UInt_t expectedParticles = 0;
    Double32_t impactParameter = 0.;

    getline(fin, line);
    istringstream eventHeaderStream(line);
    eventHeaderStream >> eventNo >> expectedParticles >> impactParameter;

    event->Clear();
    event->SetEventNr(eventNo);
    event->SetB(impactParameter);

    for(UInt_t iPa = 0; iPa < expectedParticles; ++iPa) {
      UInt_t particleNo = 0;
      Int_t originalPID = 0;
      Double32_t xMomentum = 0.;
      Double32_t yMomentum = 0.;
      Double32_t zMomentum = 0.;
      Double32_t energy = 0.;
      Double32_t mass = 0.;
      Double32_t xFormation = 0.;
      Double32_t yFormation = 0.;
      Double32_t zFormation = 0.;
      Double32_t tFormation = 0.;

      getline(fin, line);
      istringstream particleDataStream(line);
      string dummy;
      particleDataStream >> particleNo >> originalPID >> xMomentum >> yMomentum
                         >> zMomentum >> energy >> mass >> dummy >> dummy
                         >> dummy >> xFormation >> yFormation >> zFormation
                         >> tFormation;

      /* Original PID can be in one of several conventions, including
         PDG. We know which one thanks to having parsed the optns file, therefore
         as long as appropriate conversion tables are available we can easily get
         a corresponding PDG code. */
      const Int_t pdgCode =
          UPIDConverter::Instance()->GetPDGCode(originalPID, pidConvention);
      if (pdgCode == 0) {
        /* No need to print a warning, UPIDConverter ought to do it for us. */
        continue;
      }

      event->AddParticle(iPa, pdgCode, 1, 0, 0, 0, 0, child,
                         xMomentum,yMomentum, zMomentum, energy,
                         xFormation, yFormation, zFormation, tFormation, 1.);
    }

    /* Write the current event */
    tree->Fill();

    if ((iEvent % 1000) == 0)
      Info("epos2u", "Event %u processed", iEvent);

    if (fin.eof()) {
      break;
    }
  }
  fin.close();

  /* Only prepare the run header now, with all the event data processed - the
     order in which they appear in the output file is controlled by the order
     of Write() calls, moreover it doesn't really matter anyway. */
  URun *run = new URun(model.c_str(), "", aProj, zProj, pProj, aTarg, zTarg,
                       pTarg, bMin, bMax, -1, phiMin, phiMax, 0., nEvents);

  run->Write();
  tree->Write();
  outFile->Close();
  delete outFile;

  return 0;
}


Bool_t ReadOptnsFile(const char *fileName,
                     string &model,
                     Int_t &aProj, Int_t &zProj,
                     Int_t &aTarg, Int_t &zTarg,
                     Double32_t &pProj, Double32_t &bMin, Double32_t &bMax,
                     Double32_t &phiMin, Double32_t &phiMax,
                     Int_t &evReqFromModel,
                     UPIDConverter::EConvention &pidConvention)
{
  int idProj = 0;
  int idTarg = 0;
  Bool_t eventHeaderFormatOkay = kFALSE;
  Bool_t particleFormatOkay = kFALSE;

  /* Set some default values, as defined in EPOS-1.99 code. */
  bMin = 0.;
  bMax = 10000.;
  phiMin = 0.;
  phiMax = TMath::TwoPi();

  ifstream fin(fileName);
  if (!fin) {
    Error("epos2u", "Cannot open optns file %s", fileName);
    return kFALSE;
  }
  Info("epos2u", "Reading optns file %s", fileName);

  string line;
  string word;
  while (!fin.eof()) {
    getline(fin, line);
    istringstream wordStream(line);
    wordStream >> word;

    /* Comments */
    if (word[0] == '!')
      continue;

    if (word == "frame") {
      string value;
      wordStream >> value;
      if (value != "target") {
        Error("epos2u", "Only output in fixed-target frame is presently supported");
        return kFALSE;
      }
    } else if (word == "model")
      wordStream >> model;
    else if (word == "output") {
      string value;
      wordStream >> value;
      if (word == "epos")
        pidConvention = UPIDConverter::eWerner;
      else if (word == "pdg")
        pidConvention = UPIDConverter::ePDG;
      else if ((word == "osc1997a") || (word == "osc1999a")) {
        Error("epos2u", "OSCAR output formats are not supported");
        return kFALSE;
      } else {
        Error("epos2u", "Unsupported output format %s", word.c_str());
        return kFALSE;
      }
    } else if (word == "record") {
      string value;
      wordStream >> value;
      if (value == "event")
        eventHeaderFormatOkay = ReadEventHeaderFormat(wordStream);
      else if (value == "particle")
        particleFormatOkay = ReadParticleFormat(wordStream);
    } else if (word == "set") {
      wordStream >> word;
      if (word == "bminim")
        wordStream >> bMin;
      else if (word == "bmaxim")
        wordStream >> bMax;
      else if (word == "idproj")
        wordStream >> idProj;
      else if (word == "idtarg")
        wordStream >> idTarg;
      else if (word == "laproj")
        wordStream >> zProj;
      else if (word == "latarg")
        wordStream >> zTarg;
      else if (word == "maproj")
        wordStream >> aProj;
      else if (word == "matarg")
        wordStream >> aTarg;
      else if (word == "nevent")
        wordStream >> evReqFromModel;
      else if (word == "phimin")
        wordStream >> phiMin;
      else if (word == "phimax")
        wordStream >> phiMax;
      // FIXME: support other ways of specifying collision energy
      else if (word == "pProj")
        wordStream >> pProj;
    }
  }

  fin.close();

  if (eventHeaderFormatOkay == kFALSE) {
    Error("epos2u", "Unsupported event-header format found");
    return kFALSE;
  }
  if (particleFormatOkay == kFALSE) {
    Error("epos2u", "Unsupported particle format found");
    return kFALSE;
  }

  /* There is no room in URun for projectiles or targets identified
     by particle ID rather than atomic and mass number. Conversion itself
     ought to work without problems, though. */
  if ((zProj == -1) && (aProj == 1) && (idProj != 0))
    Warning("epos2u",
            "Non-nucleonic projectile (id=%d) detected. "
            "URun header will be incomplete",
            UPIDConverter::Instance()->GetPDGCode(idProj,
                                                  UPIDConverter::eWerner));
  if ((zTarg == -1) && (aTarg == 1) && (idTarg != 0))
    Warning("epos2u",
            "Non-nucleonic target (epos id=%d) detected. "
            "URun header will be incomplete",
            UPIDConverter::Instance()->GetPDGCode(idTarg,
                                                  UPIDConverter::eWerner));

  return kTRUE;
}


/* Check whether the EPOS event-header record matches our expectations,
   i.e. event number as the first field followed by:
     - number of particles listed for this event
     - the event's impact parameter
   In EPOS optns file this would look along the lines of:
     record event  nevt nptl b [...]  endrecord
   In the future we may actually adjust our record-reading procedure
   in accordance with this string instead. */
Bool_t ReadEventHeaderFormat(istringstream &recordStream)
{
  string word;

  const UInt_t keywordNumber = 3;
  const string keywordSequence[keywordNumber] = {
    "nevt",
    "nptl",
    "b"
  };
  UInt_t keywordsToGo = keywordNumber;

  while (!recordStream.eof()) {
    recordStream >> word;
    if (word != keywordSequence[keywordNumber - keywordsToGo])
      return kFALSE;
    --keywordsToGo;
    if (keywordsToGo == 0)
      return kTRUE;
  }

  return kFALSE;
}


/* Check whether the EPOS particler record matches our expectations,
   i.e. particle index as the first field followed by:
     - PID
     - all three momentum components
     - energy
     - mass
     - father index
     - mother index
     - status
     - all three spatial co-ordinates of formation point
     - time of formation
     - father PID
     - mother PID
   In EPOS optns file this would look along the lines of:
   record particle  i id p1 p2 p3 p4 p5 fa mo st x1 x2 x3 x4 idfa idmo [...]  endrecord
   In the future we may actually adjust our record-reading procedure
   in accordance with this string instead. */
Bool_t ReadParticleFormat(istringstream &recordStream)
{
  string word;

  const UInt_t keywordNumber = 16;
  const string keywordSequence[keywordNumber] = {
    "i",
    "id",
    "p1",
    "p2",
    "p3",
    "p4",
    "p5",
    "fa",
    "mo",
    "st",
    "x1",
    "x2",
    "x3",
    "x4",
    "idfa",
    "idmo"
  };
  UInt_t keywordsToGo = keywordNumber;

  while (!recordStream.eof()) {
    recordStream >> word;
    if (word != keywordSequence[keywordNumber - keywordsToGo])
      return kFALSE;
    --keywordsToGo;
    if (keywordsToGo == 0)
      return kTRUE;
  }

  return kFALSE;
}
