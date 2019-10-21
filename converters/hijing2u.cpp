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

///////////////////////////////////////////////////////////////////////////////
// Hijing2u reads HIJING events and input parameters from the Hijing output
// files, converts them to the UniGen format,
// and saves them to a root file.
//
// James Hostetter & Christoph Baumann (cbaumann@ikf.uni-frankfurt.de)
// March 2010
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>

#include <TBranch.h>
#include <TFile.h>
#include <TMath.h>
#include <TString.h>
#include <TTree.h>

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
  char *outfile;
  char c;
  int nevents;
  string dust;

  URun *ru = 0;
  string version, comment;
  int aproj, zproj, atarg, ztarg, nr, partNum;
  double b, bmin, bmax, sqrts;

  if (argc != 3) {
    //cout << "usage:   " << argv[0] << " HIJING-Output-File outfile nevents\n";
    //cout << "example:   " << argv[0] << " hijing.dat simdat.root 10\n";
    cout << "usage:   " << argv[0] << " HIJING-File ROOT-Outputfile \n";
    cout << "example:   " << argv[0] << " hijing.dat simdat.root\n";
    exit(0);
  }

  inpfile = argv[1];
  outfile = argv[2];
//  nevents = atoi(argv[3]);

  in.open(inpfile);
  if (in.fail()) bomb("cannot open input file");
  fi = TFile::Open(outfile, "RECREATE", "HIJING");
  tr = new TTree("events","HIJING tree");
  ev = new UEvent();
  tr->Branch("event", "UEvent", &ev);
  
  for (int i=1;i<=52;i++)
  in.ignore(777,'\n');
  //1st Version: Energy read from hijing library output, now: SKIP LINE, for loop does it anyhow
  //  in>>dust>>dust>>sqrts;
  for (int i=1;i<=4;i++)
  in.ignore(777,'\n');
  
  // event loop
  cout << "get global information: " << endl;
  in >> nevents >> dust >> dust >> aproj >> atarg >> zproj >> ztarg >> dust >> bmin >> bmax;
  cout << "Nevents: " << nevents << " A_P: " << aproj << " A_T: " << atarg << " Z_P: " << zproj << " Z_T: " << ztarg << endl;

  //'dust' here holds the Reaction FRAME (CMS/LAB), might be put in proper variable in future
  in >> sqrts >> dust;
  cout<< sqrts<< endl;
  in.ignore(777,'\n');
  const int bunch = 10;
  int events_processed=0;
  for (int n=0; n<nevents; n++) {
    if ((n%bunch)==0) cout << "event "  << setw(5) << n << endl;
    string line;
    char pee;
    pee=in.peek();
    if (pee==EOF) break; 
    //Get all relevant event data from hijing.dat
    in >>dust>> nr >> partNum >> b;

    ev->Clear();
	ev->SetB(b);
    ev->SetEventNr(nr);
    ev->SetPhi(0);
    ev->SetNes(1);
    events_processed++;

    int step_nr=0;
    for (int i=0;  i<partNum; i++) {
        double t,x,y,z,e,px,py,pz,mass;
        double weight = 1.0;
        int ichg,status, parent, parent_decay, mate,ID;
        int decay, child[2];
        in >> ID >> ichg >> mass >> px >> py >> pz;
        if (in.fail()) bomb("while reading tracks");
        status=parent=parent_decay=decay=child[0]=child[1]=mate=0;
        x=y=z=t=0.0;
	e=sqrt(px*px+py*py+pz*pz+mass*mass);
        ev->AddParticle(i+1,ID, status, ichg,
                        parent_decay,mate-1, decay,child,
                        px, py, pz, e, x, y, z, t, weight);
      }
      do in.get(c); while (c!='\n');
      ev->SetStepNr(step_nr++);
      ev->SetStepT(0.0);
      /*nout += */tr->Fill();
  }
  cout << events_processed << " events processed\n";
  in.close();

  // create the run object

  string generator = "HIJING";
  generator.append(version);
  double m = 0.938271998;
  double ecm = sqrts/2; // energy per nucleon in cm
  double pcm = sqrt(ecm*ecm-m*m); // momentum per nucleon in cm
  double beta = pcm / ecm;
  double gamma = 1.0/sqrt(1-beta*beta); 
  double pproj = gamma*(+pcm-beta*ecm);
  double ptarg = gamma*(-pcm-beta*ecm);
  ru = new URun(generator.data(), comment.data(), 
      aproj, zproj, pproj, 
      atarg, ztarg, ptarg, 
      bmin, bmax, -1, 0, 0, 0, events_processed);
  ru->Write();
  fi->Write();
  fi->Close();
  return 0;
}
/*****************************************************************************/
