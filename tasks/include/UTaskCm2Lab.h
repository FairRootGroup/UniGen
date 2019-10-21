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

// ------------------------------------------------------------------
// -----                         UTaskCm2Lab                    -----
// -----              Created 2006/07/04 by D. Kresan           -----
// ------------------------------------------------------------------
#ifndef _UTASK_CM2LAB_
#define _UTASK_CM2LAB_

#include "UTask.h"

class UEvent;


class UTaskCm2Lab : public UTask {

private:
    Int_t    fEvents;  // Number of processed events
    UEvent  *fEvent;   // Input event
    Bool_t   fCM;      // CM flag
    Double_t fBetaCM;  // CM velocity
    Double_t fGammaCM; // CM gamma factor

public:
    UTaskCm2Lab();
    UTaskCm2Lab(const char *name, Int_t verbose = 1);
    virtual ~UTaskCm2Lab();

    void Init();
    void Exec(Option_t *option);
    void Finish();

    ClassDef(UTaskCm2Lab, 1);
};


#endif

