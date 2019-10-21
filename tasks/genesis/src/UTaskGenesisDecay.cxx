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

#include "TParticle.h"

#include "CGenesisDecayer.h"
#include "CGenesisEvent.h"

#include "UEvent.h"
#include "UManager.h"
#include "UTaskGenesisDecay.h"


//______________________________________________________________
UTaskGenesisDecay::UTaskGenesisDecay(CGenesisEvent *genesisEvent)
  :UTask("GenesisDecay",1)
{
  // Default constructor
  fDecayer = new CGenesisDecayer();
  fGEvent = genesisEvent;
}

//______________________________________________________________
UTaskGenesisDecay::~UTaskGenesisDecay()
{
  // Destructor
  delete fDecayer;
}

//______________________________________________________________
void UTaskGenesisDecay::Init()
{
  // Initialize Genesis decayer	from fRun

  //fDecayer->Initialize();	

  UManager *mgr = UManager::Instance();

  fRun = mgr->GetRun();
  fEvent = mgr->GetEvent();
}

//______________________________________________________________
void UTaskGenesisDecay::Exec(Option_t *option)
{
  if(!fGEvent){
    Warning("Exec(Option_t*)", "Genesis event is missing !");
    return;
  }
  
  CGenesisEvent *lGEv = fDecayer->Decay(fGEvent);

  if(!lGEv) return;

  Info("Exec(Option_t*)", Form("Particles In %d Out %d",
			       fGEvent->GetNpa(),
			       lGEv->GetNpa()));
  
  TLorentzVector mom, pos;
  Int_t child[2];
  TParticle *pGenesis;
  
  // convert fGEvent to UEvent[UTaskGenesis::fEvent]
  fEvent->Clear();
  for(int ip=0; ip<lGEv->GetNpa(); ip++){
    pGenesis = lGEv->GetParticle(ip);
    pGenesis->Momentum(mom);
    pGenesis->ProductionVertex(pos);
    child[0] = pGenesis->GetDaughter(0);
    child[1] = pGenesis->GetDaughter(1);
    fEvent->AddParticle(
			ip,
			pGenesis->GetPdgCode(),
			pGenesis->GetStatusCode(),  // status
			pGenesis->GetMother(0), // parent
			0,                      // parentDecay
			0,                      // mate
			0,                      // decay
			child,
			mom,
			pos,
			0.);                    // weight
  }
}

//______________________________________________________________
void UTaskGenesisDecay::Finish()
{
}


ClassImp(UTaskGenesisDecay);
