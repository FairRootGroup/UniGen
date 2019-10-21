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

#ifndef _UTASK_KIN_
#define _UTASK_KIN_

#include "UTask.h"

class TH2F;
class URun;
class UEvent;


class UTaskKin : public UTask {

 private:
  Int_t    fEvents;            // Number of events processed
  URun    *fRun;               // Run description
  UEvent  *fEvent;             // Event
  Double_t fBeamRapidity;      // Beam rapidity
  TH2F    *fh_mom_theta_pip;   // Momentum vs. polar angle
  TH2F    *fh_mom_theta_kap;   // Momentum vs. polar angle

 public:
  UTaskKin();
  UTaskKin(const char *name, Int_t verbose = 1);
  virtual ~UTaskKin();
  void Init();
  void Exec(Option_t *option = "");
  void Finish();

  ClassDef(UTaskKin, 1);
};


#endif

