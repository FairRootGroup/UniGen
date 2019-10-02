#ifndef _UTASKFILTER_
#define _UTASKFILTER_

#include <list>

#include "UTask.h"

class UEvent;


class UTaskFilter : public UTask {

 private:
  UEvent *fEvent;                   // Event
  Int_t   fEvents;                  // Number of processed events
  std::list<Int_t> fPdgList;        // List of PDG codes
  Int_t   fMode;                    // 0 - remove particles, 1 - copy particles

 public:
  UTaskFilter();                 
  UTaskFilter(const char *name,
	      Int_t verbose = 1);
  virtual ~UTaskFilter();        

  virtual void Init();           

  virtual void Exec(Option_t *option = ""); 

  virtual void Finish();

  inline void AddParticle(Int_t pdgCode) { fPdgList.push_back(pdgCode); }
  inline void SetFilterMode(Int_t mode)  { fMode = mode;                }

  ClassDef(UTaskFilter, 1);
}; 


#endif

