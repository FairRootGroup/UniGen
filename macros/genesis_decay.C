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

// Example function for steering the decay with UManager. The
// CGenesisDecayer is wrapped by UGenesisTaskDecay to accomplish this
// functionality.
void task(const char *filename)
{
  // Load library
  gSystem->Load("libUniGen");
  gSystem->Load("libGenesis");
  gSystem->Load("libTasksGenesis");

  // Create the Manager
  UManager *mgr = new UManager();

  // Open the file
  mgr->SetInputFile(filename);
  mgr->SetOutputFile("umanager.root");

  // Conversion from UniGen to Genesis
  UGenesisTask *ug2gen = new UGenesisTask("ug2gen", 1);
  mgr->AddTask(ug2gen);

  // Genesis decayer, back conversion
  UGenesisTaskDecay *genesisDecayer =
    new UGenesisTaskDecay(ug2gen->GetGenesisEvent());
  mgr->AddTask(genesisDecayer);

  mgr->Init();

  mgr->Run();

  mgr->CloseInputFile();
  mgr->CloseOutputFile();
}
