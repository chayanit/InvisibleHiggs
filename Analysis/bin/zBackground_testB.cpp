
#include "InvisibleHiggs/Analysis/interface/ProgramOptions.h"
#include "InvisibleHiggs/Analysis/interface/Cuts.h"
#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"
#include "InvisibleHiggs/Analysis/interface/StackPlot.h"
#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"
#include "InvisibleHiggs/Analysis/interface/Datasets.h"

#include "TTree.h"
#include "TMath.h"
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
  TFile* ofile = TFile::Open( (options.oDir+std::string("/ZBackgroundFromW.root")).c_str(), "RECREATE");
  std::string oDir = options.oDir + std::string("/ZnunuFromW");

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

  TCut METNoMuon130("metNoMuon>130.");	// add here later for VBF efficiency when MET>35, MET>70 (QCD estimation)
  TCut METNo2Muon130("metNo2Muon>130.");
  TCut cutLoDPhi = cuts.cut("dPhiJJ");
  TCut cutHiDPhi("vbfDPhi>2.6");
  TCut cutMe1DPhi("vbfDPhi>1.0 && vbfDPhi<=1.8");
  TCut cutMe2DPhi("vbfDPhi>1.8 && vbfDPhi<=2.6");

  // histograms
  double dphiEdges[5] = { 0., 1.0, 1.8, 2.6, TMath::Pi() };
  //double metEdges[13] = { 0., 10., 20., 30., 40., 50., 60., 70., 80., 90., 100., 110., 120. };

  // signal MET>130
  //TH1D* hZ_W_C_noDPhi     = new TH1D("hZ_W_C_noDPhi", "", 1, 0., 1.);    // W+jets MC ctrl region
  //TH1D* hZ_BG_C_noDPhi    = new TH1D("hZ_BG_C_noDPhi", "", 1, 0., 1.);   // background MC ctrl region
  //TH1D* hZ_Data_C_noDPhi  = new TH1D("hZ_Data_C_noDPhi", "", 1, 0., 1.);  // Data W ctrl region

  TH1D* hZ_W_C_DPhi 	= new TH1D("hZ_W_C_DPhi", "", 4, dphiEdges);  	// W+jets MC ctrl region
  TH1D* hZ_BG_C_DPhi 	= new TH1D("hZ_BG_C_DPhi", "", 4, dphiEdges);  	// background MC ctrl region
  TH1D* hZ_Data_C_DPhi 	= new TH1D("hZ_Data_C_DPhi", "", 4, dphiEdges);  // Data W ctrl region

  TH1D* hZ_W_EffMu_D 	= new TH1D("hZ_W_EffMu_D", "", 1, 0., 1.);	// denominator of MuMu efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
  TH1D* hZ_W_EffMu_N 	= new TH1D("hZ_W_EffMu_N", "", 1, 0., 1.);	// numerator of MuMu efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
  
  TH1D* hZ_DY_EffVBFS_D = new TH1D("hZ_DY_EffVBFS_D", "", 1, 0., 1.);   // denominator of VBF(S) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
  //TH1D* hZ_DY_EffVBFS_N = new TH1D("hZ_DY_EffVBFS_N", "", 1, 0., 1.);   // numerator of VBF(S) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples  
  TH1D* hZ_DY_EffVBFS_NLo = new TH1D("hZ_DY_EffVBFS_NLo", "", 1, 0., 1.);   
  TH1D* hZ_DY_EffVBFS_NHi = new TH1D("hZ_DY_EffVBFS_NHi", "", 1, 0., 1.);
  TH1D* hZ_DY_EffVBFS_NMe1 = new TH1D("hZ_DY_EffVBFS_NMe1", "", 1, 0., 1.); 
  TH1D* hZ_DY_EffVBFS_NMe2 = new TH1D("hZ_DY_EffVBFS_NMe2", "", 1, 0., 1.);  

  TH1D* hZ_W_EffVBFC_D = new TH1D("hZ_W_EffVBFC_D", "", 1, 0., 1.);   // denominator of VBF(C) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
  //TH1D* hZ_W_EffVBFC_N = new TH1D("hZ_W_EffVBFC_N", "", 1, 0., 1.);   // numerator of VBF(C) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 
  TH1D* hZ_W_EffVBFC_NLo = new TH1D("hZ_W_EffVBFC_NLo", "", 1, 0., 1.);  
  TH1D* hZ_W_EffVBFC_NHi = new TH1D("hZ_W_EffVBFC_NHi", "", 1, 0., 1.);
  TH1D* hZ_W_EffVBFC_NMe1 = new TH1D("hZ_W_EffVBFC_NMe1", "", 1, 0., 1.);
  TH1D* hZ_W_EffVBFC_NMe2 = new TH1D("hZ_W_EffVBFC_NMe2", "", 1, 0., 1.);

  // loop over MC datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);
    
    TCut cutD = cuts.cutDataset(dataset.name);
    TCut wWeight("");

    // check if it's DYJets
    bool isDY = false;
    bool isWJets = false;
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
    else if (dataset.isData) {
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

    TCut cutWMuNu_C    	      = otherCuts * (cutD + cuts.wMuVBF() + cuts.cutWMu("MET"));
    // eps_mu from W trigger samples
    TCut cutEfficiencyMu_D    = otherCuts * (cutD + cuts.HLTandMETFilters() + cuts.wMuGen());
    TCut cutEfficiencyMu_N    = otherCuts * (cutD + cuts.HLTandMETFilters() + cuts.wMuGen() + cuts.cutWMu("wMu"));
    // eps_VBFS from DY-Trig
    TCut cutEfficiencyVBFS_D    = otherCuts * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass());
    //TCut cutEfficiencyVBFS_N    = otherCuts * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon130);
    TCut cutEfficiencyVBFS_NLoDPhi    = otherCuts * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon130 + cutLoDPhi);
    TCut cutEfficiencyVBFS_NHiDPhi    = otherCuts * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon130 + cutHiDPhi);
    TCut cutEfficiencyVBFS_NMe1DPhi    = otherCuts * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon130 + cutMe1DPhi);
    TCut cutEfficiencyVBFS_NMe2DPhi    = otherCuts * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon130 + cutMe2DPhi);

    // eps_VBFC from W
    TCut cutEfficiencyVBFC_D    = otherCuts * (cutD + cuts.HLTandMETFilters() + cuts.wMuGen() + cuts.cutWMu("wMu"));
    //TCut cutEfficiencyVBFC_N    = otherCuts * (cutD + cuts.HLTandMETFilters() + cuts.wMuGen() + cuts.cutWMu("wMu") + cuts.vbf() + cuts.cutWMu("MET")); 
    TCut cutEfficiencyVBFC_NLoDPhi	= otherCuts * (cutD + cuts.HLTandMETFilters() + cuts.wMuGen() + cuts.cutWMu("wMu") + cuts.vbf() + cuts.cutWMu("MET") + cutLoDPhi);
    TCut cutEfficiencyVBFC_NHiDPhi	= otherCuts * (cutD + cuts.HLTandMETFilters() + cuts.wMuGen() + cuts.cutWMu("wMu") + cuts.vbf() + cuts.cutWMu("MET") + cutHiDPhi);
    TCut cutEfficiencyVBFC_NMe1DPhi	= otherCuts * (cutD + cuts.HLTandMETFilters() + cuts.wMuGen() + cuts.cutWMu("wMu") + cuts.vbf() + cuts.cutWMu("MET") + cutMe1DPhi);
    TCut cutEfficiencyVBFC_NMe2DPhi  	= otherCuts * (cutD + cuts.HLTandMETFilters() + cuts.wMuGen() + cuts.cutWMu("wMu") + cuts.vbf() + cuts.cutWMu("MET") + cutMe2DPhi);

    // fill tmp histograms for BG estimation
    //TH1D* hZ_C_noDPhi  = new TH1D("hZ_C_noDPhi", "", 1, 0., 1.);
    TH1D* hZ_C_DPhi  = new TH1D("hZ_C_DPhi", "", 4, dphiEdges);  // this is for the actual BG estimation
    // fill tmp histograms for efficiency calculation
    TH1D* hZ_EffMu_D  = new TH1D("hZ_EffMu_D", "", 1, 0., 1.);
    TH1D* hZ_EffMu_N  = new TH1D("hZ_EffMu_N", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_D  = new TH1D("hZ_EffVBFS_D", "", 1, 0., 1.);
    //TH1D* hZ_EffVBFS_N  = new TH1D("hZ_EffVBFS_N", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_NLo  = new TH1D("hZ_EffVBFS_NLo", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_NHi  = new TH1D("hZ_EffVBFS_NHi", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_NMe1  = new TH1D("hZ_EffVBFS_NMe1", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_NMe2  = new TH1D("hZ_EffVBFS_NMe2", "", 1, 0., 1.);

    TH1D* hZ_EffVBFC_D  = new TH1D("hZ_EffVBFC_D", "", 1, 0., 1.);
    //TH1D* hZ_EffVBFC_N  = new TH1D("hZ_EffVBFC_N", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_NLo  = new TH1D("hZ_EffVBFC_NLo", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_NHi  = new TH1D("hZ_EffVBFC_NHi", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_NMe1  = new TH1D("hZ_EffVBFC_NMe1", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_NMe2  = new TH1D("hZ_EffVBFC_NMe2", "", 1, 0., 1.);

    // W control region DY is BG
    if (isDY) { 
      //tree->Draw("0.5>>hZ_C_noDPhi",    cutWMuNu_C);
      tree->Draw("vbfDPhi>>hZ_C_DPhi",    cutWMuNu_C);
      tree->Draw("0.5>>hZ_EffVBFS_D",     cutEfficiencyVBFS_D);
      //tree->Draw("0.5>>hZ_EffVBFS_N",     cutEfficiencyVBFS_N);
      tree->Draw("0.5>>hZ_EffVBFS_NLo",     cutEfficiencyVBFS_NLoDPhi);
      tree->Draw("0.5>>hZ_EffVBFS_NHi",     cutEfficiencyVBFS_NHiDPhi);
      tree->Draw("0.5>>hZ_EffVBFS_NMe1",     cutEfficiencyVBFS_NMe1DPhi);
      tree->Draw("0.5>>hZ_EffVBFS_NMe2",     cutEfficiencyVBFS_NMe2DPhi);
    }
    else if(isWJets || isEwkW) {
      //tree->Draw("0.5>>hZ_C_noDPhi",    cutWMuNu_C);
      tree->Draw("vbfDPhi>>hZ_C_DPhi",    cutWMuNu_C);
      tree->Draw("0.5>>hZ_EffMu_D",       cutEfficiencyMu_D);  
      tree->Draw("0.5>>hZ_EffMu_N",       cutEfficiencyMu_N);    
      tree->Draw("0.5>>hZ_EffVBFC_D",     cutEfficiencyVBFC_D);
      //tree->Draw("0.5>>hZ_EffVBFC_N",     cutEfficiencyVBFC_N);
      tree->Draw("0.5>>hZ_EffVBFC_NLo",     cutEfficiencyVBFC_NLoDPhi);
      tree->Draw("0.5>>hZ_EffVBFC_NHi",     cutEfficiencyVBFC_NHiDPhi);
      tree->Draw("0.5>>hZ_EffVBFC_NMe1",     cutEfficiencyVBFC_NMe1DPhi);
      tree->Draw("0.5>>hZ_EffVBFC_NMe2",     cutEfficiencyVBFC_NMe2DPhi);
    }
    else {
      //tree->Draw("0.5>>hZ_C_noDPhi",    cutWMuNu_C);
      tree->Draw("vbfDPhi>>hZ_C_DPhi", 	  cutWMuNu_C);
    }

    // weight  to lumi
    double weight = (dataset.isData) ? 1. : lumi * dataset.sigma / dataset.nEvents;

    //hZ_C_noDPhi->Scale(weight);
    hZ_C_DPhi->Scale(weight);
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

    // add to output histograms
    if (dataset.isData) {
      //hZ_Data_C_noDPhi->Add(hZ_C_noDPhi);
      hZ_Data_C_DPhi->Add(hZ_C_DPhi);
    }
    else if (isWJets || isEwkW) {
      //hZ_W_C_noDPhi->Add(hZ_C_noDPhi);
      hZ_W_C_DPhi->Add(hZ_C_DPhi);
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
      //hZ_BG_C_noDPhi->Add(hZ_C_noDPhi);
      hZ_BG_C_DPhi->Add(hZ_C_DPhi);
      hZ_DY_EffVBFS_D->Add(hZ_EffVBFS_D);
      //hZ_DY_EffVBFS_N->Add(hZ_EffVBFS_N);
      hZ_DY_EffVBFS_NLo->Add(hZ_EffVBFS_NLo);
      hZ_DY_EffVBFS_NHi->Add(hZ_EffVBFS_NHi);
      hZ_DY_EffVBFS_NMe1->Add(hZ_EffVBFS_NMe1);
      hZ_DY_EffVBFS_NMe2->Add(hZ_EffVBFS_NMe2);
    }
    else {
      //hZ_BG_C_noDPhi->Add(hZ_C_noDPhi);
      hZ_BG_C_DPhi->Add(hZ_C_DPhi);
    }

    std::cout << "  N ctrl (dphi<1.0) : " << hZ_C_DPhi->GetBinContent(1) << " +/- " << hZ_C_DPhi->GetBinError(1) << std::endl;  
    //std::cout << "  N ctrl (dphi>2.6) : " << hZ_C_DPhi->GetBinContent(3) << " +/- " << hZ_C_DPhi->GetBinError(3) << std::endl;
    
    //delete hZ_C_noDPhi;
    delete hZ_C_DPhi;
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
  double ratioBF = 6425./14428.;

  //no dPhi cut
  //TH1D* hZ_Est_C_noDPhi = new TH1D("hZ_Est_C_noDPhi", "", 1, 0., 1.); // estimated Z in ctrl region
  //TH1D* hZ_Est_S_noDPhi = new TH1D("hZ_Est_S_noDPhi", "", 1, 0., 1.); // estimated Z in bkgrnd region  
  //TH1D* hZ_Eff_S_noDPhi = new TH1D("hZ_Eff_S_noDPhi", "", 1, 0., 1.);

  // bins dPhi
  TH1D* hZ_Est_C_DPhi = new TH1D("hZ_Est_C_DPhi", "", 4, dphiEdges); // estimated Z in ctrl region
  TH1D* hZ_Est_S_DPhi = new TH1D("hZ_Est_S_DPhi", "", 4, dphiEdges); // estimated Z in bkgrnd region  
  TH1D* hZ_Eff_S_DPhi = new TH1D("hZ_Eff_S_DPhi", "", 4, dphiEdges);
 
  TH1D* hZ_W_EffMu    = new TH1D("hZ_W_EffMu", "", 1, 0., 1.);     	// epsilon mumu
  hZ_W_EffMu->Add(hZ_W_EffMu_N);
  hZ_W_EffMu->Divide(hZ_W_EffMu_D);
  double mu_syst = 0.025*hZ_W_EffMu->GetBinContent(1);      //2.5% Muon ID/Iso efficiency uncertainty from EWK-10-002
  hZ_W_EffMu->SetBinError(1,TMath::Sqrt(hZ_W_EffMu->GetBinError(1)*hZ_W_EffMu->GetBinError(1) + mu_syst*mu_syst));

  //TH1D* hZ_DY_EffVBFS = new TH1D("hZ_DY_EffVBFS", "", 1, 0., 1.);  	// epsilon_s_vbf
  TH1D* hZ_DY_EffVBFSLo = new TH1D("hZ_DY_EffVBFSLo", "", 1, 0., 1.);
  TH1D* hZ_DY_EffVBFSHi = new TH1D("hZ_DY_EffVBFSHi", "", 1, 0., 1.);
  TH1D* hZ_DY_EffVBFSMe1 = new TH1D("hZ_DY_EffVBFSMe1", "", 1, 0., 1.);
  TH1D* hZ_DY_EffVBFSMe2 = new TH1D("hZ_DY_EffVBFSMe2", "", 1, 0., 1.);

  //TH1D* hZ_W_EffVBFC  = new TH1D("hZ_W_EffVBFC", "", 1, 0., 1.);       // epsilon_c_vbf
  TH1D* hZ_W_EffVBFCLo  = new TH1D("hZ_W_EffVBFCLo", "", 1, 0., 1.);
  TH1D* hZ_W_EffVBFCHi  = new TH1D("hZ_W_EffVBFCHi", "", 1, 0., 1.);
  TH1D* hZ_W_EffVBFCMe1  = new TH1D("hZ_W_EffVBFCMe1", "", 1, 0., 1.);
  TH1D* hZ_W_EffVBFCMe2  = new TH1D("hZ_W_EffVBFCMe2", "", 1, 0., 1.);

  //TH1D* hZ_W_RatioVBF = new TH1D("hZ_W_RatioVBF", "", 1, 0., 1.);	// epsilon_s_vbf/epsilon_c_vbf
  TH1D* hZ_W_RatioVBFLo = new TH1D("hZ_W_RatioVBFLo", "", 1, 0., 1.); 
  TH1D* hZ_W_RatioVBFHi = new TH1D("hZ_W_RatioVBFHi", "", 1, 0., 1.); 
  TH1D* hZ_W_RatioVBFMe1 = new TH1D("hZ_W_RatioVBFMe1", "", 1, 0., 1.); 
  TH1D* hZ_W_RatioVBFMe2 = new TH1D("hZ_W_RatioVBFMe2", "", 1, 0., 1.); 

  //TH1D* hZ_W_TotalEff = new TH1D("hZ_W_TotalEff", "", 1, 0., 1.); 
  TH1D* hZ_W_TotalEffLo = new TH1D("hZ_W_TotalEffLo", "", 1, 0., 1.);
  TH1D* hZ_W_TotalEffMe1 = new TH1D("hZ_W_TotalEffMe1", "", 1, 0., 1.);
  TH1D* hZ_W_TotalEffMe2 = new TH1D("hZ_W_TotalEffMe2", "", 1, 0., 1.);
  TH1D* hZ_W_TotalEffHi = new TH1D("hZ_W_TotalEffHi", "", 1, 0., 1.);
  
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

  //hZ_DY_RatioVBF->SetBinError(1,TMath::Sqrt(hZ_DY_RatioVBF->GetBinError(1)*hZ_DY_RatioVBF->GetBinError(1) + eps_vbf_syst*eps_vbf_syst));

  hZ_W_TotalEffLo->Add(hZ_W_RatioVBFLo);
  hZ_W_TotalEffLo->Divide(hZ_W_EffMu);
  hZ_W_TotalEffMe1->Add(hZ_W_RatioVBFMe1);
  hZ_W_TotalEffMe1->Divide(hZ_W_EffMu);
  hZ_W_TotalEffMe2->Add(hZ_W_RatioVBFMe2);
  hZ_W_TotalEffMe2->Divide(hZ_W_EffMu);
  hZ_W_TotalEffHi->Add(hZ_W_RatioVBFHi);
  hZ_W_TotalEffHi->Divide(hZ_W_EffMu);

  //hZ_Eff_S_noDPhi->Add(hZ_W_RatioVBF);  
  //hZ_Eff_S_noDPhi->Divide(hZ_W_EffMu);
  hZ_Eff_S_DPhi->SetBinContent(1,hZ_W_TotalEffLo->GetBinContent(1));
  hZ_Eff_S_DPhi->SetBinError(1,hZ_W_TotalEffLo->GetBinError(1));
  hZ_Eff_S_DPhi->SetBinContent(2,hZ_W_TotalEffMe1->GetBinContent(1));  
  hZ_Eff_S_DPhi->SetBinError(2,hZ_W_TotalEffMe1->GetBinError(1));
  hZ_Eff_S_DPhi->SetBinContent(3,hZ_W_TotalEffMe2->GetBinContent(1));
  hZ_Eff_S_DPhi->SetBinError(3,hZ_W_TotalEffMe2->GetBinError(1));
  hZ_Eff_S_DPhi->SetBinContent(4,hZ_W_TotalEffHi->GetBinContent(1));
  hZ_Eff_S_DPhi->SetBinError(4,hZ_W_TotalEffHi->GetBinError(1));

  //for(int ibin = 1; ibin <= hZ_Eff_S_DPhi->GetNbinsX(); ++ibin) {
  //  hZ_Eff_S_DPhi->SetBinContent(ibin,hZ_W_TotalEff->GetBinContent(1));
  //  hZ_Eff_S_DPhi->SetBinError  (ibin,hZ_W_TotalEff->GetBinError(1));
  //}
  // no dPhi
  //hZ_Est_C_noDPhi->Add(hZ_Data_C_noDPhi, hZ_BG_C_noDPhi, 1., -1.);
  //hZ_Est_S_noDPhi->Add(hZ_Est_C_noDPhi,ratioBF);
  //hZ_Est_S_noDPhi->Multiply(hZ_Eff_S_noDPhi);
  // bins dPhi
  hZ_Est_C_DPhi->Add(hZ_Data_C_DPhi, hZ_BG_C_DPhi, 1., -1.);
  hZ_Est_S_DPhi->Add(hZ_Est_C_DPhi,ratioBF);
  hZ_Est_S_DPhi->Multiply(hZ_Eff_S_DPhi);

  // print out

  std::cout << std::endl;
  std::cout << "##################################### Closure test with W mu ctrl region #####################################" << std::endl;
  //std::cout << "no dphi cut" << std::endl;
  //std::cout << std::endl;
  std::cout << "  eps_mu by histogram  : " << hZ_W_EffMu->GetBinContent(1) << " +/- " << hZ_W_EffMu->GetBinError(1) << std::endl;
  //std::cout << "  eps_s_vbf by histogram  : " << hZ_DY_EffVBFS->GetBinContent(1) << " +/- " << hZ_DY_EffVBFS->GetBinError(1) << std::endl;
  //std::cout << "  eps_c_vbf by histogram  : " << hZ_W_EffVBFC->GetBinContent(1) << " +/- " << hZ_W_EffVBFC->GetBinError(1) << std::endl;
  //std::cout << "  ratio_vbf by histogram : " << hZ_W_RatioVBF->GetBinContent(1) << " +/- " << hZ_W_RatioVBF->GetBinError(1) << std::endl;
  //std::cout << "  total eff by histogram : " << hZ_Eff_S_noDPhi->GetBinContent(1) << " +/- " << hZ_Eff_S_noDPhi->GetBinError(1) << std::endl;
  //std::cout << std::endl;
  //std::cout << "  W+jets MC ctrl region : " << hZ_W_C_noDPhi->GetBinContent(1) << " +/- " << hZ_W_C_noDPhi->GetBinError(1) << std::endl;
  //std::cout << "  Background ctrl region : " << hZ_BG_C_noDPhi->GetBinContent(1) << " +/- " << hZ_BG_C_noDPhi->GetBinError(1) << std::endl;
  //std::cout << "  Data ctrl region       : " << hZ_Data_C_noDPhi->GetBinContent(1) << " +/- " << hZ_Data_C_noDPhi->GetBinError(1) << std::endl;
  //std::cout << std::endl;
  //std::cout << "  Data - BG in ctrl region : " << hZ_Est_C_noDPhi->GetBinContent(1) << " +/- " << hZ_Est_C_noDPhi->GetBinError(1) << std::endl;
  //std::cout << "  Z in sgnl region       : " << hZ_Est_S_noDPhi->GetBinContent(1) << " +/- " << hZ_Est_S_noDPhi->GetBinError(1) << std::endl;
  //std::cout << std::endl;
  std::cout << "dphi < 1.0" << std::endl;
  std::cout << std::endl;
  std::cout << "  eps_s_vbf by histogram : " << hZ_DY_EffVBFSLo->GetBinContent(1) << " +/- " << hZ_DY_EffVBFSLo->GetBinError(1) << std::endl;
  std::cout << "  eps_c_vbf by histogram : " << hZ_W_EffVBFCLo->GetBinContent(1) << " +/- " << hZ_W_EffVBFCLo->GetBinError(1) << std::endl;
  std::cout << "  ratio_vbf by histogram : " << hZ_W_RatioVBFLo->GetBinContent(1) << " +/- " << hZ_W_RatioVBFLo->GetBinError(1) << std::endl;
  std::cout << "  total eff by histogram : " << hZ_Eff_S_DPhi->GetBinContent(1) << " +/- " << hZ_Eff_S_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  W+jets MC ctrl region  : " << hZ_W_C_DPhi->GetBinContent(1) << " +/- " << hZ_W_C_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_C_DPhi->GetBinContent(1) << " +/- " << hZ_BG_C_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_C_DPhi->GetBinContent(1) << " +/- " << hZ_Data_C_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  Data - BG ctrl region  : " << hZ_Est_C_DPhi->GetBinContent(1) << " +/- " << hZ_Est_C_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Z in sgnl region       : " << hZ_Est_S_DPhi->GetBinContent(1) << " +/- " << hZ_Est_S_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "dphi > 1.0 dphi < 1.8" << std::endl;
  std::cout << std::endl;
  std::cout << "  eps_s_vbf by histogram : " << hZ_DY_EffVBFSMe1->GetBinContent(1) << " +/- " << hZ_DY_EffVBFSMe1->GetBinError(1) << std::endl;
  std::cout << "  eps_c_vbf by histogram : " << hZ_W_EffVBFCMe1->GetBinContent(1) << " +/- " << hZ_W_EffVBFCMe1->GetBinError(1) << std::endl;
  std::cout << "  ratio_vbf by histogram : " << hZ_W_RatioVBFMe1->GetBinContent(1) << " +/- " << hZ_W_RatioVBFMe1->GetBinError(1) << std::endl;
  std::cout << "  total eff by histogram : " << hZ_Eff_S_DPhi->GetBinContent(2) << " +/- " << hZ_Eff_S_DPhi->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "  W+jets MC ctrl region  : " << hZ_W_C_DPhi->GetBinContent(2) << " +/- " << hZ_W_C_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_C_DPhi->GetBinContent(2) << " +/- " << hZ_BG_C_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_C_DPhi->GetBinContent(2) << " +/- " << hZ_Data_C_DPhi->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "  Data - BG region       : " << hZ_Est_C_DPhi->GetBinContent(2) << " +/- " << hZ_Est_C_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Z in sgnl region       : " << hZ_Est_S_DPhi->GetBinContent(2) << " +/- " << hZ_Est_S_DPhi->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "dphi > 1.8 dphi < 2.6" << std::endl;
  std::cout << std::endl;
  std::cout << "  eps_s_vbf by histogram : " << hZ_DY_EffVBFSMe2->GetBinContent(1) << " +/- " << hZ_DY_EffVBFSMe2->GetBinError(1) << std::endl;
  std::cout << "  eps_c_vbf by histogram : " << hZ_W_EffVBFCMe2->GetBinContent(1) << " +/- " << hZ_W_EffVBFCMe2->GetBinError(1) << std::endl;
  std::cout << "  ratio_vbf by histogram : " << hZ_W_RatioVBFMe2->GetBinContent(1) << " +/- " << hZ_W_RatioVBFMe2->GetBinError(1) << std::endl;
  std::cout << "  total eff by histogram : " << hZ_Eff_S_DPhi->GetBinContent(3) << " +/- " << hZ_Eff_S_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  W+jets MC ctrl region  : " << hZ_W_C_DPhi->GetBinContent(3) << " +/- " << hZ_W_C_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_C_DPhi->GetBinContent(3) << " +/- " << hZ_BG_C_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_C_DPhi->GetBinContent(3) << " +/- " << hZ_Data_C_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  Data - BG region       : " << hZ_Est_C_DPhi->GetBinContent(3) << " +/- " << hZ_Est_C_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Z in sgnl region       : " << hZ_Est_S_DPhi->GetBinContent(3) << " +/- " << hZ_Est_S_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "dphi > 2.6" << std::endl;
  std::cout << std::endl;
  std::cout << "  eps_s_vbf by histogram : " << hZ_DY_EffVBFSHi->GetBinContent(1) << " +/- " << hZ_DY_EffVBFSHi->GetBinError(1) << std::endl;
  std::cout << "  eps_c_vbf by histogram : " << hZ_W_EffVBFCHi->GetBinContent(1) << " +/- " << hZ_W_EffVBFCHi->GetBinError(1) << std::endl;
  std::cout << "  ratio_vbf by histogram : " << hZ_W_RatioVBFHi->GetBinContent(1) << " +/- " << hZ_W_RatioVBFHi->GetBinError(1) << std::endl;
  std::cout << "  total eff by histogram : " << hZ_Eff_S_DPhi->GetBinContent(4) << " +/- " << hZ_Eff_S_DPhi->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "  W+jets MC ctrl region  : " << hZ_W_C_DPhi->GetBinContent(4) << " +/- " << hZ_W_C_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_C_DPhi->GetBinContent(4) << " +/- " << hZ_BG_C_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_C_DPhi->GetBinContent(4) << " +/- " << hZ_Data_C_DPhi->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "  Data - BG region       : " << hZ_Est_C_DPhi->GetBinContent(4) << " +/- " << hZ_Est_C_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Z in sgnl region       : " << hZ_Est_S_DPhi->GetBinContent(4) << " +/- " << hZ_Est_S_DPhi->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "#####################################################################################" << std::endl;
  std::cout << std::endl << std::endl;

  // get data-driven background estimates
  std::cout << "Reading Z backgrounds from : " << options.oDir+std::string("/ZBackgroundBins.root") << std::endl;
  TFile* zfile = TFile::Open( (options.oDir+std::string("/ZBackgroundBins.root")).c_str(), "READ");
  TH1D* hZ_Est_ZS_DPhi  = (TH1D*) zfile->Get("hZ_Est_S_DPhi");

  if (hZ_Est_ZS_DPhi == 0) {
    std::cerr << "Could not read Z background histogram" << std::endl;
    std::exit(1);
  }

  // draw control plots
  std::string pdfName;
  
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(111111111);
  
  double x1[4]  = {0.5, 1.4, 2.2, 2.6 + (TMath::Pi()-2.6)/2};
  double ex1[4] = {0.5, 0.4, 0.4, (TMath::Pi()-2.6)/2};
  double y1[4],ey1[4],y2[4],ey2[4];
  double diff[4],ediff[4];
  
  for(int i=0; i<4; ++i) {
	// Zmumu prediction
        y1[i]  = hZ_Est_ZS_DPhi->GetBinContent(i+1);
        ey1[i] = hZ_Est_ZS_DPhi->GetBinError(i+1);
	// Wmunu prediction
        y2[i]  = hZ_Est_S_DPhi->GetBinContent(i+1);
        ey2[i] = hZ_Est_S_DPhi->GetBinError(i+1);
  
        diff[i]  = y2[i]-y1[i];
        ediff[i] = sqrt(ey1[i]*ey1[i] + ey2[i]*ey2[i]);
  }

  TGraphErrors *graph1 = new TGraphErrors(4,x1,y1,ex1,ey1);
  TGraphErrors *graph2 = new TGraphErrors(4,x1,y2,ex1,ey2);
  TGraphErrors *graph3 = new TGraphErrors(4,x1,diff,ex1,ediff);
  TH1D *h = new TH1D("h", "", 1, 0, TMath::Pi());

  TCanvas canvas;
  canvas.SetCanvasSize(canvas.GetWindowWidth(), 1.2*canvas.GetWindowHeight());

  graph1->SetTitle("Yield comparison");
  graph1->SetMarkerStyle(20);
  graph1->SetMarkerSize(0.9);
  graph1->SetLineColor(kRed);
  graph1->SetMarkerColor(kRed);
  graph1->GetXaxis()->SetTitle("#Delta #phi_{jj}");
  graph1->GetXaxis()->SetRangeUser(0,TMath::Pi());
  graph1->GetYaxis()->SetTitle("N(Z#rightarrow #nu#nu)");
  graph1->GetYaxis()->SetRangeUser(0,300);
  graph1->Draw("AP");
  graph2->SetMarkerStyle(20);
  graph2->SetMarkerSize(0.9);
  graph2->SetLineColor(kBlue);
  graph2->SetMarkerColor(kBlue);
  graph2->Draw("P same");

  TLegend leg(0.12,0.67,0.42,0.87);
  leg.SetBorderSize(0);
  leg.SetFillColor(0);
  leg.AddEntry(graph1,"Z#rightarrow #mu#mu prediction","P");
  leg.AddEntry(graph2,"W#rightarrow #mu#nu prediction","P");
  leg.Draw();

  pdfName= oDir + std::string("/Znunu_num.pdf");
  canvas.Print(pdfName.c_str());


  h->GetXaxis()->SetTitle("#Delta #phi_{jj}");
  h->GetYaxis()->SetTitle("N(Z#rightarrow #nu#nu)_{W} - N(Z#rightarrow #nu#nu)_{Z}");
  h->GetYaxis()->SetRangeUser(-150,150);
  h->SetLineColor(kBlue);
  h->Draw();
  graph3->SetMarkerStyle(20);
  graph3->SetMarkerSize(0.9);
  graph3->SetMarkerColor(kGreen-2);
  graph3->Fit("pol0");
  h->Draw();
  graph3->Draw("SAMEP");

  pdfName= oDir + std::string("/Znunu_diff.pdf");
  canvas.Print(pdfName.c_str());

  //store histograms
  ofile->cd();
  hZ_W_C_DPhi->Write("",TObject::kOverwrite);
  hZ_BG_C_DPhi->Write("",TObject::kOverwrite);
  hZ_Data_C_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_C_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_S_DPhi->Write("",TObject::kOverwrite);
  hZ_Eff_S_DPhi->Write("",TObject::kOverwrite);
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
  hZ_DY_EffVBFSLo->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFSMe1->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFSMe2->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFSHi->Write("",TObject::kOverwrite);
  hZ_W_EffVBFCLo->Write("",TObject::kOverwrite);
  hZ_W_EffVBFCMe1->Write("",TObject::kOverwrite);
  hZ_W_EffVBFCMe2->Write("",TObject::kOverwrite);
  hZ_W_EffVBFCHi->Write("",TObject::kOverwrite);
  hZ_W_RatioVBFLo->Write("",TObject::kOverwrite);
  hZ_W_RatioVBFMe1->Write("",TObject::kOverwrite);
  hZ_W_RatioVBFMe2->Write("",TObject::kOverwrite);
  hZ_W_RatioVBFHi->Write("",TObject::kOverwrite);

  ofile->Close();    

}
