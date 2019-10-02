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

