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

#include <iostream>
using namespace std;

#include "URun.h"
#include "UEvent.h"
#include "UParticle.h"
#include "UManager.h"
#include "UTaskWrite.h"


//____________________________________________________________________
//
// UTaskWrite
//
// This is an example task of generation of the events.
//


//--------------------------------------------------------------------
UTaskWrite::UTaskWrite()
{
  // Default constructor
  fEvents = 0;
  fSteps = 0;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
UTaskWrite::UTaskWrite(const char *name, Int_t verbose)
  :UTask(name, verbose)
{
  // Standard constructor
  fEvents = 0;
  fSteps = 0;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
UTaskWrite::~UTaskWrite()
{
  // Destructor
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UTaskWrite::Init()
{
  // Initialisation of the task. Create and set the run
  // parameters. Set the event parameters
  UManager *mgr = UManager::Instance();

  // Run variables
  const char* generator = "Simple";
  const char* comment = "Test run";
  Int_t    ap = 197;
  Int_t    zp = 79;
  Double_t pp = 25.; // GeV/u
  Int_t    at = 197;
  Int_t    zt = 79;
  Double_t pt = 0.;
  Double_t bmin = 0.;
  Double_t bmax = 0.;
  Double_t phimin = 0.;
  Double_t phimax = TMath::TwoPi();
  Int_t nevents = 100;
  mgr->CreateRun(generator, comment,
		 ap, zp, pp, at, zt, pt,
		 bmin, bmax, 1, phimin, phimax, 0, nevents);

  // Event variables
  fEvent = mgr->GetEvent();
  fEvent->SetB(1);
  fEvent->SetPhi(2);
  fEvent->SetNes(40);
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UTaskWrite::Exec(Option_t *option)
{
  // Task execution. Generate 100 particles per each time step
  Int_t child[2];

  fEvent->Clear();
  fEvent->SetEventNr(fEvents);
  fEvent->SetStepNr(fSteps);
  fEvent->SetStepT(fSteps);

  // Generate particles
  for(Int_t ipa = 0; ipa < 100; ipa++) {
    fEvent->AddParticle(ipa, -11, 1, -1, -1, -1, -1, child,
			   1., 2., 3., TMath::Sqrt(14.),
			   4., 5., 6., fSteps,
			   1.);
  }

  if(0==(fEvents%10) && 39==fSteps) {
    cout << fEvents << " event generated" << endl;
  }
  
  fSteps += 1;
  if(40 == fSteps) {
    fSteps = 0;
    fEvents += 1;
  }

  //  UManager::Instance()->WriteEvent(kFALSE);
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UTaskWrite::Finish()
{
  // Finish at the end of analysis
}
//--------------------------------------------------------------------


ClassImp(UTaskWrite);

