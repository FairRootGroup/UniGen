/**
* UniGen - Common Format for Event Generators in High-energy and Nuclear Physics
* Copyright (C) 2006 - 2019 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

//--------------------------------------------------------------------
//-----                EvtGen to UniGen converter                -----
//-----             Created 2006-Nov-21 by D.Kresan              -----
//-----          (from CbmEvtGenGenerator by S.Spataro)          -----
//--------------------------------------------------------------------

#include <iostream>
#include <fstream>

#include <TMath.h>
#include <TFile.h>
#include <TTree.h>

#include "URun.h"
#include "UEvent.h"
#include "UParticle.h"

using namespace std;


const Double_t kProtonMass = 0.938271998;


//--------------------------------------------------------------------
int main(int argc, char *argv[])
{
  // Check number of input parameters
  if(argc < 4) {
    cout << "Too few parameters. Usage:" << endl
	 << "evtgen2u EvtGenFile outputFile nEvents"
	 << endl;
    return 1;
  }


  // Read input parameters
  const char *fileNameIn      = argv[1];
  const char *fileNameOut     = argv[2];
  Int_t nev = atoi(argv[3]);

  // Open input files
  cout << "-I- evtgen2u: Opening input file " << fileNameIn << endl;
  ifstream in;
  in.open(fileNameIn);
  if (!in) {
    cout << "-E- evtgen2u: Cannot open "
	 << fileNameIn << " input file!" << endl;
    return 1;
  }
  

  // Open output file
  TFile *outputFile = TFile::Open(fileNameOut, "RECREATE");
  TTree *tree = new TTree("events", "events");
  UEvent *event = new UEvent();
  tree->Branch("event", "UEvent", &event);

  
  // Run description
  Int_t aTarg = 0, zTarg = 0, aProj = 0, zProj = 0;
  Float_t bmin = 0, bmax = 0;
  Int_t evav = 1000000; // number of events in file
  Double_t pProj = 0;
  Double_t pTarg = 0;
  URun *run = new URun("EvtGen", "",
		       aProj, zProj, pProj,
		       aTarg, zTarg, pTarg,
		       bmin, bmax, -1,
		       0., TMath::TwoPi(), 0., evav);
  run->Print();
  run->Write();


  // Check number of events
  if(nev > evav) {
    cout << "-W- evtgen2u: not enough events in input file. Processing only "
	 << evav << " instead of " << nev << " requested." << endl;
    nev = evav;
  }


  // Define event variable to be read from file
  Int_t ntracks = 0, eventID = 0;

  
  // Define track variables to be read from file
  Int_t nLine = 0, pdgID = 0, nDecay = 0,
    nM1 = -1, nM2 = -1, nDF = -1, nDL = -1;
  Float_t fPx = 0., fPy = 0., fPz = 0., fE = 0.;
  Float_t fVx = 0., fVy = 0., fVz = 0., fT = 0.;
  Int_t   child[] = {-1, -1};



  // Loop over events
  for(Int_t iev = 0; iev < nev; iev++) {

    // Clear event
    event->Clear();

    // Read event header line from input file
    Int_t max_nr = 0;
    string buffer;
    in >> eventID >> ntracks;
    event->SetEventNr(eventID);
  
    if (ntracks > 0) {
      // Skip 15 lines
      for (Int_t ii=0; ii<15; ++ii) {
	getline(in, buffer);
      }

      // Loop over tracks in the event     
      for (Int_t ll=0; ll<ntracks; ll++) {
	// Read track description
        in >> nLine >> pdgID >> nDecay >> nM1 >> nM2 >> nDF >> nDL
           >> fPx >> fPy >> fPz >> fE >> fT >> fVx >> fVy >> fVz;
	max_nr = max(max_nr, nDF);
	max_nr = max(max_nr, nDL);
	if ((nDF==-1) && (nDL==-1)) {
	  // Add particle to the event
	  event->AddParticle(event->GetNpa(), pdgID, 1,
			     0, 0, 0, 0, child,
			     fPx, fPy, fPz, fE,
			     fVx, fVy, fVz, fT,
			     1.);
	}
      }//loop over tracks

      // Fill tree
      tree->Fill();

      // Control output
      if(0 == (iev%10)) {
	cout << "-I- evtgen2u : event " << iev << endl;
      }
    } else {
      cout << "-I- evtgen2u : end of input file reached" << endl;
      break;
    }
    
    // If end of input file is reached : close it and abort run
    if (in.eof()) {
      cout << "-I- evtgen2u : end of input file reached" << endl;
      break;
    }
  }// loop over events

  in.close();
  tree->Write();
  outputFile->Close();
  return 0;
}
// -------------------------------------------------------------------


