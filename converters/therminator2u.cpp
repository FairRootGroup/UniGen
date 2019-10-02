///////////////////////////////////////////////////////////////////////////////
// therminator2u reads Therminator 2 root files  and converts them to
// the UniGen format and saves on a root file.
//
// in Therminator 2 particles are divided in two groups  - in this file
// primordial particles have status 1 - other particles status 0
//
// D.Wielanek, July 2012
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <string>

#include <TBranch.h>
#include <TFile.h>
#include <TMath.h>
#include <TString.h>
#include <TTree.h>

#include "URun.h"
#include "UEvent.h"
#include "UParticle.h"


struct particle {
  Float_t mass;
  Float_t t;
  Float_t x;
  Float_t y;
  Float_t z;
  Float_t e;
  Float_t px;
  Float_t py;
  Float_t pz;
  Int_t decayed;
  Int_t pid;
  Int_t fatherpid;
  Int_t rootpid;
  Int_t eid;
  Int_t fathereid;
  UInt_t eventid;
};

struct model_parameters {
  Float_t ModelParameters_RapPRange;
  Float_t ModelParameters_RapSRange;
  Float_t ModelParameters_TauI;
  Float_t ModelParameters_TempF;
  Float_t ModelParameters_MuB;
  Float_t ModelParameters_MuI;
  Float_t ModelParameters_MuS;
  Float_t ModelParameters_MuC;
  Float_t ModelParameters_CollidingEnergy;
  Float_t ModelParameters_CentralityMin;
  Float_t ModelParameters_CentralityMax;
  Float_t ModelParameters_ImpactParameter;
  Float_t ModelParameters_TempI;
  Char_t  ModelParameters_DeviceName[30];
  Char_t  ModelParameters_CollidingSystem[30];
};

/*****************************************************************************/

/*****************************************************************************/

/* Note: technically both particl and all the local variables in this function
   could be made const, however this causes the compiler to complain because
   of signature of UEvent::AddParticle. */
void ReadParticle(UEvent *outputEvent, particle &particl)
{
  // 1 means primordial particle, 0 all others
  Int_t status = (particl.fathereid == -1) ? 1 : 0;

  Int_t decay = (particl.decayed == 1) ? 1 : -1;

  Int_t child[2] = {0, 0};

  outputEvent->AddParticle(particl.eid, particl.pid, status, particl.fathereid, 0, 0, decay, child, particl.px, particl.py, particl.pz, particl.e, particl.x, particl.y, particl.z, particl.t, 1.);
  return;
}

/*****************************************************************************/

int main(int argc, char *argv[])
{
  if (argc != 3) {
    std::cout << "usage:   " << argv[0] << " inpfile outfile\n"
        << "example: " << argv[0] << " input.root ftn14.root "
        << std::endl;
    return 0;
  }
  const char *inpfile = argv[1];
  const char *outfile = argv[2];

  particle particl;
  TFile *infile = TFile::Open(inpfile);
  TTree *intree = static_cast<TTree *>(infile->Get("particles"));
  intree->SetBranchAddress("particle", &particl);

  TFile *outputFile = TFile::Open(outfile, "RECREATE", "Therminator2");
  TTree *outputTree = new TTree("events", "Therminator2 tree");
  UEvent *outputEvent = new UEvent();
  outputTree->Branch("event", "UEvent", &outputEvent);

  /* Event loop - or to be exact, *particle* loop. Why? Therminator stores
     its event headers and particle data in two separate trees, with the
     latter linked to the former via the "eventid" data member. In other
     words, in order to parse everything "the proper way" we would have to
     parse the event tree, extract header data and ID values, then read in
     all the particles and map them to appropriate events via said IDs...
     Fortunately we can get away with not doing this by taking advantage
     of the order of entries in the Therminator particle-data tree:
      - particles from the same event always appear consecutively, and
      - the "eid" of the first particle in an event is always 0.
     In short, we can just iterate over the particle tree and mark the
     start of a new event every time particle.eid == 0. */
  const int notifyEvery = 10;
  const int total_particles = intree->GetEntries();
  int events_processed = 0;
  std::cout << "before loop" << std::endl;

  /* Read in the first particle in the file, making sure there actually is
     one (it would be a really weird input file that had no particles in it,
     then again an int comparison run once per execution adds negligible
     overhead so we might as well check). If its eid != 0, something
     is wrong; otherwise start the event loop. */
  if (total_particles > 0)
    intree->GetEntry(0);
  if (particl.eid != 0) {
    std::cout << "Error reading input file: first particle has eid="
              << particl.eid << " instead of 0." << std::endl;
    return 1;
  }
  int n = 0;
  while (n < total_particles) {
    if ((events_processed % notifyEvery) == 0)
      std::cout << "event "  << std::setw(5) << n << std::endl;

    /* Event has just begun so set its header... */
    outputEvent->Clear();
    outputEvent->SetB(0); // unknown?
    outputEvent->SetPhi(0.); // unknown?
    outputEvent->SetNes(0);
    outputEvent->SetEventNr(++events_processed);

    /* ...but don't forget about the particle itself. */
    ReadParticle(outputEvent, particl);

    /* Now increment the particle index and check the eid of the next
       particle, if any. If it is not zero, we are still in the same
       event - so keep reading. If it is zero, we leave this loop with
       the first particle of the next event already in memory. */
    ++n;
    if (n < total_particles) {
      intree->GetEntry(n);
      while (particl.eid != 0) {
        ReadParticle(outputEvent, particl);
        ++n;
        if (n == total_particles)
          break;
        intree->GetEntry(n);
      }
    }

    /* We have either found the next event or run out of input. Either
       way, save what we have already gathered. */
    outputTree->Fill();
  }
  std::cout << events_processed << " events processed\n";

  model_parameters model;
  TTree *partree = static_cast<TTree*>(infile->Get("parameters"));
  partree->SetBranchAddress("ModelParameters", &model);
  partree->GetEntry(0);

  /* Create the run header. */
  std::string generator = "Therminator";
  generator.append("2"); //version
  const std::string comment =
      std::string(model.ModelParameters_CollidingSystem) + ' ' +
      std::string(model.ModelParameters_DeviceName);
  // FIXME: as one can see there is next to no information here... Some
  // of it is available but would have to be extracted from strings; this
  // will have to be implemented.
  std::cout << "Note: storing the centrality range as bMin and bMax"
      << std::endl;
  URun *runHeader = new URun(generator.data(), comment.data(),
                             0, 0, 0., 0, 0, 0.,
                             model.ModelParameters_CentralityMin,
                             model.ModelParameters_CentralityMax,
                             -1, 0, 0, 0., events_processed);
  runHeader->Write();
  outputFile->Write();
  outputFile->Close();
  infile->Close();

  return 0;
}
/*****************************************************************************/
