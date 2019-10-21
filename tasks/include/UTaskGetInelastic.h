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

#ifndef UTASKGETINELASTIC
#define UTASKGETINELASTIC

#include <UTask.h>

class UManager;
class URun;
class UEvent;


class UTaskHSDGetInel : public UTask
{
public:

	UTaskHSDGetInel();
	UTaskHSDGetInel(const char *name, Int_t verbose = 1);
	virtual ~UTaskHSDGetInel();

	virtual void Init();

	virtual void Exec(Option_t *option = "");

	virtual void Finish();


private:

	UManager *fMgr;		//! Access to the manager
	URun *fRun;		//! Run description
	UEvent *fEvent;		//! Event

	UInt_t fAllEvents;	//!
	UInt_t fInelEvents;	//!

	ClassDef(UTaskHSDGetInel, 0);


};


class UTaskUrQMDGetInel : public UTask
{
public:

	UTaskUrQMDGetInel();
	UTaskUrQMDGetInel(const char *name, Int_t verbose = 1);
	virtual ~UTaskUrQMDGetInel();

	virtual void Init();

	virtual void Exec(Option_t *option = "");

	virtual void Finish();


private:

	UManager *fMgr;		//! Access to the manager
	URun *fRun;		//! Run description
	UEvent *fEvent;		//! Event

	UInt_t fAllEvents;	//!
	UInt_t fInelEvents;	//!
	Int_t fMinParticles;	//!

	ClassDef(UTaskUrQMDGetInel, 0);


};


#endif	/* UTASKGETINELASTIC */
