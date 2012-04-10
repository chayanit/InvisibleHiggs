#ifndef Event_h
#define Event_h

// -*- C++ -*-
//
// Package:    InvisibleHiggs/Ntuple
// Class:      Event
// 
/**\class Event Event.h InvisibleHiggs/Ntuple/interface/Event.h

 Description: Defines the invisible Higgs TTree

 Implementation:

*/
//
// Original Author:  Jim Brooke
// $Id: $
//
//

#include "TObject.h"
//#include "TString.h"
#include <vector>
//#include <functional>
//#include "boost/cstdint.hpp"



class Event { //: public TObject {
 public:
  
  enum { MAX_N_JETS=20 };

 public:

  Event();
  ~Event();

  // helper methods
  void addL1Jet(double et, double eta, double phi, int type);
  void addHltJet(double et, double eta, double phi);
  void addJet(double et, double eta, double phi, double ethad, double etem, int n60, int n90, double fhpd, int n90hits);
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

  // reco jets
  unsigned jet_N;
  std::vector<Double_t> jetEt;
  std::vector<Double_t> jetEtCorr;
  std::vector<Double_t> jetEta;
  std::vector<Double_t> jetPhi;
  std::vector<Double_t> jetEtHad;
  std::vector<Double_t> jetEtEm;
  std::vector<UInt_t> jetN60;
  std::vector<UInt_t> jetN90;
  std::vector<Double_t> jetFHPD;
  std::vector<UInt_t> jetN90Hits;

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

  // MET
  Double_t mJJ;
  Double_t caloMET;
  Double_t pfMHT;

  // Beam Halo data
  bool beamHaloCSCTight;
  bool beamHaloCSCLoose;

  //  ClassDef(Event,1); // 

};

#endif
