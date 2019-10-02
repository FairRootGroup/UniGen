#ifndef UTASK
#define UTASK

#include "TTask.h"


class UTask : public TTask {

 protected:
  Int_t fVerbose;       // Verbosity level

 public:
  UTask();                  
  UTask(const char *name,
	Int_t verbose = 1);  
  virtual ~UTask();          

  virtual void Init() = 0;                    
  virtual void Exec(Option_t *option = "") = 0; 
  virtual void Finish() = 0;                 

  inline void SetVerbose(Int_t verbose) {fVerbose = verbose;}

  ClassDef(UTask, 1);
};


#endif
