
#include "InvisibleHiggs/Analysis/interface/ProgramOptions.h"
#include "InvisibleHiggs/Analysis/interface/Cuts.h"
#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"
#include "InvisibleHiggs/Analysis/interface/StackPlot.h"
#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"
#include "InvisibleHiggs/Analysis/interface/Datasets.h"
#include "InvisibleHiggs/Analysis/interface/LeptonWeights.h"
#include "InvisibleHiggs/Analysis/interface/Constants.h"


#include "TTree.h"
#include "TMath.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TF1.h"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {

  TH1::SetDefaultSumw2();

  ProgramOptions options(argc, argv);

  double lumi = options.lumi;

  std::cout << "Integrated luminosity : " << lumi << " pb-1" << std::endl;

  Datasets datasets(options.iDir);
  datasets.readFile(options.datasetFile);

  LeptonWeights lw;

  // cuts
  Cuts cuts;
  TCut puWeight("puWeight");
  TCut trigCorr( "(trigCorrWeight>0) ? trigCorrWeight : 1." );
  TCut otherCuts = puWeight * trigCorr;

  TCut cutSignalNoMETNoDPhi = cuts.HLTandMETFilters() + cuts.leptonVeto() + cuts.vbf();
  
  // Tau cuts
  // Get puWeight etc added below if necessary
  // standard TightMjj selection - essentially signal selection but no DPhiJJ and no cjv
  TCut cutTightMjj_basic = cuts.HLTandMETFilters() + cuts.cutWTau("lVeto") + cuts.cutWTau("dijet") + cuts.cutWTau("dEtaJJ") + cuts.cutWTau("MET") + cuts.cutWTau("Mjj"); 
  TCut cutTightMjj(""); // used to add in PU, trig corr, wWeight etc

  // double dphiEdges[4] = { 0., 1.0, 2.6, TMath::Pi() };
  double dphiEdges[5] = { 0., 1.0, 1.8, 2.6, TMath::Pi() };

  // WMu histograms
  TH1D* hWMu_MCC_DPhi        = new TH1D("hWMu_MCC_DPhi", "", 4, dphiEdges);  // W+jets MC mu ctrl region
  TH1D* hWMu_MCGen_DPhi      = new TH1D("hWMu_MCGen_DPhi", "", 1, 0,1);   // W+jets MC gen level mu
  
  TH1D* hWMu_BGC_DPhi        = new TH1D("hWMu_BGC_DPhi", "", 4, dphiEdges);  // background MC ctrl region
  TH1D* hWMu_DataC_DPhi      = new TH1D("hWMu_DataC_DPhi", "", 4, dphiEdges);  // Data ctrl region
  
  // WTau histograms
  TH1D* hWTau_MCC_DPhi        = new TH1D("hWTau_MCC_DPhi", "", 4, dphiEdges);  // W+jets MC tau ctrl region
  TH1D* hWTau_MCGen_DPhi      = new TH1D("hWTau_MCGen_DPhi", "", 1, 0,1);   // W+jets MC gen level Tau
  
  TH1D* hWTau_BGC_DPhi        = new TH1D("hWTau_BGC_DPhi", "", 4, dphiEdges);  // background MC ctrl region
  TH1D* hWTau_DataC_DPhi      = new TH1D("hWTau_DataC_DPhi", "", 4, dphiEdges);  // Data ctrl region
  
  // loop over MC datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);

    TFile* file = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

    // setup cuts
    TCut wWeight("");

    TCut cutD = cuts.cutDataset(dataset.name);

    TCut cutWMu_MCC(""), cutWMu_Gen(""), cutWMu_C("");
    TCut cutWTau_MCC(""), cutWTau_Gen(""), cutWTau_C("");

    // tmp histograms - WMu
    TH1D* hWMu_MCC_DPhi_tmp   = new TH1D("hWMu_MCC_DPhi_tmp", "", 4, dphiEdges);  // W+jets MC ctrl region
    TH1D* hWMu_MCGen_DPhi_tmp = new TH1D("hWMu_MCGen_DPhi_tmp", "", 1, 0,1);  // W+jets MC gen
    TH1D* hWMu_DataC_DPhi_tmp = new TH1D("hWMu_DataC_DPhi_tmp","",4,dphiEdges);
    TH1D* hWMu_BGC_DPhi_tmp   = new TH1D("hWMu_BGC_DPhi_tmp","",4,dphiEdges);

    // tmp histograms - wTau
    TH1D* hWTau_MCC_DPhi_tmp   = new TH1D("hWTau_MCC_DPhi_tmp", "", 4, dphiEdges);  // W+jets MC ctrl region
    TH1D* hWTau_MCGen_DPhi_tmp = new TH1D("hWTau_MCGen_DPhi_tmp", "", 1, 0,1);  // W+jets MC gen
    TH1D* hWTau_DataC_DPhi_tmp = new TH1D("hWTau_DataC_DPhi_tmp","",4,dphiEdges);
    TH1D* hWTau_BGC_DPhi_tmp   = new TH1D("hWTau_BGC_DPhi_tmp","",4,dphiEdges);

    // Weight to lumi
    double weight = (dataset.isData) ? 1. : (lumi * dataset.sigma / dataset.nEvents);
    std::cout << "  weight : " << weight << std::endl;

    // check it's  W+Jets
    bool isWJets = false;
    bool isEwkW  = false;
    if (dataset.name == "WJets" ||
        dataset.name == "W1Jets" || 
        dataset.name == "W2Jets" || 
        dataset.name == "W3Jets" || 
        dataset.name == "W4Jets" ||
        dataset.name == "EWK_Wp2Jets" ||
        dataset.name == "EWK_Wm2Jets") {

      std::cout << "Analysing W MC     : " << dataset.name << std::endl;

      if (dataset.name == "EWK_Wp2Jets" || dataset.name == "EWK_Wm2Jets") isEwkW = true;
      else isWJets = true;

      if(isWJets) wWeight =  cuts.wWeight();

      // Mu cuts
      // cutWMu_C   = otherCuts * wWeight * (cuts.wMuVBF() + cuts.cutWMu("MET"));
      cutWMu_MCC = otherCuts * wWeight * (cuts.wMuGen() + cuts.wMuVBF() + cuts.cutWMu("MET"));
      cutWMu_Gen = otherCuts * wWeight * cuts.wMuGen() ;
      
      tree->Draw("vbfDPhi>>hWMu_MCC_DPhi_tmp", cutWMu_MCC);
      tree->Draw("0.5>>hWMu_MCGen_DPhi_tmp", cutWMu_Gen);

      hWMu_MCC_DPhi_tmp->Scale(weight);
      hWMu_MCGen_DPhi_tmp->Scale(weight);
      
      // add to final histogram
      hWMu_MCC_DPhi->Add(hWMu_MCC_DPhi_tmp);
      hWMu_MCGen_DPhi->Add(hWMu_MCGen_DPhi_tmp);

      // Tau cuts
      cutWTau_C   = otherCuts * wWeight * (cutTightMjj_basic + cuts.cutWTau("wTau"));
      cutWTau_MCC = otherCuts * wWeight * (cuts.wTauGen() + cutTightMjj_basic + cuts.cutWTau("wTau"));
      cutWTau_Gen = otherCuts * wWeight * cuts.wTauGen() ;
      
      tree->Draw("vbfDPhi>>hWTau_MCC_DPhi_tmp", cutWTau_MCC);
      tree->Draw("0.5>>hWTau_MCGen_DPhi_tmp", cutWTau_Gen);

      hWTau_MCC_DPhi_tmp->Scale(weight);
      hWTau_MCGen_DPhi_tmp->Scale(weight);
      
      // add to final histogram
      hWTau_MCC_DPhi->Add(hWTau_MCC_DPhi_tmp);
      hWTau_MCGen_DPhi->Add(hWTau_MCGen_DPhi_tmp);

    } else if (dataset.isData) {
     
      std::cout << "Analysing Data     : " << dataset.name << std::endl;

      cutWMu_C = (cuts.wMuVBF() + cuts.cutWMu("MET"));
      tree->Draw("vbfDPhi>>hWMu_DataC_DPhi_tmp", cutWMu_C);
      hWMu_DataC_DPhi->Add(hWMu_DataC_DPhi_tmp);

      cutWTau_C = (cutTightMjj_basic + cuts.cutWTau("wTau"));
      tree->Draw("vbfDPhi>>hWTau_DataC_DPhi_tmp", cutWTau_C);
      hWTau_DataC_DPhi->Add(hWTau_DataC_DPhi_tmp);
    
      std::cout << "  N ctrl region (dphi<1) : " << hWTau_DataC_DPhi_tmp->GetBinContent(1) << " +/- " << hWTau_DataC_DPhi_tmp->GetBinError(1) << std::endl;// debug output
    } else {
    
      std::cout << "Analysing BG MC    : " << dataset.name << std::endl;

      if (dataset.name.compare(0,17,"SignalM125_POWHEG")!=0) {

        if(dataset.name.compare(0,3,"Zvv") != 0){ // Dont need Zvv in WMu estimates
          cutWMu_C = otherCuts * (cutD + cuts.wMuVBF() + cuts.cutWMu("MET"));
          tree->Draw("vbfDPhi>>hWMu_BGC_DPhi_tmp", cutWMu_C);
          hWMu_BGC_DPhi_tmp->Scale(weight);
          hWMu_BGC_DPhi->Add(hWMu_BGC_DPhi_tmp);
        } 

        // Count number of tau in control region in bg mc
        cutWTau_C = otherCuts * (cutD + cutTightMjj_basic + cuts.cutWTau("wTau"));
        tree->Draw("vbfDPhi>>hWTau_BGC_DPhi_tmp", cutWTau_C);
        hWTau_BGC_DPhi_tmp->Scale(weight);
        hWTau_BGC_DPhi->Add(hWTau_BGC_DPhi_tmp);

        // debug output
        std::cout << "  N ctrl region (dphi<1) : " << hWTau_BGC_DPhi_tmp->GetBinContent(1) << " +/- " << hWTau_BGC_DPhi_tmp->GetBinError(1) << std::endl;
      }
    }
        
    // debug output
    // std::cout << "  N ctrl region (dphi<1) : " << hWMu_C_DPhi->GetBinContent(1) << " +/- " << hWMu_C_DPhi->GetBinError(1) << std::endl;
    
    delete hWMu_MCC_DPhi_tmp;
    delete hWMu_MCGen_DPhi_tmp;
    delete hWMu_DataC_DPhi_tmp;
    delete hWMu_BGC_DPhi_tmp;
    delete hWTau_MCC_DPhi_tmp;
    delete hWTau_MCGen_DPhi_tmp;
    delete hWTau_DataC_DPhi_tmp;
    delete hWTau_BGC_DPhi_tmp;
    file->Close();
   
  }

  // W->mu
  TH1D* hWMu_EstC_DPhi = new TH1D("hWMu_EstC_DPhi", "", 4, dphiEdges); // n_data - n_bg in WMu control
  hWMu_EstC_DPhi->Add(hWMu_DataC_DPhi, hWMu_BGC_DPhi, 1., -1.);
  TH1D* hWMu_Eff_DPhi = new TH1D("hWMu_Eff_DPhi", "", 4, dphiEdges); // n_data - n_bg in WMu control
  // Product of eps_mu and eps_VBF
  // for(int ibin = 1; ibin <= hWMu_Eff_DPhi->GetNbinsX(); ++ibin) {
    // hWMu_Eff_DPhi->SetBinContent(ibin, hWMu_MCC_DPhi->GetBinContent(ibin)/hWMu_MCGen_DPhi->GetBinContent(1));
    // hWMu_Eff_DPhi->SetBinError(ibin, sqrt( pow(hWMu_MCC_DPhi->GetBinError(ibin)/hWMu_MCGen_DPhi->GetBinContent(1),2) + pow(hWMu_MCGen_DPhi->GetBinError(1)/hWMu_Eff_DPhi->GetBinContent(ibin),2) ) );
  // }

  // W->Tau
  TH1D* hWTau_EstC_DPhi = new TH1D("hWTau_EstC_DPhi", "", 4, dphiEdges); // n_data - n_bg in WTau control
  hWTau_EstC_DPhi->Add(hWTau_DataC_DPhi, hWTau_BGC_DPhi, 1., -1.);
  TH1D* hWTau_Eff_DPhi = new TH1D("hWMu_Eff_DPhi", "", 4, dphiEdges); // n_data - n_bg in WMu control
  // Product of eps_mu and eps_VBF
  // for(int ibin = 1; ibin <= hWTau_Eff_DPhi->GetNbinsX(); ++ibin) {
    // hWTau_Eff_DPhi->SetBinContent(ibin, hWTau_MCC_DPhi->GetBinContent(ibin)/hWTau_MCGen_DPhi->GetBinContent(1));
    // hWTau_Eff_DPhi->SetBinError(ibin, sqrt( pow(hWTau_MCC_DPhi->GetBinError(ibin)/hWTau_MCGen_DPhi->GetBinContent(1),2) + pow(hWTau_MCGen_DPhi->GetBinError(1)*hWTau_Eff_DPhi->GetBinContent(ibin),2) ) );
  // }

  TH1D* h_RGEN = new TH1D("h_RGEN","",1,0,1.);
  h_RGEN->Divide(hWMu_MCGen_DPhi,hWTau_MCGen_DPhi,1.,1.);

  TH1D* h_RVBF_DPhi = new TH1D("h_RVBF","",4,dphiEdges);
  h_RVBF_DPhi->Divide(hWTau_MCC_DPhi,hWMu_MCC_DPhi,1.,1.);

  TH1D* h_R_DPhi = new TH1D("h_R_DPhi","",4,dphiEdges);
  for (int ibin = 1; ibin <= h_RVBF_DPhi->GetNbinsX(); ++ibin){
    h_R_DPhi->SetBinContent(ibin, h_RVBF_DPhi->GetBinContent(ibin) * h_RGEN->GetBinContent(1));
    h_R_DPhi->SetBinError(ibin, pow(h_RVBF_DPhi->GetBinError(ibin) * h_RGEN->GetBinContent(1),2) + pow(h_RVBF_DPhi->GetBinContent(ibin) * h_RGEN->GetBinError(1),2) );
  }

  // lets calculate some stuff
  TH1D* hWTau_Prediction_DPhi = new TH1D("hWTau_Prediction_DPhi","",4,dphiEdges);
  hWTau_Prediction_DPhi->Add(hWMu_EstC_DPhi,1.);
  hWTau_Prediction_DPhi->Multiply(h_R_DPhi);
  // hWTau_Prediction_DPhi->Multiply(hWTau_Eff_DPhi);
  // hWTau_Prediction_DPhi->Divide(hWMu_Eff_DPhi);

  double xsecRatio = (0.1125*0.6476)/(0.1057+(0.1125*0.1741));
  hWTau_Prediction_DPhi->Scale(xsecRatio);

  std::cout << std::endl;
  std::cout << "W->mu channel (dphi<1.0)" << std::endl;
  std::cout << "  W+jets reco'd in ctrl                                  : " << hWMu_MCC_DPhi->GetBinContent(1) << " +/- " << hWMu_MCC_DPhi->GetBinError(1) << std::endl;
  std::cout << "  W+jets at gen level                                    : " << hWMu_MCGen_DPhi->GetBinContent(1) << " +/- " << hWMu_MCGen_DPhi->GetBinError(1) << std::endl;
  std::cout << "  W+jets MC eff                                          : " << hWMu_Eff_DPhi->GetBinContent(1) << " +/- " << hWMu_Eff_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Background ctrl region                                 : " << hWMu_BGC_DPhi->GetBinContent(1) << " +/- " << hWMu_BGC_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Data ctrl region                                       : " << hWMu_DataC_DPhi->GetBinContent(1) << " +/- " << hWMu_DataC_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Data-bg ctrl region                                    : " << hWMu_EstC_DPhi->GetBinContent(1) << " +/- " << hWMu_EstC_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "W->tau channel (dphi<1.0)" << std::endl;
  std::cout << "  W+jets reco'd in ctrl                                  : " << hWTau_MCC_DPhi->GetBinContent(1) << " +/- " << hWTau_MCC_DPhi->GetBinError(1) << std::endl;
  std::cout << "  W+jets at gen level                                    : " << hWTau_MCGen_DPhi->GetBinContent(1) << " +/- " << hWTau_MCGen_DPhi->GetBinError(1) << std::endl;
  std::cout << "  W+jets MC eff                                          : " << hWTau_Eff_DPhi->GetBinContent(1) << " +/- " << hWTau_Eff_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Background ctrl region                                 : " << hWTau_BGC_DPhi->GetBinContent(1) << " +/- " << hWTau_BGC_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Data ctrl region                                       : " << hWTau_DataC_DPhi->GetBinContent(1) << " +/- " << hWTau_DataC_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  WTau in ctrl region observed                           : " << hWTau_EstC_DPhi->GetBinContent(1) << " +/- " << hWTau_EstC_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  Closure test: WTau from WMu                            : " << hWTau_Prediction_DPhi->GetBinContent(1) << " +/- " << hWTau_Prediction_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "W->mu channel (1.0<dphi<1.8)" << std::endl;
  std::cout << "  W+jets reco'd in ctrl                                  : " << hWMu_MCC_DPhi->GetBinContent(2) << " +/- " << hWMu_MCC_DPhi->GetBinError(2) << std::endl;
  std::cout << "  W+jets at gen level                                    : " << hWMu_MCGen_DPhi->GetBinContent(1) << " +/- " << hWMu_MCGen_DPhi->GetBinError(1) << std::endl;
  std::cout << "  W+jets MC eff                                          : " << hWMu_Eff_DPhi->GetBinContent(2) << " +/- " << hWMu_Eff_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Background ctrl region                                 : " << hWMu_BGC_DPhi->GetBinContent(2) << " +/- " << hWMu_BGC_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Data ctrl region                                       : " << hWMu_DataC_DPhi->GetBinContent(2) << " +/- " << hWMu_DataC_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Data-bg ctrl region                                    : " << hWMu_EstC_DPhi->GetBinContent(2) << " +/- " << hWMu_EstC_DPhi->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "W->tau channel (1.0<dphi<1.8)" << std::endl;
  std::cout << "  W+jets reco'd in ctrl                                  : " << hWTau_MCC_DPhi->GetBinContent(2) << " +/- " << hWTau_MCC_DPhi->GetBinError(2) << std::endl;
  std::cout << "  W+jets at gen level                                    : " << hWTau_MCGen_DPhi->GetBinContent(1) << " +/- " << hWTau_MCGen_DPhi->GetBinError(1) << std::endl;
  std::cout << "  W+jets MC eff                                          : " << hWTau_Eff_DPhi->GetBinContent(2) << " +/- " << hWTau_Eff_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Background ctrl region                                 : " << hWTau_BGC_DPhi->GetBinContent(2) << " +/- " << hWTau_BGC_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Data ctrl region                                       : " << hWTau_DataC_DPhi->GetBinContent(2) << " +/- " << hWTau_DataC_DPhi->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "  WTau in ctrl region observed                           : " << hWTau_EstC_DPhi->GetBinContent(2) << " +/- " << hWTau_EstC_DPhi->GetBinError(2) << std::endl;
  std::cout << std::endl ;
  std::cout << "  Closure test: WTau from WMu                            : " << hWTau_Prediction_DPhi->GetBinContent(2) << " +/- " << hWTau_Prediction_DPhi->GetBinError(2) << std::endl;
  
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "W->mu channel (1.8<dphi<2.6)" << std::endl;
  std::cout << "  W+jets reco'd in ctrl                                  : " << hWMu_MCC_DPhi->GetBinContent(3) << " +/- " << hWMu_MCC_DPhi->GetBinError(3) << std::endl;
  std::cout << "  W+jets at gen level                                    : " << hWMu_MCGen_DPhi->GetBinContent(1) << " +/- " << hWMu_MCGen_DPhi->GetBinError(1) << std::endl;
  std::cout << "  W+jets MC eff                                          : " << hWMu_Eff_DPhi->GetBinContent(3) << " +/- " << hWMu_Eff_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Background ctrl region                                 : " << hWMu_BGC_DPhi->GetBinContent(3) << " +/- " << hWMu_BGC_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Data ctrl region                                       : " << hWMu_DataC_DPhi->GetBinContent(3) << " +/- " << hWMu_DataC_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Data-bg ctrl region                                    : " << hWMu_EstC_DPhi->GetBinContent(3) << " +/- " << hWMu_EstC_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "W->tau channel (1.8<dphi<2.6)" << std::endl;
  std::cout << "  W+jets reco'd in ctrl                                  : " << hWTau_MCC_DPhi->GetBinContent(3) << " +/- " << hWTau_MCC_DPhi->GetBinError(3) << std::endl;
  std::cout << "  W+jets at gen level                                    : " << hWTau_MCGen_DPhi->GetBinContent(1) << " +/- " << hWTau_MCGen_DPhi->GetBinError(1) << std::endl;
  std::cout << "  W+jets MC eff                                          : " << hWTau_Eff_DPhi->GetBinContent(3) << " +/- " << hWTau_Eff_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Background ctrl region                                 : " << hWTau_BGC_DPhi->GetBinContent(3) << " +/- " << hWTau_BGC_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Data ctrl region                                       : " << hWTau_DataC_DPhi->GetBinContent(3) << " +/- " << hWTau_DataC_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  WTau in ctrl region observed                           : " << hWTau_EstC_DPhi->GetBinContent(3) << " +/- " << hWTau_EstC_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl ;
  std::cout << "  Closure test: WTau from WMu                            : " << hWTau_Prediction_DPhi->GetBinContent(3) << " +/- " << hWTau_Prediction_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "W->mu channel (dphi>2.6)" << std::endl;
  std::cout << "  W+jets reco'd in ctrl                                  : " << hWMu_MCC_DPhi->GetBinContent(4) << " +/- " << hWMu_MCC_DPhi->GetBinError(4) << std::endl;
  std::cout << "  W+jets at gen level                                    : " << hWMu_MCGen_DPhi->GetBinContent(1) << " +/- " << hWMu_MCGen_DPhi->GetBinError(1) << std::endl;
  std::cout << "  W+jets MC eff                                          : " << hWMu_Eff_DPhi->GetBinContent(4) << " +/- " << hWMu_Eff_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Background ctrl region                                 : " << hWMu_BGC_DPhi->GetBinContent(4) << " +/- " << hWMu_BGC_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Data ctrl region                                       : " << hWMu_DataC_DPhi->GetBinContent(4) << " +/- " << hWMu_DataC_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Data-bg ctrl region                                    : " << hWMu_EstC_DPhi->GetBinContent(4) << " +/- " << hWMu_EstC_DPhi->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "W->tau channel (dphi>2.6)" << std::endl;
  std::cout << "  W+jets reco'd in ctrl                                  : " << hWTau_MCC_DPhi->GetBinContent(4) << " +/- " << hWTau_MCC_DPhi->GetBinError(4) << std::endl;
  std::cout << "  W+jets at gen level                                    : " << hWTau_MCGen_DPhi->GetBinContent(1) << " +/- " << hWTau_MCGen_DPhi->GetBinError(1) << std::endl;
  std::cout << "  W+jets MC eff                                          : " << hWTau_Eff_DPhi->GetBinContent(4) << " +/- " << hWTau_Eff_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Background ctrl region                                 : " << hWTau_BGC_DPhi->GetBinContent(4) << " +/- " << hWTau_BGC_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Data ctrl region                                       : " << hWTau_DataC_DPhi->GetBinContent(4) << " +/- " << hWTau_DataC_DPhi->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "  WTau in ctrl region observed                           : " << hWTau_EstC_DPhi->GetBinContent(4) << " +/- " << hWTau_EstC_DPhi->GetBinError(4) << std::endl;
  std::cout << std::endl ;
  std::cout << "  Closure test: WTau from WMu                            : " << hWTau_Prediction_DPhi->GetBinContent(4) << " +/- " << hWTau_Prediction_DPhi->GetBinError(4) << std::endl;
  
  // TCanvas c;
