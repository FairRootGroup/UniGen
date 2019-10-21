/**
* UniGen - Common Format for Event Generators in High-energy and Nuclear Physics
* Copyright (C) 2006 - 2019 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
