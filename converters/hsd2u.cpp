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
//-----                 HSD to UniGen converter                  -----
//-----             Created 2006-Aug-09 by D.Kresan              -----
//--------------------------------------------------------------------

#include <iostream>
#include <fstream>
using namespace std;

//#include "TROOT.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"

#include "URun.h"
#include "UEvent.h"
#include "UParticle.h"

const Double_t kProtonMass = 0.938271998;


//TROOT root("hsd2u", "HSD to UniGen converter");


Int_t hsd2pdg(Int_t group, Int_t type, Int_t charge);


//--------------------------------------------------------------------
int main(int argc, char *argv[])
{
  // Check number of input parameters
  if(argc < 6) {
    cout << "Too few parameters. Usage:" << endl
	 << "hsd2u hsdInputFile baryonFile mesonFile outputFile nEvents"
	 << endl;
    return 1;
  }


  // Read input parameters
  const char *hsdInput = argv[1];
  const char *fileNameInB     = argv[2];
  const char *fileNameInM     = argv[3];
  const char *fileNameOut     = argv[4];
  Int_t nev;
  sscanf(argv[5], "%d", &nev);


  // Open input files
  cout << "-I- hsd2u: Opening input file fort.300 " << fileNameInB << endl;
  cout << "-I- hsd2u: Opening input file fort.301 " << fileNameInM << endl;
  FILE     *input           = fopen(hsdInput,"rt");//HSD input file
  ifstream *inputFileB      = new ifstream(fileNameInB);
  ifstream *inputFileM      = new ifstream(fileNameInM);
  if( ! inputFileB->is_open()) {
    cout << "-E- hsd2u: Cannot open "
	 << fileNameInB << " input file!" << endl;
    return 1;
  }
  if( ! inputFileM->is_open()) {
    cout << "-E- hsd2u: Cannot open "
	 << fileNameInM << " input file!" << endl;
    return 1;
  }
  

  // Open output file
  TFile *outputFile = TFile::Open(fileNameOut, "RECREATE");
  TTree *tree = new TTree("events", "events");
  UEvent *event = new UEvent();
  tree->Branch("event", "UEvent", &event);

  
  // Run description
  Int_t aTarg, zTarg, aProj, zProj;
  Float_t eKin, bmin, bmax, dbimp;
  Int_t num, isubs;
  Char_t dump[1024];
  /* if...fscanf  are used to skip trailing comments. Ideally we could use
     field names from those comments in order to allow out-of-order reads,
     one would however have to implement a backward-compatibiity mode */
  fscanf(input,"%i",&aTarg);
  if (fgetc(input) == ',')
    fgets(dump, 1024, input);
  fscanf(input,"%i",&zTarg);
  if (fgetc(input) == ',')
    fgets(dump, 1024, input);
  fscanf(input,"%i",&aProj);
  if (fgetc(input) == ',')
    fgets(dump, 1024, input);
  fscanf(input,"%i",&zProj);
  if (fgetc(input) == ',')
    fgets(dump, 1024, input);
  fscanf(input,"%e",&eKin);
  if (fgetc(input) == ',')
    fgets(dump, 1024, input);
  fscanf(input,"%e",&bmin);
  if (fgetc(input) == ',')
    fgets(dump, 1024, input);
  fscanf(input,"%e",&bmax);
  if (fgetc(input) == ',')
    fgets(dump, 1024, input);
  fscanf(input,"%e",&dbimp);
  if (fgetc(input) == ',')
    fgets(dump, 1024, input);
  fscanf(input,"%i",&num);
  if (fgetc(input) == ',')
    fgets(dump, 1024, input);
  fscanf(input,"%i",&isubs);
  if (fgetc(input) == ',')
    fgets(dump, 1024, input);
  Int_t evav = isubs*num;
  Double_t eLab = eKin + kProtonMass;
  Double_t sqrts = TMath::Sqrt(2*kProtonMass*kProtonMass + 2*eLab*kProtonMass);
  Double_t eCM = sqrts/2.;
  Double_t pCM = TMath::Sqrt(eCM*eCM - kProtonMass*kProtonMass);
  Double_t pProj = +pCM;
  Double_t pTarg = -pCM;
  URun *run = new URun("HSD", "",
		       aProj, zProj, pProj,
		       aTarg, zTarg, pTarg,
		       bmin, bmax, -1,
		       0., TMath::TwoPi(), 0., evav);
  run->Print();
  run->Write();


  // Check number of events
  if(nev > evav) {
    cout << "-W- hsd2u: not enough events in input file. Processing only "
	 << evav << " instead of " << nev << " requested." << endl;
    nev = evav;
  }


  // Define track variables to be read from file
  Int_t type[2], charge[2], ISUB[2], IRUN[2];
  Double_t px[2], py[2], pz[2], e[2], b[2];
  Int_t child[] = {-1, -1};


  // Read barion and meson files in parallel
  ifstream *inputFile[] = {inputFileB, inputFileM};


  // Event number
  Int_t iev = 0;
  // Control flag
  Bool_t eventChanged[] = {kFALSE, kFALSE};
  // Pdg code
  Int_t pdg;


  Int_t nPip;
  Int_t nKap;
  Int_t nProt;


  // Loop over events
  for(Int_t isub = 1; isub < isubs+1; isub++) {

    nPip = 0;
    nKap = 0;
    nProt = 0;

    for(Int_t j = 1; j < num+1; j++) {

      // Set event number
      event->SetEventNr(iev);
      
      // Read particles
      for(Int_t i = 0; i < 2; i++) {
	while(1) {
	  // If eof is reached
	  if(inputFile[i]->eof()) {
	    break;
	  }
	  // If event number has been changed
	  if(eventChanged[i]) {
	    // Do not read, reset flag
	    eventChanged[i] = kFALSE;
	  } else {
	    // Read entry
	    (*inputFile[i]) >> type[i] >> charge[i] >> ISUB[i] >> IRUN[i]
			    >> px[i] >> py[i] >> pz[i] >> e[i] >> b[i];
	    // If eof is reached
	    if(inputFile[i]->eof()) {
	      break;
	    }
	  }
	  // If next event
	  if(ISUB[i]!=isub || IRUN[i]!=j) {
	    eventChanged[i] = kTRUE;
	    break;
	  }
	  if(0==event->GetNpa() && 0==i) {
	    event->SetB(b[i]);
	  }
	  pdg = hsd2pdg(i, type[i], charge[i]);
	  if(-9999 != pdg) {
	    event->AddParticle(event->GetNpa(), pdg, 0, 0, 0, 0, 0, child,
			       px[i], py[i], pz[i], e[i], 0., 0., 0., 0., 1.);
	  }
	  switch(pdg) {
	  case 211 : nPip+=1; break;
	  case 321 : nKap+=1; break;
	  case 2212 : nProt+=1; break;
	  default : break;
	  }
	}
      }

      // Increment event counter
      iev += 1;

      // Fill the tree
      tree->Fill();

      // Clear the event
      event->Clear();

      // Stop if nev events are processed
      if(iev >= nev) {
	break;
      }
      // Stop if eof is reached for both files
      if(inputFileB->eof() && inputFileM->eof()) {
	break;
      }
    }// Loop over parallel runs

    cout << "ISUB = " << isub 
	 << ",  nPip=" << (Double_t)nPip/(Double_t)num
	 << ",  nKap=" << (Double_t)nKap/(Double_t)num
	 << ",  nProt=" << (Double_t)nProt/(Double_t)num << endl;
    
    // Stop if nev events are processed
    if(iev >= nev) {
      break;
    }
    // Stop if eof is reached
    if(inputFileB->eof() && inputFileM->eof()) {
      break;
    }
  }// Loop over subsequent runs


  // Write tree and close output file
  tree->Write();
  outputFile->Close();


  // Close input file
  inputFileB->close();
  inputFileM->close();


  // Exit
  return 0;
}
// -------------------------------------------------------------------


