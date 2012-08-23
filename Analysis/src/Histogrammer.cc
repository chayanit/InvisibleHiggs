#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TMath.h"
#include "TCut.h"

#include <vector>
#include <iostream>

Histogrammer::Histogrammer(std::string odir, std::string idir, Cuts cuts) :
  odir_(odir),
  idir_(idir),
  cuts_(cuts) {
  
  TH1D::SetDefaultSumw2();

}


Histogrammer::~Histogrammer() {


}

void Histogrammer::addDataset(std::string name,
			      double weight, 
			      std::string idir) {
  names_.push_back(name);
  weights_.push_back(weight);
  if (idir!="") idirs_.push_back(idir);
  else idirs_.push_back(idir_);

}


void Histogrammer::loop() {

  for (unsigned i=0; i<names_.size(); ++i) {
    makeHistograms(names_.at(i), weights_.at(i), idirs_.at(i));
  }

}


void Histogrammer::makeHistograms(std::string dataset, double weight, std::string idir) {

  TFile* ifile = TFile::Open( (idir+std::string("/")+dataset+std::string(".root")).c_str(), "READ");

  if (ifile==0) {
    std::cerr << "No file for " << dataset << std::endl;
    return;
  }

  TTree* tree = (TTree*) ifile->Get("invHiggsInfo/InvHiggsInfo");
  
  TFile* ofile = TFile::Open( (odir_+std::string("/")+dataset+std::string(".root")).c_str(), "RECREATE");

  // get n events to calculate weight
  int nEvents = tree->GetEntries();
  std::cout << "Dataset " << dataset << " : "  << nEvents << " events" << std::endl;

  TH1D* hjet1pt  = new TH1D("hjet1pt", "jet_{1} p_{T}:N_{events}:p_{T} [GeV]", 20, 0., 200.);
  TH1D* hjet2pt  = new TH1D("hjet2pt", "jet_{2} p_{T}:N_{events}:p_{T} [GeV]", 20, 0., 200.);
  TH1D* hjetdeta = new TH1D("hjetdeta", "#Delta #eta:N_{events}:#Delta #eta", 20, 0., 10.);
  TH1D* hvbfm    = new TH1D("hvbfm", "M_{jj}:N_{events}:M_{jj} [GeV]", 20, 0., 4000.);
  TH1D* hmet     = new TH1D("hmet", "E_T^miss:N_{events}:E_T^miss [GeV]", 20, 0., 500.);
  TH1D* hjetdphi = new TH1D("hjetdphi", "#Delta #phi_{jj}:N_{events}:#Delta #phi_{jj}", 20, 0., TMath::Pi());
  TH1D* hjmdphi  = new TH1D("hjmdphi", "#Delta #phi_{jetMET}:N_{events}:#Delta #phi_{jetMET}", 20, 0., TMath::Pi());

  TH1D* hseljet1pt  = new TH1D("hseljet1pt", "jet_{1} p_{T}:N_{events}:p_{T} [GeV]", 20, 0., 200.);
  TH1D* hseljet2pt  = new TH1D("hseljet2pt", "jet_{2} p_{T}:N_{events}:p_{T} [GeV]", 20, 0., 200.);
  TH1D* hseljetdeta = new TH1D("hseljetdeta", "#Delta #eta:N_{events}:#Delta #eta", 20, 0., 10.);
  TH1D* hselvbfm    = new TH1D("hselvbfm", "M_{jj}:N_{events}:M_{jj} [GeV]", 20, 0., 4000.);
  TH1D* hselmet     = new TH1D("hselmet", "E_T^miss:N_{events}:E_T^miss [GeV]", 20, 0., 500.);
  TH1D* hseljetdphi = new TH1D("hseljetdphi", "#Delta #phi_{jj}:N_{events}:#Delta #phi_{jj}", 20, 0., TMath::Pi());
  TH1D* hseljmdphi  = new TH1D("hseljmdphi", "#Delta #phi_{jetMET}:N_{events}:#Delta #phi_{jetMET}", 20, 0., TMath::Pi());


  TCut puWeight  ("puWeight");
  TCut allCutsWeighted = puWeight * cuts_.allCuts();

  tree->Draw("jet1Pt>>hjet1pt", "puWeight");
  tree->Draw("jet2Pt>>hjet2pt", "puWeight");
  tree->Draw("abs(jet1Eta-jet2Eta)>>hjetdeta", "puWeight");
  tree->Draw("vbfM>>hvbfm", "puWeight");
  tree->Draw("met>>hmet", "puWeight");
  tree->Draw("abs(abs(abs(jet1Phi-jet2Phi)-TMath::Pi())-TMath::Pi())>>hjetdphi", "puWeight");
  tree->Draw("min(abs(abs(abs(metPhi-jet1Phi)-TMath::Pi())-TMath::Pi()), abs(abs(abs(metPhi-jet2Phi)-TMath::Pi())-TMath::Pi()))>>hjmdphi", "puWeight");

  tree->Draw("jet1Pt>>hseljet1pt", allCutsWeighted);
  tree->Draw("jet2Pt>>hseljet2pt", allCutsWeighted);
  tree->Draw("abs(jet1Eta-jet2Eta)>>hseljetdeta", allCutsWeighted);
  tree->Draw("vbfM>>hselvbfm", allCutsWeighted);
  tree->Draw("met>>hselmet", allCutsWeighted);
  tree->Draw("abs(abs(abs(jet1Phi-jet2Phi)-TMath::Pi())-TMath::Pi())>>hseljetdphi", allCutsWeighted);
  tree->Draw("min(abs(abs(abs(metPhi-jet1Phi)-TMath::Pi())-TMath::Pi()), abs(abs(abs(metPhi-jet2Phi)-TMath::Pi())-TMath::Pi()))>>hseljmdphi", allCutsWeighted);

  ofile->cd();

  hjet1pt->Scale(weight);
  hjet2pt->Scale(weight);
  hjetdeta->Scale(weight);
  hvbfm->Scale(weight);
  hmet->Scale(weight);  
  hjetdphi->Scale(weight);
  hjmdphi->Scale(weight);
  hseljet1pt->Scale(weight);
  hseljet2pt->Scale(weight);
  hseljetdeta->Scale(weight);
  hselvbfm->Scale(weight);
  hselmet->Scale(weight);  
  hseljetdphi->Scale(weight);
  hseljmdphi->Scale(weight);

  hjet1pt->Write();
  hjet2pt->Write();
  hjetdeta->Write();
  hvbfm->Write();
  hmet->Write();
  hjetdphi->Write();
  hjmdphi->Write();
  hseljet1pt->Write();
  hseljet2pt->Write();
  hseljetdeta->Write();
  hselvbfm->Write();
  hselmet->Write();
  hseljetdphi->Write();
  hseljmdphi->Write();


  ifile->Close();
  ofile->Close();
    
}


