// ------------------------------------------------------------------
// -----                         UTaskLab2Cm                    -----
// -----              Created 2010/02/01 by D. Kresan           -----
// ------------------------------------------------------------------
#include <iostream>
using namespace std;

#include "TMath.h"

#include "UManager.h"
#include "URun.h"
#include "UEvent.h"
#include "UParticle.h"
#include "UTaskLab2Cm.h"


//___________________________________________________________________
//
// UTaskLab2Cm
//
// Task for converting the particles momenta from LAB to CM frame
//


// ------------------------------------------------------------------
UTaskLab2Cm::UTaskLab2Cm()
{
    // Default constructor
    fEvent = NULL;
}
// ------------------------------------------------------------------


// ------------------------------------------------------------------
UTaskLab2Cm::UTaskLab2Cm(const char *name, Int_t verbose)
:UTask(name, verbose)
{
    // Standard constructor
    fEvent = NULL;
}
// ------------------------------------------------------------------


// ------------------------------------------------------------------
UTaskLab2Cm::~UTaskLab2Cm()
{
    // Destructor
}
// ------------------------------------------------------------------


// ------------------------------------------------------------------
void UTaskLab2Cm::Init()
{
    // Task initialisation
    UManager *mgr = UManager::Instance();
    if(NULL == mgr) {
	cout << "-E- UTaskLab2Cm::Init: "
	    << "UManager is not instantiated!" << endl;
        return;
    }
    // Get Run description
    URun *run = mgr->GetRun();
    fCM = kFALSE;
    if(TMath::Abs(run->GetPTarg()) > 0) fCM = kTRUE;
    fBetaCM = 0.;
    if(fCM) {
        cout << "-I- UTaskLab2Cm::Init: we are in CM frame" << endl;
    } else {
	cout << "-I- UTaskLab2Cm::Init: we are in LAB frame" << endl;
	Double_t plab = run->GetPProj();
	Double_t elab = TMath::Sqrt(plab*plab + TMath::Power(0.938271998,2));
	Double_t ecm = TMath::Sqrt(TMath::Power(0.938271998,2)+
				   TMath::Power(0.938271998,2)+
				   2.*elab*0.938271998);
        cout << "-I- UTaskLab2Cm::Init: Ecm = " << ecm << " GeV" << endl;
        fBetaCM = plab / (elab + 0.938271998);
	run->SetPProj( 0.5*ecm);
        run->SetPTarg(-0.5*ecm);
    }
    fGammaCM = 1./TMath::Sqrt(1. - fBetaCM*fBetaCM);
    cout << "-I- UTaskLab2Cm::Init: boost parameters:" << endl
	<< "                             betaCM = " << fBetaCM
	<< ",  gammaCM = " << fGammaCM << endl;
    // Get event
    fEvent = mgr->GetEvent();
}
// ------------------------------------------------------------------


// ------------------------------------------------------------------
void UTaskLab2Cm::Exec(Option_t *option)
{
    // Task execution
    if(NULL == fEvent) return;

    // if we are in LAB - no action
    if(fCM) return;

    // Control output
    if(0 == (fEvents%1000)) {
	cout << "-I- UTaskLab2Cm::Exec: event " << fEvents
            << " has " << fEvent->GetNpa() << " tracks" << endl;
    }

    UParticle *particle;
    Double_t pz;
    Double_t e;
    Double_t pz1;
    Double_t ee;

    // Loop over particles
    for(Int_t i = 0; i < fEvent->GetNpa(); i++) {
	// Get particle
	particle = fEvent->GetParticle(i);
	if(NULL == particle) continue;
	// Make boost
	pz = particle->Pz();
        e = particle->E();
	pz1 = fGammaCM*(pz - fBetaCM*e);
	ee = TMath::Sqrt(e*e - pz*pz + pz1*pz1);
	// Set new momentum and energy
	particle->SetPz(pz1);
        particle->SetE(ee);
    }

    fEvents += 1;
}
// ------------------------------------------------------------------


// ------------------------------------------------------------------
void UTaskLab2Cm::Finish()
{
    // Finish of the task execution
    cout << "-I- UTaskLab2Cm::Finish: conversion completed" << endl;
}
// ------------------------------------------------------------------


ClassImp(UTaskLab2Cm);



