#ifndef UGENESISTASKDECAY_H
#define UGENESISTASKDECAY_H

#include "UTask.h"

class CGenesisDecayer;
class CGenesisEvent;
class URun;
class UEvent;

class UTaskGenesisDecay : public UTask {
 private:
  URun                *fRun;       //! Run description
  UEvent              *fEvent;     //! Event description
  CGenesisDecayer     *fDecayer;   //! Pointer to the Genesis decayer
  CGenesisEvent       *fGEvent;    //! Pointer to the Genesis event

 public:
  UTaskGenesisDecay(CGenesisEvent*);
  virtual ~UTaskGenesisDecay();
  
  virtual void Init();
  virtual void Exec(Option_t *option = "");
  virtual void Finish();
  
  ClassDef(UTaskGenesisDecay, 1);
};

#endif

