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

#ifndef UGENESISTASKDECAY_H
#define UGENESISTASKDECAY_H

#include "UTask.h"

class CGenesisDecayer;
class CGenesisEvent;
class URun;
class UEvent;

class UTaskGenesisDecay : public UTask {
 private:
  URun                *fRun;       //! Run description
  UEvent              *fEvent;     //! Event description
  CGenesisDecayer     *fDecayer;   //! Pointer to the Genesis decayer
  CGenesisEvent       *fGEvent;    //! Pointer to the Genesis event

 public:
  UTaskGenesisDecay(CGenesisEvent*);
  virtual ~UTaskGenesisDecay();
  
  virtual void Init();
  virtual void Exec(Option_t *option = "");
  virtual void Finish();
  
  ClassDef(UTaskGenesisDecay, 1);
};

#endif

