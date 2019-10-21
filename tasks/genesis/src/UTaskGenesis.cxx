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


#include "CGenesisEvent.h"
#include "CGenesisRun.h"

#include "UManager.h"
#include "UEvent.h"
#include "UParticle.h"
#include "UTaskGenesis.h"


//__________________________________________________________
UTaskGenesis::UTaskGenesis() : UTask()
{
  // Default constructor
  fRun = NULL;
  fEvent = NULL;
  fGRun = NULL;
  fGEvent = NULL;
}

//__________________________________________________________
UTaskGenesis::UTaskGenesis(const char *name, Int_t verbose)
  :UTask(name, verbose)
{
  // Standard constructor
  fRun = NULL;
  fEvent = NULL;
  fGRun = NULL;
  fGEvent = NULL;
}

//__________________________________________________________
UTaskGenesis::~UTaskGenesis()
{
  // Destructor
  if(fGEvent) delete fGEvent;
}

//__________________________________________________________
void UTaskGenesis::Init()
{
  // Initialisation. Get pointer to the run description and
  // event using the UManager. Allocate memory for the intermediate
  //  CGenesisEvent.

  UManager *mgr = UManager::Instance();

  fRun = mgr->GetRun();
  fEvent = mgr->GetEvent();

  fGEvent = new CGenesisEvent();

  Info("Init()","Initialisation complete");
}

//__________________________________________________________
void UTaskGenesis::Exec(Option_t *option)
{
  // Task executor. Convert UEvent to CGenesisEvent
  // for further usage !

  UParticle *pUGF;
  fGEvent->Clear();
  for(int ip=0; ip<fEvent->GetNpa(); ip++){
    pUGF = fEvent->GetParticle(ip);
    fGEvent->AddParticle(pUGF->GetPdg(), pUGF->GetStatus(),
			 pUGF->GetParent(), -1, pUGF->GetFirstChild(),
			 pUGF->GetLastChild(),
			 pUGF->GetMomentum(), pUGF->GetPosition());
  }
}

//__________________________________________________________
void UTaskGenesis::Finish()
{
  // Task closer
}


ClassImp(UTaskGenesis);
