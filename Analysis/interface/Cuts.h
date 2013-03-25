#ifndef Cuts_h
#define Cuts_h

#include "TCut.h"


class Cuts {
 public:

  Cuts();
  ~Cuts();

  void addCut(std::string name, std::string cut);

  TCut cut(std::string name);

  TCut allCuts();

  //  TCut nMinusOneCuts(unsigned i);
  TCut nMinusOneCuts(std::string name);

  // combinations
  TCut vbf();   // note this does not include vetoes!

  TCut allCutsNoDPhi();
  TCut allCutsHiDPhi();
  TCut allCutsLoDPhi();

  // cuts for MC efficiency estimation
  TCut zMuMuGen();     // there is a generator level Z->mumu
  TCut zMuMuGenMass(); // there is a generator level Z->mumu + mass cut
  TCut zMuMuReco();    // there is a reco level Z->mumu

  TCut wMuGen();
  TCut wMuReco();

  TCut wElGen();
  TCut wElReco();

  // z->mumu control region
  TCut zMuMuVBF();
  TCut zMuMuVBFHiDPhi();
  TCut zMuMuVBFLoDPhi();

  // W->mu control region
  TCut wMuVBF();
  TCut wMuVBFHiDPhi();
  TCut wMuVBFLoDPhi();

  // W->e control region
  TCut wElVBF();
  TCut wElVBFHiDPhi();
  TCut wElVBFLoDPhi();

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
