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
