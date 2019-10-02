// ------------------------------------------------------------------
// -----                        pluto2u                         -----
// -----          Converts Pluto ascii output to UniGen         -----
// -----            Created 16/05/2006 by R.Holzmann            -----
// ------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <map>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include "URun.h"
#include "UEvent.h"
#include "UParticle.h"
#include "UPIDConverter.h"

using namespace std;


int main(int argc, char* argv[])
{
    // Check number of input parameters
    if(argc < 4) {
	cout << "Usage: " << argv[0]
             << " inputFileName outputFileName nEvents" << endl;
        return 1;
    }

    // Get parameters
    TString inputFileName = TString(argv[1]);
    TString outputFileName = TString(argv[2]);
    Int_t nEvents = atoi(argv[3]);

    // Open input file
    ifstream inFile;
    inFile.open(inputFileName);
    if (inFile.fail()) {
	cout << "-E- main : File " << inputFileName
	     << " does not exist!" << endl;
        return 3;
    }

    // Declare event variables
    Int_t nrEvt = 0;     // event number
    Int_t nptEvt = 0;    // number of stored particles per event
    Float_t bimEvt = 0.; // impact parameter
    Float_t phiEvt = 0.; // azimuthal angle
    Float_t pmxEvt = 0.; // reference momentum

    // Declare particle variables
    Int_t idPtl = 0;      // particle id
    Float_t pPtlx = 0.;   // x-component of the particle momentum
    Float_t pPtly = 0.;   // y-component of the particle momentum
    Float_t pPtlz = 0.;   // z-component of the particle momentum
    Float_t energy = 0.;  // particle energy
    Int_t idParent = 0;   // particle number of parent
    Int_t indParent = 0;  // index of parent
    Int_t idSource = 0;   // particle number of source
    Float_t xorPtlx = 0.; // x-component of formation point
    Float_t xorPtly = 0.; // y-component of formation point
    Float_t xorPtlz = 0.; // z-component of formation point
    Float_t xorPtlt = 0.; // formation time
    Float_t weight = 1.;  // weight

    // Open output file
    TFile *outFile = TFile::Open(outputFileName, "RECREATE");
    if(NULL == outFile) {
	cout << "-E- main : File " << outputFileName
	     << " can not be opened!" << endl;
        return 4;
    }

    // Create URun and UEvent object
    URun* run = NULL;
    UEvent* event = new UEvent();

    // Create output tree
    TTree* tree = new TTree("events", "Pluto data");
    tree->Branch("event", "UEvent", &event);

    // Declare variables outside the loop
    Int_t pdgCode, pdgParent;
    Int_t child[] = {0, 0};

    Int_t mode = 0;  // Pluto input format mode

    // Loop over events
    Int_t iConverted = 0;

    for(Int_t iEvent = 0; iEvent < nEvents; iEvent++) {
      // Read event variables

      inFile >> nrEvt >> nptEvt >> pmxEvt >> bimEvt >> mode;

      if (nrEvt==0 && nptEvt==0) break;  // end of file reached
      if (inFile.eof()) break;

	// Store run variables
	if(0 == iEvent) {
	    run = new URun("pluto", "",
			   0,  0, pmxEvt, 0, 0, 0.,
			   bimEvt, bimEvt, 0,
			   0., 6.2832, 0., nEvents);
	}

        event->Clear();

	// Store event variables
	event->SetEventNr(nrEvt);
	event->SetB(bimEvt);
	event->SetPhi(phiEvt);

	// Loop over particles
	for(Int_t iPa = 0; iPa < nptEvt; iPa++) {
	    // Read particle variables

          indParent = -1;
          idParent = 0;
          idSource = 0;
          weight = 1.;
          if (mode==0) {
	    inFile >> energy >> pPtlx >> pPtly >> pPtlz >> idPtl;
          }
          else if (mode==1) {
	    inFile >> energy >> pPtlx >> pPtly >> pPtlz >> idPtl >> weight;
          }
          else if (mode==2) {
	    inFile >> energy >> pPtlx >> pPtly >> pPtlz
                   >> idPtl >> idSource >> idParent >> weight;
          }
          else if (mode==-2) {
            inFile >> energy >> pPtlx >> pPtly >> pPtlz
                   >> idPtl >> idSource >> idParent >> indParent >> weight;
          }
          else if (mode==3) {
	    inFile >> energy >> pPtlx >> pPtly >> pPtlz >> xorPtlx >> xorPtly >> xorPtlz
                   >> idPtl >> idSource >> idParent >> weight;
          }
          else if (mode==-3) {
	    inFile >> energy >> pPtlx >> pPtly >> pPtlz >> xorPtlx >> xorPtly >> xorPtlz
                   >> idPtl >> idSource >> idParent >> indParent >> weight;
          }
          else if (mode==4) {
	    inFile >> energy >> pPtlx >> pPtly >> pPtlz >> xorPtlt >> xorPtlx >> xorPtly >> xorPtlz
                   >> idPtl >> idSource >> idParent >> weight;
          }
          else if (mode==-4) {
	    inFile >> energy >> pPtlx >> pPtly >> pPtlz >> xorPtlt >> xorPtlx >> xorPtly >> xorPtlz
                   >> idPtl >> idSource >> idParent >> indParent >> weight;
          }

	  if(inFile.eof()) break;

          // Get particle IDs
          pdgCode =
              UPIDConverter::Instance()->GetPDGCode(idPtl,
                                                    UPIDConverter::ePluto);
          if (pdgCode == 0) {
            cout << "-W- main : unknown Pluto particle : "
                 << idPtl << endl;
            continue;
          }
          pdgParent =
              UPIDConverter::Instance()->GetPDGCode(idParent,
                                                    UPIDConverter::ePluto);

	    // Add particle to the event
	    event->AddParticle(iPa, pdgCode, 1, indParent, pdgParent,
			       idSource, -1, child,
			       pPtlx, pPtly, pPtlz, energy,
			       xorPtlx, xorPtly, xorPtlz, xorPtlt,
			       weight);
	}

        // Fill the output tree
	tree->Fill();

        // Control output
	if(iEvent>0 && 0 == (iEvent%10)) {
	    cout << "-I- main : Event " << iEvent << " processed." << endl;
	}

        iConverted = iEvent+1;

        // Break if end of file is reached
	if(inFile.eof()) break;
    }
    cout << "-I- main : " << iConverted << " events processed." << endl;

    // Close input file
    inFile.close();

    // Close output file
    run->Write();
    tree->Write();
    outFile->Close();

    // Exit
    return 0;
}
