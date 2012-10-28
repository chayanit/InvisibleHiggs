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
// $Id: InvHiggsInfo.h,v 1.7 2012/09/24 09:18:22 jbrooke Exp $
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

  // MET
  Double_t met;
  Double_t metPhi;
  Double_t jetMETdPhi;
  //Double_t metXYCorr;
  //Double_t metXYCorrPhi;
  //Double_t jetMETXYCorrdPhi;
  Double_t metnomuon;
  Double_t metnomuonPhi;
  //Double_t metnomuXYCorr; 
  //Double_t metnomuXYCorrPhi; 
  Double_t metnoelectron;
  Double_t metnoelectronPhi;
  //Double_t metnoelectronXYCorr; 
  //Double_t metnoelectronXYCorrPhi;

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

  // MC info
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

};

#endif