Int_t hsd2pdg(Int_t group, Int_t type, Int_t charge)
/* Translation of HSD (type, code) pairs into PDG identifiers.
   Based on subroutine TRANSPOSECODES from fritzi.F in HSD-2.5 */
{
  Int_t geant_pdg = -9999;

  if (0 == group) {
    /* BARYONS / ANTIBARYONS */
    const Int_t abstype = abs(type);
    const Int_t antisign = (type < 0 ? -1 : 1);
    const Int_t anticharge = antisign * charge;

    if (abstype == 1) {
      if (anticharge == 1)
        geant_pdg = antisign * 2212;	// p / p_bar
      else if (anticharge == 0)
        geant_pdg = antisign * 2112;	// n / n_bar
    }
    else if (abstype == 2) {
      if (anticharge == 2)
        geant_pdg = antisign * 2224;	// Delta(1232)++ / Delta(1232)--
      else if (anticharge == 1)
        geant_pdg = antisign * 2214;	// Delta(1232)+ / Delta(1232)-
      else if (anticharge == 0)
        geant_pdg = antisign * 2114;	// Delta(1232)0 / Delta(1232)0_bar
      else if (anticharge == -1)
        geant_pdg = antisign * 1114;	// Delta(1232)- / Delta(1232)+
    }
    // 3 - N(1440) - no unique PDG codes?
    // 4 - N(1535) - no unique PDG codes?
    else if (abstype == 5) {
      if (anticharge == 0)
        geant_pdg = antisign * 3122;	// Lambda0 / Lambda0_bar
    }
    else if (abstype == 6) {
      if (anticharge == 1)
        geant_pdg = antisign * 3222;	// Sigma+ / Sigma-
      else if (anticharge == 0)
        geant_pdg = antisign * 3212;	// Sigma0 / Sigma0_bar
      else if (anticharge == -1)
        geant_pdg = antisign * 3112;	// Sigma- / Sigma+
    }
    else if (abstype == 7) {
      if (anticharge == 1)
        geant_pdg = antisign * 3224;	// Sigma*+ / Sigma*-
      else if (anticharge == 0)
        geant_pdg = antisign * 3214;	// Sigma*0 / Sigma*0_bar
      else if (anticharge == -1)
        geant_pdg = antisign * 3114;	// Sigma*- / Sigma*+
    }
    else if (abstype == 8) {
      if (anticharge == 0)
        geant_pdg = antisign * 3322;	// Xi0 / Xi0_bar
      else if (anticharge == -1)
        geant_pdg = antisign * 3312;	// Xi- / Xi-_bar
    }
    else if (abstype == 9) {
      if (anticharge == 0)
        geant_pdg = antisign * 3324;	// Xi*0 / Xi*0_bar
      else if (anticharge == -1)
        geant_pdg = antisign * 3314;	// Xi*- / Xi*+
    }
    else if (abstype == 10) {
      if (anticharge == -1)
        geant_pdg = antisign * 3334;	// Omega- / Omega+
    }
    else if (abstype == 11) {
      if (anticharge == 1)
        geant_pdg = antisign * 4122;	// Lambda_c+ / Lambda_c-
    }
    else if (abstype == 12) {
      if (anticharge == 2)
        geant_pdg = antisign * 4222;	// Sigma_c++ / Sigma_c--
      else if (anticharge == 1)
        geant_pdg = antisign * 4212;	// Sigma_c+ / Sigma_c-
      else if (anticharge == 0)
        geant_pdg = antisign * 4112;	// Sigma_c0 / Sigma_c0_bar
    }
    else if (abstype == 13 || abstype == 15) {
      if (anticharge == 1)
        geant_pdg = antisign * 4232;	// Xi_c+ / Xi_c-
      else if (anticharge == 0)
        geant_pdg = antisign * 4132;	// Xi_c0 / Xi_c0_bar
    }
    else if (abstype == 14) {
      if (anticharge == 2)
        geant_pdg = antisign * 4224;	// Sigma_c*++ / Sigma_c*--
      else if (anticharge == 1)
        geant_pdg = antisign * 4214;	// Sigma_c*+ / Sigma_c*-
      else if (anticharge == 0)
        geant_pdg = antisign * 4114;	// Sigma_c*0 / Sigma_c*0_bar
    }
    // 15 - duplicate of 13 - see above
    else if (abstype == 16) {
      if (anticharge == 1)
        geant_pdg = antisign * 4322;	// Xi'_c+ / Xi'_c-
      else if (anticharge == 0)
        geant_pdg = antisign * 4312;	// Xi'_c0 / Xi'_c0_bar
    }
    else if (abstype == 17) {
      if (anticharge == 1)
        geant_pdg = antisign * 4324;	// Xi_c*+ / Xi_c*-
      else if (anticharge == 0)
        geant_pdg = antisign * 4314;	// Xi_c*0 / Xi_c*0_bar
    }
    else if (abstype == 18) {
      if (anticharge == 0)
        geant_pdg = antisign * 4332;	// Omega_c0 / Omega_c0_bar
    }
    else if (abstype == 19) {
      if (anticharge == 0)
        geant_pdg = antisign * 4334;	// Omega_c*0 / Omega_c*0_bar
    }
    else if (abstype == 20) {
      if (anticharge == 0)
        geant_pdg = antisign * 5122;	// Lambda_b0 / Lambda_b0_bar
    }
    else if (abstype == 21) {
      if (anticharge == 1)
        geant_pdg = antisign * 5222;	// Sigma_b+ / Sigma_b-
      else if (anticharge == 0)
        geant_pdg = antisign * 5212;	// Sigma_b0 / Sigma_b0_bar
      else if (anticharge == -1)
        geant_pdg = antisign * 5112;	// Sigma_b- / Sigma_b+
    }
    else if (abstype == 22) {
      if (anticharge == 1)
        geant_pdg = antisign * 5224;	// Sigma_b*+ / Sigma_b*-
      else if (anticharge == 0)
        geant_pdg = antisign * 5214;	// Sigma_b*0 / Sigma_b*0_bar
      else if (anticharge == -1)
        geant_pdg = antisign * 5114;	// Sigma_b*- / Sigma_b*+
    }
  }

  else if (1 == group) {
    /* MESONS */
    if (type == 1) {
      if (charge == 1)
        geant_pdg = 211;	// pi+
      else if (charge == 0)
        geant_pdg = 111;	// pi0
      else if(charge == -1)
        geant_pdg = -211;	// pi-
    }
    else if (type == 2) {
      if (charge == 0)
        geant_pdg = 221;	// eta
    }
    else if (type == 3) {
      if (charge == 1)
        geant_pdg = 321;	// K+
      else if (charge == -1)
        geant_pdg = -321;	// K-
    }
    else if (type == 4) {
      if (charge == 1)
        geant_pdg = 323;	// K*+
      else if (charge == -1)
        geant_pdg = -323;	// K*-
    }
    else if (type == 5) {
      if (charge == 1)
        geant_pdg = 213;	// rho+
      else if (charge == 0)
        geant_pdg = 113;	// rho0
      else if(charge == -1)
        geant_pdg = -213;	// rho-
    }
    else if (type == 6) {
      if (charge == 0)
        geant_pdg = 223;	// omega(783)
    }
    else if (type == 7) {
      if (charge == 0)
        geant_pdg = 333;	// Phi(1020)
    }
    else if (type == 8) {
      if (charge == 0)
        geant_pdg = 331;	// eta'
    }
    // 9 - a_1 - no PDG identifier?
    // 10 - not used
    else if (type == 11) {
      if (charge == 0)
        geant_pdg = 311;	// K0
    }
    else if (type == 12) {
      if (charge == 0)
        geant_pdg = -311;	// K0_bar
    }
    else if (type == 13) {
      if (charge == 0)
        geant_pdg = 313;	// K*0
    }
    else if (type == 14) {
      if (charge == 0)
        geant_pdg = -313;	// K*0_bar
    }
    else if (type == 15) {
      if (charge == 1)
        geant_pdg = 411;	// D+
      else if (charge == -1)
        geant_pdg = -411;	// D-
    }
    else if (type == 16) {
      if (charge == 0)
        geant_pdg = 421;	// D0
    }
    else if (type == 17) {
      if (charge == 0)
        geant_pdg = -421;	// D0_bar
    }
    else if (type == 18) {
      if (charge == 1)
        geant_pdg = 413;	// D*+(2010)
      else if (charge == -1)
        geant_pdg = -413;	// D*-(2010)
    }
    else if (type == 19) {
      if (charge == 0)
        geant_pdg = 423;	// D*0(2007)
    }
    else if (type == 20) {
      if (charge == 0)
        geant_pdg = -423;	// D*0_bar(2007)
    }
    // 21,22 - not used
    else if (type == 23) {
      if (charge == 1)
        geant_pdg = 431;	// D_s+(1970)
    }
    else if (type == 24) {
      if (charge == -1)
        geant_pdg = -431;	// D_s-(1970)
    }
    else if (type == 25) {
      if (charge == 1)
        geant_pdg = 433;	// D_s*+(1970)
    }
    else if (type == 26) {
      if (charge == -1)
        geant_pdg = -433;	// D_s*-(1970)
    }
    // 27-30 - not used
    else if (type == 31) {
      if (charge == 1)
        geant_pdg = 521;	// B+
      else if (charge == -1)
        geant_pdg = -521;	// B-
    }
    else if (type == 32) {
      if (charge == 0)
        geant_pdg = 511;	// B0
    }
    else if (type == 33) {
      if (charge == 0)
        geant_pdg = -511;	// B0_bar
    }
    else if (type == 34) {
      if (charge == 1)
        geant_pdg = 523;	// B*+
      else if (charge == -1)
        geant_pdg = -523;	// B*-
    }
    else if (type == 35) {
      if (charge == 0)
        geant_pdg = 513;	// B*0
    }
    else if (type == 36) {
      if (charge == 0)
        geant_pdg = -513;	// B*0_bar
    }
    else if (type == 37) {
      if (charge == 0)
        geant_pdg = 531;	// B_s0
    }
    else if (type == 38) {
      if (charge == 0)
        geant_pdg = -531;	// B_s0~
    }
    else if (type == 39) {
      if (charge == 0)
        geant_pdg = 533;	// B_s*0
    }
    else if (type == 40) {
      if (charge == 0)
        geant_pdg = -533;	// B_s*0~
    }
  }
 
  if (geant_pdg == -9999)
    cout << "-W-: hsd2u: PDG code for particle " << group << " / (" << type
         << ", " << charge << ") unknown" << endl;
  return geant_pdg;
}
