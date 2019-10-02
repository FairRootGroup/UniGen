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


