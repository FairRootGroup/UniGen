#ifndef UGENESISTASK_H
#define UGENESISTASK_H

#ifndef UTASK
#include "UTask.h"
#endif

class URun;
class UEvent;
class CGenesisEvent;
class CGenesisRun;


class UTaskGenesis : public UTask {

 private:
  URun          *fRun;     // Run description in UGF representation
  UEvent        *fEvent;   // Event description in UGF representation
  CGenesisRun   *fGRun;    // Genesis run description
  CGenesisEvent *fGEvent;  // Genesis event description
  
 public:
  UTaskGenesis();
  UTaskGenesis(const char *name, Int_t verbose = 1);
  virtual ~UTaskGenesis();
  
  virtual void Init();
  virtual void Exec(Option_t *option = "");
  virtual void Finish();

  inline CGenesisEvent* GetGenesisEvent() {return fGEvent;}
  
  ClassDef(UTaskGenesis, 1);
};

#endif

