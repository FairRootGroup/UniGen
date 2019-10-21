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

{
  // Load library
  gSystem->Load("libUniGen");
  gSystem->Load("libTasks");

  // Create manager
  UManager *mgr = new UManager();

  // Open output file
  char* unigen = getenv("UNIGEN");
  TString fileName = (TString(unigen)+"/data/test.root");
  mgr->SetOutputFile(fileName.Data());

  // Simple Generator
  UTaskWrite *task = new UTaskWrite("write", 1);
  mgr->AddTask(task);

  mgr->Init();

  mgr->Run(4000);
}
