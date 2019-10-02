#include <iostream>
using namespace std;

#include "TH2F.h"

#include "UManager.h"
#include "URun.h"
#include "UEvent.h"
#include "UParticle.h"
#include "UTaskKin.h"


//____________________________________________________________________
// 
// UTaskKin
// 
// Task for ploting the kinematics of the particle:
// 1. momentum vs. polar angle
//


// -------------------------------------------------------------------
UTaskKin::UTaskKin()
{
  // Default constructor
  fEvents = 0;
  fh_mom_theta_pip = NULL;
  fh_mom_theta_kap = NULL;
}
// -------------------------------------------------------------------


// -------------------------------------------------------------------
UTaskKin::UTaskKin(const char *name, Int_t verbose)
  :UTask(name, verbose)
{
  // Standard constructor
  fEvents = 0;
  fh_mom_theta_pip = NULL;
  fh_mom_theta_kap = NULL;
}
// -------------------------------------------------------------------


// -------------------------------------------------------------------
UTaskKin::~UTaskKin()
{
  // Destructor
}
// -------------------------------------------------------------------


// -------------------------------------------------------------------
void UTaskKin::Init()
{
  // Initialisation of the task
  UManager *mgr = UManager::Instance();
  if(! mgr) {
    cout << "-E- UTaskKin::Init: "
	 << "manager is not instantiated!" << endl;
    return;
  }
  fRun = mgr->GetRun();
  if(! fRun) {
    cout << "-W- UTaskKin::Init: "
	 << "no run description" << endl;
  }
  fEvent = mgr->GetEvent();
  if(! fEvent) {
    cout << "-W- UTaskKin::Init: "
	 << "no pointer to the event" << endl;
  }

  // Parameters for boost
  Double_t pBeam = Double_t(fRun->GetAProj())*fRun->GetPProj();
  Double_t eBeam = fRun->GetProjectileEnergy();
  fBeamRapidity = 0.5*TMath::Log((eBeam+pBeam)/(eBeam-pBeam));
  cout << "-I- UTaskKin::Init: "
       << "beam rapidity: " << fBeamRapidity << endl;

  fh_mom_theta_pip = new TH2F("h_mom_theta_pip", "",
			      100, 0., 10.,
			      45, 0., 45.);
  fh_mom_theta_kap = new TH2F("h_mom_theta_kap", "",
			      100, 0., 10.,
			      45, 0., 45.);
}
// -------------------------------------------------------------------


// -------------------------------------------------------------------
void UTaskKin::Exec(Option_t *option)
{
  // Task execution
  if(!fRun || !fEvent) return;

  // Declare variables outside the loop
  UParticle *part;
  Int_t pdgCode;
  Double_t pt;
  Double_t y;
  Double_t pz;
  Double_t mom;
  Double_t theta;

  // Loop over the particles
  for(Int_t iPa = 0; iPa < fEvent->GetNpa(); iPa++) {
    // Get pointer to the particle
    part = fEvent->GetParticle(iPa);
    if(NULL == part) continue;
    // Take only pi+ and K+
    pdgCode = part->GetPdg();
    if(211!=pdgCode && 321!=pdgCode) continue;
    // Get momentum and theta
    pt = part->GetMomentum().Pt();
    y = fBeamRapidity + part->GetMomentum().Rapidity();
    pz = part->GetMomentum().Mt()*TMath::SinH(y);
    mom = TMath::Sqrt(pt*pt + pz*pz);
    theta = 180./TMath::Pi()*TMath::ATan2(pt, pz);
    // Fill histogramm
    if(211 == pdgCode) {
      fh_mom_theta_pip->Fill(mom, theta);
    } else if(321 == pdgCode) {
      fh_mom_theta_kap->Fill(mom, theta);
    }
  }

  if(0 == (fEvents%10)) {
    cout << "-I- UTaskKin::Exec: "
	 << "event " << fEvents << " completed" << endl;
  }
  fEvents += 1;
}
// -------------------------------------------------------------------


// -------------------------------------------------------------------
void UTaskKin::Finish()
{
  if(fh_mom_theta_pip) fh_mom_theta_pip->Write();
  if(fh_mom_theta_kap) fh_mom_theta_kap->Write();
}
// -------------------------------------------------------------------


ClassImp(UTaskKin);


