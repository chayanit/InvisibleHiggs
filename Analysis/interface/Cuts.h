#ifndef Cuts_h
#define Cuts_h

#include "TCut.h"


class Cuts {
 public:

  Cuts();
  ~Cuts();

  void addCut(std::string name, std::string cut);

  TCut cut(unsigned i);

  TCut allCuts();

  TCut nMinusOneCuts(unsigned i);

  // special combinations
  TCut zMuMu();
  TCut zMuMuLoDPhi();
  TCut zMuMuHiDPhi();

  TCut wMu();
  TCut wMuLoDPhi();
  TCut wMuHiDPhi();

  TCut wEl();
  TCut wElLoDPhi();
  TCut wElHiDPhi();

  // QCD control regions
  TCut qcdLoose();
  TCut qcdLooseHiDPhi();
  TCut qcdLooseLoDPhi();
  TCut qcdTightHiDPhi();

 private:
  
  std::vector<std::string> names_;
  std::vector<TCut> cuts_;

};

#endif
