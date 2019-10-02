// ------------------------------------------------------------------
// -----                         UTaskLab2Cm                    -----
// -----              Created 2010/02/01 by D. Kresan           -----
// ------------------------------------------------------------------
#ifndef _UTASK_LAB2CM_
#define _UTASK_LAB2CM_

#include "UTask.h"

class UEvent;


class UTaskLab2Cm : public UTask {

private:
    Int_t    fEvents;  // Number of processed events
    UEvent  *fEvent;   // Input event
    Bool_t   fCM;      // CM flag
    Double_t fBetaCM;  // CM velocity
    Double_t fGammaCM; // CM gamma factor

public:
    UTaskLab2Cm();
    UTaskLab2Cm(const char *name, Int_t verbose = 1);
    virtual ~UTaskLab2Cm();

    void Init();
    void Exec(Option_t *option);
    void Finish();

    ClassDef(UTaskLab2Cm, 1);
};


#endif

