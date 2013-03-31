#ifndef Cuts_h
#define Cuts_h

#include "TCut.h"


class Cuts {
 public:

  Cuts();
  ~Cuts();

  // setters
  void addCut(std::string name, std::string cut);
  void addZMuMuCut(std::string name, std::string cut);
  void addWMuCut(std::string name, std::string cut);
  void addWElCut(std::string name, std::string cut);
  void addQCDCut(std::string name, std::string cut);

  // getters
  TCut cut(std::string name);
  std::string cutName(unsigned i);

  unsigned nCuts() { return cuts_.size(); }
  unsigned nZMuMuCuts() { return cutsZMuMu_.size(); }
  unsigned nWMuCuts() { return cutsWMu_.size(); }
  unsigned nWElCuts() { return cutsWEl_.size(); }
  unsigned nQCDCuts() { return cutsQCD_.size(); }

  TCut allCuts();

  //  TCut nMinusOneCuts(unsigned i);
  TCut nMinusOneCuts(std::string name);

  // cutflow
  TCut cutflow(unsigned i);

  // dataset-dependent cut
  TCut cutDataset(std::string name);

  // combinations
  TCut HLTandMETFilters();
  TCut vbf();   // note this does not include vetoes!

  TCut allCutsNoDPhi();
  TCut allCutsHiDPhi();
  TCut allCutsLoDPhi();

  // cuts for BG estimation

  // Z
  TCut zMuMuGen();   	  // there is a generator level Z->mumu
  TCut zMuMuGenPt100();   // there is a generator level Z->mumu + pT cut
  TCut zMuMuGenMass();    // there is a generator level Z->mumu + mass cut
  TCut zMuMuReco();       // there is a reco level Z->mumu
  TCut zMuMuVBF();
  TCut zMuMuGenPt100VBF();
  TCut cutflowZMuMu(unsigned i);
  unsigned nCutsZMuMu()  { return cutsZMuMu_.size(); }

  // W-> mu
  TCut wMuGen();
  TCut wMuGen0P();
  TCut wMuReco();
  TCut wMuVBF();
  TCut cutflowWMu(unsigned i);
  unsigned nCutsWMu() { return cutsWMu_.size(); }

  // W-> e
  TCut wElGen();
  TCut wElGen0P();
  TCut wElReco();
  TCut wElVBF();
  TCut cutflowWEl(unsigned i);
  unsigned nCutsWEl() { return cutsWEl_.size(); }

  // QCD
  TCut qcdNoMET();
  TCut qcdLoose();
  TCut qcdTightHiDPhi();
  TCut cutflowQCD(unsigned i);
  unsigned nCutsQCD() { return cutsQCD_.size(); }

 private:
  
  std::vector<std::string> names_;
  std::vector<TCut> cuts_;

  std::vector<std::string> namesZMuMu_;
  std::vector<TCut> cutsZMuMu_;

  std::vector<std::string> namesWMu_;
  std::vector<TCut> cutsWMu_;

  std::vector<std::string> namesWEl_;
  std::vector<TCut> cutsWEl_;

  std::vector<std::string> namesQCD_;
  std::vector<TCut> cutsQCD_;

};

#endif
