#include "InvisibleHiggs/Analysis/interface/Cuts.h"


Cuts::Cuts() {

  addCut("trigger",    "hltResult2>0.");
  addCut("metFilter",  "metflag0 && metflag1 && metflag2 && metflag3 && metflag4 && metflag5 && metflag6 && metflag7 && metflag8");// && !(abs(jet1Phi+1.5)<0.3 && abs(jet1Eta+2.3)<0.3) && !(abs(jet1Phi+0.7)<0.3 && abs(jet1Eta-1.6)<0.3) && !(abs(jet2Phi+1.5)<0.3 && abs(jet2Eta+2.3)<0.3) && !(abs(jet2Phi+0.7)<0.3 && abs(jet2Eta-1.6)<0.3) && !(abs(jet3Phi+1.5)<0.3 && abs(jet3Eta+2.3)<0.3) && !(abs(jet3Phi+0.7)<0.3 && abs(jet3Eta-1.6)<0.3)");
  addCut("EVeto",      "ele1Pt<10.");
  addCut("MuVeto",     "mu1Pt<10.");
  addCut("dijet",      "jet1Pt>50.&&abs(jet1Eta)<4.7&&jet2Pt>50.&&abs(jet2Eta)<4.7");
  addCut("sgnEtaJJ",   "(jet1Eta*jet2Eta)<0.");
  addCut("dEtaJJ",     "vbfDEta>4.2");
  addCut("MET",        "met>130.");
  addCut("Mjj",        "vbfM>1100.");
  addCut("CJV",        "cenJetEt<30.");
  addCut("dPhiJJ",     "vbfDPhi<1.0");

  addZMuMuCut("trigger",   "hltResult2>0. && metflag0 && metflag1 && metflag2 && metflag3 && metflag4 && metflag5 && metflag6 && metflag7 && metflag8");
  addZMuMuCut("z",         "zChannel==1 && zMass>60. && zMass<120.");
  addZMuMuCut("lVeto",     "ele1Pt<10. && mu3Pt<10.");
  addZMuMuCut("dijet",     "jet1Pt>50.&&abs(jet1Eta)<4.7&&jet2Pt>50.&&abs(jet2Eta)<4.7 && (jet1Eta*jet2Eta)<0.");
  addZMuMuCut("dEtaJJ",    "vbfDEta>4.2");
  addZMuMuCut("MET",       "metNo2Muon>130.");
  addZMuMuCut("Mjj",       "vbfM>1100.");
  addZMuMuCut("CJV",       "cenJetEt<30.");
  addZMuMuCut("dPhiJJ",    "vbfDPhi<1.0");

  addWMuCut("trigger",   "hltResult2>0. && metflag0 && metflag1 && metflag2 && metflag3 && metflag4 && metflag5 && metflag6 && metflag7 && metflag8");
  addWMuCut("wMu",       "nW>0 && wChannel==1");
  addWMuCut("lVeto",     "ele1Pt<10. && mu2Pt<10.");
  addWMuCut("dijet",     "jet1Pt>50.&&abs(jet1Eta)<4.7&&jet2Pt>50.&&abs(jet2Eta)<4.7 && (jet1Eta*jet2Eta)<0.");
  addWMuCut("dEtaJJ",    "vbfDEta>4.2");
  addWMuCut("MET",       "metNoWLepton>130.");
  addWMuCut("Mjj",       "vbfM>1100.");
  addWMuCut("CJV",       "cenJetEt<30.");
  addWMuCut("dPhiJJ",    "vbfDPhi<1.0");

  addWElCut("trigger",   "hltResult2>0. && metflag0 && metflag1 && metflag2 && metflag3 && metflag4 && metflag5 && metflag6 && metflag7 && metflag8");
  addWElCut("wEl",       "nW>0 && wChannel==2");
  addWElCut("lVeto",     "ele2Pt<10. && mu1Pt<10.");
  addWElCut("dijet",     "jet1Pt>50.&&abs(jet1Eta)<4.7&&jet2Pt>50.&&abs(jet2Eta)<4.7 && (jet1Eta*jet2Eta)<0.");
  addWElCut("dEtaJJ",    "vbfDEta>4.2");
  addWElCut("MET",       "met>130.");
  addWElCut("Mjj",       "vbfM>1100.");
  addWElCut("CJV",       "cenJetEt<30.");
  addWElCut("dPhiJJ",    "vbfDPhi<1.0");

}

Cuts::~Cuts() {


}


void Cuts::addCut(std::string name, std::string cut) {
  names_.push_back(name);
  TCut c(cut.c_str());
  cuts_.push_back(c);
}

void Cuts::addZMuMuCut(std::string name, std::string cut) {
  namesZMuMu_.push_back(name);
  TCut c(cut.c_str());
  cutsZMuMu_.push_back(c);
}

void Cuts::addWMuCut(std::string name, std::string cut) {
  namesWMu_.push_back(name);
  TCut c(cut.c_str());
  cutsWMu_.push_back(c);
}

