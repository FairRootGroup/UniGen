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

#ifndef UPIDCONVERTER_H
#define UPIDCONVERTER_H

#include <map>

#include <TNamed.h>


class UPIDConverter : public TNamed {

 public:
  typedef enum {
    ePDG = -1,
    ePluto,
    eUrQMD,
    eUrQMDv4,
    eWerner
  } EConvention;

  UPIDConverter();

  Int_t GetPDGCode(const Int_t pid, const EConvention pidType);

  static UPIDConverter* Instance();


 private:
  // We should use 'EConvention' as the map key type, however this confused rootcint.
  // For the time being use UInt_t instead, hopefully fix this later (FIXME).
  typedef std::map<UInt_t, std::map<Int_t, Int_t> > ConversionTableMap;

  void LoadConversionTable(const EConvention pidType);

  ConversionTableMap fConversionTables;
  static UPIDConverter* fgInstance;

  ClassDef(UPIDConverter, 0);


};


#endif
