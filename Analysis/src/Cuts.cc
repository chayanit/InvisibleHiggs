#include "InvisibleHiggs/Analysis/interface/Cuts.h"


Cuts::Cuts() {

  addCut("trigger",    "hltResult2>0.");
  addCut("metFilter",  "metflag1 && metflag2 && metflag3 && metflag4 && metflag5 && metflag6");
  addCut("dijet",      "jet1Pt>50.&&abs(jet1Eta)<4.7&&jet2Pt>50.&&abs(jet2Eta)<4.7");
  addCut("sgnEtaJJ",   "(jet1Eta*jet2Eta)<0.");
  addCut("dEtaJJ",     "abs(jet1Eta-jet2Eta)>4.2");
  addCut("Mjj",        "vbfM>1200.");
  addCut("MET",        "met>110.");
  addCut("dPhiJetMET", "min(abs(abs(abs(metPhi-jet1Phi)-TMath::Pi())-TMath::Pi()), abs(abs(abs(metPhi-jet2Phi)-TMath::Pi())-TMath::Pi()))>0.5");
  addCut("EVeto",      "ele1Pt<10.");
  addCut("MuVeto",     "mu1Pt<10.");

}

Cuts::~Cuts() {


}


void Cuts::addCut(std::string name, std::string cut) {

  names_.push_back(name);
  TCut c(cut.c_str());
  cuts_.push_back(c);

}


TCut Cuts::cut(unsigned i) {
  return cuts_.at(i);
}


TCut Cuts::allCuts() {

  TCut tmp;
  for (std::vector<TCut>::const_iterator c=cuts_.begin(); c!=cuts_.end(); ++c) {
    tmp = tmp && (*c);
  }
  return tmp;

}

TCut Cuts::nMinusOneCuts(unsigned i) {

  TCut tmp;
  for (unsigned j=0; j<cuts_.size(); ++j) {
    if (j!=i) tmp = tmp && cuts_.at(j);
  }
  return tmp;  

}

// Cuts::dump(std::ostream o) {

  


// }
