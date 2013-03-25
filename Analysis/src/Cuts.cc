#include "InvisibleHiggs/Analysis/interface/Cuts.h"


Cuts::Cuts() {

  addCut("trigger",    "hltResult2>0.");
  addCut("metFilter",  "metflag1 && metflag2 && metflag3 && metflag4 && metflag5 && metflag6");
  addCut("EVeto",      "ele1Pt<10.");
  addCut("MuVeto",     "mu1Pt<10.");
  addCut("dijet",      "jet1Pt>50.&&abs(jet1Eta)<4.7&&jet2Pt>50.&&abs(jet2Eta)<4.7");
  addCut("sgnEtaJJ",   "(jet1Eta*jet2Eta)<0.");
  addCut("dEtaJJ",     "vbfDEta>4.2");
  addCut("MET",        "met>130.");
  addCut("Mjj",        "vbfM>1200.");
  addCut("dPhiJJ",     "vbfDPhi<1.0");

}

Cuts::~Cuts() {


}


void Cuts::addCut(std::string name, std::string cut) {

  names_.push_back(name);
  TCut c(cut.c_str());
  cuts_.push_back(c);

}


// TCut Cuts::cut(unsigned i) {
//   return cuts_.at(i);
// }


TCut Cuts::cut(std::string s) {
  for (unsigned i=0; i<names_.size(); ++i) {
    if (names_.at(i) == s) return cuts_.at(i);
  }
  return TCut();
}


TCut Cuts::allCuts() {

  TCut tmp;
  for (std::vector<TCut>::const_iterator c=cuts_.begin(); c!=cuts_.end(); ++c) {
    tmp = tmp + (*c);
  }
  return tmp;

}

// TCut Cuts::nMinusOneCuts(unsigned i) {

//   TCut tmp;
//   for (unsigned j=0; j<cuts_.size(); ++j) {
//     if (j!=i) tmp = tmp && cuts_.at(j);
//   }
//   return tmp;  

// }

TCut Cuts::nMinusOneCuts(std::string name) {

  TCut tmp;
  for (unsigned j=0; j<cuts_.size(); ++j) {
    if (name!=names_.at(j)) tmp = tmp && cuts_.at(j);
  }
  return tmp;  

}


// Cuts::dump(std::ostream o) {

  


// }

// combinations
TCut Cuts::vbf() {
  TCut tmp = cut("dijet");
  tmp += cut("sgnEtaJJ");
  tmp += cut("dEtaJJ");
  tmp += cut("Mjj");
  return tmp;
}

TCut Cuts::allCutsNoDPhi() {
  TCut tmp = nMinusOneCuts("dPhiJJ");
  return tmp;
}

TCut Cuts::allCutsHiDPhi() {
  TCut tmp = allCutsNoDPhi();
  tmp += TCut("vbfDPhi>2.6");
  return tmp;
}

TCut Cuts::allCutsLoDPhi() {
  TCut tmp = allCutsNoDPhi();
  tmp += TCut("vbfDPhi<1.0");
  return tmp;
}


// control regions
TCut Cuts::zMuMuGen() {
  TCut tmp("zltype==2");
  return tmp;
}

TCut Cuts::zMuMuGenMass() {
  TCut tmp("zltype==2 && zgenmass>60. && zgenmass<120.");
  return tmp;
}

TCut Cuts::zMuMuReco() {
  TCut tmp("zChannel==1 && zMass>60. && zMass<120.");
  return tmp;
}


TCut Cuts::wMuGen() {
  TCut tmp("wltype==2");
  return tmp;
}

TCut Cuts::wMuReco() {
  TCut tmp("wChannel==1 && wMt>60. && wMt<120.");
  return tmp;
}

TCut Cuts::wElGen() {
  TCut tmp("wltype==1");
  return tmp;
}

TCut Cuts::wElReco() {
  TCut tmp("wChannel==2 && wMt>60. && wMt<120.");
  return tmp;
}

TCut Cuts::zMuMuVBF() {
  TCut tmp = cut("trigger");
  tmp += cut("metFilter");
  tmp += zMuMuReco();
  tmp += TCut("mu3Pt<10");
  tmp += cut("EVeto");
  tmp += vbf();
  tmp += TCut("metNoMuon>130.");
  return tmp;
}

TCut Cuts::zMuMuVBFHiDPhi() {
  TCut tmp = zMuMuVBF();
  tmp += TCut("vbfDPhi>2.6");
  return tmp;
}

TCut Cuts::zMuMuVBFLoDPhi() {
  TCut tmp = zMuMuVBF();
  tmp += cut("dPhiJJ");
  return tmp;
}



// W->mu regions
TCut Cuts::wMuVBF() {
  TCut tmp = cut("trigger");
  tmp += cut("metFilter");
  tmp += wMuReco();
  tmp += TCut("mu2Pt<10");
  tmp += cut("EVeto");
  tmp += vbf();
  tmp += TCut("met>130.");
  return tmp;
}

TCut Cuts::wMuVBFHiDPhi() {
  TCut tmp = wMuVBF();
  tmp += TCut("vbfDPhi>2.6");
  return tmp;
}

TCut Cuts::wMuVBFLoDPhi() {
  TCut tmp = wMuVBF();
  tmp += cut("dPhiJJ");
  return tmp;
}


// W->mu regions
TCut Cuts::wElVBF() {
  TCut tmp = cut("trigger");
  tmp += cut("metFilter");
  tmp += wElReco();
  tmp += TCut("ele2Pt<10");
  tmp += cut("MVeto");
  tmp += vbf();
  tmp += TCut("metNoElectron>130.");
  return tmp;
}

TCut Cuts::wElVBFHiDPhi() {
  TCut tmp = wElVBF();
  tmp += TCut("vbfDPhi>2.6");
  return tmp;
}

TCut Cuts::wElVBFLoDPhi() {
  TCut tmp = wElVBF();
  tmp += cut("dPhiJJ");
  return tmp;
}


// QCD regions
TCut Cuts::qcdLoose() {
  TCut tmp = cut("trigger");
  tmp += cut("metFilter");
  tmp += cut("EVeto");
  tmp += cut("MVeto");
  tmp += vbf();
  tmp += TCut("met>70.");
  return tmp;
}


TCut Cuts::qcdLooseHiDPhi() {
  TCut tmp = qcdLoose();
  tmp += TCut("vbfDPhi>2.6");
  return tmp;
}


TCut Cuts::qcdLooseLoDPhi() {
  TCut tmp = qcdLoose();
  tmp += cut("dPhiJJ");
  return tmp;
}


TCut Cuts::qcdTightHiDPhi() {
  TCut tmp = cut("trigger");
  tmp += cut("metFilter");
  tmp += cut("EVeto");
  tmp += cut("MVeto");
  tmp += vbf();
  tmp += cut("met");
  tmp += TCut("vbfDPhi>2.6");
  return tmp;
}


