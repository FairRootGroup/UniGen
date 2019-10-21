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

#ifndef UGENESISTASK_H
#define UGENESISTASK_H

#ifndef UTASK
#include "UTask.h"
#endif

class URun;
class UEvent;
class CGenesisEvent;
class CGenesisRun;


class UTaskGenesis : public UTask {

 private:
  URun          *fRun;     // Run description in UGF representation
  UEvent        *fEvent;   // Event description in UGF representation
  CGenesisRun   *fGRun;    // Genesis run description
  CGenesisEvent *fGEvent;  // Genesis event description
  
 public:
  UTaskGenesis();
  UTaskGenesis(const char *name, Int_t verbose = 1);
  virtual ~UTaskGenesis();
  
  virtual void Init();
  virtual void Exec(Option_t *option = "");
  virtual void Finish();

  inline CGenesisEvent* GetGenesisEvent() {return fGEvent;}
  
  ClassDef(UTaskGenesis, 1);
};

#endif

