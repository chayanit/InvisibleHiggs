
#include "InvisibleHiggs/Analysis/interface/ProgramOptions.h"
#include "InvisibleHiggs/Analysis/interface/Cuts.h"
#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"
#include "InvisibleHiggs/Analysis/interface/StackPlot.h"
#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"
#include "InvisibleHiggs/Analysis/interface/Datasets.h"
#include "InvisibleHiggs/Analysis/interface/Constants.h"


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

  std::cout << "Integrated luminosity : " << lumi << " pb-1" << std::endl;

  Datasets datasets(options.iDir);
  datasets.readFile(options.datasetFile);

  // output file
  TFile* ofile = TFile::Open( (options.oDir+std::string("/WeWmuclosure.root")).c_str(), "RECREATE");
  std::string oDir = options.oDir + std::string("/WeWmuclosure");

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

  TCut puWeight("puWeight"); 
  TCut trigCorr( "(trigCorrWeight>0) ? trigCorrWeight : 1." );
  TCut cutnoMjjnoMET = cuts.cutWMu("trigger")+cuts.cutWMu("dijet")+cuts.cutWMu("dEtaJJ")+cuts.cutWMu("CJV")+cuts.cutWMu("dPhiJJ");
  
  double dphiEdges[5] = { 0., 1.0, 1.8, 2.6, TMath::Pi() };
  double etaEdges[5]  = { -2.1, -1.0, 0.0, 1.0, +2.1 };
  double mjjEdges[6]  = { 800., 1000., 1200., 1600., 2000., 3000. };
  double metEdges[6]  = { 110., 130., 150., 200., 300., 500. }

  TH1D* hWMu_WC_GEN   = new TH1D("hWMu_WC_GEN", "", 1, 0., 1.);   
  TH1D* hWEl_WC_GEN   = new TH1D("hWEl_WC_GEN", "", 1, 0., 1.);

  // bins of dPhijj
  TH1D* hWMu_VBF_DPhi   = new TH1D("hWMu_VBF_DPhi", "", 4, dphiEdges);
  TH1D* hWMu_BGC_DPhi   = new TH1D("hWMu_BGC_DPhi", "", 4, dphiEdges);           // background MC ctrl region
  TH1D* hWMu_DataC_DPhi = new TH1D("hWMu_DataC_DPhi", "", 4, dphiEdges);         // Data ctrl region
  
  TH1D* hWEl_VBF_DPhi   = new TH1D("hWEl_VBF_DPhi", "", 4, dphiEdges);
  TH1D* hWEl_BGC_DPhi   = new TH1D("hWEl_BGC_DPhi", "", 4, dphiEdges);
  TH1D* hWEl_DataC_DPhi = new TH1D("hWEl_DataC_DPhi", "", 4, dphiEdges);
  // bins of W daughter rapidity
  TH1D* hWMu_VBF_Eta   = new TH1D("hWMu_VBF_Eta", "", 4, etaEdges);
  TH1D* hWMu_BGC_Eta   = new TH1D("hWMu_BGC_Eta", "", 4, etaEdges);          
  TH1D* hWMu_DataC_Eta = new TH1D("hWMu_DataC_Eta", "", 4, etaEdges);         
  
  TH1D* hWEl_VBF_Eta   = new TH1D("hWEl_VBF_Eta", "", 4, etaEdges);
  TH1D* hWEl_BGC_Eta   = new TH1D("hWEl_BGC_Eta", "", 4, etaEdges);
  TH1D* hWEl_DataC_Eta = new TH1D("hWEl_DataC_Eta", "", 4, etaEdges);
  // bins of Mjj
  TH1D* hWMu_VBF_Mjj   = new TH1D("hWMu_VBF_Mjj", "", 5, mjjEdges);
  TH1D* hWMu_BGC_Mjj   = new TH1D("hWMu_BGC_Mjj", "", 5, mjjEdges);           
  TH1D* hWMu_DataC_Mjj = new TH1D("hWMu_DataC_Mjj", "", 5, mjjEdges);         
 
  TH1D* hWEl_VBF_Mjj   = new TH1D("hWEl_VBF_Mjj", "", 5, mjjEdges);
  TH1D* hWEl_BGC_Mjj   = new TH1D("hWEl_BGC_Mjj", "", 5, mjjEdges);
  TH1D* hWEl_DataC_Mjj = new TH1D("hWEl_DataC_Mjj", "", 5, mjjEdges);
  // bins of MET
  TH1D* hWMu_VBF_MET   = new TH1D("hWMu_VBF_MET", "", 5, metEdges);
  TH1D* hWMu_BGC_MET   = new TH1D("hWMu_BGC_MET", "", 5, metEdges);           
  TH1D* hWMu_DataC_MET = new TH1D("hWMu_DataC_MET", "", 5, metEdges);         
 
  TH1D* hWEl_VBF_MET   = new TH1D("hWEl_VBF_MET", "", 5, metEdges);
  TH1D* hWEl_BGC_MET   = new TH1D("hWEl_BGC_MET", "", 5, metEdges);
  TH1D* hWEl_DataC_MET = new TH1D("hWEl_DataC_MET", "", 5, metEdges);

  // loop over MC datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);

    TFile* file = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

    // setup cuts
    TCut cutD = cuts.cutDataset(dataset.name);
    TCut wWeight("");

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
    }
    else if (dataset.isData) {
      std::cout << "Analysing Data     : " << dataset.name << std::endl;
    }
    else {
      std::cout << "Analysing BG MC     : " << dataset.name << std::endl;
    }

    TCut otherCuts = puWeight * trigCorr * wWeight;

    // tmp histograms
    TH1D* hWMu_GEN  = new TH1D("hWMu_GEN", "",  1, 0., 1.);     // for genlevel ratio 
    TH1D* hWEl_GEN  = new TH1D("hWEl_GEN", "",  1, 0., 1.);     // for genlevel ratio

    TH1D* hWMu_C_DPhi  = new TH1D("hWMu_C_DPhi", "",  4, dphiEdges);  // for data and bkg in WMu control region
    TH1D* hWMu_WC_DPhi = new TH1D("hWMu_WC_DPhi", "", 4, dphiEdges);  // for W MC ratio of VBF efficiency
    TH1D* hWEl_C_DPhi  = new TH1D("hWEl_C_DPhi", "",  4, dphiEdges);  // for data and bkg in WEl control region (observed)
    TH1D* hWEl_WC_DPhi = new TH1D("hWEl_WC_DPhi", "", 4, dphiEdges);  // for W MC (MC prediction) and ratio of VBF efficiency

    TH1D* hWMu_C_Eta  = new TH1D("hWMu_C_Eta", "",  4, etaEdges);  
    TH1D* hWMu_WC_Eta = new TH1D("hWMu_WC_Eta", "", 4, etaEdges);  
    TH1D* hWEl_C_Eta  = new TH1D("hWEl_C_Eta", "",  4, etaEdges); 
    TH1D* hWEl_WC_Eta = new TH1D("hWEl_WC_Eta", "", 4, etaEdges);  

    TH1D* hWMu_C_Mjj  = new TH1D("hWMu_C_Mjj", "",  5, mjjEdges);  
    TH1D* hWMu_WC_Mjj = new TH1D("hWMu_WC_Mjj", "", 5, mjjEdges);  
    TH1D* hWEl_C_Mjj  = new TH1D("hWEl_C_Mjj", "",  5, mjjEdges); 
    TH1D* hWEl_WC_Mjj = new TH1D("hWEl_WC_Mjj", "", 5, mjjEdges);  

    TH1D* hWMu_C_MET  = new TH1D("hWMu_C_MET", "",  5, mjjEdges);  
    TH1D* hWMu_WC_MET = new TH1D("hWMu_WC_MET", "", 5, mjjEdges);  
    TH1D* hWEl_C_MET  = new TH1D("hWEl_C_MET", "",  5, mjjEdges); 
    TH1D* hWEl_WC_MET = new TH1D("hWEl_WC_MET", "", 5, mjjEdges);  
    // Cut 
    TCut cutWMu_GEN  = otherCuts * cuts.wMuGen();
    TCut cutWEl_GEN  = otherCuts * cuts.wElGen();

    TCut cutWMu_C_noDPhi  = otherCuts * (cutD + cuts.wMuVBF() + cuts.cutWMu("MET"));
    TCut cutWMu_WC_noDPhi = otherCuts * (cuts.wMuGen() + cuts.wMuVBF() + cuts.cutWMu("MET"));
    TCut cutWEl_C_noDPhi  = otherCuts * (cutD + cuts.wElVBF() + cuts.cutWEl("MET"));
    TCut cutWEl_WC_noDPhi = otherCuts * (cuts.wElGen() + cuts.wElVBF() + cuts.cutWEl("MET"));

    TCut cutWMu_C_noEta  = otherCuts * (cutD + cuts.wMuVBF() + cuts.cutWMu("MET") + cuts.cutWMu("dPhiJJ"));
    TCut cutWMu_WC_noEta = otherCuts * (cuts.wMuGen() + cuts.wMuVBF() + cuts.cutWMu("MET") + cuts.cutWMu("dPhiJJ"))
    TCut cutWEl_C_noEta  = otherCuts * (cutD + cuts.wElVBF() + cuts.cutWEl("MET") + cuts.cutWMu("dPhiJJ"));
    TCut cutWEl_WC_noEta = otherCuts * (cuts.wElGen() + cuts.wElVBF() + cuts.cutWEl("MET") + cuts.cutWMu("dPhiJJ"));

    TCut cutWMu_C_noMjj  = otherCuts * (cutD + cuts.cutWMu("wMu") + cuts.cutWMu("lVeto") + cutnoMjjnoMET + cuts.cutWMu("MET"));
    TCut cutWMu_WC_noMjj = otherCuts * (cuts.wMuGen() + cuts.cutWMu("wMu") + cuts.cutWMu("lVeto") + cutnoMjjnoMET + cuts.cutWMu("MET"));
    TCut cutWEl_C_noMjj  = otherCuts * (cutD + cuts.cutWEl("wEl") + cuts.cutWEl("lVeto") + cutnoMjjnoMET + cuts.cutWEl("MET"));
    TCut cutWEl_WC_noMjj = otherCuts * (cuts.wElGen() + cuts.cutWEl("wEl") + cuts.cutWEl("lVeto") + cutnoMjjnoMET + cuts.cutWEl("MET"));

    TCut cutWMu_C_noMET  = otherCuts * (cutD + cuts.cutWMu("wMu") + cuts.cutWMu("lVeto") + cutnoMjjnoMET + cuts.cutWMu("Mjj"));
    TCut cutWMu_WC_noMET = otherCuts * (cuts.wMuGen() + cuts.cutWMu("wMu") + cuts.cutWMu("lVeto") + cutnoMjjnoMET + cuts.cutWMu("Mjj"));
    TCut cutWEl_C_noMET  = otherCuts * (cutD + cuts.cutWEl("wEl") + cuts.cutWEl("lVeto") + cutnoMjjnoMET + cuts.cutWEl("Mjj"));
    TCut cutWEl_WC_noMET = otherCuts * (cuts.wElGen() + cuts.cutWEl("wEl") + cuts.cutWEl("lVeto") + cutnoMjjnoMET + cuts.cutWEl("Mjj"));

    tree->Draw("0.5>>hWMu_GEN", cutWMu_GEN);
    tree->Draw("0.5>>hWEl_GEN", cutWEl_GEN);

    tree->Draw("vbfDPhi>>hWMu_C_DPhi",  cutWMu_C_noDPhi);
    tree->Draw("vbfDPhi>>hWMu_WC_DPhi", cutWMu_WC_noDPhi);
    tree->Draw("vbfDPhi>>hWEl_C_DPhi",  cutWEl_C_noDPhi);
    tree->Draw("vbfDPhi>>hWEl_WC_DPhi", cutWEl_WC_noDPhi);
 
    tree->Draw("wDaulEta>>hWMu_C_Eta",  cutWMu_C_noEta);
    tree->Draw("wDaulEta>>hWMu_WC_Eta", cutWMu_WC_noEta);
    tree->Draw("wDaulEta>>hWEl_C_Eta",  cutWEl_C_noEta);
    tree->Draw("wDaulEta>>hWEl_WC_Eta", cutWEl_WC_noEta);

    tree->Draw("vbfM>>hWMu_C_Mjj",  cutWMu_C_noMjj);
    tree->Draw("vbfM>>hWMu_WC_Mjj", cutWMu_WC_noMjj);
    tree->Draw("vbfM>>hWEl_C_Mjj",  cutWEl_C_noMjj);
    tree->Draw("vbfM>>hWEl_WC_Mjj", cutWEl_WC_noMjj);

    tree->Draw("metNoWLepton>>hWMu_C_MET",  cutWMu_C_noMET);
    tree->Draw("metNoWLepton>>hWMu_WC_MET", cutWMu_WC_noMET);
    tree->Draw("met>>hWEl_C_MET",  cutWEl_C_noMET);
    tree->Draw("met>>hWEl_WC_MET", cutWEl_WC_noMET);

    double weight = (dataset.isData) ? 1. : lumi * dataset.sigma / dataset.nEvents;
    std::cout << "  weight : " << weight << std::endl;

    hWMu_GEN->Scale(weight);
    hWEl_GEN->Scale(weight);

    hWMu_C_DPhi->Scale(weight);
    hWMu_WC_DPhi->Scale(weight);
    hWEl_C_DPhi->Scale(weight);
    hWEl_WC_DPhi->Scale(weight);

    hWMu_C_Eta->Scale(weight);
    hWMu_WC_Eta->Scale(weight);
    hWEl_C_Eta->Scale(weight);
    hWEl_WC_Eta->Scale(weight);

    hWMu_C_Mjj->Scale(weight);
    hWMu_WC_Mjj->Scale(weight);
    hWEl_C_Mjj->Scale(weight);
    hWEl_WC_Mjj->Scale(weight);

    hWMu_C_MET->Scale(weight);
    hWMu_WC_MET->Scale(weight);
    hWEl_C_MET->Scale(weight);
    hWEl_WC_MET->Scale(weight);

    // add to final histogram
    if (isWJets || isEwkW) {
    	hWMu_WC_GEN->Add(hWMu_GEN);
    	hWEl_WC_GEN->Add(hWEl_GEN);

    	hWMu_VBF_DPhi->Add(hWMu_WC_DPhi);
    	hWEl_VBF_DPhi->Add(hWEl_WC_DPhi);
    	hWMu_VBF_Eta->Add(hWMu_WC_Eta);
    	hWEl_VBF_Eta->Add(hWEl_WC_Eta);
    	hWMu_VBF_Mjj->Add(hWMu_WC_Mjj);
    	hWEl_VBF_Mjj->Add(hWEl_WC_Mjj);
    	hWMu_VBF_MET->Add(hWMu_WC_MET);
    	hWEl_VBF_MET->Add(hWEl_WC_MET);
    }
    else if (dataset.isData) {
    	hWMu_DataC_DPhi->Add(hWMu_C_DPhi);
    	hWEl_DataC_DPhi->Add(hWEl_C_DPhi);
    	hWMu_DataC_Eta->Add(hWMu_C_Eta);
    	hWEl_DataC_Eta->Add(hWEl_C_Eta);
    	hWMu_DataC_Mjj->Add(hWMu_C_Mjj);
    	hWEl_DataC_Mjj->Add(hWEl_C_Mjj);
    	hWMu_DataC_MET->Add(hWMu_C_MET);
    	hWEl_DataC_MET->Add(hWEl_C_MET);
    }
    else {
    	hWMu_BGC_DPhi->Add(hWMu_C_DPhi);
    	hWEl_BGC_DPhi->Add(hWEl_C_DPhi);
    	hWMu_BGC_Eta->Add(hWMu_C_Eta);
    	hWEl_BGC_Eta->Add(hWEl_C_Eta);
    	hWMu_BGC_Mjj->Add(hWMu_C_Mjj);
    	hWEl_BGC_Mjj->Add(hWEl_C_Mjj);
    	hWMu_BGC_MET->Add(hWMu_C_MET);
    	hWEl_BGC_MET->Add(hWEl_C_MET);
    }

    // debug output
    std::cout << "  N ctrl region (dphi<1) : " << hWMu_C_DPhi->GetBinContent(1) << " +/- " << hWMu_C_DPhi->GetBinError(1) << std::endl;
    
    delete hWMu_GEN;
    delete hWEl_GEN;
    delete hWMu_C_DPhi;
    delete hWMu_WC_DPhi;
    delete hWEl_C_DPhi;
    delete hWEl_WC_DPhi;
    delete hWMu_C_Eta;
    delete hWMu_WC_Eta;
    delete hWEl_C_Eta;
    delete hWEl_WC_Eta;
    delete hWMu_C_Mjj;
    delete hWMu_WC_Mjj;
    delete hWEl_C_Mjj;
    delete hWEl_WC_Mjj;
    delete hWMu_C_MET;
    delete hWMu_WC_MET;
    delete hWEl_C_MET;
    delete hWEl_WC_MET;

    delete tree;
    file->Close();
   
  }

  // create histograms with the background estimate

  //For WEl prediction
  TH1D* hWEl_RVBF_DPhi = new TH1D("hWEl_RVBF_DPhi", "", 4, dphiEdges);  // ratio of sngl/ctrl
  TH1D* hWEl_RGEN_DPhi = new TH1D("hWEl_RGEN_DPhi", "", 1, 0., 1.);
  TH1D* hWEl_R_DPhi    = new TH1D("hWEl_R_DPhi", "", 4, dphiEdges);

  TH1D* hWEl_EstC_DPhi = new TH1D("hWEl_EstC_DPhi", "", 4, dphiEdges);
  TH1D* hWEl_EstS_DPhi = new TH1D("hWEl_EstS_DPhi", "", 4, dphiEdges);

  hWEl_RVBF_DPhi->Divide(hWEl_VBF_DPhi, hWMu_VBF_DPhi, 1., 1.);
  hWEl_RGEN_DPhi->Divide(hWMu_GEN_DPhi, hWEl_GEN_DPhi, 1., 1.); 
  for(int ibin = 1; ibin <= hWEl_R_DPhi->GetNbinsX(); ++ibin) {
        hWEl_R_DPhi->SetBinContent(ibin, hWEl_RVBF_DPhi->GetBinContent(ibin) * hWEl_RGEN_DPhi->GetBinContent(1));
        hWEl_R_DPhi->SetBinError(ibin, hWEl_R_DPhi->GetBinContent(ibin) * sqrt(pow(hWEl_RVBF_DPhi->GetBinError(ibin)/hWEl_RVBF_DPhi->GetBinContent(ibin),2) + pow(hWEl_RGEN_DPhi->GetBinError(1)/hWEl_RGEN_DPhi->GetBinContent(1),2)));
  }

  // For WEl prediction from WMu
  hWEl_EstC_DPhi->Add(hWMu_DataC_DPhi, hWMu_BGC_DPhi, 1., -1.);		// equals to observed WMu
  hWEl_EstS_DPhi->Multiply(hWEl_EstC_DPhi, hWEl_R_DPhi, 1., 1.);

 
  std::cout << std::endl;
  std::cout << "##################################### Closure test WMu-WEl control region #####################################" << std::endl;
  std::cout << std::endl;
  std::cout << "  GEN ratio by histogram for WEl : " << hWEl_RGEN_DPhi->GetBinContent(1) << " +/- " << hWEl_RGEN_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "dphi < 1.0" << std::endl;
  std::cout << std::endl;
  std::cout << "W->el prediction from W->mu " << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_DPhi->GetBinContent(1) << " +/- " << hWEl_RVBF_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_DPhi->GetBinContent(1) << " +/- " << hWEl_R_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_DPhi->GetBinContent(1) << " +/- " << hWMu_DataC_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_DPhi->GetBinContent(1) << " +/- " << hWMu_BGC_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_DPhi->GetBinContent(1) << " +/- " << hWEl_EstS_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Observed WEl           : " << hWMu_EstC_DPhi->GetBinContent(1) << " +/- " << hWMu_EstC_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "dphi > 1.0 dphi < 1.8" << std::endl;
  std::cout << std::endl;
  std::cout << "W->el prediction from W->mu " << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_DPhi->GetBinContent(2) << " +/- " << hWEl_RVBF_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_DPhi->GetBinContent(2) << " +/- " << hWEl_R_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_DPhi->GetBinContent(2) << " +/- " << hWMu_DataC_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_DPhi->GetBinContent(2) << " +/- " << hWMu_BGC_DPhi->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_DPhi->GetBinContent(2) << " +/- " << hWEl_EstS_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Observed WEl           : " << hWMu_EstC_DPhi->GetBinContent(2) << " +/- " << hWMu_EstC_DPhi->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "dphi > 1.8 dphi < 2.6" << std::endl;
  std::cout << std::endl;
  std::cout << "W->el prediction from W->mu " << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_DPhi->GetBinContent(3) << " +/- " << hWEl_RVBF_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_DPhi->GetBinContent(3) << " +/- " << hWEl_R_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_DPhi->GetBinContent(3) << " +/- " << hWMu_DataC_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_DPhi->GetBinContent(3) << " +/- " << hWMu_BGC_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_DPhi->GetBinContent(3) << " +/- " << hWEl_EstS_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Observed WEl           : " << hWMu_EstC_DPhi->GetBinContent(3) << " +/- " << hWMu_EstC_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "dphi > 2.6" << std::endl;
  std::cout << std::endl;
  std::cout << "W->el prediction from W->mu " << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_DPhi->GetBinContent(4) << " +/- " << hWEl_RVBF_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_DPhi->GetBinContent(4) << " +/- " << hWEl_R_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_DPhi->GetBinContent(4) << " +/- " << hWMu_DataC_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_DPhi->GetBinContent(4) << " +/- " << hWMu_BGC_DPhi->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_DPhi->GetBinContent(4) << " +/- " << hWEl_EstS_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Observed WEl           : " << hWMu_EstC_DPhi->GetBinContent(4) << " +/- " << hWMu_EstC_DPhi->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "#####################################################################################" << std::endl;
  std::cout << std::endl;

  // draw control plots
  std::string pdfName;

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(111111111);

  double x1[4]  = {0.5, 1.4, 2.2, 2.6 + (TMath::Pi()-2.6)/2};
  double ex1[4] = {0.5, 0.4, 0.4, (TMath::Pi()-2.6)/2};
  double y1[4],ey1[4],y2[4],ey2[4];	// WMu closure
  double y3[4],ey3[4],y4[4],ey4[4];	// WEl closure
  double diff1[4],ediff1[4];
  double diff2[4],ediff2[4];
  double frac[4],efrac[4];
  double y_syst[4],e_syst[4];

  for(int i=0; i<4; ++i) {
        y1[i]  = hWMu_EstS_DPhi->GetBinContent(i+1);	//Predicted WMu
        ey1[i] = hWMu_EstS_DPhi->GetBinError(i+1);	
        y2[i]  = hWEl_EstC_DPhi->GetBinContent(i+1);	//Observed WMu
        ey2[i] = hWEl_EstC_DPhi->GetBinError(i+1);
        diff1[i]  = y1[i]-y2[i];
        ediff1[i] = sqrt(ey1[i]*ey1[i] + ey2[i]*ey2[i]);

	y3[i]  = hWEl_EstS_DPhi->GetBinContent(i+1);	//Predicted WEl
	ey3[i] = hWEl_EstS_DPhi->GetBinError(i+1);	
	y4[i]  = hWMu_EstC_DPhi->GetBinContent(i+1);  	//Observed WEl
	ey4[i] = hWMu_EstC_DPhi->GetBinError(i+1);
        diff2[i]  = y3[i]-y4[i];
        ediff2[i] = sqrt(ey3[i]*ey3[i] + ey4[i]*ey4[i]);
        y_syst[i] = 0.;
        e_syst[i] = 0.14;  
        if(y3[i] > 0) frac[i]  = (y3[i]-y4[i])/y4[i];
        efrac[i] = sqrt(pow(ey3[i]/y3[i],2) + pow(ey4[i]/y4[i],2));
  }
  TGraphErrors *gp1 = new TGraphErrors(4,x1,y1,ex1,ey1);
  TGraphErrors *gp2 = new TGraphErrors(4,x1,y2,ex1,ey2);
  TGraphErrors *gp3 = new TGraphErrors(4,x1,diff1,ex1,ediff1);
  TGraphErrors *gp4 = new TGraphErrors(4,x1,y3,ex1,ey3);
  TGraphErrors *gp5 = new TGraphErrors(4,x1,y4,ex1,ey4);
  TGraphErrors *gp6 = new TGraphErrors(4,x1,diff2,ex1,ediff2);
  TGraphErrors *gp7 = new TGraphErrors(4,x1,frac,ex1,efrac);
  TGraphErrors *gp8 = new TGraphErrors(4,x1,y_syst,ex1,e_syst);
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
  gp1->GetYaxis()->SetTitle("N(W#rightarrow #mu#nu)");
  gp1->GetYaxis()->SetRangeUser(100,500);
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

  pdfName= oDir + std::string("/Wmunu_num.pdf");
  canvas.Print(pdfName.c_str());
 
  h->Draw();
  h->GetXaxis()->SetTitle("#Delta #phi_{jj}");
  h->GetYaxis()->SetTitle("Predicted - Observed");
  h->GetYaxis()->SetRangeUser(-100,250);
  h->SetLineColor(kBlue);
  h->Draw();
  gp3->SetMarkerStyle(20);
  gp3->SetMarkerSize(0.9);
  gp3->SetMarkerColor(kGreen-2);
  gp3->Fit("pol0");
  h->Draw();
  gp3->Draw("P same");

  pdfName= oDir + std::string("/Wmunu_diff.pdf");
  canvas.Print(pdfName.c_str());

  gp4->SetTitle("");
  gp4->SetMarkerStyle(20);
  gp4->SetMarkerSize(0.9);
  gp4->SetLineColor(kRed);
  gp4->SetMarkerColor(kRed);
  gp4->GetXaxis()->SetTitle("#Delta #phi_{jj}");
  gp4->GetXaxis()->SetRangeUser(0,TMath::Pi());
  gp4->GetYaxis()->SetTitle("N(W#rightarrow e#nu)");
  gp4->GetYaxis()->SetRangeUser(0,120);
  gp4->Draw("AP");
  gp5->SetMarkerStyle(20);
  gp5->SetMarkerSize(0.9);
  gp5->SetLineColor(kBlue);
  gp5->SetMarkerColor(kBlue);
  gp5->Draw("P same");
  leg.Draw();

  pdfName= oDir + std::string("/Welnu_num.pdf");
  canvas.Print(pdfName.c_str());

  h->Draw();
  h->GetXaxis()->SetTitle("#Delta #phi_{jj}");
  h->GetYaxis()->SetTitle("Predicted - Observed");
  h->GetYaxis()->SetRangeUser(-80,80);
  gp6->SetMarkerStyle(20);
  gp6->SetMarkerSize(0.9);
  gp6->SetMarkerColor(kGreen-2);
  TF1 *f1 = new TF1("f1","pol0",0,TMath::Pi());
  gp6->Fit("f1","R");
  h->Draw();
  gp6->Draw("SAMEP");

  pdfName= oDir + std::string("/Welnu_diff.pdf");
  canvas.Print(pdfName.c_str());

  h->GetXaxis()->SetTitle("#Delta #phi_{jj}");
  h->GetYaxis()->SetTitle("#frac{Predicted - Observed}{Observed}");
  h->GetYaxis()->SetRangeUser(-2,2);
  h->SetLineColor(kBlue);
  h->SetLineWidth(2);
  h->Draw();
  gp8->SetLineColor(kGray+2);
  gp8->SetLineWidth(0);
  gp8->SetFillColor(kGray+2);
  gp8->SetFillStyle(3002);
  gp7->SetMarkerStyle(20);
  gp7->SetMarkerSize(1.2);
  gp7->SetMarkerColor(kGreen-2);
  gp7->Fit("f1","R");
  h->Draw();
  gp8->Draw("2 same");
  gp7->Draw("P same");

  TLegend leg2(0.12,0.67,0.40,0.87);
  leg2.SetBorderSize(0);
  leg2.SetFillColor(0);
  leg2.AddEntry(f1,"pol0 fit (0 < #Delta #phi_{jj} < #pi)","l");
  leg2.AddEntry(gp8,"Systematic error","f");
  leg2.Draw();
  pdfName= oDir + std::string("/Welnu_frac.pdf");
  canvas.Print(pdfName.c_str());

  // store histograms
  ofile->cd(); 
  hWMu_VBF_DPhi->Write("",TObject::kOverwrite);
  hWMu_GEN_DPhi->Write("",TObject::kOverwrite); 
  hWMu_BGC_DPhi->Write("",TObject::kOverwrite); 
  hWMu_DataC_DPhi->Write("",TObject::kOverwrite);
  hWEl_VBF_DPhi->Write("",TObject::kOverwrite);
  hWEl_GEN_DPhi->Write("",TObject::kOverwrite);
  hWEl_BGC_DPhi->Write("",TObject::kOverwrite);
  hWEl_DataC_DPhi->Write("",TObject::kOverwrite);
  hWMu_RGEN_DPhi->Write("",TObject::kOverwrite);
  hWMu_RVBF_DPhi->Write("",TObject::kOverwrite);
  hWMu_R_DPhi->Write("",TObject::kOverwrite);
  hWMu_EstC_DPhi->Write("",TObject::kOverwrite);
  hWMu_EstS_DPhi->Write("",TObject::kOverwrite);
  hWEl_RGEN_DPhi->Write("",TObject::kOverwrite);
  hWEl_RVBF_DPhi->Write("",TObject::kOverwrite);  
  hWEl_R_DPhi->Write("",TObject::kOverwrite);
  hWEl_EstC_DPhi->Write("",TObject::kOverwrite);
  hWEl_EstS_DPhi->Write("",TObject::kOverwrite);

  ofile->Close();    

}
