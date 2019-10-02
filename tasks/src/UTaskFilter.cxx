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

