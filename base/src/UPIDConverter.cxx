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

#include <UPIDConverter.h>

#include <TROOT.h>
#include <TSystem.h>

#include <fstream>
#include <string>

ClassImp(UPIDConverter);

UPIDConverter* UPIDConverter::fgInstance = NULL;


UPIDConverter::UPIDConverter() :
    TNamed("PIDConverter", "Any-to-PDG particle-ID converter")
{
  if (fgInstance) {
    Warning("UPIDConverter", "object already instantiated");
  } else {
    fgInstance = this;
    gROOT->GetListOfSpecials()->Add(this);
  }
}

Int_t UPIDConverter::GetPDGCode(const Int_t pid, const EConvention pidType)
{
  /* Trivial case first */
  if (pidType == ePDG)
    return pid;

  ConversionTableMap::const_iterator mIt = fConversionTables.find(pidType);

  /* Either pidType has been set to an unknown value or the conversion table
     for specified convention hasn't been loaded yet. Assume the latter and
     try to do so now; if we still cannot find the table afterwards then
     something is indeed wrong. */
  if (mIt == fConversionTables.end()) {
    LoadConversionTable(pidType);
    mIt = fConversionTables.find(pidType);
    if (mIt == fConversionTables.end()) {
      Error("GetPDGCode",
            "PID conversion table not available for convention=%u",
            pidType);
      return 0;
    }
  }

  const std::map<Int_t, Int_t>& conversionTable = mIt->second;
  std::map<Int_t, Int_t>::const_iterator pidIt = conversionTable.find(pid);
  if (pidIt == conversionTable.end()) {
    Warning("GetPDGCode", "PDG code not found for convention=%u, pid=%d",
            pidType, pid);
    return 0;
  }

  return pidIt->second;
}


UPIDConverter* UPIDConverter::Instance()
{
  return (fgInstance) ? fgInstance : new UPIDConverter();
}


void UPIDConverter::LoadConversionTable(const EConvention pidType)
{
  std::string dataFileName = "/input/";
  switch (pidType) {
    case (ePluto):
      dataFileName += "pluto_pdg.dat";
      break;
    case (eUrQMD):
      dataFileName += "urqmd_pdg.dat";
      break;
    case (eUrQMDv4):
      dataFileName += "urqmdv4_pdg.dat";
      break;
    case (eWerner):
      // VENUS, NEXUS, EPOS
      dataFileName += "werner_pdg.dat";
      break;
    default:
      Error("LoadConversionTable",
            "I do not know where to find conversion table for convention=%u",
            pidType);
      return;
  }

  std::string unigenBase = gSystem->Getenv("UNIGEN");
  unigenBase += dataFileName;
  const char* fullFileName = unigenBase.c_str();
  std::ifstream fin;
  fin.open(fullFileName);
  if (! fin.good()) {
    Error("LoadConversionTable",
          "Failed to open conversion-table file %s", fullFileName);
    return;
  }

  std::map<Int_t, Int_t>& conversionTable = fConversionTables[pidType];
  Int_t localPid;
  Int_t pdgPid;
  // The file format for UrQMD 4.0 has changed, so we need to handle the
  // input differently
  if (pidType == eUrQMDv4) {
    while (1) {
      int iso3;
      std::string name;
      // FIXME: we might want to make this more robust against malformed input
      if (fin.eof())
        break;
      fin >> localPid >> iso3 >> pdgPid >> name;
      int id;
      // calculate out of the urqmd particle and the isospin value a unique
      // value whcih is then used to map urqmd ids to pdg ids.
      // The same conversion is used when accessing the infor from the map.
      if (localPid >= 0) {
        id = 1000 * (iso3 + 3) + localPid;
      }
      else {
        id = -1000 * (iso3 + 3) + localPid;
      }
      conversionTable[id] = pdgPid;
    }
  }
  else {
    while (1) {
      // FIXME: we might want to make this more robust against malformed input
      if (fin.eof())
        break;
      fin >> localPid >> pdgPid;
      conversionTable[localPid] = pdgPid;
    }
  }
  fin.close();

  return;
}
