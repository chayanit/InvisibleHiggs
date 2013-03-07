#include "InvisibleHiggs/Analysis/interface/Cuts.h"


Cuts::Cuts() {

  addCut("trigger",    "hltResult2>0.");
  addCut("metFilter",  "metflag1 && metflag2 && metflag3 && metflag4 && metflag5 && metflag6");
  addCut("dijet",      "jet1Pt>50.&&abs(jet1Eta)<4.7&&jet2Pt>50.&&abs(jet2Eta)<4.7");
  addCut("sgnEtaJJ",   "(jet1Eta*jet2Eta)<0.");
  addCut("dEtaJJ",     "vbfDEta>4.2");
  addCut("Mjj",        "vbfM>1200.");
  addCut("MET",        "met>130.");
  addCut("EVeto",      "ele1Pt<10.");
  addCut("MuVeto",     "mu1Pt<10.");
  addCut("dPhiJJ",     "vbfDPhi<1.0");

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

TCut Cuts::qcdLoose() {
  TCut tmp = cut(0);
  tmp += cut(1);
  tmp += cut(2);
  tmp += cut(3);
  tmp += cut(4);
  tmp += cut(5);
  tmp += TCut("met>70.");
  tmp += cut(7);
  tmp += cut(8);
  return tmp;
}


TCut Cuts::qcdLooseHiDPhi() {
  TCut tmp = qcdLoose();
  tmp += TCut("vbfDPhi>2.6");
  return tmp;
}


TCut Cuts::qcdLooseLoDPhi() {
  TCut tmp = qcdLoose();
  tmp += cut(9);
  return tmp;
}


TCut Cuts::qcdTightHiDPhi() {
  TCut tmp = cut(0);
  tmp += cut(1);
  tmp += cut(2);
  tmp += cut(3);
  tmp += cut(4);
  tmp += cut(5);
  tmp += cut(6);
  tmp += cut(7);
  tmp += cut(8);
  tmp += TCut("vbfDPhi>2.6");
  return tmp;
}


