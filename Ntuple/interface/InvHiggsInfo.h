#ifndef InvHiggsInfo_h
#define InvHiggsInfo_h

// -*- C++ -*-
//
// Package:    InvisibleHiggs/Ntuple
// Class:      InvHiggsInfo
// 
/**\class InvHiggsInfo InvHiggsInfo.h InvisibleHiggs/Ntuple/interface/InvHiggsInfo.h

 Description: Defines the invisible Higgs TTree

 Implementation:

*/
//
// Original Author:  Jim Brooke
// $Id: InvHiggsInfo.h,v 1.22 2013/07/10 13:15:31 chayanit Exp $
//
//

#include "TObject.h"
//#include "TString.h"
#include <vector>
//#include <functional>
//#include "boost/cstdint.hpp"



class InvHiggsInfo { //: public TObject {
 public:

  InvHiggsInfo();
  ~InvHiggsInfo();

  void Dump();

 public:
  
  // event
  ULong_t id;
  ULong_t bx;
  ULong_t lb;
  ULong_t run;

  // trigger
  bool l1Result;
  double l1ET;
  double l1MET;
  double l1ETPhi;
  double l1HT;
  double l1MHT;
  double l1HTPhi;
  bool hltResult1;
  bool hltResult2;
  bool hltResult3;
  bool hltResult4;
  
  // MET filters
  bool metflag0;
  bool metflag1;
  bool metflag2;
  bool metflag3;
  bool metflag4;
  bool metflag5;
  bool metflag6;
  bool metflag7;
  bool metflag8;

  // Jets
  Int_t    jet1Index;
  Double_t jet1Pt;
  Double_t jet1Eta;
  Double_t jet1Phi;
  Double_t jet1M;
  Double_t jet1PUMVA;
  Int_t    jet1PUFlag;
  Double_t jet1unc;
  Double_t jet1DeltaT;
 
  Int_t    jet2Index;
  Double_t jet2Pt;
  Double_t jet2Eta;
  Double_t jet2Phi;
  Double_t jet2M;
  Double_t jet2PUMVA;
  Int_t    jet2PUFlag;
  Double_t jet2unc;
  Double_t jet2DeltaT;

  // MET (need XY correction?)
  Double_t met;
  Double_t metPhi;
  Double_t jmDPhi;
  Double_t jmDPhiN1;
  Double_t jmDPhiN2;
  Double_t jmDPhiNMin;
  Double_t metNoMuon;
  Double_t metNoMuonPhi; 
  Double_t metNoElectron;
  Double_t metNoElectronPhi;
  Double_t metNo2Muon;
  Double_t metNo2MuonPhi;
  Double_t jmNo2MuonDPhi;
  Double_t jmNo2MuonDPhiN1;
  Double_t jmNo2MuonDPhiN2;
  Double_t jmNo2MuonDPhiNMin;
  Double_t metNoWLepton;
  Double_t metNoWLeptonPhi;
  Double_t jmNoWLeptonDPhi;
  Double_t jmNoWLeptonDPhiN1;
  Double_t jmNoWLeptonDPhiN2;
  Double_t jmNoWLeptonDPhiNMin;

  // MHT
  Double_t mht;
  Double_t mhtPhi;

  // VBF jet variables
  Double_t vbfEt;
  Double_t vbfEta;
  Double_t vbfPhi;
  Double_t vbfM;
  Double_t vbfDEta;
  Double_t vbfDPhi;

  // a jet with pT > 20 with |eta|<2.4 (ie in tracker) 
  Int_t numTrackerJet3;

  // Third jet
  Double_t jet3Et;
  Double_t jet3Eta;
  Double_t jet3Phi;
  Double_t jet3M;

  // 3rd jet Zeppenfeld variable
  Double_t jet3EtaStar;

  // Central jet
  Double_t cenJetEt;
  Double_t cenJetEta;
  Double_t cenJetPhi;
  Double_t cenJetM;
  Double_t cenJetEtaStar;

