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
