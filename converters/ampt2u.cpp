///////////////////////////////////////////////////////////////////////////////
// ampt2u reads AMPT events and input parameters from the ampt.dat and 
// input.ampt ascii files, converts them to the UniGen format,
// and saves them to a root file.
//
// ampt.dat contains all events and the freeze-out coordinates of their
// particles.
//
// input.ampt contains the input parameters of the events. 
//
// James Hostetter, July 2008
// 
// compiler warnings fixed by DM in Nov 2014
///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TBranch.h>
#include "TMath.h"
#include "UEvent.h"
#include "UParticle.h"
#include "URun.h"
using namespace std;

TFile *fi;
TTree *tr;
UEvent *ev;

/*****************************************************************************/
void bomb(const char *myst) {
  cerr<<"Error: "<<myst<<", bombing"<<endl;
  exit(-1);
}
/*****************************************************************************/
int main(int argc, char *argv[]) {
  ifstream in;
  char *inpfile;
  char *inpfile2;
  char *outfile;
  char c;
  int nevents;
  string dust;

  URun *ru = 0;
  string version, comment;
  int aproj, zproj, atarg, ztarg, nr, partNum;
  double b, bmin, bmax, sqrts;

  if (argc != 5) {
    cout << "usage:   " << argv[0] << " AMPT-Output-File AMPT-Input-File outfile nevents\n";
    cout << "example:   " << argv[0] << " ampt.dat input.ampt simdat.root 10\n";
    exit(0);
  }

  inpfile = argv[1];
  inpfile2 = argv[2];
  outfile = argv[3];
  nevents = atoi(argv[4]);

  in.open(inpfile2);
  if (in.fail()) bomb("cannot open input.ampt");
  in >> sqrts;
  in.ignore(777,'\n');
  in.ignore(777,'\n');
  in.ignore(777,'\n');
  in.ignore(777,'\n');
  in >> aproj;
  in.ignore(777,'\n');
  in >> zproj;
  in.ignore(777,'\n');
  in >> atarg;
  in.ignore(777,'\n');
  in >> ztarg;
  in.ignore(777,'\n');
  in.ignore(777,'\n');
  in >> bmin;
  in.ignore(777,'\n');
  in >> bmax;
  in.close();
  in.open(inpfile);
  if (in.fail()) bomb("cannot open ampt.dat");
  fi = TFile::Open(outfile, "RECREATE", "AMPT");
  tr = new TTree("events","AMPT tree");
  ev = new UEvent();
  tr->Branch("event", "UEvent", &ev);

  // event loop

  const int bunch = 10;

  int events_processed=0;
  for (int n=0; n<nevents; n++) {
    if ((n%bunch)==0) cout << "event "  << setw(5) << n << endl;
    string line;
    char pee;
    pee=in.peek();
    if (pee==EOF) break; 
    //Get all relevant event data from input.ampt and ampt.dat
    in >> nr >> dust >> partNum;
    in >> b; 
    in.ignore(777,'\n');
    ev->Clear();
    ev->SetEventNr(nr);
    ev->SetB(b);
    ev->SetPhi(0);
    ev->SetNes(1); 
    events_processed++;

    int step_nr=0;
    if (pee==EOF) break; 
    for (int i=0;  i<partNum; i++) {
      double t,x,y,z,e,px,py,pz,mass;
      int status, parent, parent_decay, mate, ID;
      int decay, child[2];
      in >> ID  >> px >> py >> pz >> mass;
      in >> x >> y >> z >> t;
      if (in.fail()) bomb("while reading tracks");
      status=parent=parent_decay=decay=child[0]=child[1]=mate=0;
      e = std::sqrt(px*px + py*py + pz*pz + mass*mass);
      ev->AddParticle(i+1,ID, status, parent,
	  parent_decay,mate, decay,child,
	  px, py, pz, e, x, y, z, t, 1.);
    }
    do in.get(c); while (c!='\n');
    ev->SetStepNr(step_nr++);
    ev->SetStepT(0.0);
    tr->Fill();
  }
  cout << events_processed << " events processed\n";
  in.close();

  // create the run object

  string generator = "AMPT";
  generator.append(version);
  double m = 0.938271998;
  double ecm = sqrts/2; // energy per nucleon in cm
  double pcm = sqrt(ecm*ecm-m*m); // momentum per nucleon in cm
//  double beta = pcm / ecm;
//  double gamma = 1.0/sqrt(1-beta*beta);
//  double pproj = gamma*(+pcm-beta*ecm);
  cout<<sqrts<<endl;
  cout<<pcm<<endl;
//  double ptarg = gamma*(-pcm-beta*ecm);
  cout<<(-pcm)<<endl;
  ru = new URun(generator.data(), comment.data(), 
      aproj, zproj, pcm,
      atarg, ztarg, -pcm,
      bmin, bmax, -1, 0, 0, 0, events_processed);
  ru->Write();
  fi->Write();
  fi->Close();
  return 0;
}
/*****************************************************************************/

