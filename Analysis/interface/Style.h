
#include "TROOT.h"
#include "TStyle.h"


Style() {

  TStyle *default = new TStyle("Default","Default Style");
  style->SetOptStat(0);
  gROOT->SetStyle(style);  

}
