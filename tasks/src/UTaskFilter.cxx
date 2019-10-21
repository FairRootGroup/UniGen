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

#include "UManager.h"
#include "URun.h"
#include "UEvent.h"
#include "UParticle.h"
#include "UTaskFilter.h"


//____________________________________________________________________
//
// UTaskFilter
//
// Example task for filtering of the events.
// If SetFilterMode(0) was called, particles added using
// AddParticle(pdgCode) method will be removed from file.
// If SetFilterMode(1) was called, particles added using
// AddParticle(pdgCode) method will be copied to output file,
// the rest will be removed.
// The default filter mode value is 1.
//


//--------------------------------------------------------------------
UTaskFilter::UTaskFilter()
{
  // Default constructor
  fEvent = NULL;
  fEvents = 0;
  fMode = 1;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
UTaskFilter::UTaskFilter(const char *name, Int_t verbose)
  : UTask(name, verbose)
{
  // Standard constructor
  fEvent = NULL;
  fEvents = 0;
  fMode = 1;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
UTaskFilter::~UTaskFilter()
{
  // Destructor
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UTaskFilter::Init()
{
  // Initialisation. Get pointer to the event

  // Get pointer to the UManager
  UManager *mgr = UManager::Instance();
  if(NULL == mgr) {
    cout << "-E- UManager::Init: "
	 << "UManager is not instantiated!" << endl;
    return;
  }

  // Get pointer to the event
  fEvent = mgr->GetEvent();
  if(NULL == mgr) {
    cout << "-W- UManager::Init: "
	 << "no event!" << endl;
  }

  cout << "-I- UTaskFilter::Init: initialisation completed."
       << endl;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UTaskFilter::Exec(Option_t *option)
{
  // Task execution. Filter the events, depending
  // on the value of fMode

  // No event - no action
  if(NULL == fEvent) return;

  // Loop over particles
  UParticle *p;
  Bool_t del;
  for(Int_t iPa = 0; iPa < fEvent->GetNpa(); iPa++) {
    // Get pointer to the particle
    p = fEvent->GetParticle(iPa);
    if(NULL == p) continue;

    // Erase the particle, if not in list
    del = (find(fPdgList.begin(), fPdgList.end(), p->GetPdg()) ==
	   fPdgList.end());
    
    // Inverse, in case of mode 0
    if(0 == fMode) {
      del = ! del;
    }
    
    // Delete this particle
    if(del) {
      fEvent->RemoveAt(iPa);
      iPa--;
    }
  }

  // Control output
  if(0 == (fEvents%10)) {
    cout << "-I- UTaskFilter::Exec: "
	 << "event " << fEvents << " finished." << endl;
  }

  // Increment counter
  fEvents += 1;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UTaskFilter::Finish()
{
  // Finish at the end of analysis
}
//--------------------------------------------------------------------


ClassImp(UTaskFilter);

