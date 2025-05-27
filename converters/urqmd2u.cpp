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
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <string>

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
int converturqmd4(int ityp, int iso3)
{
  // translate UrQMD pid code to pdg code

  /* UrQMD PIDs are in fact composite - a particle is fully defined by the
     type specifier (ityp), the charge (ichg) and in case of baryons, the
     third component isospin (iso3). For simplicity, our conversion tables
     collapse these into a single number
     as follows:
      - propagate the sign of ityp (particle-antiparticle distinction for
        baryons, strangeness-antistrangeness distinction for mesons) to that
        of the stored value;
      - shift the iso3 range from -3..3 to 0..6 to make sure it doesn't
        interfere with the above;
      - multiply shifted iso3 value by +/-1000 and add it to type.
        The latter is guaranteed to be smaller than 1000. The largest used
        value is 222. That way no ambiguities
        occur. */
  int id;
  if (ityp >= 0)
    id = 1000 * (iso3 + 3) + ityp;
  else
    id = -1000 * (iso3 + 3) + ityp;

  /* This will return 0 for unknown input values. */
  return UPIDConverter::Instance()->GetPDGCode(id, UPIDConverter::eUrQMDv4);
}
/*****************************************************************************/
int main(int argc, char *argv[]) {

  ifstream in;
  char *inpfile;
  char *outfile;
  char c;
  int nevents;
  string dust;
  UPIDConverter::EConvention urqmdtype;

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

    in >> dust >> dust >> version >> dust >> dust >> dust >> filetype;

    //extract major and minor versions
    int int_version = atoi(version.c_str());
    int major_version = int_version/10000;
    int minor_version = (int_version%10000)/100;
    // UrQMD 4.0 wrote to output in the begining a wrong version info
    // All version info from 3.5 onward are used as UrQMD 4.0
    if (major_version >=3 && minor_version >=5 ) {
      major_version = 4;
      minor_version = 0;
    }

    in  >> dust >> dust >> dust >> aproj >> zproj >> dust >> dust >> dust >> atarg >> ztarg;
    in >> dust >> dust >> dust >> beta >> dust >> dust;
    in >> dust >> b >> bmin >> bmax >> dust >> sigma;
    if (major_version == 4) {
      in >> dust >> dust >> dust >> dust >> dust >> dust >> dust >> dust >> dust >> dust >> dust >> dust;
    }
    in >> dust >> eos >> dust >> elab >> dust >> sqrts >> dust >> plab;
    in >> dust >> nr >> dust >> dust >> dust;
    in >> dust >> dust >> time >> dust >> dtime;
    in.ignore(777,'\n'); // ignore the rest of the line

    // After we know the version we can already load the proper conversion tables
    if (major_version == 3) {
      urqmdtype = UPIDConverter::eUrQMD;
    } 
    else if (major_version == 4) {
      urqmdtype = UPIDConverter::eUrQMDv4;
    }
    else {
      cerr<<"Error: UrQmd version "<< version << "not known"<<endl;
      exit(-1);
    }    

    
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
	status=parent_decay=decay=child[0]=child[1]=0;
        parent=-1;
        int pdg_id{0};
        if (urqmdtype == UPIDConverter::eUrQMD) {
          pdg_id=trapco(ityp, ichg);
        }
        else if ( urqmdtype == UPIDConverter::eUrQMDv4 ) {
          pdg_id=converturqmd4(ityp, iso3);
        }
	ev->AddParticle(i, pdg_id, status, parent,
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
  std::cout << "pproj: " << pproj << "\n";
  std::cout << "beta: " << beta << "\n";
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
