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

// ------------------------------------------------------------------
// -----                        venus2u                         -----
// -----            Created 10/04/2006 by D.Kresan              -----
// ------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <map>

#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>

#include "URun.h"
#include "UEvent.h"
#include "UParticle.h"
#include "UPIDConverter.h"

using namespace std;


Bool_t ReadOptnsFile(const TString &name,
		     Int_t &aProj, Int_t &zProj,
		     Int_t &aTarg, Int_t &zTarg,
		     Float_t &pnll, Float_t &bMin, Float_t &bMax,
		     Float_t &phiMin, Float_t &phiMax, Int_t &evreq);


const Double_t kProtonMass = 0.938271998;


int main(int argc, char* argv[])
{
  // Check number of input parameters
  if(argc < 5) {
    cout << "Usage : " << endl
	 << argv[0] << " optnsFileName inputFileName outputFileName nEvents"
	 << endl;
    return 1;
  }

  // Get parameters
  TString optnsFileName = TString(argv[1]);
  TString inputFileName = TString(argv[2]);
  TString outputFileName = TString(argv[3]);
  Int_t nEvents;
  sscanf(argv[4], "%d", &nEvents);

  // Open output file
  TFile *outFile = TFile::Open(outputFileName, "RECREATE");
  if(NULL == outFile) {
    cout << "-E- main : " << outputFileName
	 << " can not be opened!" << endl;
    return 3;
  }
  UEvent* event = new UEvent();
  TTree* tree = new TTree("events", "Event Data");
  tree->Branch("event", "UEvent", &event);


  // Read optns file
  Int_t aProj, zProj, aTarg, zTarg;
  Float_t pnll, bMin, bMax, phiMin, phiMax;
  Int_t evreq;
  if(kFALSE == ReadOptnsFile(optnsFileName, aProj, zProj, aTarg, zTarg,
			     pnll, bMin, bMax, phiMin, phiMax, evreq)) {
    return 4;
  }

  // Store run variables
  Double_t eLab = pnll + kProtonMass;
  Double_t sqrts = TMath::Sqrt(2*kProtonMass*kProtonMass + 2*eLab*kProtonMass);
  Double_t eCM = sqrts/2.;
  Double_t pCM = TMath::Sqrt(eCM*eCM - kProtonMass*kProtonMass);
  Double_t pProj = +pCM;
  Double_t pTarg = -pCM;
  URun* run = new URun("venus", "",
		       aProj, zProj, pProj,
		       aTarg, zTarg, pTarg,
		       bMin, bMax, -1,
		       phiMin, phiMax, 0., evreq);
  run->Print();


  // Open input file
  FILE* file = fopen(inputFileName.Data(), "r");
  if(NULL == file) {
    cout << "-E- main : " << inputFileName
	 << " does not exist!" << endl;
    return 5;
  }

  // Declare event variables
  Int_t nrevt = 0;     // event number
  Int_t nptevt = 0;    // number of stored particles per event
  Float_t bimevt = 0.; // impact parameter
  Float_t phievt = 0.; // angle of impact parameter
  Int_t kolevt = 0;    // number of collisions
  Int_t colevt = 0;    // effective number of collisions
  Float_t pmxevt = 0.; // reference momentum
  Float_t egyevt = 0.; // pp cm energy
  Int_t npjevt = 0;    // number of primary projectile participants
  Int_t ntgevt = 0;    // number of primary target participants

  // Declare particle variables
  Int_t i = 0;          // particle number
  Int_t idptl = 0;      // particle id
  Float_t pptlx = 0.;   // x-component of the particle momentum
  Float_t pptly = 0.;   // y-component of the particle momentum
  Float_t pptlz = 0.;   // z-component of the particle momentum
  Float_t energy = 0.;  // particle energy
  Float_t mass = 0.;    // particle mass
  Int_t iorptl = 0;     // particle number of father (<0 no father)
  Int_t jorptl = 0;     // particle number of mother (<0 no mother)
  Int_t istptl = 0;     // generation flag: last gen. (0) or not (1) or ghost (2)
  Float_t xorptlx = 0.; // x-component of formation point
  Float_t xorptly = 0.; // y-component of formation point
  Float_t xorptlz = 0.; // z-component of formation point
  Float_t xorptlt = 0.; // formation time
  Float_t tivptlf = 0.; // formation time (always in the pp-cms!)
  Float_t tivptld = 0.; // destruction time (always in the pp-cms!)

  // Declare variables outside the loop
  Int_t pdgCode;
  Int_t child[] = {0, 0};

  // Loop over events
  for(Int_t iEvent = 0; iEvent < nEvents; iEvent++) {
    // Read event variables
    fseek(file, 4, SEEK_CUR);
    fread((void*)&nrevt, 4, 1, file);
    fread((void*)&nptevt, 4, 1, file);
    fread((void*)&bimevt, 4, 1, file);
    fread((void*)&phievt, 4, 1, file);
    fread((void*)&kolevt, 4, 1, file);
    fread((void*)&colevt, 4, 1, file);
    fread((void*)&pmxevt, 4, 1, file);
    fread((void*)&egyevt, 4, 1, file);
    fread((void*)&npjevt, 4, 1, file);
    fread((void*)&ntgevt, 4, 1, file);
    fseek(file, 4, SEEK_CUR);
    if(feof(file)) {
      break;
    }

    // Store event variables
    event->SetEventNr(nrevt);
    event->SetB(bimevt);
    event->SetPhi(phievt);

    event->Clear();

    // Loop over particles
    for(Int_t iPa = 0; iPa < nptevt; iPa++) {
      // Read particle variables
      fseek(file, 4, SEEK_CUR);
      fread((void*)&i, 4, 1, file);
      fread((void*)&idptl, 4, 1, file);
      fread((void*)&pptlx, 4, 1, file);
      fread((void*)&pptly, 4, 1, file);
      fread((void*)&pptlz, 4, 1, file);
      fread((void*)&energy, 4, 1, file);
      fread((void*)&mass, 4, 1, file);
      fread((void*)&iorptl, 4, 1, file);
      fread((void*)&jorptl, 4, 1, file);
      fread((void*)&istptl, 4, 1, file);
      fread((void*)&xorptlx, 4, 1, file);
      fread((void*)&xorptly, 4, 1, file);
      fread((void*)&xorptlz, 4, 1, file);
      fread((void*)&xorptlt, 4, 1, file);
      fread((void*)&tivptlf, 4, 1, file);
      fread((void*)&tivptld, 4, 1, file);
      fseek(file, 4, SEEK_CUR);
      if(feof(file)) {
	break;
      }

      // Get PDG code
      pdgCode =
          UPIDConverter::Instance()->GetPDGCode(idptl,
                                                UPIDConverter::eWerner);
      if (pdgCode == 0) {
	cout << "-W- main : unknown VENUS particle : "
	     << idptl << endl;
	continue;
      }

      // Add particle to the event
      event->AddParticle(iPa, pdgCode, 1, iorptl, 0,
			 0, 0, child,
			 pptlx, pptly, pptlz, energy,
			 xorptlx, xorptly, xorptlz, xorptlt,
			 1.);
    }

    // Fill the output tree
    tree->Fill();

    // Control output
    if(0 == (iEvent%10)) {
      cout << "-I- main : Event " << iEvent << " processed." << endl;
    }

    // Break if end of file is reached
    if(feof(file)) {
      break;
    }
  }

  // Close input file
  fclose(file);

  // Close output file
  run->Write();
  tree->Write();
  outFile->Close();

  // Exit
  return 0;
}


