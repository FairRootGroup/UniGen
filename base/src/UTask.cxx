#include "UTask.h"


//____________________________________________________________________
//
// UTask
//
// Base abstract class for tasks. User derived classe has to
// override pure virtual methods Init, Exec and Finish
//


//--------------------------------------------------------------------
UTask::UTask()
{
  // Default constructor
  fVerbose = 1;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
UTask::UTask(const char *name, Int_t verbose)
  : TTask(name, "UTask")
{
  // Standard constructor
  fVerbose = verbose;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
UTask::~UTask()
{
  // Destructor
}
//--------------------------------------------------------------------


ClassImp(UTask);

