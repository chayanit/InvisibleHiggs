#include "InvisibleHiggs/Ntuple/interface/InvHiggsInfo.h"

#include <iostream>
#include <algorithm>


InvHiggsInfo::InvHiggsInfo() :
  id(0),
  bx(0),
  lb(0),
  run(0),
  l1Result(false),
  hltResult1(false),
  hltResult2(false),
  jet1Index(-1),
  jet1Pt(0.),
  jet1Eta(0.),
  jet1Phi(0.),
  jet1M(0.),
  jet1PUMVA(0.),
  jet1PUFlag(0),
  jet2Index(-1),
  jet2Pt(0.),
  jet2Eta(0.),
  jet2Phi(0.),
  jet2M(0.),
  jet2PUMVA(0.),
  jet2PUFlag(0),
  met(0.),
  metPhi(0.),
  jetMETdPhi(0.),
  mht(0.),
  mhtPhi(0.),
  vbfEt(0.),
  vbfEta(0.),
  vbfPhi(0.),
  vbfM(0.),
  vbfDEta(0.),
  vbfDPhi(0.),
  jet3Et(0.),
  jet3Eta(0.),
  jet3Phi(0.),
  jet3M(0.),
  jet3EtaStar(0.),
  cenJetEt(0.),
  cenJetEta(0.),
  cenJetPhi(0.),
  cenJetM(0.),
  ele1Pt(0.),
  ele1Eta(0.),
  ele1Phi(0.),
  ele2Pt(0.),
  ele2Eta(0.),
  ele2Phi(0.),
  mu1Pt(0.),
  mu1Eta(0.),
  mu1Phi(0.),
  mu2Pt(0.),
  mu2Eta(0.),
  mu2Phi(0.),
  nW(0),
  wPt(0.),
  wEta(0.),
  wPhi(0.),
  wMt(0.),
  wChannel(0.),
  nZ(0),
  zPt(0.),
  zEta(0.),
  zPhi(0.),
  zMass(0.),
  zChannel(0.),
  nVtx(0),
  puWeight(1.),
  mcHiggsMass(0.),
  mcHiggsPt(0.),
  mcHiggsEta(0.),
  mcHiggsPhi(0.),
  mcQ1Pt(0.),
  mcQ1Eta(0.),
  mcQ1Phi(0.),
  mcQ1M(0.),
  mcQ2Pt(0.),
  mcQ2Eta(0.),
  mcQ2Phi(0.),
  mcQ2M(0.),
  mcVBFEt(0.),
  mcVBFEta(0.),
  mcVBFPhi(0.),
  mcVBFM(0.),
  mcVBFDEta(0.),
  mcVBFDPhi(0.)
{

}

InvHiggsInfo::~InvHiggsInfo() { }

void InvHiggsInfo::Dump() {

  using namespace std;

  cout << "Event Dump " << endl;
  cout << "Run          " << run << endl;
  cout << "Event        " << id << endl;
  cout << "LB           " << lb << endl;
}


//#if !defined(__CINT__)
//  ClassImp(Event)
//#endif
