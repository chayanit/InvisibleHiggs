#ifndef InvHiggsEvent_h
#define InvHiggsEvent_h

// -*- C++ -*-
//
// Package:    InvisibleHiggs/Ntuple
// Class:      InvHiggsEvent
// 
/**\class InvHiggsEvent InvHiggsEvent.h InvisibleHiggs/Ntuple/interface/InvHiggsEvent.h

 Description: Defines the invisible Higgs TTree

 Implementation:

*/
//
// Original Author:  Jim Brooke
// $Id: InvHiggsEvent.h,v 1.1 2012/04/26 23:14:30 jbrooke Exp $
//
//

#include "TObject.h"
//#include "TString.h"
#include <vector>
//#include <functional>
//#include "boost/cstdint.hpp"



class InvHiggsEvent { //: public TObject {
 public:
  
  enum { MAX_N_JETS=20 };

 public:

  InvHiggsEvent();
  ~InvHiggsEvent();

  // helper methods
  void addL1Jet(double et, double eta, double phi, int type);
  void addHltJet(double et, double eta, double phi);
  void addCaloJet(double et, double etcorr, double eta, double phi, double emf, int n60, int n90, double fhpd, double frbx, int n90hits);
  void addPFJet(double et, double etcorr, double eta, double phi, double emf);
  void addMuon(double pt, double eta, double phi, int type);
  void addElectron(double pt, double eta, double phi);
  void addVertex(double z, double rho, double phi, int ndof);
  
  void Dump();
  
  
 public:  // data
  
  // event
  ULong_t id;
  ULong_t bx;
  ULong_t lb;
  ULong_t run;

  // trigger
  bool l1Result;
  bool hltResult;

  unsigned l1Jet_N;
  std::vector<Double_t> l1JetEt;
  std::vector<Double_t> l1JetEta;
  std::vector<Double_t> l1JetPhi;
  std::vector<UInt_t> l1JetType;

  double l1MET;

  unsigned hltJet_N;
  std::vector<Double_t> hltJetE;
  std::vector<Double_t> hltJetEt;
  std::vector<Double_t> hltJetEta;
  std::vector<Double_t> hltJetPhi;
  std::vector<UInt_t> hltJetType;

  double hltCaloMET;
  double hltPFMHT;

  // calo jets
  unsigned caloJet_N;
  std::vector<Double_t> caloJetEt;
  std::vector<Double_t> caloJetEtCorr;
  std::vector<Double_t> caloJetEta;
  std::vector<Double_t> caloJetPhi;
  std::vector<Double_t> caloJetEMF;
  std::vector<UInt_t> caloJetN60;
  std::vector<UInt_t> caloJetN90;
  std::vector<Double_t> caloJetFHPD;
  std::vector<Double_t> caloJetFRBX;
  std::vector<UInt_t> caloJetN90Hits;

  // PF jets
  unsigned pfJet_N;
  std::vector<Double_t> pfJetEt;
  std::vector<Double_t> pfJetEtCorr;
  std::vector<Double_t> pfJetEta;
  std::vector<Double_t> pfJetPhi;
  std::vector<Double_t> pfJetEMF;

  // reco muons
  unsigned mu_N;
  std::vector<Double_t> muPt;
  std::vector<Double_t> muEta;
  std::vector<Double_t> muPhi;
  std::vector<UInt_t> muType;

  // reco electrons
  unsigned ele_N;
  std::vector<Double_t> elePt;
  std::vector<Double_t> eleEta;
  std::vector<Double_t> elePhi;

  // vertices
  unsigned vtx_N;
  std::vector<Double_t> vtxZ;
  std::vector<Double_t> vtxRho;
  std::vector<Double_t> vtxPhi;
  std::vector<UInt_t> vtxNDOF;

  // Global variables
  Double_t caloMjj;
  Double_t pfMjj;
  Double_t caloMET;
  Double_t caloMETSig;
  Double_t pfMET;
  Double_t pfMETSig;
  Double_t pfMHT;
  Double_t mEE;
  Double_t mMuMu;

  // Beam Halo ID
  bool beamHalo;

};

#endif
