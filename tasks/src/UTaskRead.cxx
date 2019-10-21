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
#include "UManager.h"
#include "UTaskRead.h"


//--------------------------------------------------------------------
UTaskRead::UTaskRead()
{
  // Default constructor
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
UTaskRead::UTaskRead(const char *name, Int_t verbose)
  :UTask(name, verbose)
{
  // Standard constructor
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
UTaskRead::~UTaskRead()
{
  // Destructor
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UTaskRead::Init()
{
  // Initialisation. Get pointer to the run description and
  // event using the UManager
  UManager *mgr = UManager::Instance();

  fRun = mgr->GetRun();

  fEvent = mgr->GetEvent();

  cout << "-I- UTaskRead::Init: Initialisation completed." << endl;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UTaskRead::Exec(Option_t *option)
{
  // Task execution. Print particles for the last time step of
  // each event
  if(0==fEvent->GetEventNr() && 39==fEvent->GetStepNr()) {
    // Print
    fEvent->Print("all");
  }
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UTaskRead::Finish()
{
  // Print out run description
  fRun->Print();
}
//--------------------------------------------------------------------


ClassImp(UTaskRead);