// draw control plots
  std::string pdfName;

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(111111111);

  double x1[4]  = {0.5, 1.4, 2.2, 2.6 + (TMath::Pi()-2.6)/2};
  double ex1[4] = {0.5, 0.4, 0.4, (TMath::Pi()-2.6)/2};
  double y1[4],ey1[4],y2[4],ey2[4]; // WMu closure
  double diff1[4],ediff1[4];
  double y_syst[4],e_syst[4]; // for systematic bands

  for(int i=0; i<4; ++i) {
        y1[i]  = hWTau_Prediction_DPhi->GetBinContent(i+1);  //Predicted WMu
        ey1[i] = hWTau_Prediction_DPhi->GetBinError(i+1);
        y2[i]  = hWTau_EstC_DPhi->GetBinContent(i+1);  //Observed WMu
        ey2[i] = hWTau_EstC_DPhi->GetBinError(i+1);  
        // diff1[i]  = y1[i]-y2[i];
        diff1[i]  = 1-(y2[i]/y1[i]);
        // ediff1[i] = sqrt(ey1[i]*ey1[i] + ey2[i]*ey2[i]);
        ediff1[i] = sqrt(pow(ey2[i]/y1[i],2) + pow(ey1[i]*y2[i]/(y1[i]*y1[i]),2) );
        y_syst[i] = 0.;
        // e_syst[i] = (0.08+0.05)*hWTau_Prediction_DPhi->GetBinContent(i+1); // 8% data/MC scale factor unc. 
        e_syst[i] = (0.08+0.05)*y2[i]/y1[i]; // 8% data/MC scale factor unc. 
  }
  std::cout << y1[3] << " " << y2[3] << " " << diff1[3] <<std::endl;
  TGraphErrors *gp1 = new TGraphErrors(4,x1,y1,ex1,ey1);
  TGraphErrors *gp2 = new TGraphErrors(4,x1,y2,ex1,ey2);
  TGraphErrors *gp3 = new TGraphErrors(4,x1,diff1,ex1,ediff1);
  TGraphErrors *gp4 = new TGraphErrors(4,x1,y_syst,ex1,e_syst); // systematic error bands
  TH1D *h = new TH1D("h", "", 1, 0, TMath::Pi());

  TCanvas canvas; 
  canvas.SetCanvasSize(canvas.GetWindowWidth(), 1.2*canvas.GetWindowHeight());

  gp1->SetTitle("");
  gp1->SetMarkerStyle(20);
  gp1->SetMarkerSize(0.9);
  gp1->SetLineColor(kRed);
  gp1->SetMarkerColor(kRed);
  gp1->GetXaxis()->SetTitle("#Delta #phi_{jj}");
  gp1->GetXaxis()->SetRangeUser(0,TMath::Pi());
  gp1->GetYaxis()->SetTitle("N(W#rightarrow #tau#nu)");
  gp1->GetYaxis()->SetTitleOffset(1.2);
  gp1->GetYaxis()->SetRangeUser(0,100);
  gp1->Draw("AP");
  gp2->SetMarkerStyle(20);
  gp2->SetMarkerSize(0.9);
  gp2->SetLineColor(kBlue);
  gp2->SetMarkerColor(kBlue);
  gp2->Draw("P same");

  TLegend leg(0.12,0.67,0.32,0.87);
  leg.SetBorderSize(0);
  leg.SetFillColor(0);
  leg.AddEntry(gp1,"predicted","P");
  leg.AddEntry(gp2,"observed","P");
  leg.Draw();

  pdfName= options.oDir + std::string("/Wtaunu_num.pdf");
  canvas.Print(pdfName.c_str());
 
  h->Draw();
  h->GetXaxis()->SetTitle("#Delta #phi_{jj}");
  h->GetYaxis()->SetTitle("#frac{Predicted - Observed}{Predicted}");
  h->GetYaxis()->SetTitleOffset(1.2);
  // h->GetYaxis()->SetRangeUser(-50,70);
  h->GetYaxis()->SetRangeUser(-2,2);
  h->SetLineColor(kBlue);
  h->Draw();
  gp4->SetFillColor(4);
  gp4->SetFillStyle(3002);
  gp3->SetMarkerStyle(20);
  gp3->SetMarkerSize(0.9);
  gp3->SetMarkerColor(kGreen-2);
  TF1 *f1 = new TF1("f1","pol0",0,2.6); //To do a fit in first 3 bins only
  gp3->Fit("f1","R");
  h->Draw();
  gp4->Draw("2 same");
  gp3->Draw("P same");

  TLegend leg2(0.12,0.67,0.40,0.87);
  leg2.SetBorderSize(0);
  leg2.SetFillColor(0);
  leg2.AddEntry(f1,"pol0 fit (0 < #Delta #phi_{jj} < 2.6)","l");
  leg2.AddEntry(gp4,"Systematic error","f");
  leg2.Draw();
  pdfName= options.oDir + std::string("/Wtaunu_diff.pdf");
  canvas.Print(pdfName.c_str());


}
