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

#ifndef _UTASKFILTER_
#define _UTASKFILTER_

#include <list>

#include "UTask.h"

class UEvent;


class UTaskFilter : public UTask {

 private:
  UEvent *fEvent;                   // Event
  Int_t   fEvents;                  // Number of processed events
  std::list<Int_t> fPdgList;        // List of PDG codes
  Int_t   fMode;                    // 0 - remove particles, 1 - copy particles

 public:
  UTaskFilter();                 
  UTaskFilter(const char *name,
	      Int_t verbose = 1);
  virtual ~UTaskFilter();        

  virtual void Init();           

  virtual void Exec(Option_t *option = ""); 

  virtual void Finish();

  inline void AddParticle(Int_t pdgCode) { fPdgList.push_back(pdgCode); }
  inline void SetFilterMode(Int_t mode)  { fMode = mode;                }

  ClassDef(UTaskFilter, 1);
}; 


#endif

