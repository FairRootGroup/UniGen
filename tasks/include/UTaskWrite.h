#ifndef UTASKWRITE
#define UTASKWRITE

#include "UTask.h"

class URun;
class UEvent;


class UTaskWrite : public UTask {

 private:
  UEvent *fEvent;     // Event

  Int_t fEvents;      // Number of events
  Int_t fSteps;       // Number of steps in an event

 public:
  UTaskWrite();
  UTaskWrite(const char *name, Int_t verbose = 1);
  virtual ~UTaskWrite();

  virtual void Init();

  virtual void Exec(Option_t *option = "");

  virtual void Finish();

  ClassDef(UTaskWrite, 1);
};


#endif
