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

