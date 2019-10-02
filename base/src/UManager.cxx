#include <iostream>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TList.h"

#include "UTask.h"
#include "URun.h"
#include "UEvent.h"
#include "UManager.h"


//____________________________________________________________________
//
// UManager
// 
// This class is the singleton manager for organising I/O and task
// execution.
// SetInputFile and SetOutputFile are used to set the input and
// output files.
// User tasks, which are derived from the UTask class, with
// implemented Init and Exec functions, are to be added using
// AddTask function.
// Init and Run functions make initialisation and execution
//


UManager *UManager::fgInstance = NULL;


//--------------------------------------------------------------------
UManager::UManager()
{
  // Default constructor
  if(NULL != fgInstance) {
    Fatal("UManager", "Singleton class instantiated twice!");
  }
  fgInstance = this;
  fInFile = NULL;
  fOutFile = NULL;
  fInTree = NULL;
  fOutTree = NULL;
  fEvent = NULL;
  fRun = NULL;
  fTaskList = new TList();
  fWrite = kTRUE;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
UManager::~UManager()
{
  // Destructor
  fgInstance = NULL;
  CloseInputFile();
  CloseOutputFile();
  fInTree = NULL;
  fOutTree = NULL;
  fEvent = NULL;
  fRun = NULL;
  delete fTaskList;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
UManager* UManager::Instance()
{
  // Get instance of singleton manager
  return fgInstance;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UManager::SetInputFile(const char *name)
{
  // Open the input file and get the run description and tree of events,
  // which later can be accessed by GetRun and GetEvent respectively.
  // name - name of the input file
  fInFile = TFile::Open(name);
  if(NULL == fInFile) {
    Fatal("SetInputFile", "Input file does not exist!");
  }

  fRun = (URun*) fInFile->Get("run");

  fInTree = (TTree*) fInFile->Get("events");
  if(NULL == fInTree) {
    Fatal("SetInputFile", "Input file has no events tree!");
  }
  cout << "-I- UManager : Input file has " << fInTree->GetEntries() << " events"
       << endl;

  if(NULL == fEvent) {
      fEvent = new UEvent();
  }
  fInTree->SetBranchAddress("event", &fEvent);
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UManager::CloseInputFile()
{
  // Close the input file
  if(NULL != fInFile) {
    fInFile->Close();
    fInFile = NULL;
  }
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UManager::SetOutputFile(const char *name, Bool_t writeTree)
{
  // Open the output file. Create the output tree with event branch.
  // name - name of the output file
  fOutFile = TFile::Open(name, "RECREATE");
  if(NULL == fOutFile) {
    Fatal("SetOutputFile", "Output file can not be created!");
  }
  if(writeTree) {
    fOutTree = new TTree("events", "Event data");
    if(NULL == fEvent) {
      fEvent = new UEvent();
    }
    fOutTree->Branch("event", &fEvent);
  }
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UManager::CloseOutputFile()
{
  // Close the output file
  if(NULL != fOutFile) {
    fOutFile->Close();
    fOutFile = NULL;
  }
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
Int_t UManager::GetEntries()
{
  // Get the number of entries in input tree
  if(NULL == fInTree) return 0;
  return fInTree->GetEntries();
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UManager::GetEntry(Int_t i)
{
  // Get entry from the input tree.
  // i - index of entry
  if(NULL == fInTree) return;
  fInTree->GetEntry(i);
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UManager::Fill()
{
  // Fill the output tree
  if(NULL != fOutTree) {
    if(fWrite) {
      fOutTree->Fill();
    } else {
      fWrite = kTRUE;
    }
  }
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UManager::AddTask(UTask *task)
{
  // Add task to the task list.
  // task - pointer to the task
  fTaskList->Add(task);
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UManager::CreateRun(const char* generator, const char* comment, Int_t aProj,
			 Int_t zProj, Double_t pProj, Int_t aTarg, Int_t zTarg,
			 Double_t pTarg, Double_t bMin, Double_t bMax, Int_t bWeight,
			 Double_t phiMin, Double_t phiMax, Double_t sigma, Int_t nEvents)
{
  // Creates the run description, which later can be accessed by GetRun method
  fRun = new URun(generator, comment,
		  aProj, zProj, pProj, aTarg, zTarg, pTarg,
		  bMin, bMax, bWeight, phiMin, phiMax, sigma, nEvents);
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UManager::Init()
{
  // Run initialisation of the tasks
  UTask *task;
  for(Int_t i = 0; i < fTaskList->GetSize(); i++) {
    task = (UTask*) fTaskList->At(i);
    task->Init();
  }
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UManager::Run(Int_t a, Int_t b)
{
  // Execute the tasks.
  // Run()    - process all events from input tree.
  // Run(i)   - process only i-th event
  // Run(i,j) - process events from i-th to j-th
  if(NULL==fInTree && NULL==fOutTree) return;

  UTask *task;
  if(NULL==fInTree) {

    for(Int_t i = 0; i < a; i++) {
      for(Int_t i = 0; i < fTaskList->GetSize(); i++) {
	task = (UTask*) fTaskList->At(i);
	task->Exec();
      }
      Fill();
    }

  } else {

    if(-1==a && -1==b) {
      for(Int_t iEv = 0; iEv < GetEntries(); iEv++) {
	GetEntry(iEv);
	for(Int_t i = 0; i < fTaskList->GetSize(); i++) {
	  task = (UTask*) fTaskList->At(i);
	  task->Exec();
	}
	Fill();
      }
    } else if(a>-1 && -1==b) {
      GetEntry(a);
      for(Int_t i = 0; i < fTaskList->GetSize(); i++) {
	task = (UTask*) fTaskList->At(i);
	task->Exec();
      }
      Fill();
    } else {
      for(Int_t iEv = a; iEv < b; iEv++) {
	GetEntry(iEv);
	for(Int_t i = 0; i < fTaskList->GetSize(); i++) {
	  task = (UTask*) fTaskList->At(i);
	  task->Exec();
	}
	Fill();
      }
    }

  }

  for(Int_t i = 0; i < fTaskList->GetSize(); i++) {
    task = (UTask*) fTaskList->At(i);
    task->Finish();
  }

  if(NULL != fOutFile) {
    if(NULL != fOutTree) {
      fOutFile->cd();
      fOutTree->Write();
    }
    if(NULL != fRun) {
      fOutFile->cd();
      fRun->Write();
    }
  }
}
//--------------------------------------------------------------------



ClassImp(UManager);


