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
// $Id: InvHiggsInfo.h,v 1.13 2013/03/12 13:39:59 chayanit Exp $
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
  bool hltResult1;
  bool hltResult2;

  // MET filters
  bool metflag0;
  bool metflag1;
  bool metflag2;
  bool metflag3;
  bool metflag4;
  bool metflag5;
  bool metflag6;

  // Jets
  Int_t    jet1Index;
  Double_t jet1Pt;
  Double_t jet1Eta;
  Double_t jet1Phi;
  Double_t jet1M;
  Double_t jet1PUMVA;
  Int_t    jet1PUFlag;
  Double_t jet1unc;

  Int_t    jet2Index;
  Double_t jet2Pt;
  Double_t jet2Eta;
  Double_t jet2Phi;
  Double_t jet2M;
  Double_t jet2PUMVA;
  Int_t    jet2PUFlag;
  Double_t jet2unc;
  
  // Jets shiftup re-ordering
  Int_t    jet1Index_shiftup;
  Double_t jet1Pt_shiftup;
  Double_t jet1Eta_shiftup;
  Double_t jet1Phi_shiftup;
  Double_t jet1M_shiftup;
  Double_t jet1PUMVA_shiftup;
  Int_t    jet1PUFlag_shiftup;
  Double_t jet1unc_shiftup;

  Int_t    jet2Index_shiftup;
  Double_t jet2Pt_shiftup;
  Double_t jet2Eta_shiftup;
  Double_t jet2Phi_shiftup;
  Double_t jet2M_shiftup;
  Double_t jet2PUMVA_shiftup;
  Int_t    jet2PUFlag_shiftup;
  Double_t jet2unc_shiftup;

  // Jets shiftdown re-ordering
  Int_t    jet1Index_shiftdown;
  Double_t jet1Pt_shiftdown;
  Double_t jet1Eta_shiftdown;
  Double_t jet1Phi_shiftdown;
  Double_t jet1M_shiftdown;
  Double_t jet1PUMVA_shiftdown;
  Int_t    jet1PUFlag_shiftdown;
  Double_t jet1unc_shiftdown;

  Int_t    jet2Index_shiftdown;
  Double_t jet2Pt_shiftdown;
  Double_t jet2Eta_shiftdown;
  Double_t jet2Phi_shiftdown;
  Double_t jet2M_shiftdown;
  Double_t jet2PUMVA_shiftdown;
  Int_t    jet2PUFlag_shiftdown;
  Double_t jet2unc_shiftdown;

  // MET (need XY correction?)
  Double_t met;
  Double_t metPhi;
  Double_t jetMETdPhi;
  Double_t metNoMuon;
  Double_t metNoMuonPhi; 
  Double_t metNoElectron;
  Double_t metNoElectronPhi;
  Double_t metNo2Muon;
  Double_t metNo2MuonPhi;
  Double_t metNoWLepton;
  Double_t metNoWLeptonPhi;
  // 
  // MET Uncertainty
  Double_t metCentral;
  Double_t metCentral_Phi;
  Double_t metCentral_NoMuon;
  Double_t metCentral_NoMuonPhi; 
  Double_t metCentral_NoElectron;
  Double_t metCentral_NoElectronPhi;
  Double_t metCentral_No2Muon;
  Double_t metCentral_No2MuonPhi;
  Double_t metCentral_NoWLepton;
  Double_t metCentral_NoWLeptonPhi;

  Double_t metJESup;
  Double_t metJESup_Phi;
  Double_t metJESup_NoMuon;
  Double_t metJESup_NoMuonPhi; 
  Double_t metJESup_NoElectron;
  Double_t metJESup_NoElectronPhi;
  Double_t metJESup_No2Muon;
  Double_t metJESup_No2MuonPhi;
  Double_t metJESup_NoWLepton;
  Double_t metJESup_NoWLeptonPhi;

  Double_t metJESdown;
  Double_t metJESdown_Phi;
  Double_t metJESdown_NoMuon;
  Double_t metJESdown_NoMuonPhi; 
  Double_t metJESdown_NoElectron;
  Double_t metJESdown_NoElectronPhi;
  Double_t metJESdown_No2Muon;
  Double_t metJESdown_No2MuonPhi;
  Double_t metJESdown_NoWLepton;
  Double_t metJESdown_NoWLeptonPhi;

  Double_t metJERup;
  Double_t metJERup_Phi;
  Double_t metJERup_NoMuon;
  Double_t metJERup_NoMuonPhi; 
  Double_t metJERup_NoElectron;
  Double_t metJERup_NoElectronPhi;
  Double_t metJERup_No2Muon;
  Double_t metJERup_No2MuonPhi;
  Double_t metJERup_NoWLepton;
  Double_t metJERup_NoWLeptonPhi;

  Double_t metJERdown;
  Double_t metJERdown_Phi;
  Double_t metJERdown_NoMuon;
  Double_t metJERdown_NoMuonPhi; 
  Double_t metJERdown_NoElectron;
  Double_t metJERdown_NoElectronPhi;
  Double_t metJERdown_No2Muon;
  Double_t metJERdown_No2MuonPhi;
  Double_t metJERdown_NoWLepton;
  Double_t metJERdown_NoWLeptonPhi;

  Double_t metUnclusteredup;
  Double_t metUnclusteredup_Phi;
  Double_t metUnclusteredup_NoMuon;
  Double_t metUnclusteredup_NoMuonPhi; 
  Double_t metUnclusteredup_NoElectron;
  Double_t metUnclusteredup_NoElectronPhi;
  Double_t metUnclusteredup_No2Muon;
  Double_t metUnclusteredup_No2MuonPhi;
  Double_t metUnclusteredup_NoWLepton;
  Double_t metUnclusteredup_NoWLeptonPhi;

  Double_t metUnclustereddown;
  Double_t metUnclustereddown_Phi;
  Double_t metUnclustereddown_NoMuon;
  Double_t metUnclustereddown_NoMuonPhi; 
  Double_t metUnclustereddown_NoElectron;
  Double_t metUnclustereddown_NoElectronPhi;
  Double_t metUnclustereddown_No2Muon;
  Double_t metUnclustereddown_No2MuonPhi;
  Double_t metUnclustereddown_NoWLepton;
  Double_t metUnclustereddown_NoWLeptonPhi;
  
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
