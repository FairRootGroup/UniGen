// Author A.Bercuci [A.Bercuci@gsi.de]
// Data   17.June.2006
// Version 1.0
//
// Simple format convertor from Genesis to UniGen data
// Usage : genesis2u genesis.root [ugf.root] [nevents]
//
// if the output file is not specified it will be named using 
// the name of the genesis.root file to which "ugf_" will be 
// prepended
//
#include <stdio.h>
#include <stdlib.h>

#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TParticle.h>

#include <UEvent.h>
#include <URun.h>
#include <UParticle.h>

#include <CGenesisEvent.h>
#include <CGenesisRun.h>

int main(int argc, const char **argv)
{
	if(argc < 2 || argc > 4){
		printf("Usage : genesis2u genesis.root [ugf.root] [nevents]\n");
		return 1;
	}
	TString file=argv[1];
	if(!file.EndsWith(".root")){
		printf("Usage : genesis2u genesis.root [ugf.root] [nevents]\n");
		return 1;	
	}
	TFile *fGenesis = TFile::Open(file.Data());
	TTree *tGenesis=(TTree*)fGenesis->Get("tData");
	if(!tGenesis){
		printf("** ERROR ** TTree \"tGenesis\" not found on file.\n");
		return 1;		
	}	
	if(!tGenesis->FindBranch("event")){
		printf("** ERROR ** TBranch \"event\" not found on tree.\n");
		return 1;			
	}
	CGenesisRun *rGenesis=(CGenesisRun*)fGenesis->Get("GenesisRun");
	if(!rGenesis){
		printf("** ERROR ** CGenesisRun object not found on file.\n");
		return 1;		
	}
	CGenesisEvent *eGenesis=new CGenesisEvent();
	tGenesis->SetBranchAddress("event",&eGenesis);
	TParticle *pGenesis;
	
	
// build Genesis hierachy
	if(argc >= 3) file=argv[2];
	else file.Prepend("ugf_");
	TFile *fUGF = TFile::Open(file.Data(), "RECREATE");
	Double_t bMin=0., bMax=0.;
	Int_t bWeight=0;
	Double_t phiMin=0., phiMax=0., sigma=0.;
	URun *rUGF=new URun("Genesis",rGenesis->GetTitle(),
	                    rGenesis->GetAProj(),
	                    rGenesis->GetZProj(),
	                    rGenesis->GetPProj(),
	                    rGenesis->GetATarg(),
	                    rGenesis->GetZTarg(),
	                    rGenesis->GetPTarg(),
	                    bMin, bMax, bWeight,
	                    phiMin,phiMax,sigma,
	                    tGenesis->GetEntries());
	UEvent *eUGF=new UEvent();
	TTree *tUGF=new TTree("events","UGF converted from Genesis");
	tUGF->Branch("event",&eUGF);
	UParticle *pUGF;
	
	int nevents = tGenesis->GetEntries();
	if((argc == 4) && (tGenesis->GetEntries() > atoi(argv[3]))) nevents=atoi(argv[3]);

//start conversion
	TLorentzVector mom, pos;
	Int_t child[2];
	printf("Converting "); fflush(stdout);
	for(int ipart=0; ipart<nevents; ipart++){
		tGenesis->GetEntry(ipart);
		printf("."); fflush(stdout);
		eUGF->Clear();
		for(int ip=0; ip<eGenesis->GetNpa(); ip++){ 
			pGenesis = eGenesis->GetParticle(ip);
			pGenesis->Momentum(mom);
			pGenesis->ProductionVertex(pos);
			child[0] = pGenesis->GetDaughter(0);
			child[1] = pGenesis->GetDaughter(1);
			eUGF->AddParticle(
				ip,                     // index
				pGenesis->GetPdgCode(), // pdg
				pGenesis->GetStatusCode(),  // status    
				pGenesis->GetMother(0), // parent
				0,                      // parentDecay
				0,                      // mate
				0,                      // decay  
				child,
				mom,
				pos,
				0.);                    // weight
		}
		tUGF->Fill();
	}
	printf("Done.\n");
	
// save UG file
	fUGF->cd();
	rUGF->Write();
	tUGF->Write();
	fUGF->Close();

// close Genesis file
	fGenesis->cd();
	fGenesis->Close();
}