Bool_t ReadOptnsFile(const TString &name,
		     Int_t &aProj, Int_t &zProj,
		     Int_t &aTarg, Int_t &zTarg,
		     Float_t &pnll, Float_t &bMin, Float_t &bMax,
		     Float_t &phiMin, Float_t &phiMax, Int_t &evreq)
{
  // Open file
  FILE *file = fopen(name.Data(), "r");
  if(NULL == file) {
    cout << "-E- venus2u: cannot open optns file " << name << endl;
    return kFALSE;
  }

  cout << "-I- venus2u: reading optns file " << name << endl;

  char s[1000];
  char sd[1000];
  TString str;
  Int_t ival;
  Float_t fval;


  // Check for output format
  ival = -1;
  while(! feof(file)) {
    // Read string from file
    fgets(s, 1000, file);
    // Check for eof
    if(feof(file)) break;

    str = (TString) s;
    if(str.Contains("set istore")) {
      sscanf(s, "set istore %d %s", &ival, sd);
      if(1 != ival) {
	cout << "-E- venus2u: "
	     << "only istore option no 1 is supported !" << endl;
	fclose(file);
	return kFALSE;
      }
    }
  }
  if(-1 == ival) {
    cout << "-E- venus2u: "
	 << "only istore option no 1 is supported !" << endl;
    fclose(file);
    return kFALSE;
  }


  // Check for output in CM frame
  ival = -1;
  fseek(file, 0, SEEK_SET);
  while(! feof(file)) {
    // Read string from file
    fgets(s, 1000, file);
    // Check for eof
    if(feof(file)) break;

    str = (TString) s;
    if(str.Contains("set labsys")) {
      sscanf(s, "set labsys %d %s", &ival, sd);
      if(0 != ival) {
	cout << "-E- venus2u: "
	     << "only output in NN CM frame is supported !" << endl;
	fclose(file);
	return kFALSE;
      }
    }
  }
  if(-1 == ival) {
    cout << "-E- venus2u: "
	 << "only output in NN CM frame is supported !" << endl;
    fclose(file);
    return kFALSE;
  }


  // Read variables
  bMin = 0.;
  phiMin = 0.;
  phiMax = TMath::TwoPi();
  fseek(file, 0, SEEK_SET);
  while(! feof(file)) {
    // Read string from file
    fgets(s, 1000, file);
    // Check for eof
    if(feof(file)) break;

    str = (TString) s;
    if(str.Contains("set laproj")) {
      sscanf(s, "set laproj %d %s", &ival, sd);
      zProj = ival;
    } else if(str.Contains("set maproj")) {
      sscanf(s, "set maproj %d %s", &ival, sd);
      aProj = ival;
    } else if(str.Contains("set latarg")) {
      sscanf(s, "set latarg %d %s", &ival, sd);
      zTarg = ival;
    } else if(str.Contains("set matarg")) {
      sscanf(s, "set matarg %d %s", &ival, sd);
      aTarg = ival;
    } else if(str.Contains("set pnll")) {
      sscanf(s, "set pnll %f %s", &fval, sd);
      pnll = fval;
    } else if(str.Contains("set bminim")) {
      sscanf(s, "set bminim %f %s", &fval, sd);
      bMin = fval;
    } else if(str.Contains("set bmaxim")) {
      sscanf(s, "set bmaxim %f %s", &fval, sd);
      bMax = fval;
    } else if(str.Contains("set phimin")) {
      sscanf(s, "set phimin %f %s", &fval, sd);
      phiMin = fval;
    } else if(str.Contains("set phimax")) {
      sscanf(s, "set phimax %f %s", &fval, sd);
      phiMax = fval;
    } else if(str.Contains("set nevent")) {
      sscanf(s, "set nevent %d %s", &ival, sd);
      evreq = ival;
    }
  }

  // Close the file
  fclose(file);

  return kTRUE;
}





