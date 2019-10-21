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

