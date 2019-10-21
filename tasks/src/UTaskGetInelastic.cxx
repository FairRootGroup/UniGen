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

#include <TObjArray.h>
#include <TObjString.h>

#include <UEvent.h>
#include <UManager.h>
#include <UParticle.h>
#include <URun.h>

#include <UTaskGetInelastic.h>


//--------------------------------------------------------------------
UTaskHSDGetInel::UTaskHSDGetInel() : fAllEvents(0), fInelEvents(0)
{ }

//--------------------------------------------------------------------


//--------------------------------------------------------------------
UTaskHSDGetInel::UTaskHSDGetInel(const char *name, Int_t verbose)
  : UTask(name, verbose), fAllEvents(0), fInelEvents(0)
{ }

//--------------------------------------------------------------------


//--------------------------------------------------------------------
UTaskHSDGetInel::~UTaskHSDGetInel()
{ }

//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UTaskHSDGetInel::Init()
{
  // Initialisation. Get pointer to the run description and
  // event using the UManager
  fMgr = UManager::Instance();
  fRun = fMgr->GetRun();
  fEvent = fMgr->GetEvent();

  TString gen;
  fRun->GetGenerator(gen);
  if (gen.BeginsWith("HSD") == kFALSE) {
    std::cerr << "-E- UTaskHSDGetInel::Init: Invalid generator " << gen << '!' << std::endl;
    return;
  }

  std::cout << "-I- UTaskHSDGetInel::Init: Initialisation completed." << std::endl;
}

//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UTaskHSDGetInel::Exec(Option_t * option)
/* Task execution. Count events containing inelastic collisions,
   defined in HSD as having produced at least one meson.
*/
{
  Int_t nMesons = 0;

  ++fAllEvents;
  fMgr->WriteEvent(kFALSE);

  // Loop over the particles, counting mesons
  for (Int_t iPa = 0; iPa < fEvent->GetNpa(); ++iPa) {
    const UParticle *part = fEvent->GetParticle(iPa);
    if (part == NULL)
      continue;
    const Int_t abspdg = TMath::Abs(part->GetPdg());
    if ((abspdg >= 100) && (abspdg < 1000))
      ++nMesons;
  }

  if (nMesons == 0)
    return;
  fMgr->WriteEvent(kTRUE);
  ++fInelEvents;
}

//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UTaskHSDGetInel::Finish()
{
  std::cout << "-I- UTaskHSDGetInel::Finish: Found "
      << fInelEvents << " / " << fAllEvents << " inelastic events."
      << std::endl;
}

//--------------------------------------------------------------------


//--------------------------------------------------------------------
UTaskUrQMDGetInel::UTaskUrQMDGetInel() : fAllEvents(0), fInelEvents(0)
{ }

//--------------------------------------------------------------------


//--------------------------------------------------------------------
UTaskUrQMDGetInel::UTaskUrQMDGetInel(const char *name, Int_t verbose)
  : UTask(name, verbose), fAllEvents(0), fInelEvents(0)
{ }

//--------------------------------------------------------------------


//--------------------------------------------------------------------
UTaskUrQMDGetInel::~UTaskUrQMDGetInel()
{ }

//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UTaskUrQMDGetInel::Init()
{
  // Initialisation. Get pointer to the run description and
  // event using the UManager
  fMgr = UManager::Instance();
  fRun = fMgr->GetRun();
  fEvent = fMgr->GetEvent();

  TString gen;
  fRun->GetGenerator(gen);
  if (gen.BeginsWith("UrQMD") == kFALSE) {
    std::cerr << "-E- UTaskHSDGetInel::Init: Invalid generator " << gen << '!' << std::endl;
    return;
  }

  fMinParticles = fRun->GetAProj() + fRun->GetATarg();

  std::cout << "-I- UTaskUrQMDGetInel::Init: Initialisation completed." << std::endl;
}

//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UTaskUrQMDGetInel::Exec(Option_t * option)
/* Task execution. Count events containing inelastic collisions,
   defined in UrQMD as events not meeting ANY of the following:
    - having the all-collision counter at zero;
    - having the inelastic-collision counter at zero;
    - containing A_projectile + A_target particles.
*/
{
  TString comment;
  ++fAllEvents;
  fMgr->WriteEvent(kFALSE);

  fEvent->GetComment(comment);
  TObjArray *counters = comment.Tokenize(" \t");
  if (counters->GetEntriesFast() != 8) {
    std::cout << "-W-: UTaskUrQMDGetInel::Exec: Malformed \"special\" field, dropping the event." << std::endl;
    return;
  }
  const Int_t ncoll = dynamic_cast<TObjString *>(counters->At(0))->GetString().Atoi();
  const Int_t ninel = dynamic_cast<TObjString *>(counters->At(2))->GetString().Atoi();
  delete counters;
  const Int_t npart = fEvent->GetNpa();

  if ((ncoll == 0) || (ninel == 0) || (npart == fMinParticles))
    return;

  fMgr->WriteEvent(kTRUE);
  ++fInelEvents;
}

//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UTaskUrQMDGetInel::Finish()
{
  std::cout << "-I- UTaskUrQMDGetInel::Finish: Found "
            << fInelEvents << " / " << fAllEvents << " inelastic events."
            << std::endl;
}

//--------------------------------------------------------------------


ClassImp(UTaskHSDGetInel);
ClassImp(UTaskUrQMDGetInel);
