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
// $Id: InvHiggsInfo.h,v 1.4 2012/08/03 10:44:08 jbrooke Exp $
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

  // jets
  Double_t jet1Pt;
  Double_t jet1Eta;
  Double_t jet1Phi;
  Double_t jet1Mass;
  Double_t jet1PUMVA;
  Int_t    jet1PUFlag;

  Double_t jet2Pt;
  Double_t jet2Eta;
  Double_t jet2Phi;
  Double_t jet2Mass;
  Double_t jet2PUMVA;
  Int_t    jet2PUFlag;

  // Global variables
  Double_t met;
  Double_t metPhi;
  Double_t mht;
  Double_t mhtPhi;

  // VBF jet variables
  Double_t vbfEt;
  Double_t vbfEta;
  Double_t vbfPhi;
  Double_t vbfM;

  Double_t vbfJet3Phi;
  Double_t vbfNearJetEt;
  Double_t vbfNearJetDR;
  Double_t vbfCenJetEt;

  Double_t jetMETdPhi;

  // electrons
  Double_t ele1Pt;
  Double_t ele1Eta;
  Double_t ele1Phi;

  Double_t ele2Pt;
  Double_t ele2Eta;
  Double_t ele2Phi;

  // muons
  Double_t mu1Pt;
  Double_t mu1Eta;
  Double_t mu1Phi;

  Double_t mu2Pt;
  Double_t mu2Eta;
  Double_t mu2Phi;

  // W candidates
  Int_t nW;
  Double_t wPt;
  Double_t wEta;
  Double_t wPhi;
  Double_t wMt;
  Double_t wChannel;

  // Z candidates
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

  Double_t mcVBFJet3Phi;
  Double_t mcVBFNearJetEt;
  Double_t mcVBFNearJetDR;
  Double_t mcVBFCenJetEt;


};

#endif
