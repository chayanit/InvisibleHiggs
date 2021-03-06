#include "InvisibleHiggs/Ntuple/interface/InvHiggsEvent.h"

#include <iostream>
#include <algorithm>


InvHiggsEvent::InvHiggsEvent() :
  id(0),
  bx(0),
  lb(0),
  run(0),
  l1Result(false),
  hltResult1(false),
  hltResult2(false),
  caloMjj(0.),
  pfMjj(0.),
  caloMET(0.),
  caloMETSig(0.),
  pfMET(0.),
  pfMETSig(0.),
  pfMHT(0.),
  pfMHTSig(0.),
  mEE(0.),
  mMuMu(0.),
  vbfJet1Index(-1),
  vbfJet2Index(-1),
  vbfEt(0.),
  vbfEta(0.),
  vbfPhi(0.),
  vbfM(0.),
  vbfJet3Phi(0.),
  vbfNearJetEt(0.),
  vbfNearJetDR(0.),
  vbfCenJetEt(0.),
  beamHalo(false),
  caloJet_N(0),
  caloJetEt(0),
  caloJetEtCorr(0),
  caloJetEta(0),
  caloJetPhi(0),
  caloJetEMF(0),
  caloJetN60(0),
  caloJetN90(0),
  caloJetFHPD(0),
  caloJetFRBX(0),
  caloJetN90Hits(0),
  pfJet_N(0),
  pfJetEt(0),
  pfJetEtCorr(0),
  pfJetEta(0),
  pfJetPhi(0),
  pfNeutHadFrac(0.),
  pfNeutEMFrac(0.),
  pfChargHadFrac(0.),
  pfChargEMFrac(0.),
  pfNConst(0),
  pfChargedMult(0),
  mu_N(0),
  muPt(0),
  muEta(0),
  muPhi(0),
  muType(0),
  ele_N(0),
  elePt(0),
  eleEta(0),
  elePhi(0),
  l1Jet_N(0),
  l1JetEt(0),
  l1JetEta(0),
  l1JetPhi(0),
  l1JetType(0),
  l1MET(0.),
  hltJet_N(0),
  hltJetEt(0),
  hltJetEta(0),
  hltJetPhi(0),
  hltJetType(0),
  hltCaloMET(0.),
  hltPFMHT(0.),
  vtx_N(0),
  vtxZ(0),
  vtxRho(0),
  vtxPhi(0),
  vtxNDOF(0)
			    //  puWeight(0.)
{

}

InvHiggsEvent::~InvHiggsEvent() { }


void InvHiggsEvent::addL1Jet(double et, double eta, double phi, int type) {
  if (l1Jet_N < MAX_N_JETS) {
    l1JetEt.push_back(et);
    l1JetEta.push_back(eta);
    l1JetPhi.push_back(phi);
    l1JetType.push_back(type);
    ++l1Jet_N;
  }
}

void InvHiggsEvent::addHltJet(double et, double eta, double phi) {
 if (hltJet_N < MAX_N_JETS) {
   hltJetEt.push_back(et);
   hltJetEta.push_back(eta);
   hltJetPhi.push_back(phi);
   //   hltJetType.push_back(type);
   ++hltJet_N;
 }
}

void InvHiggsEvent::addCaloJet(double et, double etcorr, double eta, double phi, double emf, int n60, int n90, double fhpd, double frbx, int n90hits) {
  if (caloJet_N < MAX_N_JETS) {
    caloJetEt.push_back(et);
    caloJetEtCorr.push_back(etcorr);
    caloJetEta.push_back(eta);
    caloJetPhi.push_back(phi);
    caloJetEMF.push_back(emf);
    caloJetN60.push_back(n60);
    caloJetN90.push_back(n90);
    caloJetFHPD.push_back(fhpd);
    caloJetFRBX.push_back(frbx);
    caloJetN90Hits.push_back(n90hits);
    ++caloJet_N;
  }
}

void InvHiggsEvent::addPFJet(double et, double etcorr, double eta, double phi, double nhf, double nef, double chf, double cef, int nconst, int cmult) {
  if (pfJet_N < MAX_N_JETS) {
    pfJetEt.push_back(et);
    pfJetEtCorr.push_back(etcorr);
    pfJetEta.push_back(eta);
    pfJetPhi.push_back(phi);
    pfNeutHadFrac.push_back(nhf);
    pfNeutEMFrac.push_back(nef);
    pfChargHadFrac.push_back(chf);
    pfChargEMFrac.push_back(cef);
    pfNConst.push_back(nconst);
    pfChargedMult.push_back(cmult);
    ++pfJet_N;
  }
}

void InvHiggsEvent::addMuon(double pt, double eta, double phi, int type) { 
  muPt.push_back(pt);
  muEta.push_back(eta);
  muPhi.push_back(phi);
  muType.push_back(type);
  ++mu_N;
}

void InvHiggsEvent::addElectron(double pt, double eta, double phi) { 
  elePt.push_back(pt);
  eleEta.push_back(eta);
  elePhi.push_back(phi);
  ++ele_N;
}

void InvHiggsEvent::addVertex(double z, double rho, double phi, int ndof) {
  vtxZ.push_back(z);
  vtxRho.push_back(rho);
  vtxPhi.push_back(phi);
  vtxNDOF.push_back(ndof);
  ++vtx_N;
}


void InvHiggsEvent::Dump() {

  using namespace std;

  cout << "Event Dump " << endl;
  cout << "Run          " << run << endl;
  cout << "Event        " << id << endl;
  cout << "LB           " << lb << endl;
}


//#if !defined(__CINT__)
//  ClassImp(Event)
//#endif
