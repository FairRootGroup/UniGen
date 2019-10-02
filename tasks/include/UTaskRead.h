#ifndef UTASKREAD
#define UTASKREAD

#include "UTask.h"

class URun;
class UEvent;

class UTaskRead : public UTask {

 private:
  URun *fRun;       // Run description
  UEvent *fEvent;   // Event

 public:
  UTaskRead();
  UTaskRead(const char *name,
	Int_t verbose = 1);
  virtual ~UTaskRead();

  virtual void Init();

  virtual void Exec(Option_t *option = "");

  virtual void Finish();

  ClassDef(UTaskRead, 1);
};


#endif

