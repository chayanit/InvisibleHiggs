
#include "InvisibleHiggs/Analysis/interface/ProgramOptions.h"
#include "InvisibleHiggs/Analysis/interface/Cuts.h"
#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"
#include "InvisibleHiggs/Analysis/interface/StackPlot.h"
#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"
#include "InvisibleHiggs/Analysis/interface/Datasets.h"

#include "TTree.h"
#include "TMath.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TAxis.h"
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {

  TH1::SetDefaultSumw2();

  ProgramOptions options(argc, argv);

  double lumi = options.lumi;

  // input datasets
  Datasets datasets(options.iDir);
  datasets.readFile(options.datasetFile);
 
  // output file
  TFile* ofile = TFile::Open( (options.oDir+std::string("/ZMuMufromW.root")).c_str(), "RECREATE");
  std::string oDir = options.oDir + std::string("/ZmumuFromW");

  if (oDir!="") {
    boost::filesystem::path opath(oDir);
    if (!exists(opath)) {
      std::cout << "Creating output directory : " << oDir << std::endl;
      boost::filesystem::create_directory(opath);
    }
    std::cout << "Writing results to " << oDir << std::endl;
  }

  // cuts
  Cuts cuts;
  unsigned nCutsZMuMu = cuts.nCutsZMuMu();

  TCut puWeight("puWeight");
  TCut trigCorr( "(trigCorrWeight>0) ? trigCorrWeight : 1." );

  // For lepton weights
  TCut muTightWeight    = cuts.muTightWeight(options.leptCorr);
  TCut lVetoWeight  	= cuts.elVetoWeight(options.leptCorr) * cuts.muVetoWeight(options.leptCorr);

  TCut METNo2Muon130("metNo2Muon>130.");
  TCut cutLoDPhi = cuts.cut("dPhiJJ");
  TCut cutHiDPhi("vbfDPhi>2.6");
  TCut cutMe1DPhi("vbfDPhi>1.0 && vbfDPhi<=1.8");
  TCut cutMe2DPhi("vbfDPhi>1.8 && vbfDPhi<=2.6");

  // histograms
  double dphiEdges[5] = { 0., 1.0, 1.8, 2.6, TMath::Pi() };

  // Observed signal MET>130
  TH1D* hZ_DY_C_DPhi 	= new TH1D("hZ_DY_C_DPhi", "", 4, dphiEdges);  // Z+jets MC ctrl region
  TH1D* hZ_BG_ZC_DPhi 	= new TH1D("hZ_BG_ZC_DPhi", "", 4, dphiEdges);  // background MC ctrl region
  TH1D* hZ_Data_ZC_DPhi = new TH1D("hZ_Data_ZC_DPhi", "", 4, dphiEdges);  // Data ctrl region
  // Predicted from Wmunu
  TH1D* hZ_W_C_DPhi 	= new TH1D("hZ_W_C_DPhi", "", 4, dphiEdges);  	// W+jets MC ctrl region
  TH1D* hZ_BG_WC_DPhi 	= new TH1D("hZ_BG_WC_DPhi", "", 4, dphiEdges);  	// background MC ctrl region
  TH1D* hZ_Data_WC_DPhi = new TH1D("hZ_Data_WC_DPhi", "", 4, dphiEdges);  // Data W ctrl region

  TH1D* hZ_W_EffMu_D 	= new TH1D("hZ_W_EffMu_D", "", 1, 0., 1.);	// denominator of MuMu efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
  TH1D* hZ_W_EffMu_N 	= new TH1D("hZ_W_EffMu_N", "", 1, 0., 1.);	// numerator of MuMu efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples

  TH1D* hZ_DY_EffMuMu_D = new TH1D("hZ_DY_EffMuMu_D", "", 1, 0., 1.);   // denominator of MuMu efficiency from DY + DY_EWK samples
  TH1D* hZ_DY_EffMuMu_N = new TH1D("hZ_DY_EffMuMu_N", "", 1, 0., 1.);   // numerator of MuMu efficiency from DY + DY_EWK samples 
  
  TH1D* hZ_DY_EffVBFS_D = new TH1D("hZ_DY_EffVBFS_D", "", 1, 0., 1.);   // denominator of VBF(S) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
  //TH1D* hZ_DY_EffVBFS_N  = new TH1D("hZ_DY_EffVBFS_N", "", 1, 0., 1.);   // numerator of VBF(S) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples  
  TH1D* hZ_DY_EffVBFS_NLo  = new TH1D("hZ_DY_EffVBFS_NLo", "", 1, 0., 1.);   
  TH1D* hZ_DY_EffVBFS_NHi  = new TH1D("hZ_DY_EffVBFS_NHi", "", 1, 0., 1.);
  TH1D* hZ_DY_EffVBFS_NMe1 = new TH1D("hZ_DY_EffVBFS_NMe1", "", 1, 0., 1.); 
  TH1D* hZ_DY_EffVBFS_NMe2 = new TH1D("hZ_DY_EffVBFS_NMe2", "", 1, 0., 1.);  

  TH1D* hZ_W_EffVBFC_D 	   = new TH1D("hZ_W_EffVBFC_D", "", 1, 0., 1.);   // denominator of VBF(C) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
  //TH1D* hZ_W_EffVBFC_N   = new TH1D("hZ_W_EffVBFC_N", "", 1, 0., 1.);   // numerator of VBF(C) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 
  TH1D* hZ_W_EffVBFC_NLo   = new TH1D("hZ_W_EffVBFC_NLo", "", 1, 0., 1.);  
  TH1D* hZ_W_EffVBFC_NHi   = new TH1D("hZ_W_EffVBFC_NHi", "", 1, 0., 1.);
  TH1D* hZ_W_EffVBFC_NMe1  = new TH1D("hZ_W_EffVBFC_NMe1", "", 1, 0., 1.);
  TH1D* hZ_W_EffVBFC_NMe2  = new TH1D("hZ_W_EffVBFC_NMe2", "", 1, 0., 1.);

  // loop over MC datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);
    
    TCut cutD = cuts.cutDataset(dataset.name);
    TCut wWeight("");

    // bit of a fudge for mutight weight - data doesn't have the var in it, so need to add an exception (see below)
    // and then restore it for all other MC
    muTightWeight=cuts.muTightWeight(options.leptCorr);

    // check if it's DYJets
    bool isDY = false;
    bool isWJets = false;
    bool isQCD = false;
    bool isEwkW = false;
    if (dataset.name.compare(0,2,"DY")==0) {
      isDY = true;
      std::cout << "Analysing DY->ll MC : " << dataset.name << std::endl;
    }
    else if (dataset.name == "WJets" ||
        dataset.name == "W1Jets" ||
        dataset.name == "W2Jets" ||
        dataset.name == "W3Jets" ||
        dataset.name == "W4Jets" ||
        dataset.name == "EWK_Wp2Jets" ||
        dataset.name == "EWK_Wm2Jets") {

      if (dataset.name == "EWK_Wp2Jets" || dataset.name == "EWK_Wm2Jets") isEwkW = true;
      else isWJets = true;

      if(isWJets) wWeight =  cuts.wWeight();
      std::cout << "Analysing W MC     : " << dataset.name << std::endl;
    }
    else if (dataset.name.compare(0,3,"QCD")==0) {
      isQCD = true;
      std::cout << "Analysing QCD : " << dataset.name << std::endl;
    }
    else if (dataset.isData) {
      muTightWeight="";
      std::cout << "Analysing Data      : " << dataset.name << std::endl;
    }
    else {
      std::cout << "Analysing BG MC     : " << dataset.name << std::endl;
    }

    // get file & tree
    TFile* file = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

    // set up cuts
    TCut otherCuts = puWeight * trigCorr * wWeight;

    TCut cutZMuMu_C    	      = otherCuts * muTightWeight * (cutD + cuts.zMuMuVBF() + METNo2Muon130);
    TCut cutWMuNu_C    	      = otherCuts * muTightWeight * (cutD + cuts.wMuVBF() + cuts.cutWMu("MET"));
    // eps_mu from W trigger samples
    TCut cutEfficiencyMu_D    = otherCuts * (cutD + cuts.HLTandMETFilters() + cuts.wMuGen());
    TCut cutEfficiencyMu_N    = otherCuts * muTightWeight * (cutD + cuts.HLTandMETFilters() + cuts.wMuGen() + cuts.cutWMu("wMu"));

    TCut cutEfficiencyMuMu_D    = otherCuts * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen());
    TCut cutEfficiencyMuMu_N    = otherCuts * muTightWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco());
    // eps_VBFS from DY-Trig
    TCut cutEfficiencyVBFS_D    = otherCuts * muTightWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco());
    //TCut cutEfficiencyVBFS_N  = otherCuts * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon130);
    TCut cutEfficiencyVBFS_NLoDPhi  = otherCuts * muTightWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon130 + cutLoDPhi);
    TCut cutEfficiencyVBFS_NHiDPhi  = otherCuts * muTightWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon130 + cutHiDPhi);
    TCut cutEfficiencyVBFS_NMe1DPhi = otherCuts * muTightWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon130 + cutMe1DPhi);
    TCut cutEfficiencyVBFS_NMe2DPhi = otherCuts * muTightWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon130 + cutMe2DPhi);

    // eps_VBFC from W
    TCut cutEfficiencyVBFC_D    = otherCuts * muTightWeight * (cutD + cuts.HLTandMETFilters() + cuts.wMuGen() + cuts.cutWMu("wMu"));
    //TCut cutEfficiencyVBFC_N  = otherCuts * (cutD + cuts.HLTandMETFilters() + cuts.wMuGen() + cuts.cutWMu("wMu") + cuts.vbf() + cuts.cutWMu("MET")); 
    TCut cutEfficiencyVBFC_NLoDPhi  = otherCuts * muTightWeight * (cutD + cuts.HLTandMETFilters() + cuts.wMuGen() + cuts.cutWMu("wMu") + cuts.vbf() + cuts.cutWMu("MET") + cutLoDPhi);
    TCut cutEfficiencyVBFC_NHiDPhi  = otherCuts * muTightWeight * (cutD + cuts.HLTandMETFilters() + cuts.wMuGen() + cuts.cutWMu("wMu") + cuts.vbf() + cuts.cutWMu("MET") + cutHiDPhi);
    TCut cutEfficiencyVBFC_NMe1DPhi = otherCuts * muTightWeight * (cutD + cuts.HLTandMETFilters() + cuts.wMuGen() + cuts.cutWMu("wMu") + cuts.vbf() + cuts.cutWMu("MET") + cutMe1DPhi);
    TCut cutEfficiencyVBFC_NMe2DPhi = otherCuts * muTightWeight * (cutD + cuts.HLTandMETFilters() + cuts.wMuGen() + cuts.cutWMu("wMu") + cuts.vbf() + cuts.cutWMu("MET") + cutMe2DPhi);

    // fill tmp histograms for BG estimation
    //observed
    //TH1D* hZ_ZC_noDPhi  	= new TH1D("hZ_ZC_noDPhi", "", 1, 0., 1.);
    TH1D* hZ_ZC_DPhi  		= new TH1D("hZ_ZC_DPhi", "", 4, dphiEdges);  
    //predicted
    //TH1D* hZ_WC_noDPhi  	= new TH1D("hZ_WC_noDPhi", "", 1, 0., 1.);
    TH1D* hZ_WC_DPhi  		= new TH1D("hZ_WC_DPhi", "", 4, dphiEdges);  // this is for the actual BG estimation
    // fill tmp histograms for efficiency calculation
    TH1D* hZ_EffMuMu_D  	= new TH1D("hZ_EffMuMu_D",   "", 1, 0., 1.);
    TH1D* hZ_EffMuMu_N  	= new TH1D("hZ_EffMuMu_N",   "", 1, 0., 1.);
    TH1D* hZ_EffMu_D  		= new TH1D("hZ_EffMu_D",     "", 1, 0., 1.);
    TH1D* hZ_EffMu_N  		= new TH1D("hZ_EffMu_N",     "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_D  	= new TH1D("hZ_EffVBFS_D",   "", 1, 0., 1.);
    //TH1D* hZ_EffVBFS_N  	= new TH1D("hZ_EffVBFS_N",   "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_NLo  	= new TH1D("hZ_EffVBFS_NLo", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_NHi  	= new TH1D("hZ_EffVBFS_NHi", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_NMe1  	= new TH1D("hZ_EffVBFS_NMe1","", 1, 0., 1.);
    TH1D* hZ_EffVBFS_NMe2  	= new TH1D("hZ_EffVBFS_NMe2","", 1, 0., 1.);

    TH1D* hZ_EffVBFC_D  	= new TH1D("hZ_EffVBFC_D",   "", 1, 0., 1.);
    //TH1D* hZ_EffVBFC_N  	= new TH1D("hZ_EffVBFC_N",   "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_NLo  	= new TH1D("hZ_EffVBFC_NLo", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_NHi  	= new TH1D("hZ_EffVBFC_NHi", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_NMe1  	= new TH1D("hZ_EffVBFC_NMe1","", 1, 0., 1.);
    TH1D* hZ_EffVBFC_NMe2  	= new TH1D("hZ_EffVBFC_NMe2","", 1, 0., 1.);

    // W control region DY is BG
    if (isDY) { 
      //tree->Draw("0.5>>hZ_ZC_noDPhi",    cutZMuMu_C);
      tree->Draw("vbfDPhi>>hZ_ZC_DPhi",    cutZMuMu_C);

      //tree->Draw("0.5>>hZ_WC_noDPhi",    cutWMuNu_C);
      tree->Draw("vbfDPhi>>hZ_WC_DPhi",    cutWMuNu_C);
      tree->Draw("0.5>>hZ_EffMuMu_D",      cutEfficiencyMuMu_D);
      tree->Draw("0.5>>hZ_EffMuMu_N",      cutEfficiencyMuMu_N);
      tree->Draw("0.5>>hZ_EffVBFS_D",      cutEfficiencyVBFS_D);
      //tree->Draw("0.5>>hZ_EffVBFS_N",    cutEfficiencyVBFS_N);
      tree->Draw("0.5>>hZ_EffVBFS_NLo",    cutEfficiencyVBFS_NLoDPhi);
      tree->Draw("0.5>>hZ_EffVBFS_NHi",    cutEfficiencyVBFS_NHiDPhi);
      tree->Draw("0.5>>hZ_EffVBFS_NMe1",   cutEfficiencyVBFS_NMe1DPhi);
      tree->Draw("0.5>>hZ_EffVBFS_NMe2",   cutEfficiencyVBFS_NMe2DPhi);
    }
    else if(isWJets || isEwkW) {
      //tree->Draw("0.5>>hZ_WC_noDPhi",    cutWMuNu_C);
      tree->Draw("vbfDPhi>>hZ_WC_DPhi",    cutWMuNu_C);
      tree->Draw("0.5>>hZ_EffMu_D",        cutEfficiencyMu_D);  
      tree->Draw("0.5>>hZ_EffMu_N",        cutEfficiencyMu_N);    
      tree->Draw("0.5>>hZ_EffVBFC_D",      cutEfficiencyVBFC_D);
      //tree->Draw("0.5>>hZ_EffVBFC_N",    cutEfficiencyVBFC_N);
      tree->Draw("0.5>>hZ_EffVBFC_NLo",    cutEfficiencyVBFC_NLoDPhi);
      tree->Draw("0.5>>hZ_EffVBFC_NHi",    cutEfficiencyVBFC_NHiDPhi);
      tree->Draw("0.5>>hZ_EffVBFC_NMe1",   cutEfficiencyVBFC_NMe1DPhi);
      tree->Draw("0.5>>hZ_EffVBFC_NMe2",   cutEfficiencyVBFC_NMe2DPhi);
    }
    else if(isQCD) {
      //tree->Draw("0.5>>hZ_C_noDPhi",     cutWMuNu_C);
      tree->Draw("vbfDPhi>>hZ_WC_DPhi",    cutWMuNu_C);
    }
    else {
      tree->Draw("vbfDPhi>>hZ_ZC_DPhi",    cutZMuMu_C);
      tree->Draw("vbfDPhi>>hZ_WC_DPhi",    cutWMuNu_C);
    }

    // weight  to lumi
    double weight = (dataset.isData) ? 1. : lumi * dataset.sigma / dataset.nEvents;

    hZ_ZC_DPhi->Scale(weight);
    hZ_WC_DPhi->Scale(weight);
    hZ_EffVBFS_D->Scale(weight);
    //hZ_EffVBFS_N->Scale(weight);
    hZ_EffVBFS_NLo->Scale(weight);
    hZ_EffVBFS_NHi->Scale(weight);
    hZ_EffVBFS_NMe1->Scale(weight);
    hZ_EffVBFS_NMe2->Scale(weight);
    hZ_EffVBFC_D->Scale(weight);
    //hZ_EffVBFC_N->Scale(weight);
    hZ_EffVBFC_NLo->Scale(weight);
    hZ_EffVBFC_NHi->Scale(weight);
    hZ_EffVBFC_NMe1->Scale(weight);
    hZ_EffVBFC_NMe2->Scale(weight);
    hZ_EffMu_D->Scale(weight);
    hZ_EffMu_N->Scale(weight);
    hZ_EffMuMu_D->Scale(weight);
    hZ_EffMuMu_N->Scale(weight);

    // add to output histograms
    if (dataset.isData) {
      hZ_Data_ZC_DPhi->Add(hZ_ZC_DPhi);
      hZ_Data_WC_DPhi->Add(hZ_WC_DPhi);
    }
    else if (isWJets || isEwkW) {
      //hZ_W_C_noDPhi->Add(hZ_C_noDPhi);
      hZ_W_C_DPhi->Add(hZ_WC_DPhi);
      hZ_W_EffMu_D->Add(hZ_EffMu_D);
      hZ_W_EffMu_N->Add(hZ_EffMu_N);
      hZ_W_EffVBFC_D->Add(hZ_EffVBFC_D);
      //hZ_W_EffVBFC_N->Add(hZ_EffVBFC_N);
      hZ_W_EffVBFC_NLo->Add(hZ_EffVBFC_NLo);
      hZ_W_EffVBFC_NHi->Add(hZ_EffVBFC_NHi);
      hZ_W_EffVBFC_NMe1->Add(hZ_EffVBFC_NMe1);
      hZ_W_EffVBFC_NMe2->Add(hZ_EffVBFC_NMe2);
    }
    else if (isDY) {
      hZ_DY_C_DPhi->Add(hZ_ZC_DPhi);
      hZ_BG_WC_DPhi->Add(hZ_WC_DPhi);

      hZ_DY_EffMuMu_D->Add(hZ_EffMuMu_D);
      hZ_DY_EffMuMu_N->Add(hZ_EffMuMu_N);
      hZ_DY_EffVBFS_D->Add(hZ_EffVBFS_D);
      //hZ_DY_EffVBFS_N->Add(hZ_EffVBFS_N);
      hZ_DY_EffVBFS_NLo->Add(hZ_EffVBFS_NLo);
      hZ_DY_EffVBFS_NHi->Add(hZ_EffVBFS_NHi);
      hZ_DY_EffVBFS_NMe1->Add(hZ_EffVBFS_NMe1);
      hZ_DY_EffVBFS_NMe2->Add(hZ_EffVBFS_NMe2);
    }
    else if (isQCD) {
      //hZ_BG_WC_DPhi->Add(hZ_WC_DPhi);
    }
    else {
      hZ_BG_WC_DPhi->Add(hZ_WC_DPhi);
      hZ_BG_ZC_DPhi->Add(hZ_ZC_DPhi);
    }

    std::cout << "  N_Z ctrl (dphi<1.0) : " << hZ_ZC_DPhi->GetBinContent(1) << " +/- " << hZ_ZC_DPhi->GetBinError(1) << std::endl;
    std::cout << "  N_W ctrl (dphi<1.0) : " << hZ_WC_DPhi->GetBinContent(1) << " +/- " << hZ_WC_DPhi->GetBinError(1) << std::endl;
    
    delete hZ_WC_DPhi;
    delete hZ_ZC_DPhi;
    delete hZ_EffMuMu_D;
    delete hZ_EffMuMu_N;
    delete hZ_EffMu_D;
    delete hZ_EffMu_N;
    delete hZ_EffVBFS_D;
    //delete hZ_EffVBFS_N;
    delete hZ_EffVBFS_NLo;
    delete hZ_EffVBFS_NMe1;
    delete hZ_EffVBFS_NMe2;
    delete hZ_EffVBFS_NHi;
    delete hZ_EffVBFC_D;
    //delete hZ_EffVBFC_N;
    delete hZ_EffVBFC_NLo;
    delete hZ_EffVBFC_NMe1;
    delete hZ_EffVBFC_NMe2;
    delete hZ_EffVBFC_NHi;

    // clean up
    delete tree;
    file->Close();
   
  }

  // numbers - calculate these from MC in this program later!
  //double ratioBF = 5.626;  //  MCFM + NLO
  double ratioBF = 1144./14428.;

  TH1D* hZ_Est_ZC_DPhi = new TH1D("hZ_Est_ZC_DPhi", "", 4, dphiEdges); 

  // bins dPhi
  TH1D* hZ_Est_WC_DPhi = new TH1D("hZ_Est_WC_DPhi", "", 4, dphiEdges); 
  TH1D* hZ_Est_WS_DPhi = new TH1D("hZ_Est_WS_DPhi", "", 4, dphiEdges); 
  TH1D* hZ_Eff_WS_DPhi = new TH1D("hZ_Eff_WS_DPhi", "", 4, dphiEdges);
 
  TH1D* hZ_W_EffMu    = new TH1D("hZ_W_EffMu", "", 1, 0., 1.);     	// epsilon mumu
  hZ_W_EffMu->Add(hZ_W_EffMu_N);
  hZ_W_EffMu->Divide(hZ_W_EffMu_D);
  //double mu_syst = 0.025*hZ_W_EffMu->GetBinContent(1);      //2.5% Muon ID/Iso efficiency uncertainty from EWK-10-002
  //hZ_W_EffMu->SetBinError(1,TMath::Sqrt(hZ_W_EffMu->GetBinError(1)*hZ_W_EffMu->GetBinError(1) + mu_syst*mu_syst));

  TH1D* hZ_DY_EffMuMu = new TH1D("hZ_DY_EffMuMu", "", 1, 0., 1.);       // epsilon mumu
  hZ_DY_EffMuMu->Add(hZ_DY_EffMuMu_N);
  hZ_DY_EffMuMu->Divide(hZ_DY_EffMuMu_D);
  //mu_syst = 0.025*hZ_DY_EffMuMu->GetBinContent(1);      //2.5% Muon ID/Iso efficiency uncertainty from EWK-10-002
  //hZ_DY_EffMuMu->SetBinError(1,TMath::Sqrt(hZ_DY_EffMuMu->GetBinError(1)*hZ_DY_EffMuMu->GetBinError(1) + mu_syst*mu_syst));

  //TH1D* hZ_DY_EffVBFS  = new TH1D("hZ_DY_EffVBFS", "", 1, 0., 1.);  	// epsilon_s_vbf
  TH1D* hZ_DY_EffVBFSLo  = new TH1D("hZ_DY_EffVBFSLo", "", 1, 0., 1.);
  TH1D* hZ_DY_EffVBFSHi  = new TH1D("hZ_DY_EffVBFSHi", "", 1, 0., 1.);
  TH1D* hZ_DY_EffVBFSMe1 = new TH1D("hZ_DY_EffVBFSMe1", "", 1, 0., 1.);
  TH1D* hZ_DY_EffVBFSMe2 = new TH1D("hZ_DY_EffVBFSMe2", "", 1, 0., 1.);

  //TH1D* hZ_W_EffVBFC   = new TH1D("hZ_W_EffVBFC", "", 1, 0., 1.);       // epsilon_c_vbf
  TH1D* hZ_W_EffVBFCLo   = new TH1D("hZ_W_EffVBFCLo", "", 1, 0., 1.);
  TH1D* hZ_W_EffVBFCHi   = new TH1D("hZ_W_EffVBFCHi", "", 1, 0., 1.);
  TH1D* hZ_W_EffVBFCMe1  = new TH1D("hZ_W_EffVBFCMe1", "", 1, 0., 1.);
  TH1D* hZ_W_EffVBFCMe2  = new TH1D("hZ_W_EffVBFCMe2", "", 1, 0., 1.);

  //TH1D* hZ_W_RatioVBF  = new TH1D("hZ_W_RatioVBF", "", 1, 0., 1.);	// epsilon_s_vbf/epsilon_c_vbf
  TH1D* hZ_W_RatioVBFLo  = new TH1D("hZ_W_RatioVBFLo", "", 1, 0., 1.); 
  TH1D* hZ_W_RatioVBFHi  = new TH1D("hZ_W_RatioVBFHi", "", 1, 0., 1.); 
  TH1D* hZ_W_RatioVBFMe1 = new TH1D("hZ_W_RatioVBFMe1", "", 1, 0., 1.); 
  TH1D* hZ_W_RatioVBFMe2 = new TH1D("hZ_W_RatioVBFMe2", "", 1, 0., 1.); 

  //TH1D* hZ_W_TotalEff  = new TH1D("hZ_W_TotalEff", "", 1, 0., 1.); 
  TH1D* hZ_W_TotalEffLo  = new TH1D("hZ_W_TotalEffLo", "", 1, 0., 1.);
  TH1D* hZ_W_TotalEffMe1 = new TH1D("hZ_W_TotalEffMe1", "", 1, 0., 1.);
  TH1D* hZ_W_TotalEffMe2 = new TH1D("hZ_W_TotalEffMe2", "", 1, 0., 1.);
  TH1D* hZ_W_TotalEffHi  = new TH1D("hZ_W_TotalEffHi", "", 1, 0., 1.);
  
  //hZ_DY_EffVBFS->Add(hZ_DY_EffVBFS_N);
  //hZ_DY_EffVBFS->Divide(hZ_DY_EffVBFS_D);
  hZ_DY_EffVBFSLo->Add(hZ_DY_EffVBFS_NLo);
  hZ_DY_EffVBFSLo->Divide(hZ_DY_EffVBFS_D);
  hZ_DY_EffVBFSHi->Add(hZ_DY_EffVBFS_NHi);
  hZ_DY_EffVBFSHi->Divide(hZ_DY_EffVBFS_D);
  hZ_DY_EffVBFSMe1->Add(hZ_DY_EffVBFS_NMe1);
  hZ_DY_EffVBFSMe1->Divide(hZ_DY_EffVBFS_D);
  hZ_DY_EffVBFSMe2->Add(hZ_DY_EffVBFS_NMe2);
  hZ_DY_EffVBFSMe2->Divide(hZ_DY_EffVBFS_D);

  //hZ_W_EffVBFC->Add(hZ_W_EffVBFC_N);
  //hZ_W_EffVBFC->Divide(hZ_W_EffVBFC_D);
  hZ_W_EffVBFCLo->Add(hZ_W_EffVBFC_NLo);
  hZ_W_EffVBFCLo->Divide(hZ_W_EffVBFC_D);
  hZ_W_EffVBFCMe1->Add(hZ_W_EffVBFC_NMe1);
  hZ_W_EffVBFCMe1->Divide(hZ_W_EffVBFC_D);
  hZ_W_EffVBFCMe2->Add(hZ_W_EffVBFC_NMe2);
  hZ_W_EffVBFCMe2->Divide(hZ_W_EffVBFC_D);
  hZ_W_EffVBFCHi->Add(hZ_W_EffVBFC_NHi);
  hZ_W_EffVBFCHi->Divide(hZ_W_EffVBFC_D);

  //hZ_W_RatioVBF->Add(hZ_DY_EffVBFS);
  //hZ_W_RatioVBF->Divide(hZ_W_EffVBFC);
  hZ_W_RatioVBFLo->Add(hZ_DY_EffVBFSLo);
  hZ_W_RatioVBFLo->Divide(hZ_W_EffVBFCLo);
  hZ_W_RatioVBFMe1->Add(hZ_DY_EffVBFSMe1);
  hZ_W_RatioVBFMe1->Divide(hZ_W_EffVBFCMe1);
  hZ_W_RatioVBFMe2->Add(hZ_DY_EffVBFSMe2);
  hZ_W_RatioVBFMe2->Divide(hZ_W_EffVBFCMe2);
  hZ_W_RatioVBFHi->Add(hZ_DY_EffVBFSHi);
  hZ_W_RatioVBFHi->Divide(hZ_W_EffVBFCHi);

  hZ_W_TotalEffLo->Add(hZ_W_RatioVBFLo);
  hZ_W_TotalEffLo->Multiply(hZ_DY_EffMuMu);
  hZ_W_TotalEffLo->Divide(hZ_W_EffMu);
  hZ_W_TotalEffMe1->Add(hZ_W_RatioVBFMe1);
  hZ_W_TotalEffMe1->Multiply(hZ_DY_EffMuMu);
  hZ_W_TotalEffMe1->Divide(hZ_W_EffMu);
  hZ_W_TotalEffMe2->Add(hZ_W_RatioVBFMe2);
  hZ_W_TotalEffMe2->Multiply(hZ_DY_EffMuMu);
  hZ_W_TotalEffMe2->Divide(hZ_W_EffMu);
  hZ_W_TotalEffHi->Add(hZ_W_RatioVBFHi);
  hZ_W_TotalEffHi->Multiply(hZ_DY_EffMuMu);
  hZ_W_TotalEffHi->Divide(hZ_W_EffMu);

  //hZ_Eff_S_noDPhi->Add(hZ_W_RatioVBF);  
  //hZ_Eff_S_noDPhi->Divide(hZ_W_EffMu);
  hZ_Eff_WS_DPhi->SetBinContent(1,hZ_W_TotalEffLo->GetBinContent(1));
  hZ_Eff_WS_DPhi->SetBinError(1,hZ_W_TotalEffLo->GetBinError(1));
  hZ_Eff_WS_DPhi->SetBinContent(2,hZ_W_TotalEffMe1->GetBinContent(1));  
  hZ_Eff_WS_DPhi->SetBinError(2,hZ_W_TotalEffMe1->GetBinError(1));
  hZ_Eff_WS_DPhi->SetBinContent(3,hZ_W_TotalEffMe2->GetBinContent(1));
  hZ_Eff_WS_DPhi->SetBinError(3,hZ_W_TotalEffMe2->GetBinError(1));
  hZ_Eff_WS_DPhi->SetBinContent(4,hZ_W_TotalEffHi->GetBinContent(1));
  hZ_Eff_WS_DPhi->SetBinError(4,hZ_W_TotalEffHi->GetBinError(1));

  //for(int ibin = 1; ibin <= hZ_Eff_S_DPhi->GetNbinsX(); ++ibin) {
  //  hZ_Eff_S_DPhi->SetBinContent(ibin,hZ_W_TotalEff->GetBinContent(1));
  //  hZ_Eff_S_DPhi->SetBinError  (ibin,hZ_W_TotalEff->GetBinError(1));
  //}
  // Observed 
  hZ_Est_ZC_DPhi->Add(hZ_Data_ZC_DPhi, hZ_BG_ZC_DPhi, 1., -1.);
  // Predicted
  hZ_Est_WC_DPhi->Add(hZ_Data_WC_DPhi, hZ_BG_WC_DPhi, 1., -1.);
  hZ_Est_WS_DPhi->Add(hZ_Est_WC_DPhi,ratioBF);
  hZ_Est_WS_DPhi->Multiply(hZ_Eff_WS_DPhi);

  // print out

  std::cout << std::endl;
  std::cout << "##################################### Cross-check with W mu ctrl region #####################################" << std::endl;
  std::cout << std::endl;
  std::cout << "  eps_mumu by histogram  : " << hZ_DY_EffMuMu->GetBinContent(1) << " +/- " << hZ_DY_EffMuMu->GetBinError(1) << std::endl;
  std::cout << "  eps_mu by histogram    : " << hZ_W_EffMu->GetBinContent(1) << " +/- " << hZ_W_EffMu->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "dphi < 1.0" << std::endl;
  std::cout << std::endl;
  std::cout << "  eps_s_vbf by histogram : " << hZ_DY_EffVBFSLo->GetBinContent(1) << " +/- " << hZ_DY_EffVBFSLo->GetBinError(1) << std::endl;
  std::cout << "  eps_c_vbf by histogram : " << hZ_W_EffVBFCLo->GetBinContent(1) << " +/- " << hZ_W_EffVBFCLo->GetBinError(1) << std::endl;
  std::cout << "  ratio_vbf by histogram : " << hZ_W_RatioVBFLo->GetBinContent(1) << " +/- " << hZ_W_RatioVBFLo->GetBinError(1) << std::endl;
  std::cout << "  total eff by histogram : " << hZ_Eff_WS_DPhi->GetBinContent(1) << " +/- " << hZ_Eff_WS_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  W+jets MC ctrl region  : " << hZ_W_C_DPhi->GetBinContent(1) << " +/- " << hZ_W_C_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_WC_DPhi->GetBinContent(1) << " +/- " << hZ_BG_WC_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_WC_DPhi->GetBinContent(1) << " +/- " << hZ_Data_WC_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Data - BG              : " << hZ_Est_WC_DPhi->GetBinContent(1) << " +/- " << hZ_Est_WC_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted Zmumu        : " << hZ_Est_WS_DPhi->GetBinContent(1) << " +/- " << hZ_Est_WS_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Observed Zmumu         : " << hZ_Est_ZC_DPhi->GetBinContent(1) << " +/- " << hZ_Est_ZC_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "dphi > 1.0 dphi < 1.8" << std::endl;
  std::cout << std::endl;
  std::cout << "  eps_s_vbf by histogram : " << hZ_DY_EffVBFSMe1->GetBinContent(1) << " +/- " << hZ_DY_EffVBFSMe1->GetBinError(1) << std::endl;
  std::cout << "  eps_c_vbf by histogram : " << hZ_W_EffVBFCMe1->GetBinContent(1) << " +/- " << hZ_W_EffVBFCMe1->GetBinError(1) << std::endl;
  std::cout << "  ratio_vbf by histogram : " << hZ_W_RatioVBFMe1->GetBinContent(1) << " +/- " << hZ_W_RatioVBFMe1->GetBinError(1) << std::endl;
  std::cout << "  total eff by histogram : " << hZ_Eff_WS_DPhi->GetBinContent(2) << " +/- " << hZ_Eff_WS_DPhi->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "  W+jets MC ctrl region  : " << hZ_W_C_DPhi->GetBinContent(2) << " +/- " << hZ_W_C_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_WC_DPhi->GetBinContent(2) << " +/- " << hZ_BG_WC_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_WC_DPhi->GetBinContent(2) << " +/- " << hZ_Data_WC_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Data - BG     	 : " << hZ_Est_WC_DPhi->GetBinContent(2) << " +/- " << hZ_Est_WC_DPhi->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted Zmumu        : " << hZ_Est_WS_DPhi->GetBinContent(2) << " +/- " << hZ_Est_WS_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Observed Zmumu         : " << hZ_Est_ZC_DPhi->GetBinContent(2) << " +/- " << hZ_Est_ZC_DPhi->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "dphi > 1.8 dphi < 2.6" << std::endl;
  std::cout << std::endl;
  std::cout << "  eps_s_vbf by histogram : " << hZ_DY_EffVBFSMe2->GetBinContent(1) << " +/- " << hZ_DY_EffVBFSMe2->GetBinError(1) << std::endl;
  std::cout << "  eps_c_vbf by histogram : " << hZ_W_EffVBFCMe2->GetBinContent(1) << " +/- " << hZ_W_EffVBFCMe2->GetBinError(1) << std::endl;
  std::cout << "  ratio_vbf by histogram : " << hZ_W_RatioVBFMe2->GetBinContent(1) << " +/- " << hZ_W_RatioVBFMe2->GetBinError(1) << std::endl;
  std::cout << "  total eff by histogram : " << hZ_Eff_WS_DPhi->GetBinContent(3) << " +/- " << hZ_Eff_WS_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  W+jets MC ctrl region  : " << hZ_W_C_DPhi->GetBinContent(3) << " +/- " << hZ_W_C_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_WC_DPhi->GetBinContent(3) << " +/- " << hZ_BG_WC_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_WC_DPhi->GetBinContent(3) << " +/- " << hZ_Data_WC_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Data - BG       	 : " << hZ_Est_WC_DPhi->GetBinContent(3) << " +/- " << hZ_Est_WC_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted Zmumu        : " << hZ_Est_WS_DPhi->GetBinContent(3) << " +/- " << hZ_Est_WS_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Observed Zmumu       	 : " << hZ_Est_ZC_DPhi->GetBinContent(3) << " +/- " << hZ_Est_ZC_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "dphi > 2.6" << std::endl;
  std::cout << std::endl;
  std::cout << "  eps_s_vbf by histogram : " << hZ_DY_EffVBFSHi->GetBinContent(1) << " +/- " << hZ_DY_EffVBFSHi->GetBinError(1) << std::endl;
  std::cout << "  eps_c_vbf by histogram : " << hZ_W_EffVBFCHi->GetBinContent(1) << " +/- " << hZ_W_EffVBFCHi->GetBinError(1) << std::endl;
  std::cout << "  ratio_vbf by histogram : " << hZ_W_RatioVBFHi->GetBinContent(1) << " +/- " << hZ_W_RatioVBFHi->GetBinError(1) << std::endl;
  std::cout << "  total eff by histogram : " << hZ_Eff_WS_DPhi->GetBinContent(4) << " +/- " << hZ_Eff_WS_DPhi->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "  W+jets MC ctrl region  : " << hZ_W_C_DPhi->GetBinContent(4) << " +/- " << hZ_W_C_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_WC_DPhi->GetBinContent(4) << " +/- " << hZ_BG_WC_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_WC_DPhi->GetBinContent(4) << " +/- " << hZ_Data_WC_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Data - BG region       : " << hZ_Est_WC_DPhi->GetBinContent(4) << " +/- " << hZ_Est_WC_DPhi->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted Zmumu        : " << hZ_Est_WS_DPhi->GetBinContent(4) << " +/- " << hZ_Est_WS_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Observed Zmumu         : " << hZ_Est_ZC_DPhi->GetBinContent(4) << " +/- " << hZ_Est_ZC_DPhi->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "#####################################################################################" << std::endl;
  std::cout << std::endl;

  // draw control plots
  std::string pdfName;

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(111111111);

  double x1[4]  = {0.5, 1.4, 2.2, 2.6 + (TMath::Pi()-2.6)/2};
  double ex1[4] = {0.5, 0.4, 0.4, (TMath::Pi()-2.6)/2};
  double y1[4],ey1[4],y2[4],ey2[4],y3[4],ey3[4];
  double diff[4],ediff[4];
  double frac[4],efrac[4];
  double y_syst[4],e_syst[4];

  for(int i=0; i<4; ++i) {
        y1[i]  = hZ_Est_WS_DPhi->GetBinContent(i+1);     //Prediction
        ey1[i] = hZ_Est_WS_DPhi->GetBinError(i+1);
        y2[i]  = hZ_Est_ZC_DPhi->GetBinContent(i+1);     //Observation
        ey2[i] = hZ_Est_ZC_DPhi->GetBinError(i+1); 
	y3[i]  = hZ_DY_C_DPhi->GetBinContent(i+1);       //MC Prediction
        ey3[i] = hZ_DY_C_DPhi->GetBinError(i+1);

	diff[i]  = y1[i]-y2[i];
	ediff[i] = sqrt(ey1[i]*ey1[i] + ey2[i]*ey2[i]);
        y_syst[i] = 0.;
        e_syst[i] = 0.21;

	if(y1[i] > 0) frac[i]  = (y1[i]-y2[i])/y2[i];
	efrac[i] = (y1[i]/y2[i])*sqrt(pow(ey1[i]/y1[i],2) + pow(ey2[i]/y2[i],2));
  }

  TGraphErrors *graph1 = new TGraphErrors(4,x1,y1,ex1,ey1);
  TGraphErrors *graph2 = new TGraphErrors(4,x1,y2,ex1,ey2);
  TGraphErrors *graph6 = new TGraphErrors(4,x1,y3,ex1,ey3);
  TGraphErrors *graph3 = new TGraphErrors(4,x1,diff,ex1,ediff);
  TGraphErrors *graph4 = new TGraphErrors(4,x1,frac,ex1,efrac);
  TGraphErrors *graph5 = new TGraphErrors(4,x1,y_syst,ex1,e_syst);
  TH1D *h = new TH1D("h", "", 1, 0, TMath::Pi());

  TCanvas canvas;
  canvas.SetCanvasSize(canvas.GetWindowWidth(), 1.2*canvas.GetWindowHeight());

  graph1->SetTitle("");
  graph1->SetMarkerStyle(20);
  graph1->SetMarkerSize(0.9);
  graph1->SetLineColor(kRed);
  graph1->SetMarkerColor(kRed);  
  graph1->GetXaxis()->SetTitle("#Delta #phi_{jj}");
  graph1->GetXaxis()->SetRangeUser(0,TMath::Pi());
  graph1->GetYaxis()->SetTitle("N(Z#rightarrow #mu#mu)");
  graph1->GetYaxis()->SetRangeUser(0,45);
  graph1->Draw("AP");  
  graph2->SetMarkerStyle(20);
  graph2->SetMarkerSize(0.9);
  graph2->SetLineColor(kBlue);
  graph2->SetMarkerColor(kBlue);
  graph2->Draw("P same");
  graph6->SetMarkerStyle(20);
  graph6->SetMarkerSize(0.9);
  graph6->SetLineColor(kViolet);
  graph6->SetMarkerColor(kViolet);
  graph6->Draw("P same");

  TLegend leg(0.12,0.67,0.37,0.88);
  leg.SetBorderSize(0);
  leg.SetFillColor(0);
  leg.AddEntry(graph1,"predicted (data)","P");
  leg.AddEntry(graph2,"observed (data)","P");
  leg.AddEntry(graph6,"predicted (MC)","P");
  leg.Draw();

  pdfName= oDir + std::string("/Zmumu_num.pdf");
  canvas.Print(pdfName.c_str());

  h->GetXaxis()->SetTitle("#Delta #phi_{jj}");
  h->GetYaxis()->SetTitle("Predicted - Observed");
  h->GetYaxis()->SetRangeUser(-20,20);
  h->SetLineColor(kBlue);
  h->Draw();
  graph3->SetMarkerStyle(20);
  graph3->SetMarkerSize(0.9);
  graph3->SetMarkerColor(kGreen-2);
  TF1 *f1 = new TF1("f1","pol0",0,TMath::Pi());
  graph3->Fit("f1","R");
  h->Draw();
  graph3->Draw("SAMEP");

  pdfName= oDir + std::string("/Zmumu_diff.pdf");
  canvas.Print(pdfName.c_str());

  h->GetXaxis()->SetTitle("#Delta #phi_{jj}");
  h->GetYaxis()->SetTitle("#frac{Predicted - Observed}{Observed}");
  h->GetYaxis()->SetTitleOffset(1.2);
  h->GetYaxis()->SetRangeUser(-2,2);
  h->SetLineColor(kBlue);
  h->SetLineWidth(2);
  h->Draw();
  graph5->SetLineColor(kGray+2);
  graph5->SetLineWidth(0);
  graph5->SetFillColor(kGray+2);
  graph5->SetFillStyle(3002);
  graph4->SetMarkerStyle(20);
  graph4->SetMarkerSize(1.2);
  graph4->SetMarkerColor(kGreen-2);
  graph4->Fit("f1","R");
  h->Draw();
  graph5->Draw("2 same");
  graph4->Draw("P same");

  TLegend leg2(0.12,0.67,0.40,0.87);
  leg2.SetBorderSize(0);
  leg2.SetFillColor(0);
  leg2.AddEntry(f1,"pol0 fit (0 < #Delta #phi_{jj} < #pi)","l");
  leg2.AddEntry(graph5,"Systematic error","f");
  leg2.Draw();
  pdfName= oDir + std::string("/Zmumu_frac.pdf");
  canvas.Print(pdfName.c_str());

  //store histograms
  ofile->cd();
  hZ_DY_C_DPhi->Write("",TObject::kOverwrite);
  hZ_BG_ZC_DPhi->Write("",TObject::kOverwrite);
  hZ_Data_ZC_DPhi->Write("",TObject::kOverwrite);
  hZ_W_C_DPhi->Write("",TObject::kOverwrite);
  hZ_BG_WC_DPhi->Write("",TObject::kOverwrite);
  hZ_Data_WC_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_ZC_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_WC_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_WS_DPhi->Write("",TObject::kOverwrite);
  hZ_Eff_WS_DPhi->Write("",TObject::kOverwrite);
  hZ_DY_EffMuMu_D->Write("",TObject::kOverwrite);
  hZ_DY_EffMuMu_N->Write("",TObject::kOverwrite);
  hZ_W_EffMu_D->Write("",TObject::kOverwrite);
  hZ_W_EffMu_N->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFS_D->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFS_NLo->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFS_NMe1->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFS_NMe2->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFS_NHi->Write("",TObject::kOverwrite);
  hZ_W_EffVBFC_D->Write("",TObject::kOverwrite);
  hZ_W_EffVBFC_NLo->Write("",TObject::kOverwrite);
  hZ_W_EffVBFC_NMe1->Write("",TObject::kOverwrite);
  hZ_W_EffVBFC_NMe2->Write("",TObject::kOverwrite);
  hZ_W_EffVBFC_NHi->Write("",TObject::kOverwrite);
  hZ_W_EffMu->Write("",TObject::kOverwrite);
  //hZ_DY_EffVBFS->Write("",TObject::kOverwrite);
  //hZ_W_EffVBFC->Write("",TObject::kOverwrite);
  //hZ_W_RatioVBF->Write("",TObject::kOverwrite);
  //hZ_W_TotalEff->Write("",TObject::kOverwrite);
 
  ofile->Close();    

}
