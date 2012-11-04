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
// $Id: InvHiggsInfo.h,v 1.9 2012/10/28 21:25:51 srimanob Exp $
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

  Int_t    jet2Index;
  Double_t jet2Pt;
  Double_t jet2Eta;
  Double_t jet2Phi;
  Double_t jet2M;
  Double_t jet2PUMVA;
  Int_t    jet2PUFlag;

  // MET (need XY correction?)
  Double_t met;
  Double_t metPhi;
  Double_t jetMETdPhi;
  //
  Double_t metNoMuon;
  Double_t metNoMuonPhi; 
  Double_t metNoLooseMuon;
  Double_t metNoLooseMuonPhi;
  // 
  //
  Double_t metNoElectron;
  Double_t metNoElectronPhi;
  Double_t metNoLooseElectron;
  Double_t metNoLooseElectronPhi;
  // 
  //

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

  Double_t ele2Pt;
  Double_t ele2Eta;
  Double_t ele2Phi;

  // Muons
  Double_t mu1Pt;
  Double_t mu1Eta;
  Double_t mu1Phi;

  Double_t mu2Pt;
  Double_t mu2Eta;
  Double_t mu2Phi;

  // Ws
  Int_t nW;
  Double_t wPt;
  Double_t wEta;
  Double_t wPhi;
  Double_t wMt;
  Double_t wChannel;

  // Zs
  Int_t nZ;
  Double_t zPt;
  Double_t zEta;
  Double_t zPhi;
  Double_t zMass;
  Double_t zChannel;

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
