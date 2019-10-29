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
// urqmd2u reads UrQMD events from the ftn13 or ftn14 ascii files and
// converts them to the UniGen format and saves on a root file.
//
// ftn14 contains snapshots at given times (event steps). The event steps 
// are converted to separate events. 
//
// ftn13 contains the final snapshot and the freeze-out coordinates. 
// The freeze-out coordinates are used. The final snapshot coordinates 
// are discarded.
//
// D.Miskowiec, February 2006
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>

#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TBranch.h>
#include <TMath.h>

#include "URun.h"
#include "UEvent.h"
#include "UParticle.h"
#include "UPIDConverter.h"

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
int trapco(int ityp, int ichg)
{
  // translate UrQMD pid code to pdg code

  /* UrQMD PIDs are in fact composite - a particle is fully defined by the
     type specifier (ityp), the charge (ichg) and in case of baryons, the
     third component isospin (iso3; ignored by us at present). For
     simplicity, our conversion tables collapse these into a single number
     as follows:
      - propagate the sign of ityp (particle-antiparticle distinction for
        baryons, strangeness-antistrangeness distinction for mesons) to that
        of the stored value;
      - shift the ichg range from -2..2 (UrQMD does not support nuclear
        fragments other than protons and neutrons so all particles it
        produces fall in this range) to 0..4 to make sure it doesn't
        interfere with the above;
      - multiply shifted charge by +/-1000 and add it to type. The latter
        is guaranteed to be smaller than 1000 (baryon types are one- or
        two-digit, meson types three-digit) so that way no ambiguities
        occur. */
  int id;
  if (ityp >= 0)
    id = 1000 * (ichg + 2) + ityp;
  else
    id = -1000 * (ichg + 2) + ityp;

  /* This will return 0 for unknown input values. */
  return UPIDConverter::Instance()->GetPDGCode(id, UPIDConverter::eUrQMD);
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
  int filetype, eos, aproj, zproj, atarg, ztarg, nr;
  double beta, b, bmin, bmax, sigma, elab, plab, sqrts, time, dtime;

  if (argc != 4) {
    cout << "usage:   " << argv[0] << " inpfile outfile nevents\n";
    cout << "example: " << argv[0] << " ftn14 ftn14.root 10\n";
    exit(0);
  }

  inpfile = argv[1];
  outfile = argv[2];
  nevents = atoi(argv[3]);

  int nout=0;
  in.open(inpfile);
  if (in.fail()) bomb("cannot open input file");

  fi = TFile::Open(outfile, "RECREATE", "UrQMD");

  tr = new TTree("events","UrQMD tree");
  ev = new UEvent();
  tr->Branch("event", "UEvent", &ev);

  // event loop

  const int bunch = 10;

  int events_processed=0;
  for (int n=0; n<nevents; n++) {
    if ((n%bunch)==0) cout << "event "  << setw(5) << n << endl;
    string line;

    in >> dust >> dust >> version >> dust >> dust;
    in >> dust >> filetype >> dust >> dust >> dust >> aproj >> zproj;
    in >> dust >> dust >> dust >> atarg >> ztarg; 
    in >> dust >> dust >> dust >> beta >> dust >> dust;
    in >> dust >> b >> bmin >> bmax >> dust >> sigma;
    in >> dust >> eos >> dust >> elab >> dust >> sqrts >> dust >> plab;
    in >> dust >> nr >> dust >> dust >> dust;
    in >> dust >> dust >> time >> dust >> dtime;
    in.ignore(777,'\n'); // ignore the rest of the line

    comment.clear();
    // read 3 lines of options and 4 lines of params
    for (int i=0; i<100; i++) {
      getline(in,line);
      if(0 == line.substr(0, 4).compare("pvec"))
      {
          break;
      }
      comment.append(line);
      comment.append("\n");
    }
    //in.ignore(777,'\n'); 

    ev->Clear();
    ev->SetEventNr(nr);
    ev->SetB(b);
    ev->SetPhi(0);
    ev->SetNes((int) (time/dtime));
    events_processed++;

    int step_nr=0;
    char pee;
    while (1) { // loop over time slices
      int mult;
      double step_time;
      pee=in.peek();
      if (pee=='U') break; 
      if (pee==EOF) break; 
      in >> mult >> step_time; 
      in.ignore(777,'\n'); // ignore the rest of the line
      getline(in,line);
      ev->SetComment(line.data());
      for (int i=0;  i<mult; i++) {
	double t,x,y,z,e,px,py,pz;
	double weight = 1.0;
	int ityp, iso3, ichg, status, parent, parent_decay, mate;
	int decay, child[2];
	in >> t >> x >> y >> z;
	in >> e >> px >> py >> pz >> dust;
	in >> ityp >> iso3 >> ichg >> mate >> dust >> dust;
	if (filetype==13) { // freeze-out coordinates
	  in >> t >> x >> y >> z;
	  in >> e >> px >> py >> pz;
	}
	if (in.fail()) bomb("while reading tracks");
	status=parent=parent_decay=decay=child[0]=child[1]=0;
	ev->AddParticle(i, trapco(ityp, ichg), status, parent,
			parent_decay, mate-1, decay, child,
			px, py, pz, e, x, y, z, t, weight);
      }
      do in.get(c); while (c!='\n');
      ev->SetStepNr(step_nr++);
      ev->SetStepT(step_time);
      nout += tr->Fill();
    }
    if (pee==EOF) break; 
  }
  in.close();
  cout << events_processed << " events processed\n";

  // create the run object

  string generator = "UrQMD";
  generator.append(version);
  double m = 0.938271998;
  double ecm = sqrts/2; // energy per nucleon in cm
  double pcm = sqrt(ecm*ecm-m*m); // momentum per nucleon in cm
  double gamma = 1.0/sqrt(1-beta*beta); 
  double pproj = gamma*(+pcm-beta*ecm);
  double ptarg = gamma*(-pcm-beta*ecm);
  ru = new URun(generator.data(), comment.data(), 
		aproj, zproj, pproj, 
		atarg, ztarg, ptarg, 
		bmin, bmax, -1, 0, 0, sigma, events_processed);
  ru->Write();
  fi->Write();
  fi->Close();
  return nout;
  return 0;
}
/*****************************************************************************/
