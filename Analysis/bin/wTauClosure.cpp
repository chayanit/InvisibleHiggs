
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
#include "TGraphAsymmErrors.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TF1.h"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <fstream>

using namespace constants;


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
  // TCut cutnoMjjnoMET = cuts.cutWMu("trigger")+cuts.cutWMu("dijet")+cuts.cutWMu("dEtaJJ")+cuts.cutWMu("CJV")+cuts.cutWMu("dPhiJJ"); //for W regions

  // double dphiEdges[4] = { 0., 1.0, 2.6, TMath::Pi() };
  double dphiEdges[5]     = { 0., 1.0, 1.8, 2.6, TMath::Pi() };
  double METEdges[5]      = { 100., 150., 250., 300., 500. };
  double CenJetEtEdges[5] = { 10., 30., 60., 100. ,150};

  // DPhiJJ histograms
  // WMu histograms
  TH1D* hWMu_MCC_DPhi    = new TH1D("hWMu_MCC_DPhi", "", 4, dphiEdges);  // W+jets MC mu ctrl region
  TH1D* hWMu_MCGen_DPhi  = new TH1D("hWMu_MCGen_DPhi", "", 1, 0,1);   // W+jets MC gen level mu
  
  TH1D* hWMu_BGC_DPhi    = new TH1D("hWMu_BGC_DPhi", "", 4, dphiEdges);  // background MC ctrl region
  TH1D* hWMu_DataC_DPhi  = new TH1D("hWMu_DataC_DPhi", "", 4, dphiEdges);  // Data ctrl region
  
  // WTau histograms
  TH1D* hWTau_MCC_DPhi   = new TH1D("hWTau_MCC_DPhi", "", 4, dphiEdges);  // W+jets MC tau ctrl region - for CJV ratio
  TH1D* hWTau_MCGen_DPhi = new TH1D("hWTau_MCGen_DPhi", "", 1, 0,1);   // W+jets MC gen level Tau - for ID 
  
  TH1D* hWTau_BGC_DPhi   = new TH1D("hWTau_BGC_DPhi", "", 4, dphiEdges);  // background MC ctrl region
  TH1D* hWTau_DataC_DPhi = new TH1D("hWTau_DataC_DPhi", "", 4, dphiEdges);  // Data ctrl region

  // MET histograms
  // WMu histograms
  TH1D* hWMu_MCC_MET    = new TH1D("hWMu_MCC_MET", "", 4, METEdges);  // W+jets MC mu ctrl region
  TH1D* hWMu_BGC_MET    = new TH1D("hWMu_BGC_MET", "", 4, METEdges);  // background MC ctrl region
  TH1D* hWMu_DataC_MET  = new TH1D("hWMu_DataC_MET", "", 4, METEdges);  // Data ctrl region
  
  // WTau histograms
  TH1D* hWTau_MCC_MET   = new TH1D("hWTau_MCC_MET", "", 4, METEdges);  // W+jets MC tau ctrl region - for CJV ratio
  TH1D* hWTau_BGC_MET   = new TH1D("hWTau_BGC_MET", "", 4, METEdges);  // background MC ctrl region
  TH1D* hWTau_DataC_MET = new TH1D("hWTau_DataC_MET", "", 4, METEdges);  // Data ctrl region  // MET histograms
 
  // Central Jet Et histograms 
  // WMu histograms
  TH1D* hWMu_MCC_CenJetEt    = new TH1D("hWMu_MCC_CenJetEt", "", 4, CenJetEtEdges);  // W+jets MC mu ctrl region
  TH1D* hWMu_BGC_CenJetEt    = new TH1D("hWMu_BGC_CenJetEt", "", 4, CenJetEtEdges);  // background MC ctrl region
  TH1D* hWMu_DataC_CenJetEt  = new TH1D("hWMu_DataC_CenJetEt", "", 4, CenJetEtEdges);  // Data ctrl region
  
  // WTau histograms
  TH1D* hWTau_MCC_CenJetEt   = new TH1D("hWTau_MCC_CenJetEt", "", 4, CenJetEtEdges);  // W+jets MC tau ctrl region - for CJV ratio
  TH1D* hWTau_BGC_CenJetEt   = new TH1D("hWTau_BGC_CenJetEt", "", 4, CenJetEtEdges);  // background MC ctrl region
  TH1D* hWTau_DataC_CenJetEt = new TH1D("hWTau_DataC_CenJetEt", "", 4, CenJetEtEdges);  // Data ctrl region

  // loop over MC datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);

    TFile* file = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

    // setup cuts
    TCut wWeight("");

    TCut cutD = cuts.cutDataset(dataset.name);

    TCut cutWMu_MCC_DPhi      = cuts.wMuGen() + cuts.wMuVBF() + cuts.cutWMu("MET");
    TCut cutWMu_C_DPhi        = cutD + cuts.wMuVBF() + cuts.cutWMu("MET"); // WMu MC control region; WMu MC gen only; control region for data/bg - all for DPhiJJ
    TCut cutWMu_Gen           = cuts.wMuGen();
    TCut cutWTau_MCC_DPhi     = cuts.wTauGen() + cutTightMjj_basic + cuts.cutWTau("wTau");
    TCut cutWTau_C_DPhi       = cutD + cutTightMjj_basic + cuts.cutWTau("wTau"); // WTau MC control region; WTau MC gen only; control region for data/bg - all for DPhiJJ
    TCut cutWTau_Gen          = cuts.wTauGen();
    
    TCut cutWMu_MCC_MET       = cuts.wMuGen() + cuts.wMuVBF() + cuts.cutWMu("dPhiJJ");
    TCut cutWMu_C_MET         = cutD + cuts.wMuVBF() + cuts.cutWMu("dPhiJJ");
    TCut cutWTau_MCC_MET      = cuts.wTauGen() + cuts.cutWTau("wTau") + cuts.cutWTau("trigger") + cuts.cutWTau("lVeto") + cuts.cutWTau("dijet") + cuts.cutWTau("dEtaJJ") + cuts.cutWTau("Mjj") + cuts.cutWTau("dPhiJJ");
    TCut cutWTau_C_MET        = cutD + cuts.cutWTau("wTau") + cuts.cutWTau("trigger") + cuts.cutWTau("lVeto") + cuts.cutWTau("dijet") + cuts.cutWTau("dEtaJJ") + cuts.cutWTau("Mjj") + cuts.cutWTau("dPhiJJ");
    
    TCut cutWMu_MCC_CenJetEt  = cuts.wMuGen() + cuts.cutWMu("trigger") + cuts.cutWMu("lVeto") + cuts.cutWMu("wMu") + cuts.cutWMu("dijet") + cuts.cutWMu("dEtaJJ") + cuts.cutWMu("Mjj") + cuts.cutWMu("MET") + cuts.cutWMu("dPhiJJ");
    TCut cutWMu_C_CenJetEt    = cutD + cuts.cutWMu("trigger") + cuts.cutWMu("lVeto") + cuts.cutWMu("wMu") + cuts.cutWMu("dijet") + cuts.cutWMu("dEtaJJ") + cuts.cutWMu("Mjj") + cuts.cutWMu("MET") + cuts.cutWMu("dPhiJJ");
    TCut cutWTau_MCC_CenJetEt = cuts.wTauGen() + cutTightMjj_basic + cuts.cutWTau("wTau") + cuts.cutWTau("dPhiJJ");
    TCut cutWTau_C_CenJetEt   = cutD + cutTightMjj_basic + cuts.cutWTau("wTau") + cuts.cutWTau("dPhiJJ");

    // TEMP HISTS - DPhiJJ
    // tmp histograms - WMu
    TH1D* hWMu_MCC_DPhi_tmp        = new TH1D("hWMu_MCC_DPhi_tmp",   "",4,dphiEdges);  // W+jets MC ctrl region
    TH1D* hWMu_MCGen_DPhi_tmp      = new TH1D("hWMu_MCGen_DPhi_tmp", "",1, 0,1);  // W+jets Mu MC gen
    TH1D* hWMu_DataC_DPhi_tmp      = new TH1D("hWMu_DataC_DPhi_tmp", "",4,dphiEdges); // Data in WMu ctrl region
    TH1D* hWMu_BGC_DPhi_tmp        = new TH1D("hWMu_BGC_DPhi_tmp",   "",4,dphiEdges); //BG in WMu ctrl region
    
    // tmp histograms - wTau
    TH1D* hWTau_MCC_DPhi_tmp       = new TH1D("hWTau_MCC_DPhi_tmp",   "",4,dphiEdges);  // W+jets MC ctrl region
    TH1D* hWTau_MCGen_DPhi_tmp     = new TH1D("hWTau_MCGen_DPhi_tmp", "",1, 0,1);  // W+jets Tau MC gen
    TH1D* hWTau_DataC_DPhi_tmp     = new TH1D("hWTau_DataC_DPhi_tmp", "",4,dphiEdges); // Data in WTau ctrl region
    TH1D* hWTau_BGC_DPhi_tmp       = new TH1D("hWTau_BGC_DPhi_tmp",   "",4,dphiEdges); // BG in WTau ctrl region
    
    // TEMP HISTS - MET
    // tmp histograms - WMu
    TH1D* hWMu_MCC_MET_tmp         = new TH1D("hWMu_MCC_MET_tmp",   "",4,METEdges);  // W+jets MC ctrl region
    TH1D* hWMu_DataC_MET_tmp       = new TH1D("hWMu_DataC_MET_tmp", "",4,METEdges);
    TH1D* hWMu_BGC_MET_tmp         = new TH1D("hWMu_BGC_MET_tmp",   "",4,METEdges);
    
    // tmp histograms - wTau
    TH1D* hWTau_MCC_MET_tmp        = new TH1D("hWTau_MCC_MET_tmp",   "",4,METEdges);  // W+jets MC ctrl region
    TH1D* hWTau_DataC_MET_tmp      = new TH1D("hWTau_DataC_MET_tmp", "",4,METEdges);
    TH1D* hWTau_BGC_MET_tmp        = new TH1D("hWTau_BGC_MET_tmp",   "",4,METEdges);
    
    // TEMP HISTS - CenJetEt
    // tmp histograms - WMu
    TH1D* hWMu_MCC_CenJetEt_tmp    = new TH1D("hWMu_MCC_CenJetEt_tmp",   "",4,CenJetEtEdges);  // W+jets MC ctrl region
    TH1D* hWMu_DataC_CenJetEt_tmp  = new TH1D("hWMu_DataC_CenJetEt_tmp", "",4,CenJetEtEdges);
    TH1D* hWMu_BGC_CenJetEt_tmp    = new TH1D("hWMu_BGC_CenJetEt_tmp",   "",4,CenJetEtEdges);
    
    // tmp histograms - wTau
    TH1D* hWTau_MCC_CenJetEt_tmp   = new TH1D("hWTau_MCC_CenJetEt_tmp",   "",4,CenJetEtEdges);  // W+jets MC ctrl region
    TH1D* hWTau_DataC_CenJetEt_tmp = new TH1D("hWTau_DataC_CenJetEt_tmp", "",4,CenJetEtEdges);
    TH1D* hWTau_BGC_CenJetEt_tmp   = new TH1D("hWTau_BGC_CenJetEt_tmp",   "",4,CenJetEtEdges);

    // Weight to lumi
    double weight = (dataset.isData) ? 1. : (lumi * dataset.sigma / dataset.nEvents);

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
      std::cout << "  weight : " << weight << std::endl;

      if (dataset.name == "EWK_Wp2Jets" || dataset.name == "EWK_Wm2Jets") isEwkW = true;
      else isWJets = true;

      if(isWJets) wWeight =  cuts.wWeight();

      // Mu cuts
      tree->Draw("vbfDPhi>>hWMu_MCC_DPhi_tmp", otherCuts * wWeight * cutWMu_MCC_DPhi);
      tree->Draw("0.5>>hWMu_MCGen_DPhi_tmp", otherCuts * wWeight * cutWMu_Gen);
      tree->Draw("metNoWLepton>>hWMu_MCC_MET_tmp", otherCuts * wWeight * cutWMu_MCC_MET);
      tree->Draw("cenJetEt>>hWMu_MCC_CenJetEt_tmp", otherCuts * wWeight * cutWMu_MCC_CenJetEt);

      hWMu_MCC_DPhi_tmp->Scale(weight);
      hWMu_MCGen_DPhi_tmp->Scale(weight);
      hWMu_MCC_MET_tmp->Scale(weight);
      hWMu_MCC_CenJetEt_tmp->Scale(weight);
      
      // add to final histogram
      hWMu_MCC_DPhi->Add(hWMu_MCC_DPhi_tmp);
      hWMu_MCGen_DPhi->Add(hWMu_MCGen_DPhi_tmp);
      hWMu_MCC_MET->Add(hWMu_MCC_MET_tmp);
      hWMu_MCC_CenJetEt->Add(hWMu_MCC_CenJetEt_tmp);

      tree->Draw("vbfDPhi>>hWTau_MCC_DPhi_tmp", otherCuts * wWeight * cutWTau_MCC_DPhi);
      tree->Draw("0.5>>hWTau_MCGen_DPhi_tmp", otherCuts * wWeight * cutWTau_Gen);
      tree->Draw("met>>hWTau_MCC_MET_tmp", otherCuts * wWeight * cutWTau_MCC_MET);
      tree->Draw("cenJetEt>>hWTau_MCC_CenJetEt_tmp", otherCuts * wWeight * cutWTau_MCC_CenJetEt);

      hWTau_MCC_DPhi_tmp->Scale(weight);
      hWTau_MCGen_DPhi_tmp->Scale(weight);
      hWTau_MCC_MET_tmp->Scale(weight);
      hWTau_MCC_CenJetEt_tmp->Scale(weight);
      
      // add to final histogram
      hWTau_MCC_DPhi->Add(hWTau_MCC_DPhi_tmp);
      hWTau_MCGen_DPhi->Add(hWTau_MCGen_DPhi_tmp);
      hWTau_MCC_MET->Add(hWTau_MCC_MET_tmp);
      hWTau_MCC_CenJetEt->Add(hWTau_MCC_CenJetEt_tmp);

    } else if (dataset.isData) {
     
      std::cout << "Analysing Data     : " << dataset.name << std::endl;

      // Mu region
      tree->Draw("vbfDPhi>>hWMu_DataC_DPhi_tmp", cutWMu_C_DPhi);
      hWMu_DataC_DPhi->Add(hWMu_DataC_DPhi_tmp);

      tree->Draw("metNoWLepton>>hWMu_DataC_MET_tmp", cutWMu_C_MET);
      hWMu_DataC_MET->Add(hWMu_DataC_MET_tmp);

      tree->Draw("cenJetEt>>hWMu_DataC_CenJetEt_tmp", cutWMu_C_CenJetEt);
      hWMu_DataC_CenJetEt->Add(hWMu_DataC_CenJetEt_tmp);

      // Tau region
      tree->Draw("vbfDPhi>>hWTau_DataC_DPhi_tmp", cutWTau_C_DPhi);
      hWTau_DataC_DPhi->Add(hWTau_DataC_DPhi_tmp);

      tree->Draw("met>>hWTau_DataC_MET_tmp", cutWTau_C_MET);
      hWTau_DataC_MET->Add(hWTau_DataC_MET_tmp);

      tree->Draw("cenJetEt>>hWTau_DataC_CenJetEt_tmp", cutWTau_C_CenJetEt);
      hWTau_DataC_CenJetEt->Add(hWTau_DataC_CenJetEt_tmp);
    
      std::cout << "  N ctrl region (dphi<1) : " << hWTau_DataC_DPhi_tmp->GetBinContent(1) << " +/- " << hWTau_DataC_DPhi_tmp->GetBinError(1) << std::endl;// debug output

    } else {
    
      std::cout << "Analysing BG MC    : " << dataset.name << std::endl;
      std::cout << "  weight : " << weight << std::endl;

      if (dataset.name.compare(0,17,"SignalM125_POWHEG")!=0 || dataset.name.compare(0,3,"QCD") != 0) {

        if(dataset.name.compare(0,3,"Zvv") != 0){ // Dont need Zvv in WMu estimates
          tree->Draw("vbfDPhi>>hWMu_BGC_DPhi_tmp", otherCuts * cutWMu_C_DPhi);
          tree->Draw("metNoWLepton>>hWMu_BGC_MET_tmp", otherCuts * cutWMu_C_MET);
          tree->Draw("cenJetEt>>hWMu_BGC_CenJetEt_tmp", otherCuts * cutWMu_C_CenJetEt);
          
          hWMu_BGC_DPhi_tmp->Scale(weight);
          hWMu_BGC_MET_tmp->Scale(weight);
          hWMu_BGC_CenJetEt_tmp->Scale(weight);
          
          hWMu_BGC_DPhi->Add(hWMu_BGC_DPhi_tmp);
          hWMu_BGC_MET->Add(hWMu_BGC_MET_tmp);
          hWMu_BGC_CenJetEt->Add(hWMu_BGC_CenJetEt_tmp);
        } 

        // Count number of tau in control region in bg mc
        tree->Draw("vbfDPhi>>hWTau_BGC_DPhi_tmp", otherCuts * cutWTau_C_DPhi);
        tree->Draw("met>>hWTau_BGC_MET_tmp", otherCuts * cutWTau_C_MET);
        tree->Draw("cenJetEt>>hWTau_BGC_CenJetEt_tmp", otherCuts * cutWTau_C_CenJetEt);

        hWTau_BGC_DPhi_tmp->Scale(weight);
        hWTau_BGC_MET_tmp->Scale(weight);
        hWTau_BGC_CenJetEt_tmp->Scale(weight);

        hWTau_BGC_DPhi->Add(hWTau_BGC_DPhi_tmp);
        hWTau_BGC_MET->Add(hWTau_BGC_MET_tmp);
        hWTau_BGC_CenJetEt->Add(hWTau_BGC_CenJetEt_tmp);

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

    delete hWMu_MCC_MET_tmp;
    delete hWMu_DataC_MET_tmp;
    delete hWMu_BGC_MET_tmp;
    
    delete hWTau_MCC_MET_tmp;
    delete hWTau_DataC_MET_tmp;
    delete hWTau_BGC_MET_tmp;

    delete hWMu_MCC_CenJetEt_tmp;
    delete hWMu_DataC_CenJetEt_tmp;
    delete hWMu_BGC_CenJetEt_tmp;
    
    delete hWTau_MCC_CenJetEt_tmp;
    delete hWTau_DataC_CenJetEt_tmp;
    delete hWTau_BGC_CenJetEt_tmp;

    file->Close();
   
  } // end of datasets loop

  for (int i=1; i<=hWTau_BGC_DPhi->GetNbinsX(); ++i) hWTau_BGC_DPhi->SetBinError(i,0.); // no stat error from mc
  for (int i=1; i<=hWMu_BGC_DPhi->GetNbinsX(); ++i) hWMu_BGC_DPhi->SetBinError(i,0.); // no stat error from mc

  for (int i=1; i<=hWTau_BGC_MET->GetNbinsX(); ++i) hWTau_BGC_MET->SetBinError(i,0.); // no stat error from mc
  for (int i=1; i<=hWMu_BGC_MET->GetNbinsX(); ++i) hWMu_BGC_MET->SetBinError(i,0.); // no stat error from mc

  for (int i=1; i<=hWTau_BGC_CenJetEt->GetNbinsX(); ++i) hWTau_BGC_CenJetEt->SetBinError(i,0.); // no stat error from mc
  for (int i=1; i<=hWMu_BGC_CenJetEt->GetNbinsX(); ++i) hWMu_BGC_CenJetEt->SetBinError(i,0.); // no stat error from mc

  // W->mu
  TH1D* hWMu_EstC_DPhi = new TH1D("hWMu_EstC_DPhi", "", 4, dphiEdges); // n_data - n_bg in WMu control
  hWMu_EstC_DPhi->Add(hWMu_DataC_DPhi, hWMu_BGC_DPhi, 1., -1.);
  TH1D* hWMu_EstC_MET = new TH1D("hWMu_EstC_MET", "", 4, METEdges); // n_data - n_bg in WMu control
  hWMu_EstC_MET->Add(hWMu_DataC_MET, hWMu_BGC_MET, 1., -1.);
  TH1D* hWMu_EstC_CenJetEt = new TH1D("hWMu_EstC_CenJetEt", "", 4, CenJetEtEdges); // n_data - n_bg in WMu control
  hWMu_EstC_CenJetEt->Add(hWMu_DataC_CenJetEt, hWMu_BGC_CenJetEt, 1., -1.);

  // W->Tau
  TH1D* hWTau_EstC_DPhi = new TH1D("hWTau_EstC_DPhi", "", 4, dphiEdges); // n_data - n_bg in WTau control
  hWTau_EstC_DPhi->Add(hWTau_DataC_DPhi, hWTau_BGC_DPhi, 1., -1.);
  TH1D* hWTau_EstC_MET = new TH1D("hWTau_EstC_MET", "", 4, METEdges); // n_data - n_bg in WTau control
  hWTau_EstC_MET->Add(hWTau_DataC_MET, hWTau_BGC_MET, 1., -1.);
  TH1D* hWTau_EstC_CenJetEt = new TH1D("hWTau_EstC_CenJetEt", "", 4, CenJetEtEdges); // n_data - n_bg in WTau control
  hWTau_EstC_CenJetEt->Add(hWTau_DataC_CenJetEt, hWTau_BGC_CenJetEt, 1., -1.);

  TH1D* h_RGEN = new TH1D("h_RGEN","",1,0,1.);
  h_RGEN->Divide(hWMu_MCGen_DPhi,hWTau_MCGen_DPhi,1.,1.);

  TH1D* h_RVBF_DPhi = new TH1D("h_RVBF_DPhi","",4,dphiEdges);
  h_RVBF_DPhi->Divide(hWTau_MCC_DPhi,hWMu_MCC_DPhi,1.,1.);
  TH1D* h_RVBF_MET = new TH1D("h_RVBF_MET","",4,METEdges);
  h_RVBF_MET->Divide(hWTau_MCC_MET,hWMu_MCC_MET,1.,1.);
  TH1D* h_RVBF_CenJetEt = new TH1D("h_RVBF_CenJetEt","",4,CenJetEtEdges);
  h_RVBF_CenJetEt->Divide(hWTau_MCC_CenJetEt,hWMu_MCC_CenJetEt,1.,1.);

  for (int i=1; i<=h_RGEN->GetNbinsX(); ++i)h_RGEN->SetBinError(i,0.); // no stat error from mc
  for (int i=1; i<=h_RVBF_DPhi->GetNbinsX(); ++i) h_RVBF_DPhi->SetBinError(i,0.); // no stat error from mc
  for (int i=1; i<=h_RVBF_MET->GetNbinsX(); ++i) h_RVBF_MET->SetBinError(i,0.); // no stat error from mc
  for (int i=1; i<=h_RVBF_CenJetEt->GetNbinsX(); ++i) h_RVBF_CenJetEt->SetBinError(i,0.); // no stat error from mc
  
  TH1D* h_R_DPhi     = new TH1D("h_R_DPhi","",4,dphiEdges);
  TH1D* h_R_MET      = new TH1D("h_R_MET","",4,METEdges);
  TH1D* h_R_CenJetEt = new TH1D("h_R_CenJetEt","",4,CenJetEtEdges);

  for (int ibin = 1; ibin <= h_RVBF_DPhi->GetNbinsX(); ++ibin){
    h_R_DPhi->SetBinContent(ibin, h_RVBF_DPhi->GetBinContent(ibin) * h_RGEN->GetBinContent(1));
    h_R_DPhi->SetBinError(ibin, 0);//pow(h_RVBF_DPhi->GetBinError(ibin) * h_RGEN->GetBinContent(1),2) + pow(h_RVBF_DPhi->GetBinContent(ibin) * h_RGEN->GetBinError(1),2) );
  }
  for (int ibin = 1; ibin <= h_RVBF_MET->GetNbinsX(); ++ibin){
    h_R_MET->SetBinContent(ibin, h_RVBF_MET->GetBinContent(ibin) * h_RGEN->GetBinContent(1));
    h_R_MET->SetBinError(ibin, 0);//pow(h_RVBF_MET->GetBinError(ibin) * h_RGEN->GetBinContent(1),2) + pow(h_RVBF_MET->GetBinContent(ibin) * h_RGEN->GetBinError(1),2) );
  }
  for (int ibin = 1; ibin <= h_RVBF_CenJetEt->GetNbinsX(); ++ibin){
    h_R_CenJetEt->SetBinContent(ibin, h_RVBF_CenJetEt->GetBinContent(ibin) * h_RGEN->GetBinContent(1));
    h_R_CenJetEt->SetBinError(ibin, 0);//pow(h_RVBF_CenJetEt->GetBinError(ibin) * h_RGEN->GetBinContent(1),2) + pow(h_RVBF_CenJetEt->GetBinContent(ibin) * h_RGEN->GetBinError(1),2) );
  }

  // lets calculate some stuff
  TH1D* hWTau_Prediction_DPhi = new TH1D("hWTau_Prediction_DPhi","",4,dphiEdges);
  hWTau_Prediction_DPhi->Add(hWMu_EstC_DPhi,1.);
  hWTau_Prediction_DPhi->Multiply(h_R_DPhi);

  TH1D* hWTau_Prediction_MET = new TH1D("hWTau_Prediction_MET","",4,METEdges);
  hWTau_Prediction_MET->Add(hWMu_EstC_MET,1.);
  hWTau_Prediction_MET->Multiply(h_R_MET);

  TH1D* hWTau_Prediction_CenJetEt = new TH1D("hWTau_Prediction_CenJetEt","",4,CenJetEtEdges);
  hWTau_Prediction_CenJetEt->Add(hWMu_EstC_CenJetEt,1.);
  hWTau_Prediction_CenJetEt->Multiply(h_R_CenJetEt);

  double xsecRatio = (0.1125*0.6476)/(0.1057+(0.1125*0.1741)); // Ratio of w->Tau cross-section to w-?mu cross section (includes w->mu and w->tau->mu)
  hWTau_Prediction_DPhi->Scale(xsecRatio);
  hWTau_Prediction_MET->Scale(xsecRatio);
  hWTau_Prediction_CenJetEt->Scale(xsecRatio);

  ///////////////////////////
  // Lots of screen output //
  ///////////////////////////
  std::cout << std::endl;
  std::cout << "***************** W Tau closure test from W Mu *****************" << std::endl;
  std::cout << std::endl;
  std::cout << "--- Binned by DPhiJJ ---" << std::endl;
  for (int a = 0; a < 4; a++){
    std::cout << std::endl;
    std::cout << "Bin: " << dphiEdges[a] << " < dphi < " << dphiEdges[a+1] << "" << std::endl;
    std::cout << "W->mu channel" << std::endl;
    std::cout << "  W+jets reco'd in ctrl                                  : " << hWMu_MCC_DPhi->GetBinContent(a+1) << " +/- " << hWMu_MCC_DPhi->GetBinError(a+1) << std::endl;
    std::cout << "  W+jets at gen level                                    : " << hWMu_MCGen_DPhi->GetBinContent(a+1) << " +/- " << hWMu_MCGen_DPhi->GetBinError(a+1) << std::endl;
    std::cout << "  Background ctrl region                                 : " << hWMu_BGC_DPhi->GetBinContent(a+1) << " +/- " << hWMu_BGC_DPhi->GetBinError(a+1) << std::endl;
    std::cout << "  Data ctrl region                                       : " << hWMu_DataC_DPhi->GetBinContent(a+1) << " +/- " << hWMu_DataC_DPhi->GetBinError(a+1) << std::endl;
    std::cout << "  Data-bg ctrl region                                    : " << hWMu_EstC_DPhi->GetBinContent(a+1) << " +/- " << hWMu_EstC_DPhi->GetBinError(a+1) << std::endl;
    std::cout << std::endl;
    std::cout << "W->tau channel" << std::endl;
    std::cout << "  W+jets reco'd in ctrl (MC estimate in ctrl)            : " << hWTau_MCC_DPhi->GetBinContent(a+1) << " +/- " << hWTau_MCC_DPhi->GetBinError(a+1) << std::endl;
    std::cout << "  W+jets at gen level                                    : " << hWTau_MCGen_DPhi->GetBinContent(a+1) << " +/- " << hWTau_MCGen_DPhi->GetBinError(a+1) << std::endl;
    std::cout << "  Background ctrl region                                 : " << hWTau_BGC_DPhi->GetBinContent(a+1) << " +/- " << hWTau_BGC_DPhi->GetBinError(a+1) << std::endl;
    std::cout << "  Data ctrl region                                       : " << hWTau_DataC_DPhi->GetBinContent(a+1) << " +/- " << hWTau_DataC_DPhi->GetBinError(a+1) << std::endl;
    std::cout << std::endl;
    std::cout << "  WTau in ctrl region observed                           : " << hWTau_EstC_DPhi->GetBinContent(a+1) << " +/- " << hWTau_EstC_DPhi->GetBinError(a+1) << std::endl;
    std::cout << std::endl;
    std::cout << "  Closure test: WTau from WMu                            : " << hWTau_Prediction_DPhi->GetBinContent(a+1) << " +/- " << hWTau_Prediction_DPhi->GetBinError(a+1) << std::endl;
    std::cout << std::endl;
  }

  std::cout << std::endl;
  std::cout << "--- Binned by MET ---" << std::endl;
  for (int b = 0; b < 4; b++) {
    std::cout << std::endl;
    std::cout << "Bin: " << METEdges[b] << " < MET < " << METEdges[b+1] << "" << std::endl;
    std::cout << "W->mu channel" << std::endl;
    std::cout << "  W+jets reco'd in ctrl                                  : " << hWMu_MCC_MET->GetBinContent(b+1) << " +/- " << hWMu_MCC_MET->GetBinError(b+1) << std::endl;
    std::cout << "  Background ctrl region                                 : " << hWMu_BGC_MET->GetBinContent(b+1) << " +/- " << hWMu_BGC_MET->GetBinError(b+1) << std::endl;
    std::cout << "  Data ctrl region                                       : " << hWMu_DataC_MET->GetBinContent(b+1) << " +/- " << hWMu_DataC_MET->GetBinError(b+1) << std::endl;
    std::cout << "  Data-bg ctrl region                                    : " << hWMu_EstC_MET->GetBinContent(b+1) << " +/- " << hWMu_EstC_MET->GetBinError(b+1) << std::endl;
    std::cout << std::endl;
    std::cout << "W->tau channel" << std::endl;
    std::cout << "  W+jets reco'd in ctrl (MC estimate in ctrl)            : " << hWTau_MCC_MET->GetBinContent(b+1) << " +/- " << hWTau_MCC_MET->GetBinError(b+1) << std::endl;
    std::cout << "  Background ctrl region                                 : " << hWTau_BGC_MET->GetBinContent(b+1) << " +/- " << hWTau_BGC_MET->GetBinError(b+1) << std::endl;
    std::cout << "  Data ctrl region                                       : " << hWTau_DataC_MET->GetBinContent(b+1) << " +/- " << hWTau_DataC_MET->GetBinError(b+1) << std::endl;
    std::cout << std::endl;
    std::cout << "  WTau in ctrl region observed                           : " << hWTau_EstC_MET->GetBinContent(b+1) << " +/- " << hWTau_EstC_MET->GetBinError(b+1) << std::endl;
    std::cout << std::endl;
    std::cout << "  Closure test: WTau from WMu                            : " << hWTau_Prediction_MET->GetBinContent(b+1) << " +/- " << hWTau_Prediction_MET->GetBinError(b+1) << std::endl;
    std::cout << std::endl;
  }

  std::cout << std::endl;
  std::cout << "--- Binned by CenJetEt ---" << std::endl;
  for (int c = 0; c<4; c++){
    std::cout << std::endl;
    std::cout << "Bin: " << CenJetEtEdges[c] << " < Central Jet Et < " << CenJetEtEdges[c+1] << "" << std::endl;
    std::cout << "W->mu channel" << std::endl;
    std::cout << "  W+jets reco'd in ctrl                                  : " << hWMu_MCC_CenJetEt->GetBinContent(c+1) << " +/- " << hWMu_MCC_CenJetEt->GetBinError(c+1) << std::endl;
    std::cout << "  Background ctrl region                                 : " << hWMu_BGC_CenJetEt->GetBinContent(c+1) << " +/- " << hWMu_BGC_CenJetEt->GetBinError(c+1) << std::endl;
    std::cout << "  Data ctrl region                                       : " << hWMu_DataC_CenJetEt->GetBinContent(c+1) << " +/- " << hWMu_DataC_CenJetEt->GetBinError(c+1) << std::endl;
    std::cout << "  Data-bg ctrl region                                    : " << hWMu_EstC_CenJetEt->GetBinContent(c+1) << " +/- " << hWMu_EstC_CenJetEt->GetBinError(c+1) << std::endl;
    std::cout << std::endl;
    std::cout << "W->tau channel" << std::endl;
    std::cout << "  W+jets reco'd in ctrl (MC estimate in ctrl)            : " << hWTau_MCC_CenJetEt->GetBinContent(c+1) << " +/- " << hWTau_MCC_CenJetEt->GetBinError(c+1) << std::endl;
    std::cout << "  Background ctrl region                                 : " << hWTau_BGC_CenJetEt->GetBinContent(c+1) << " +/- " << hWTau_BGC_CenJetEt->GetBinError(c+1) << std::endl;
    std::cout << "  Data ctrl region                                       : " << hWTau_DataC_CenJetEt->GetBinContent(c+1) << " +/- " << hWTau_DataC_CenJetEt->GetBinError(c+1) << std::endl;
    std::cout << std::endl;
    std::cout << "  WTau in ctrl region observed                           : " << hWTau_EstC_CenJetEt->GetBinContent(c+1) << " +/- " << hWTau_EstC_CenJetEt->GetBinError(c+1) << std::endl;
    std::cout << std::endl;
    std::cout << "  Closure test: WTau from WMu                            : " << hWTau_Prediction_CenJetEt->GetBinContent(c+1) << " +/- " << hWTau_Prediction_CenJetEt->GetBinError(c+1) << std::endl;
    std::cout << std::endl;
  }

  ////////////////////////
  // draw closure plots //
  ////////////////////////
  std::string pdfName;

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(111111111);

  double x_dPhi[4]  = {0.5, 1.4, 2.2, 2.6 + (TMath::Pi()-2.6)/2};
  double ex_dPhi[4] = {0.5, 0.4, 0.4, (TMath::Pi()-2.6)/2};
  double y_dPhi1[4],ey_dPhi1[4],y_dPhi2[4],ey_dPhi2[4],y_dPhi3[4],ey_dPhi3[4]; // For raw numbers - prediceted (data), observed, predicted (MC)
  double frac_dPhi[4],efrac_dPhi[4]; // For Frac diff

  double x_MET[4]   = {125., 200., 275., 400.};
  double ex_MET[4]  = {25, 50, 25, 100};
  double y_MET1[4],ey_MET1[4],y_MET2[4],ey_MET2[4],y_MET3[4],ey_MET3[4]; // For raw numbers - prediceted (data), observed, predicted (MC)
  double frac_MET[4],efrac_MET[4]; // For Frac diff

  double x_CenJetEt[4]  = {20, 45, 80, 125};
  double ex_CenJetEt[4] = {10, 15, 20, 25};
  double y_CenJetEt1[4],ey_CenJetEt1[4],y_CenJetEt2[4],ey_CenJetEt2[4],y_CenJetEt3[4],ey_CenJetEt3[4]; // For raw numbers - prediceted (data), observed, predicted (MC)
  double frac_CenJetEt[4],efrac_CenJetEt[4]; // For Frac diff

  double y_syst[4],e_syst[4]; // for systematic bands

  for(int i=0; i<4; ++i) {
        y_syst[i] = 0.;
        e_syst[i] = sqrt( pow(0.2,2) + pow(constants::syst_WTau,2) ); // 20% from MC stats, plus whatever the current value of Tau syst from TAU ID, w-e contamination, JER etc

        y_dPhi1[i]  = hWTau_Prediction_DPhi->GetBinContent(i+1);  //Predicted WMu
        ey_dPhi1[i] = hWTau_Prediction_DPhi->GetBinError(i+1);
        y_dPhi2[i]  = hWTau_EstC_DPhi->GetBinContent(i+1);  //Observed WMu
        ey_dPhi2[i] = hWTau_EstC_DPhi->GetBinError(i+1);  
        y_dPhi3[i]  = hWTau_MCC_DPhi->GetBinContent(i+1);
        ey_dPhi3[i] = hWTau_MCC_DPhi->GetBinError(i+1);
        frac_dPhi[i]  = (y_dPhi1[i]/y_dPhi2[i])-1.;
        efrac_dPhi[i] = (y_dPhi1[i]/y_dPhi2[i])*sqrt(pow(ey_dPhi1[i]/y_dPhi1[i],2) + pow(ey_dPhi2[i]/y_dPhi2[i],2));

        y_MET1[i]  = hWTau_Prediction_MET->GetBinContent(i+1);  //Predicted WMu
        ey_MET1[i] = hWTau_Prediction_MET->GetBinError(i+1);
        y_MET2[i]  = hWTau_EstC_MET->GetBinContent(i+1);  //Observed WMu
        ey_MET2[i] = hWTau_EstC_MET->GetBinError(i+1);  
        y_MET3[i]  = hWTau_MCC_MET->GetBinContent(i+1);
        ey_MET3[i] = hWTau_MCC_MET->GetBinError(i+1);
        frac_MET[i]  = (y_MET1[i]/y_MET2[i])-1.;
        efrac_MET[i] = (y_MET1[i]/y_MET2[i])*sqrt(pow(ey_MET1[i]/y_MET1[i],2) + pow(ey_MET2[i]/y_MET2[i],2));

        y_CenJetEt1[i]  = hWTau_Prediction_CenJetEt->GetBinContent(i+1);  //Predicted WMu
        ey_CenJetEt1[i] = hWTau_Prediction_CenJetEt->GetBinError(i+1);
        y_CenJetEt2[i]  = hWTau_EstC_CenJetEt->GetBinContent(i+1);  //Observed WMu
        ey_CenJetEt2[i] = hWTau_EstC_CenJetEt->GetBinError(i+1);  
        y_CenJetEt3[i]  = hWTau_MCC_CenJetEt->GetBinContent(i+1);
        ey_CenJetEt3[i] = hWTau_MCC_CenJetEt->GetBinError(i+1);
        frac_CenJetEt[i]  = (y_CenJetEt1[i]/y_CenJetEt2[i])-1.;
        efrac_CenJetEt[i] = (y_CenJetEt1[i]/y_CenJetEt2[i])*sqrt(pow(ey_CenJetEt1[i]/y_CenJetEt1[i],2) + pow(ey_CenJetEt2[i]/y_CenJetEt2[i],2));

  }

  TGraphErrors *gp_dPhi1 = new TGraphErrors(4,x_dPhi,y_dPhi1,ex_dPhi,ey_dPhi1);
  TGraphErrors *gp_dPhi2 = new TGraphErrors(4,x_dPhi,y_dPhi2,ex_dPhi,ey_dPhi2);
  TGraphErrors *gp_dPhi3 = new TGraphErrors(4,x_dPhi,y_dPhi3,ex_dPhi,ey_dPhi3);
  TGraphErrors *gp_dPhiF = new TGraphErrors(4,x_dPhi,frac_dPhi,ex_dPhi,efrac_dPhi);
  TGraphErrors *gp_dPhiS = new TGraphErrors(4,x_dPhi,y_syst,ex_dPhi,e_syst); // systematic error bands

  TGraphErrors *gp_MET1 = new TGraphErrors(4,x_MET,y_MET1,ex_MET,ey_MET1);
  TGraphErrors *gp_MET2 = new TGraphErrors(4,x_MET,y_MET2,ex_MET,ey_MET2);
  TGraphErrors *gp_MET3 = new TGraphErrors(4,x_MET,y_MET3,ex_MET,ey_MET3);
  TGraphErrors *gp_METF = new TGraphErrors(4,x_MET,frac_MET,ex_MET,efrac_MET);
  TGraphErrors *gp_METS = new TGraphErrors(4,x_MET,y_syst,ex_MET,e_syst); // systematic error bands

  TGraphErrors *gp_CenJetEt1 = new TGraphErrors(4,x_CenJetEt,y_CenJetEt1,ex_CenJetEt,ey_CenJetEt1);
  TGraphErrors *gp_CenJetEt2 = new TGraphErrors(4,x_CenJetEt,y_CenJetEt2,ex_CenJetEt,ey_CenJetEt2);
  TGraphErrors *gp_CenJetEt3 = new TGraphErrors(4,x_CenJetEt,y_CenJetEt3,ex_CenJetEt,ey_CenJetEt3);
  TGraphErrors *gp_CenJetEtF = new TGraphErrors(4,x_CenJetEt,frac_CenJetEt,ex_CenJetEt,efrac_CenJetEt);
  TGraphErrors *gp_CenJetEtS = new TGraphErrors(4,x_CenJetEt,y_syst,ex_CenJetEt,e_syst); // systematic error bands

  TH1D *h_dPhi     = new TH1D("h_dPhi", "", 1, 0, TMath::Pi()); // For axes
  TH1D *h_MET      = new TH1D("h_MET", "", 1, 100, 500); // For axes
  TH1D *h_CenJetEt = new TH1D("h_CenJetEt", "", 1, 10, 150); // For axes

  TCanvas canvas; 
  canvas.SetCanvasSize(canvas.GetWindowWidth(), 1.2*canvas.GetWindowHeight());

  ///////////////////
  // DPhiJJ graphs //
  ///////////////////
  // Absolute numbers
  gp_dPhi1->SetTitle("");
  gp_dPhi1->SetMarkerStyle(20);
  gp_dPhi1->SetMarkerSize(0.9);
  gp_dPhi1->SetLineColor(kRed);
  gp_dPhi1->SetMarkerColor(kRed);
  gp_dPhi1->GetXaxis()->SetTitle("#Delta #phi_{jj}");
  gp_dPhi1->GetXaxis()->SetRangeUser(0,TMath::Pi());
  gp_dPhi1->GetYaxis()->SetTitle("N(W#rightarrow #tau#nu)");
  gp_dPhi1->GetYaxis()->SetTitleOffset(1.2);
  gp_dPhi1->GetYaxis()->SetRangeUser(0,100);
  gp_dPhi1->Draw("AP");
  gp_dPhi2->SetMarkerStyle(20);
  gp_dPhi2->SetMarkerSize(0.9);
  gp_dPhi2->SetLineColor(kBlue);
  gp_dPhi2->SetMarkerColor(kBlue);
  gp_dPhi2->Draw("P same");
  gp_dPhi3->SetMarkerStyle(20);
  gp_dPhi3->SetMarkerSize(0.9);
  gp_dPhi3->SetLineColor(kViolet);
  gp_dPhi3->SetMarkerColor(kViolet);
  gp_dPhi3->Draw("P same");

  TLegend leg(0.12,0.67,0.32,0.87);
  leg.SetBorderSize(0);
  leg.SetFillColor(0);
  leg.AddEntry(gp_dPhi1,"predicted (data)","P");
  leg.AddEntry(gp_dPhi2,"observed (data)","P");
  leg.AddEntry(gp_dPhi3,"predicted (MC)","P");
  leg.Draw();

  pdfName= options.oDir + std::string("/Wtaunu_dphi_num.pdf");
  canvas.Print(pdfName.c_str());

   // Fractional diff
  h_dPhi->Draw();
  h_dPhi->GetXaxis()->SetTitle("#Delta #phi_{jj}");
  h_dPhi->GetYaxis()->SetTitle("#frac{Predicted - Observed}{Observed}");
  h_dPhi->GetYaxis()->SetTitleOffset(1.2);
  h_dPhi->GetYaxis()->SetRangeUser(-1.5,3);
  h_dPhi->SetLineColor(kBlue);
  h_dPhi->SetLineWidth(2);
  h_dPhi->Draw();
  gp_dPhiS->SetLineColor(kGray);
  gp_dPhiS->SetLineWidth(0);
  gp_dPhiS->SetFillColor(kGray+2);
  gp_dPhiS->SetFillStyle(3002);
  gp_dPhiF->SetMarkerStyle(20);
  gp_dPhiF->SetMarkerSize(1.2);
  gp_dPhiF->SetMarkerColor(kGreen-2);
  TF1 *f_dPhi = new TF1("f_dPhi","pol0",0,2.6); //To do a fit in first 3 bins only
  gp_dPhiF->Fit("f_dPhi","R");
  h_dPhi->Draw();
  gp_dPhiS->Draw("2 same");
  gp_dPhiF->Draw("P same");

  TLegend leg2_dphi(0.12,0.67,0.40,0.87);
  leg2_dphi.SetBorderSize(0);
  leg2_dphi.SetFillColor(0);
  leg2_dphi.AddEntry(f_dPhi,"pol0 fit (0 < #Delta #phi_{jj} < 2.6)","l");
  leg2_dphi.AddEntry(gp_dPhiS,"Systematic error","f");
  leg2_dphi.Draw();
  pdfName= options.oDir + std::string("/Wtaunu_dphi_frac.pdf");
  canvas.Print(pdfName.c_str());

  ////////////////
  // MET graphs //
  ////////////////
  gp_MET1->SetTitle("");
  gp_MET1->SetMarkerStyle(20);
  gp_MET1->SetMarkerSize(0.9);
  gp_MET1->SetLineColor(kRed);
  gp_MET1->SetMarkerColor(kRed);
  gp_MET1->GetXaxis()->SetTitle("#slash{E}_{T} [GeV]");
  gp_MET1->GetXaxis()->SetRangeUser(100,500);
  gp_MET1->GetYaxis()->SetTitle("N(W#rightarrow #tau#nu)");
  gp_MET1->GetYaxis()->SetTitleOffset(1.2);
  gp_MET1->GetYaxis()->SetRangeUser(0,60);
  gp_MET1->Draw("AP");
  gp_MET2->SetMarkerStyle(20);
  gp_MET2->SetMarkerSize(0.9);
  gp_MET2->SetLineColor(kBlue);
  gp_MET2->SetMarkerColor(kBlue);
  gp_MET2->Draw("P same");
  gp_MET3->SetMarkerStyle(20);
  gp_MET3->SetMarkerSize(0.9);
  gp_MET3->SetLineColor(kViolet);
  gp_MET3->SetMarkerColor(kViolet);
  gp_MET3->Draw("P same");

  leg.Draw();

  pdfName= options.oDir + std::string("/Wtaunu_met_num.pdf");
  canvas.Print(pdfName.c_str());
 
  h_MET->Draw();
  h_MET->GetXaxis()->SetTitle("#slash{E}_{T} [GeV]");
  h_MET->GetYaxis()->SetTitle("#frac{Predicted - Observed}{Observed}");
  h_MET->GetYaxis()->SetTitleOffset(1.2);
  h_MET->GetYaxis()->SetRangeUser(-9,6);
  h_MET->SetLineColor(kBlue);
  h_MET->SetLineWidth(2);
  h_MET->Draw();
  gp_METS->SetLineColor(kGray);
  gp_METS->SetLineWidth(0);
  gp_METS->SetFillColor(kGray+2);
  gp_METS->SetFillStyle(3002);
  gp_METF->SetMarkerStyle(20);
  gp_METF->SetMarkerSize(1.2);
  gp_METF->SetMarkerColor(kGreen-2);
  TF1 *f_MET = new TF1("f_MET","pol0",150,300);
  gp_METF->Fit("f_MET","R");
  h_MET->Draw();
  gp_METS->Draw("2 same");
  gp_METF->Draw("P same");

  TLegend leg2_met(0.12,0.67,0.40,0.87);
  leg2_met.SetBorderSize(0);
  leg2_met.SetFillColor(0);
  leg2_met.AddEntry(f_MET,"pol0 fit (150 < #slash{E}_{T} < 300 GeV","l");
  leg2_met.AddEntry(gp_dPhiS,"Systematic error","f");
  leg2_met.Draw();
  pdfName= options.oDir + std::string("/Wtaunu_met_frac.pdf");
  canvas.Print(pdfName.c_str());

  /////////////////////
  // CenJetEt graphs //
  /////////////////////
  gp_CenJetEt1->SetTitle("");
  gp_CenJetEt1->SetMarkerStyle(20);
  gp_CenJetEt1->SetMarkerSize(0.9);
  gp_CenJetEt1->SetLineColor(kRed);
  gp_CenJetEt1->SetMarkerColor(kRed);
  gp_CenJetEt1->GetXaxis()->SetTitle("Central Jet E_{T} [Gev]");
  gp_CenJetEt1->GetXaxis()->SetRangeUser(10,150);
  gp_CenJetEt1->GetYaxis()->SetTitle("N(W#rightarrow #tau#nu)");
  gp_CenJetEt1->GetYaxis()->SetTitleOffset(1.2);
  gp_CenJetEt1->GetYaxis()->SetRangeUser(0,60);
  gp_CenJetEt1->Draw("AP");
  gp_CenJetEt2->SetMarkerStyle(20);
  gp_CenJetEt2->SetMarkerSize(0.9);
  gp_CenJetEt2->SetLineColor(kBlue);
  gp_CenJetEt2->SetMarkerColor(kBlue);
  gp_CenJetEt2->Draw("P same");
  gp_CenJetEt3->SetMarkerStyle(20);
  gp_CenJetEt3->SetMarkerSize(0.9);
  gp_CenJetEt3->SetLineColor(kViolet);
  gp_CenJetEt3->SetMarkerColor(kViolet);
  gp_CenJetEt3->Draw("P same");

  leg.Draw();

  pdfName= options.oDir + std::string("/Wtaunu_CenJetEt_num.pdf");
  canvas.Print(pdfName.c_str());
 
  h_CenJetEt->Draw();
  h_CenJetEt->GetXaxis()->SetTitle("Central Jet E_{T} [Gev]");
  h_CenJetEt->GetYaxis()->SetTitle("#frac{Predicted - Observed}{Observed}");
  h_CenJetEt->GetYaxis()->SetTitleOffset(1.2);
  h_CenJetEt->GetYaxis()->SetRangeUser(-1.5,5);
  h_CenJetEt->SetLineColor(kBlue);
  h_CenJetEt->SetLineWidth(2);
  h_CenJetEt->Draw();
  gp_CenJetEtS->SetLineColor(kGray);
  gp_CenJetEtS->SetLineWidth(0);
  gp_CenJetEtS->SetFillColor(kGray+2);
  gp_CenJetEtS->SetFillStyle(3002);
  gp_CenJetEtF->SetMarkerStyle(20);
  gp_CenJetEtF->SetMarkerSize(1.2);
  gp_CenJetEtF->SetMarkerColor(kGreen-2);
  TF1 *f_CenJetEt = new TF1("f_CenJetEt","pol0",10,150); 
  gp_CenJetEtF->Fit("f_CenJetEt","R");
  h_CenJetEt->Draw();
  gp_CenJetEtS->Draw("2 same");
  gp_CenJetEtF->Draw("P same");

  TLegend leg2_cenjet(0.12,0.67,0.40,0.87);
  leg2_cenjet.SetBorderSize(0);
  leg2_cenjet.SetFillColor(0);
  leg2_cenjet.AddEntry(f_CenJetEt,"pol0 fit","l");
  leg2_cenjet.AddEntry(gp_dPhiS,"Systematic error","f");
  leg2_cenjet.Draw();

  pdfName= options.oDir + std::string("/Wtaunu_CenJetEt_frac.pdf");
  canvas.Print(pdfName.c_str());
}