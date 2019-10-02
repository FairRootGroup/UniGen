#ifndef UMANAGER
#define UMANAGER

#include "TObject.h"

class TFile;
class TTree;
class TList;

class UTask;
class URun;
class UEvent;


class UManager : public TObject {

 private:
  static UManager *fgInstance;   // Instance of the manager
  TList  *fTaskList;             // List of tasks
  TFile  *fInFile;               // Input file
  TFile  *fOutFile;              // Output file
  TTree  *fInTree;               // Input tree
  TTree  *fOutTree;              // Output tree
  URun   *fRun;                  // Run object
  UEvent *fEvent;                // Event
  Bool_t  fWrite;                // Output flag

 public:
  UManager();           
  virtual ~UManager(); 

  static UManager* Instance(); 

  void SetInputFile(const char *fileName);  
  void CloseInputFile();                  
  void SetOutputFile(const char *fileName, Bool_t writeTree = kTRUE); 
  void CloseOutputFile();                 

  Int_t GetEntries();                    
  void GetEntry(Int_t i);                 

  void AddTask(UTask *task);               

  void Init();                            
  void Run(Int_t a = -1, Int_t b = -1);    

  void WriteEvent(Bool_t flag = kTRUE) {fWrite = flag;}

  void Fill();                             

  void CreateRun(const char* generator, const char* comment, Int_t aProj,
		 Int_t zProj, Double_t pProj, Int_t aTarg, Int_t zTarg,
		 Double_t pTarg, Double_t bMin, Double_t bMax, Int_t bWeight,
		 Double_t phiMin, Double_t phiMax, Double_t sigma, Int_t nEvents);

  inline URun* GetRun() const {return fRun;}        
  inline UEvent* GetEvent() const {return fEvent;} 

  ClassDef(UManager, 1);
};


#endif