  // Electrons
  Double_t ele1Pt;
  Double_t ele1Eta;
  Double_t ele1Phi;
  Double_t ele1M;

  Double_t ele2Pt;
  Double_t ele2Eta;
  Double_t ele2Phi;
  Double_t ele2M;

  Double_t ele3Pt;
  Double_t ele3Eta;
  Double_t ele3Phi;
  Double_t ele3M;

  // Muons
  Double_t mu1Pt;
  Double_t mu1Eta;
  Double_t mu1Phi;
  Double_t mu1M;

  Double_t mu2Pt;
  Double_t mu2Eta;
  Double_t mu2Phi;
  Double_t mu2M;

  Double_t mu3Pt;
  Double_t mu3Eta;
  Double_t mu3Phi;
  Double_t mu3M;

  // Taus
  Int_t nTaus_tot;
  Int_t nTaus_pass;
  Double_t tau_dz;
  Double_t tau1Pt;
  Double_t tau1Eta;
  Double_t tau1Phi;
  Double_t tau1M;
  Double_t tau1mT;
  Double_t tau1dR;
  Double_t tau2Pt;
  Double_t tau2Eta;
  Double_t tau2Phi; 
  Double_t tau2M;
  Double_t tau2mT;
  Int_t nTaus_match;

  // Ws
  Int_t nW;
  Double_t wPt;
  Double_t wEta;
  Double_t wPhi;
  Double_t wMt;
  Double_t wChannel;
  Double_t wDaulPt;
  Double_t wDaulEta;
  Double_t wDaulPhi;
  Double_t wDaulM;
  Double_t wDaulCharge;

  // Zs
  Int_t nZ;
  Double_t zPt;
  Double_t zEta;
  Double_t zPhi;
  Double_t zMass;
  Double_t zChannel;
  Double_t zDau1Pt;
  Double_t zDau1Eta;
  Double_t zDau1Phi;
  Double_t zDau1M;
  Double_t zDau1Charge;
  Double_t zDau2Pt;
  Double_t zDau2Eta;
  Double_t zDau2Phi;
  Double_t zDau2M;
  Double_t zDau2Charge;

  // PU
  Int_t nVtx;
  Double_t puWeight;

  // MC corrections
  Double_t trigCorrWeight;

  // MC info -- Higgs
  Double_t mcHiggsMass;
  Double_t mcHiggsPt;
  Double_t mcHiggsEta;
  Double_t mcHiggsPhi;

  Double_t mcQ1Pt;
  Double_t mcQ1Eta;
  Double_t mcQ1Phi;
  Double_t mcQ1M;

  Double_t mcQ2Pt;
  Double_t mcQ2Eta;
  Double_t mcQ2Phi;
  Double_t mcQ2M;

  Double_t mcVBFEt;
  Double_t mcVBFEta;
  Double_t mcVBFPhi;
  Double_t mcVBFM;

  Double_t mcVBFDEta;
  Double_t mcVBFDPhi;

  // MC info -- W
  Double_t wgennj;
  Double_t wgenmass;
  Double_t wgenpt;
  Double_t wgenmt;
  Double_t wgeneta;
  Double_t wgenphi;
  Double_t wgene;
  Int_t    wltype;
  Double_t wlpt;
  Double_t wleta;
  Double_t wlphi;
  Double_t wle;
  Int_t    wtauhadron;
  Double_t wmetpt;
  Double_t wmeteta;
  Double_t wmetphi;
  Double_t wmete;

  // MC info -- Z
  Double_t zgenmass;
  Double_t zgenpt;
  Double_t zgenmt;
  Double_t zgeneta;
  Double_t zgenphi;
  Double_t zgene;
  Int_t    zltype;
  Double_t zlmpt;
  Double_t zlmeta;
  Double_t zlmphi;
  Double_t zlme;
  Double_t zlppt;
  Double_t zlpeta;
  Double_t zlpphi;
  Double_t zlpe;

};

#endif