void Cuts::addWElCut(std::string name, std::string cut) {
  namesWEl_.push_back(name);
  TCut c(cut.c_str());
  cutsWEl_.push_back(c);
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


TCut Cuts::cutZMuMu(std::string s) {
  for (unsigned i=0; i<namesZMuMu_.size(); ++i) {
    if (namesZMuMu_.at(i) == s) return cutsZMuMu_.at(i);
  }
  return TCut();
}


TCut Cuts::cutWMu(std::string s) {
  for (unsigned i=0; i<namesWMu_.size(); ++i) {
    if (namesWMu_.at(i) == s) return cutsWMu_.at(i);
  }
  return TCut();
}


TCut Cuts::cutWEl(std::string s) {
  for (unsigned i=0; i<namesWEl_.size(); ++i) {
    if (namesWEl_.at(i) == s) return cutsWEl_.at(i);
  }
  return TCut();
}


std::string Cuts::cutName(unsigned i) {
  return names_.at(i);
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


TCut Cuts::cutflow(unsigned i) {
  TCut tmp;
  for (unsigned j=0; j<i+1; ++j) {
    tmp += cuts_.at(j);
  }
  return tmp;
}


TCut Cuts::cutDataset(std::string name) {

  TCut tmp;
  if (name == "WJets") return tmp;//"wgennj==0");
  else if (name == "DYJetsToLL" || name == "DYJetsToLL_NoTrig") return TCut("zgenpt<100.");
  else return tmp;

}


TCut Cuts::wWeight() {
  return TCut("(1.0*(wgennj==0)+0.369253*(wgennj==1)+0.11401*(wgennj==2)+0.0771589*(wgennj==3)+0.03849*(wgennj==4))");
}


// combinations
TCut Cuts::HLTandMETFilters() {
  TCut tmp = cut("trigger");
  tmp += cut("metFilter");
  return tmp;
}

TCut Cuts::leptonVeto() {
  TCut tmp = cut("EVeto");
  tmp += cut("MuVEto");
  return tmp;
}

TCut Cuts::vbf() {
  TCut tmp = cut("dijet");
  tmp += cut("sgnEtaJJ");
  tmp += cut("dEtaJJ");
  tmp += cut("Mjj");
  tmp += cut("CJV");
  return tmp;
}

TCut Cuts::vbfloose() {
  TCut tmp = cut("dijet");
  tmp += cut("sgnEtaJJ");
  tmp += TCut("vbfM>1000.");
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

TCut Cuts::zMuMuVBF() {
  TCut tmp = cut("trigger");
  tmp += cut("metFilter");
  tmp += zMuMuReco();
  tmp += TCut("mu3Pt<10");
  tmp += cut("EVeto");
  tmp += vbf();
  return tmp;
}

TCut Cuts::zMuMuVBFLoose() {
  TCut tmp = cut("trigger");
  tmp += cut("metFilter");
  tmp += zMuMuReco();
  tmp += vbfloose();
  tmp += TCut("metNo2Muon>130.");
  return tmp;
}

TCut Cuts::cutflowZMuMu(unsigned i) {
  TCut tmp("");
  for (unsigned j=0; j<i+1; ++j) {
    tmp += cutsZMuMu_.at(j);
  }
  return tmp;
}



// W -> mu
TCut Cuts::wMuGen() {
  TCut tmp("wltype==2 || (wltype==3 && wtauhadron==3)");
  return tmp;
}

TCut Cuts::wMuVBF() {
  TCut tmp = cutWMu("trigger");
  tmp += cutWMu("wMu");
  tmp += cutWMu("lVeto");
  tmp += cutWMu("dijet");
  tmp += cutWMu("dEtaJJ");
  tmp += cutWMu("Mjj");
  tmp += cutWMu("CJV");
  return tmp;
}

TCut Cuts::cutflowWMu(unsigned i) {
  TCut tmp("");
  for (unsigned j=0; j<i+1; ++j) {
    tmp += cutsWMu_.at(j);
  }
  return tmp;
}


// W -> e
TCut Cuts::wElGen() {
  TCut tmp("wltype==1 || (wltype==3 && wtauhadron==2)");
  return tmp;
}

TCut Cuts::wElVBF() {
  TCut tmp = cutWEl("trigger");
  tmp += cutWEl("wEl");
  tmp += cutWEl("lVeto");
  tmp += cutWEl("dijet");
  tmp += cutWEl("dEtaJJ");
  tmp += cutWEl("Mjj");
  tmp += cutWEl("CJV");
  return tmp;
}

TCut Cuts::cutflowWEl(unsigned i) {
  TCut tmp("");
  for (unsigned j=0; j<i+1; ++j) {
    tmp += cutsWEl_.at(j);
  }
  return tmp;
}


// W -> tau
TCut Cuts::wTauGen() {
  TCut tmp("wltype==3 && wtauhadron==1");
  return tmp;
}



// QCD regions
TCut Cuts::qcdNoMET() {
  TCut tmp = cut("trigger");
  tmp += cut("metFilter");
  tmp += cut("EVeto");
  tmp += cut("MuVeto");
  tmp += vbf();
  return tmp;
}

TCut Cuts::qcdLoose2() {
  TCut tmp = cut("trigger");
  tmp += cut("metFilter");
  tmp += cut("EVeto");
  tmp += cut("MuVeto");
  tmp += vbf();
  tmp += TCut("met>90.");
  return tmp;
}

TCut Cuts::qcdLoose() {
  TCut tmp = cut("trigger");
  tmp += cut("metFilter");
  tmp += cut("EVeto");
  tmp += cut("MuVeto");
  tmp += vbf();
  tmp += TCut("met>100.");
  return tmp;
}

TCut Cuts::qcdTightHiDPhi() {
  TCut tmp = cut("trigger");
  tmp += cut("metFilter");
  tmp += cut("EVeto");
  tmp += cut("MuVeto");
  tmp += vbf();
  tmp += cut("MET");
  tmp += TCut("vbfDPhi>2.6");
  return tmp;
}

TCut Cuts::cutflowQCD(unsigned i) {
  TCut tmp("");
  for (unsigned j=0; j<i+1; ++j) {
    tmp += cutsQCD_.at(i);
  }
  return tmp;
}

