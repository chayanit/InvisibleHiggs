
#include "InvisibleHiggs/Analysis/interface/ProgramOptions.h"
#include "InvisibleHiggs/Analysis/interface/Cuts.h"
#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"
#include "InvisibleHiggs/Analysis/interface/StackPlot.h"
#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"
#include "InvisibleHiggs/Analysis/interface/Datasets.h"
#include "InvisibleHiggs/Analysis/interface/Constants.h"


#include "TTree.h"
#include "THStack.h"
#include "TMath.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TPaveText.h"
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
  //TFile* ofile = TFile::Open( (options.oDir+std::string("/WeWmuclosure.root")).c_str(), "RECREATE");
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

  // For lepton weights
  TCut muTightWeight    = cuts.muTightWeight(options.leptCorr);
  TCut elTightWeight    = cuts.elTightWeight(options.leptCorr);
  TCut lVetoWeight      = cuts.elVetoWeight(options.leptCorr) * cuts.muVetoWeight(options.leptCorr);

  TCut puWeight("puWeight"); 
  TCut trigCorr( "(trigCorrWeight>0) ? trigCorrWeight : 1." );
  TCut cutnoMjjnoMET = cuts.cutWMu("trigger")+cuts.cutWMu("dijet")+cuts.cutWMu("dEtaJJ")+cuts.cutWMu("CJV")+cuts.cutWMu("dPhiJJ");
  TCut cutnoMETnoCJV = cuts.cutWMu("trigger")+cuts.cutWMu("dijet")+cuts.cutWMu("dEtaJJ")+cuts.cutWMu("Mjj")+cuts.cutWMu("dPhiJJ");
  
  double dphiEdges[5] = { 0., 1.0, 1.8, 2.6, TMath::Pi() };
  double etaEdges[5]  = { 0., 0.5, 1.0, 1.5, 2.1 };
  //double etaEdges[5]  = { -2.1, -1.0, 0.0, 1.0, +2.1 };
  double mjjEdges[6]  = { 800., 1000., 1200., 1600., 2000., 3000. };
  double metEdges[5]  = { 100., 150., 200., 300., 500. };
  double cjvEdges[5]  = { 10., 30., 60., 100., 150.}; 

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
  TH1D* hWMu_VBF_MET   = new TH1D("hWMu_VBF_MET", "", 4, metEdges);
  TH1D* hWMu_BGC_MET   = new TH1D("hWMu_BGC_MET", "", 4, metEdges);           
  TH1D* hWMu_DataC_MET = new TH1D("hWMu_DataC_MET", "", 4, metEdges);         
 
  TH1D* hWEl_VBF_MET   = new TH1D("hWEl_VBF_MET", "", 4, metEdges);
  TH1D* hWEl_BGC_MET   = new TH1D("hWEl_BGC_MET", "", 4, metEdges);
  TH1D* hWEl_DataC_MET = new TH1D("hWEl_DataC_MET", "", 4, metEdges);
  // bins of Central Jet Et
  TH1D* hWMu_VBF_CJV   = new TH1D("hWMu_VBF_CJV", "", 4, cjvEdges);
  TH1D* hWMu_BGC_CJV   = new TH1D("hWMu_BGC_CJV", "", 4, cjvEdges);
  TH1D* hWMu_DataC_CJV = new TH1D("hWMu_DataC_CJV", "", 4, cjvEdges);

  TH1D* hWEl_VBF_CJV   = new TH1D("hWEl_VBF_CJV", "", 4, cjvEdges);
  TH1D* hWEl_BGC_CJV   = new TH1D("hWEl_BGC_CJV", "", 4, cjvEdges);
  TH1D* hWEl_DataC_CJV = new TH1D("hWEl_DataC_CJV", "", 4, cjvEdges);
  // loop over MC datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);

    TFile* file = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

    // setup cuts
    TCut cutD = cuts.cutDataset(dataset.name);
    TCut wWeight("");

    if (dataset.name.compare(0,3,"QCD") == 0) continue;

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

    TCut otherCutsMu = puWeight * trigCorr * wWeight;
    TCut otherCutsEl = puWeight * trigCorr * wWeight;
    if (!dataset.isData){
      otherCutsMu *= muTightWeight;
      otherCutsEl *= elTightWeight;
    }
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

    TH1D* hWMu_C_MET  = new TH1D("hWMu_C_MET", "",  4, metEdges);  
    TH1D* hWMu_WC_MET = new TH1D("hWMu_WC_MET", "", 4, metEdges);  
    TH1D* hWEl_C_MET  = new TH1D("hWEl_C_MET", "",  4, metEdges); 
    TH1D* hWEl_WC_MET = new TH1D("hWEl_WC_MET", "", 4, metEdges);  

    TH1D* hWMu_C_CJV  = new TH1D("hWMu_C_CJV", "",  4, cjvEdges);  
    TH1D* hWMu_WC_CJV = new TH1D("hWMu_WC_CJV", "", 4, cjvEdges);  
    TH1D* hWEl_C_CJV  = new TH1D("hWEl_C_CJV", "",  4, cjvEdges); 
    TH1D* hWEl_WC_CJV = new TH1D("hWEl_WC_CJV", "", 4, cjvEdges);

    // Cut 
    TCut cutWMu_GEN  = otherCutsMu * cuts.wMuGen();
    TCut cutWEl_GEN  = otherCutsEl * cuts.wElGen();

    TCut cutWMu_C_noDPhi  = otherCutsMu * (cutD + cuts.wMuVBF() + cuts.cutWMu("MET"));
    TCut cutWMu_WC_noDPhi = otherCutsMu * (cuts.wMuGen() + cuts.wMuVBF() + cuts.cutWMu("MET"));
    TCut cutWEl_C_noDPhi  = otherCutsEl * (cutD + cuts.wElVBF() + cuts.cutWEl("MET"));
    TCut cutWEl_WC_noDPhi = otherCutsEl * (cuts.wElGen() + cuts.wElVBF() + cuts.cutWEl("MET"));

    TCut cutWMu_C_noEta  = otherCutsMu * (cutD + cuts.wMuVBF() + cuts.cutWMu("MET") + cuts.cutWMu("dPhiJJ"));
    TCut cutWMu_WC_noEta = otherCutsMu * (cuts.wMuGen() + cuts.wMuVBF() + cuts.cutWMu("MET") + cuts.cutWMu("dPhiJJ"));
    TCut cutWEl_C_noEta  = otherCutsEl * (cutD + cuts.wElVBF() + cuts.cutWEl("MET") + cuts.cutWMu("dPhiJJ"));
    TCut cutWEl_WC_noEta = otherCutsEl * (cuts.wElGen() + cuts.wElVBF() + cuts.cutWEl("MET") + cuts.cutWMu("dPhiJJ"));

    TCut cutWMu_C_noMjj  = otherCutsMu * (cutD + cuts.cutWMu("wMu") + cuts.cutWMu("lVeto") + cutnoMjjnoMET + cuts.cutWMu("MET"));
    TCut cutWMu_WC_noMjj = otherCutsMu * (cuts.wMuGen() + cuts.cutWMu("wMu") + cuts.cutWMu("lVeto") + cutnoMjjnoMET + cuts.cutWMu("MET"));
    TCut cutWEl_C_noMjj  = otherCutsEl * (cutD + cuts.cutWEl("wEl") + cuts.cutWEl("lVeto") + cutnoMjjnoMET + cuts.cutWEl("MET"));
    TCut cutWEl_WC_noMjj = otherCutsEl * (cuts.wElGen() + cuts.cutWEl("wEl") + cuts.cutWEl("lVeto") + cutnoMjjnoMET + cuts.cutWEl("MET"));

    TCut cutWMu_C_noMET  = otherCutsMu * (cutD + cuts.cutWMu("wMu") + cuts.cutWMu("lVeto") + cutnoMjjnoMET + cuts.cutWMu("Mjj"));
    TCut cutWMu_WC_noMET = otherCutsMu * (cuts.wMuGen() + cuts.cutWMu("wMu") + cuts.cutWMu("lVeto") + cutnoMjjnoMET + cuts.cutWMu("Mjj"));
    TCut cutWEl_C_noMET  = otherCutsEl * (cutD + cuts.cutWEl("wEl") + cuts.cutWEl("lVeto") + cutnoMjjnoMET + cuts.cutWEl("Mjj"));
    TCut cutWEl_WC_noMET = otherCutsEl * (cuts.wElGen() + cuts.cutWEl("wEl") + cuts.cutWEl("lVeto") + cutnoMjjnoMET + cuts.cutWEl("Mjj"));

    TCut cutWMu_C_noCJV  = otherCutsMu * (cutD + cuts.cutWMu("wMu") + cuts.cutWMu("lVeto") + cutnoMETnoCJV + cuts.cutWMu("MET"));
    TCut cutWMu_WC_noCJV = otherCutsMu * (cuts.wMuGen() + cuts.cutWMu("wMu") + cuts.cutWMu("lVeto") + cutnoMETnoCJV + cuts.cutWMu("MET"));
    TCut cutWEl_C_noCJV  = otherCutsEl * (cutD + cuts.cutWEl("wEl") + cuts.cutWEl("lVeto") + cutnoMETnoCJV + cuts.cutWEl("MET"));
    TCut cutWEl_WC_noCJV = otherCutsEl * (cuts.wElGen() + cuts.cutWEl("wEl") + cuts.cutWEl("lVeto") + cutnoMETnoCJV + cuts.cutWEl("MET"));

    tree->Draw("0.5>>hWMu_GEN", cutWMu_GEN);
    tree->Draw("0.5>>hWEl_GEN", cutWEl_GEN);

    tree->Draw("vbfDPhi>>hWMu_C_DPhi",  cutWMu_C_noDPhi);
    tree->Draw("vbfDPhi>>hWMu_WC_DPhi", cutWMu_WC_noDPhi);
    tree->Draw("vbfDPhi>>hWEl_C_DPhi",  cutWEl_C_noDPhi);
    tree->Draw("vbfDPhi>>hWEl_WC_DPhi", cutWEl_WC_noDPhi);
 
    tree->Draw("abs(wDaulEta)>>hWMu_C_Eta",  cutWMu_C_noEta);
    tree->Draw("abs(wDaulEta)>>hWMu_WC_Eta", cutWMu_WC_noEta);
    tree->Draw("abs(wDaulEta)>>hWEl_C_Eta",  cutWEl_C_noEta);
    tree->Draw("abs(wDaulEta)>>hWEl_WC_Eta", cutWEl_WC_noEta);

    tree->Draw("vbfM>>hWMu_C_Mjj",  cutWMu_C_noMjj);
    tree->Draw("vbfM>>hWMu_WC_Mjj", cutWMu_WC_noMjj);
    tree->Draw("vbfM>>hWEl_C_Mjj",  cutWEl_C_noMjj);
    tree->Draw("vbfM>>hWEl_WC_Mjj", cutWEl_WC_noMjj);

    tree->Draw("metNoWLepton>>hWMu_C_MET",  cutWMu_C_noMET);
    tree->Draw("metNoWLepton>>hWMu_WC_MET", cutWMu_WC_noMET);
    tree->Draw("met>>hWEl_C_MET",  cutWEl_C_noMET);
    tree->Draw("met>>hWEl_WC_MET", cutWEl_WC_noMET);

    tree->Draw("cenJetEt>>hWMu_C_CJV",  cutWMu_C_noCJV);
    tree->Draw("cenJetEt>>hWMu_WC_CJV", cutWMu_WC_noCJV);
    tree->Draw("cenJetEt>>hWEl_C_CJV",  cutWEl_C_noCJV);
    tree->Draw("cenJetEt>>hWEl_WC_CJV", cutWEl_WC_noCJV);

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

    hWMu_C_CJV->Scale(weight);
    hWMu_WC_CJV->Scale(weight);
    hWEl_C_CJV->Scale(weight);
    hWEl_WC_CJV->Scale(weight);

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
    	hWMu_VBF_CJV->Add(hWMu_WC_CJV);
    	hWEl_VBF_CJV->Add(hWEl_WC_CJV);
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
    	hWMu_DataC_CJV->Add(hWMu_C_CJV);
    	hWEl_DataC_CJV->Add(hWEl_C_CJV);
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
    	hWMu_BGC_CJV->Add(hWMu_C_CJV);
    	hWEl_BGC_CJV->Add(hWEl_C_CJV);
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
    delete hWMu_C_CJV;
    delete hWMu_WC_CJV;
    delete hWEl_C_CJV;
    delete hWEl_WC_CJV;

    delete tree;
    file->Close();
   
  }

  // create histograms with the background estimate

  //For WEl prediction
  TH1D* hWEl_RGEN      = new TH1D("hWEl_RGEN", "", 1, 0., 1.);
  hWEl_RGEN->Divide(hWMu_WC_GEN, hWEl_WC_GEN, 1., 1.); 

  TH1D* hWEl_RVBF_DPhi = new TH1D("hWEl_RVBF_DPhi", "", 4, dphiEdges);  // ratio of sngl/ctrl
  TH1D* hWEl_R_DPhi    = new TH1D("hWEl_R_DPhi", "", 4, dphiEdges);
  TH1D* hWMu_EstC_DPhi = new TH1D("hWMu_EstC_DPhi", "", 4, dphiEdges);
  TH1D* hWEl_EstS_DPhi = new TH1D("hWEl_EstS_DPhi", "", 4, dphiEdges);
  TH1D* hWEl_EstC_DPhi = new TH1D("hWEl_EstC_DPhi", "", 4, dphiEdges);

  hWEl_RVBF_DPhi->Divide(hWEl_VBF_DPhi, hWMu_VBF_DPhi, 1., 1.);
  for(int ibin = 1; ibin <= hWEl_R_DPhi->GetNbinsX(); ++ibin) {
        hWEl_R_DPhi->SetBinContent(ibin, hWEl_RVBF_DPhi->GetBinContent(ibin) * hWEl_RGEN->GetBinContent(1));
        hWEl_R_DPhi->SetBinError(ibin, hWEl_R_DPhi->GetBinContent(ibin) * sqrt(pow(hWEl_RVBF_DPhi->GetBinError(ibin)/hWEl_RVBF_DPhi->GetBinContent(ibin),2) + pow(hWEl_RGEN->GetBinError(1)/hWEl_RGEN->GetBinContent(1),2)));
  }
  hWMu_EstC_DPhi->Add(hWMu_DataC_DPhi, hWMu_BGC_DPhi, 1., -1.);
  hWEl_EstS_DPhi->Multiply(hWMu_EstC_DPhi, hWEl_R_DPhi, 1., 1.);       // Prediction
  hWEl_EstC_DPhi->Add(hWEl_DataC_DPhi, hWEl_BGC_DPhi, 1., -1.);        // Observation

  // bins Eta
  TH1D* hWEl_RVBF_Eta = new TH1D("hWEl_RVBF_Eta", "", 4, etaEdges);  
  TH1D* hWEl_R_Eta    = new TH1D("hWEl_R_Eta", "", 4, etaEdges);
  TH1D* hWMu_EstC_Eta = new TH1D("hWMu_EstC_Eta", "", 4, etaEdges);
  TH1D* hWEl_EstS_Eta = new TH1D("hWEl_EstS_Eta", "", 4, etaEdges);
  TH1D* hWEl_EstC_Eta = new TH1D("hWEl_EstC_Eta", "", 4, etaEdges);

  hWEl_RVBF_Eta->Divide(hWEl_VBF_Eta, hWMu_VBF_Eta, 1., 1.);
  for(int ibin = 1; ibin <= hWEl_R_Eta->GetNbinsX(); ++ibin) {
        hWEl_R_Eta->SetBinContent(ibin, hWEl_RVBF_Eta->GetBinContent(ibin) * hWEl_RGEN->GetBinContent(1));
        hWEl_R_Eta->SetBinError(ibin, hWEl_R_Eta->GetBinContent(ibin) * sqrt(pow(hWEl_RVBF_Eta->GetBinError(ibin)/hWEl_RVBF_Eta->GetBinContent(ibin),2) + pow(hWEl_RGEN->GetBinError(1)/hWEl_RGEN->GetBinContent(1),2)));
  }
  hWMu_EstC_Eta->Add(hWMu_DataC_Eta, hWMu_BGC_Eta, 1., -1.);
  hWEl_EstS_Eta->Multiply(hWMu_EstC_Eta, hWEl_R_Eta, 1., 1.);       
  hWEl_EstC_Eta->Add(hWEl_DataC_Eta, hWEl_BGC_Eta, 1., -1.); 

  // bins Mjj
  TH1D* hWEl_RVBF_Mjj = new TH1D("hWEl_RVBF_Mjj", "", 5, mjjEdges);  
  TH1D* hWEl_R_Mjj    = new TH1D("hWEl_R_Mjj", "", 5, mjjEdges);
  TH1D* hWMu_EstC_Mjj = new TH1D("hWMu_EstC_Mjj", "", 5, mjjEdges);
  TH1D* hWEl_EstS_Mjj = new TH1D("hWEl_EstS_Mjj", "", 5, mjjEdges);
  TH1D* hWEl_EstC_Mjj = new TH1D("hWEl_EstC_Mjj", "", 5, mjjEdges);

  hWEl_RVBF_Mjj->Divide(hWEl_VBF_Mjj, hWMu_VBF_Mjj, 1., 1.);
  for(int ibin = 1; ibin <= hWEl_R_Mjj->GetNbinsX(); ++ibin) {
        hWEl_R_Mjj->SetBinContent(ibin, hWEl_RVBF_Mjj->GetBinContent(ibin) * hWEl_RGEN->GetBinContent(1));
        hWEl_R_Mjj->SetBinError(ibin, hWEl_R_Mjj->GetBinContent(ibin) * sqrt(pow(hWEl_RVBF_Mjj->GetBinError(ibin)/hWEl_RVBF_Mjj->GetBinContent(ibin),2) + pow(hWEl_RGEN->GetBinError(1)/hWEl_RGEN->GetBinContent(1),2)));
  }
  hWMu_EstC_Mjj->Add(hWMu_DataC_Mjj, hWMu_BGC_Mjj, 1., -1.);
  hWEl_EstS_Mjj->Multiply(hWMu_EstC_Mjj, hWEl_R_Mjj, 1., 1.);       
  hWEl_EstC_Mjj->Add(hWEl_DataC_Mjj, hWEl_BGC_Mjj, 1., -1.); 

  // bins MET
  TH1D* hWEl_RVBF_MET = new TH1D("hWEl_RVBF_MET", "", 4, metEdges);  
  TH1D* hWEl_R_MET    = new TH1D("hWEl_R_MET", "", 4, metEdges);
  TH1D* hWMu_EstC_MET = new TH1D("hWMu_EstC_MET", "", 4, metEdges);
  TH1D* hWEl_EstS_MET = new TH1D("hWEl_EstS_MET", "", 4, metEdges);
  TH1D* hWEl_EstC_MET = new TH1D("hWEl_EstC_MET", "", 4, metEdges);

  hWEl_RVBF_MET->Divide(hWEl_VBF_MET, hWMu_VBF_MET, 1., 1.);
  for(int ibin = 1; ibin <= hWEl_R_MET->GetNbinsX(); ++ibin) {
        hWEl_R_MET->SetBinContent(ibin, hWEl_RVBF_MET->GetBinContent(ibin) * hWEl_RGEN->GetBinContent(1));
        hWEl_R_MET->SetBinError(ibin, hWEl_R_MET->GetBinContent(ibin) * sqrt(pow(hWEl_RVBF_MET->GetBinError(ibin)/hWEl_RVBF_MET->GetBinContent(ibin),2) + pow(hWEl_RGEN->GetBinError(1)/hWEl_RGEN->GetBinContent(1),2)));
  }
  hWMu_EstC_MET->Add(hWMu_DataC_MET, hWMu_BGC_MET, 1., -1.);
  hWEl_EstS_MET->Multiply(hWMu_EstC_MET, hWEl_R_MET, 1., 1.);       
  hWEl_EstC_MET->Add(hWEl_DataC_MET, hWEl_BGC_MET, 1., -1.); 

  // bins CJV
  TH1D* hWEl_RVBF_CJV = new TH1D("hWEl_RVBF_CJV", "", 4, cjvEdges);  
  TH1D* hWEl_R_CJV    = new TH1D("hWEl_R_CJV", "", 4, cjvEdges);
  TH1D* hWMu_EstC_CJV = new TH1D("hWMu_EstC_CJV", "", 4, cjvEdges);
  TH1D* hWEl_EstS_CJV = new TH1D("hWEl_EstS_CJV", "", 4, cjvEdges);
  TH1D* hWEl_EstC_CJV = new TH1D("hWEl_EstC_CJV", "", 4, cjvEdges);

  hWEl_RVBF_CJV->Divide(hWEl_VBF_CJV, hWMu_VBF_CJV, 1., 1.);
  for(int ibin = 1; ibin <= hWEl_R_CJV->GetNbinsX(); ++ibin) {
        hWEl_R_CJV->SetBinContent(ibin, hWEl_RVBF_CJV->GetBinContent(ibin) * hWEl_RGEN->GetBinContent(1));
        hWEl_R_CJV->SetBinError(ibin, hWEl_R_CJV->GetBinContent(ibin) * sqrt(pow(hWEl_RVBF_CJV->GetBinError(ibin)/hWEl_RVBF_CJV->GetBinContent(ibin),2) + pow(hWEl_RGEN->GetBinError(1)/hWEl_RGEN->GetBinContent(1),2)));
  }
  hWMu_EstC_CJV->Add(hWMu_DataC_CJV, hWMu_BGC_CJV, 1., -1.);
  hWEl_EstS_CJV->Multiply(hWMu_EstC_CJV, hWEl_R_CJV, 1., 1.);       
  hWEl_EstC_CJV->Add(hWEl_DataC_CJV, hWEl_BGC_CJV, 1., -1.);
 
  std::cout << std::endl;
  std::cout << "##################################### Closure test WMu-WEl control region #####################################" << std::endl;
  std::cout << std::endl;
  std::cout << "  GEN ratio by histogram for WEl : " << hWEl_RGEN->GetBinContent(1) << " +/- " << hWEl_RGEN->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "##### Bins dPhi_jj #####" << std::endl;
  std::cout << std::endl;
  std::cout << "  dphi < 1.0" << std::endl;
  std::cout << std::endl;;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_DPhi->GetBinContent(1) << " +/- " << hWEl_RVBF_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_DPhi->GetBinContent(1) << " +/- " << hWEl_R_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_DPhi->GetBinContent(1) << " +/- " << hWMu_DataC_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_DPhi->GetBinContent(1) << " +/- " << hWMu_BGC_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_DPhi->GetBinContent(1) << " +/- " << hWEl_EstS_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_DPhi->GetBinContent(1) << " +/- " << hWEl_EstC_DPhi->GetBinError(1) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_DPhi->GetBinContent(1) << " +/- " << hWEl_VBF_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  dphi > 1.0 dphi < 1.8" << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_DPhi->GetBinContent(2) << " +/- " << hWEl_RVBF_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_DPhi->GetBinContent(2) << " +/- " << hWEl_R_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_DPhi->GetBinContent(2) << " +/- " << hWMu_DataC_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_DPhi->GetBinContent(2) << " +/- " << hWMu_BGC_DPhi->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_DPhi->GetBinContent(2) << " +/- " << hWEl_EstS_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_DPhi->GetBinContent(2) << " +/- " << hWEl_EstC_DPhi->GetBinError(2) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_DPhi->GetBinContent(2) << " +/- " << hWEl_VBF_DPhi->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "  dphi > 1.8 dphi < 2.6" << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_DPhi->GetBinContent(3) << " +/- " << hWEl_RVBF_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_DPhi->GetBinContent(3) << " +/- " << hWEl_R_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_DPhi->GetBinContent(3) << " +/- " << hWMu_DataC_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_DPhi->GetBinContent(3) << " +/- " << hWMu_BGC_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_DPhi->GetBinContent(3) << " +/- " << hWEl_EstS_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_DPhi->GetBinContent(3) << " +/- " << hWEl_EstC_DPhi->GetBinError(3) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_DPhi->GetBinContent(3) << " +/- " << hWEl_VBF_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  dphi > 2.6" << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_DPhi->GetBinContent(4) << " +/- " << hWEl_RVBF_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_DPhi->GetBinContent(4) << " +/- " << hWEl_R_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_DPhi->GetBinContent(4) << " +/- " << hWMu_DataC_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_DPhi->GetBinContent(4) << " +/- " << hWMu_BGC_DPhi->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_DPhi->GetBinContent(4) << " +/- " << hWEl_EstS_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_DPhi->GetBinContent(4) << " +/- " << hWEl_EstC_DPhi->GetBinError(4) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_DPhi->GetBinContent(4) << " +/- " << hWEl_VBF_DPhi->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "##### Bins W daughter rapidity #####" << std::endl;
  std::cout << std::endl;
  //std::cout << "  -2.1 < dphi < -1.0 " << std::endl;
  std::cout << "  0.0 < dphi < 0.5 " << std::endl;
  std::cout << std::endl;;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_Eta->GetBinContent(1) << " +/- " << hWEl_RVBF_Eta->GetBinError(1) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_Eta->GetBinContent(1) << " +/- " << hWEl_R_Eta->GetBinError(1) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_Eta->GetBinContent(1) << " +/- " << hWMu_DataC_Eta->GetBinError(1) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_Eta->GetBinContent(1) << " +/- " << hWMu_BGC_Eta->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_Eta->GetBinContent(1) << " +/- " << hWEl_EstS_Eta->GetBinError(1) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_Eta->GetBinContent(1) << " +/- " << hWEl_EstC_Eta->GetBinError(1) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_Eta->GetBinContent(1) << " +/- " << hWEl_VBF_Eta->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  0.5 < dphi < 1.0 " << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_Eta->GetBinContent(2) << " +/- " << hWEl_RVBF_Eta->GetBinError(2) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_Eta->GetBinContent(2) << " +/- " << hWEl_R_Eta->GetBinError(2) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_Eta->GetBinContent(2) << " +/- " << hWMu_DataC_Eta->GetBinError(2) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_Eta->GetBinContent(2) << " +/- " << hWMu_BGC_Eta->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_Eta->GetBinContent(2) << " +/- " << hWEl_EstS_Eta->GetBinError(2) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_Eta->GetBinContent(2) << " +/- " << hWEl_EstC_Eta->GetBinError(2) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_Eta->GetBinContent(2) << " +/- " << hWEl_VBF_Eta->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "  1.0 < dphi < 1.5 " << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_Eta->GetBinContent(3) << " +/- " << hWEl_RVBF_Eta->GetBinError(3) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_Eta->GetBinContent(3) << " +/- " << hWEl_R_Eta->GetBinError(3) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_Eta->GetBinContent(3) << " +/- " << hWMu_DataC_Eta->GetBinError(3) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_Eta->GetBinContent(3) << " +/- " << hWMu_BGC_Eta->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_Eta->GetBinContent(3) << " +/- " << hWEl_EstS_Eta->GetBinError(3) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_Eta->GetBinContent(3) << " +/- " << hWEl_EstC_Eta->GetBinError(3) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_Eta->GetBinContent(3) << " +/- " << hWEl_VBF_Eta->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  1.5 < dphi < 2.1 " << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_Eta->GetBinContent(4) << " +/- " << hWEl_RVBF_Eta->GetBinError(4) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_Eta->GetBinContent(4) << " +/- " << hWEl_R_Eta->GetBinError(4) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_Eta->GetBinContent(4) << " +/- " << hWMu_DataC_Eta->GetBinError(4) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_Eta->GetBinContent(4) << " +/- " << hWMu_BGC_Eta->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_Eta->GetBinContent(4) << " +/- " << hWEl_EstS_Eta->GetBinError(4) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_Eta->GetBinContent(4) << " +/- " << hWEl_EstC_Eta->GetBinError(4) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_Eta->GetBinContent(4) << " +/- " << hWEl_VBF_Eta->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "##### Bins dijet invariant mass (Mjj) #####" << std::endl;
  std::cout << std::endl;
  std::cout << "  800 < Mjj < 1000 " << std::endl;
  std::cout << std::endl;;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_Mjj->GetBinContent(1) << " +/- " << hWEl_RVBF_Mjj->GetBinError(1) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_Mjj->GetBinContent(1) << " +/- " << hWEl_R_Mjj->GetBinError(1) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_Mjj->GetBinContent(1) << " +/- " << hWMu_DataC_Mjj->GetBinError(1) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_Mjj->GetBinContent(1) << " +/- " << hWMu_BGC_Mjj->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_Mjj->GetBinContent(1) << " +/- " << hWEl_EstS_Mjj->GetBinError(1) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_Mjj->GetBinContent(1) << " +/- " << hWEl_EstC_Mjj->GetBinError(1) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_Mjj->GetBinContent(1) << " +/- " << hWEl_VBF_Mjj->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  1000 < Mjj < 1200 " << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_Mjj->GetBinContent(2) << " +/- " << hWEl_RVBF_Mjj->GetBinError(2) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_Mjj->GetBinContent(2) << " +/- " << hWEl_R_Mjj->GetBinError(2) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_Mjj->GetBinContent(2) << " +/- " << hWMu_DataC_Mjj->GetBinError(2) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_Mjj->GetBinContent(2) << " +/- " << hWMu_BGC_Mjj->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_Mjj->GetBinContent(2) << " +/- " << hWEl_EstS_Mjj->GetBinError(2) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_Mjj->GetBinContent(2) << " +/- " << hWEl_EstC_Mjj->GetBinError(2) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_Mjj->GetBinContent(2) << " +/- " << hWEl_VBF_Mjj->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "  1200 < Mjj < 1600 " << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_Mjj->GetBinContent(3) << " +/- " << hWEl_RVBF_Mjj->GetBinError(3) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_Mjj->GetBinContent(3) << " +/- " << hWEl_R_Mjj->GetBinError(3) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_Mjj->GetBinContent(3) << " +/- " << hWMu_DataC_Mjj->GetBinError(3) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_Mjj->GetBinContent(3) << " +/- " << hWMu_BGC_Mjj->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_Mjj->GetBinContent(3) << " +/- " << hWEl_EstS_Mjj->GetBinError(3) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_Mjj->GetBinContent(3) << " +/- " << hWEl_EstC_Mjj->GetBinError(3) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_Mjj->GetBinContent(3) << " +/- " << hWEl_VBF_Mjj->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  1600 < Mjj < 2000 " << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_Mjj->GetBinContent(4) << " +/- " << hWEl_RVBF_Mjj->GetBinError(4) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_Mjj->GetBinContent(4) << " +/- " << hWEl_R_Mjj->GetBinError(4) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_Mjj->GetBinContent(4) << " +/- " << hWMu_DataC_Mjj->GetBinError(4) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_Mjj->GetBinContent(4) << " +/- " << hWMu_BGC_Mjj->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_Mjj->GetBinContent(4) << " +/- " << hWEl_EstS_Mjj->GetBinError(4) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_Mjj->GetBinContent(4) << " +/- " << hWEl_EstC_Mjj->GetBinError(4) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_Mjj->GetBinContent(4) << " +/- " << hWEl_VBF_Mjj->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "  2000 < Mjj < 3000 " << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_Mjj->GetBinContent(5) << " +/- " << hWEl_RVBF_Mjj->GetBinError(5) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_Mjj->GetBinContent(5) << " +/- " << hWEl_R_Mjj->GetBinError(5) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_Mjj->GetBinContent(5) << " +/- " << hWMu_DataC_Mjj->GetBinError(5) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_Mjj->GetBinContent(5) << " +/- " << hWMu_BGC_Mjj->GetBinError(5) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_Mjj->GetBinContent(5) << " +/- " << hWEl_EstS_Mjj->GetBinError(5) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_Mjj->GetBinContent(5) << " +/- " << hWEl_EstC_Mjj->GetBinError(5) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_Mjj->GetBinContent(5) << " +/- " << hWEl_VBF_Mjj->GetBinError(5) << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "##### Bins MET #####" << std::endl;
  std::cout << std::endl;
  std::cout << "  100 < MET < 150 " << std::endl;
  std::cout << std::endl;;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_MET->GetBinContent(1) << " +/- " << hWEl_RVBF_MET->GetBinError(1) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_MET->GetBinContent(1) << " +/- " << hWEl_R_MET->GetBinError(1) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_MET->GetBinContent(1) << " +/- " << hWMu_DataC_MET->GetBinError(1) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_MET->GetBinContent(1) << " +/- " << hWMu_BGC_MET->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_MET->GetBinContent(1) << " +/- " << hWEl_EstS_MET->GetBinError(1) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_MET->GetBinContent(1) << " +/- " << hWEl_EstC_MET->GetBinError(1) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_MET->GetBinContent(1) << " +/- " << hWEl_VBF_MET->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  150 < MET < 200 " << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_MET->GetBinContent(2) << " +/- " << hWEl_RVBF_MET->GetBinError(2) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_MET->GetBinContent(2) << " +/- " << hWEl_R_MET->GetBinError(2) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_MET->GetBinContent(2) << " +/- " << hWMu_DataC_MET->GetBinError(2) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_MET->GetBinContent(2) << " +/- " << hWMu_BGC_MET->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_MET->GetBinContent(2) << " +/- " << hWEl_EstS_MET->GetBinError(2) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_MET->GetBinContent(2) << " +/- " << hWEl_EstC_MET->GetBinError(2) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_MET->GetBinContent(2) << " +/- " << hWEl_VBF_MET->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "  200 < MET < 300 " << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_MET->GetBinContent(3) << " +/- " << hWEl_RVBF_MET->GetBinError(3) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_MET->GetBinContent(3) << " +/- " << hWEl_R_MET->GetBinError(3) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_MET->GetBinContent(3) << " +/- " << hWMu_DataC_MET->GetBinError(3) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_MET->GetBinContent(3) << " +/- " << hWMu_BGC_MET->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_MET->GetBinContent(3) << " +/- " << hWEl_EstS_MET->GetBinError(3) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_MET->GetBinContent(3) << " +/- " << hWEl_EstC_MET->GetBinError(3) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_MET->GetBinContent(3) << " +/- " << hWEl_VBF_MET->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  300 < MET < 500 " << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_MET->GetBinContent(4) << " +/- " << hWEl_RVBF_MET->GetBinError(4) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_MET->GetBinContent(4) << " +/- " << hWEl_R_MET->GetBinError(4) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_MET->GetBinContent(4) << " +/- " << hWMu_DataC_MET->GetBinError(4) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_MET->GetBinContent(4) << " +/- " << hWMu_BGC_MET->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_MET->GetBinContent(4) << " +/- " << hWEl_EstS_MET->GetBinError(4) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_MET->GetBinContent(4) << " +/- " << hWEl_EstC_MET->GetBinError(4) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_MET->GetBinContent(4) << " +/- " << hWEl_VBF_MET->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "##### Bins CJV #####" << std::endl;
  std::cout << std::endl;
  std::cout << "  10 < CJV < 30 " << std::endl;
  std::cout << std::endl;;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_CJV->GetBinContent(1) << " +/- " << hWEl_RVBF_CJV->GetBinError(1) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_CJV->GetBinContent(1) << " +/- " << hWEl_R_CJV->GetBinError(1) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_CJV->GetBinContent(1) << " +/- " << hWMu_DataC_CJV->GetBinError(1) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_CJV->GetBinContent(1) << " +/- " << hWMu_BGC_CJV->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_CJV->GetBinContent(1) << " +/- " << hWEl_EstS_CJV->GetBinError(1) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_CJV->GetBinContent(1) << " +/- " << hWEl_EstC_CJV->GetBinError(1) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_CJV->GetBinContent(1) << " +/- " << hWEl_VBF_CJV->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  30 < CJV < 60 " << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_CJV->GetBinContent(2) << " +/- " << hWEl_RVBF_CJV->GetBinError(2) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_CJV->GetBinContent(2) << " +/- " << hWEl_R_CJV->GetBinError(2) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_CJV->GetBinContent(2) << " +/- " << hWMu_DataC_CJV->GetBinError(2) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_CJV->GetBinContent(2) << " +/- " << hWMu_BGC_CJV->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_CJV->GetBinContent(2) << " +/- " << hWEl_EstS_CJV->GetBinError(2) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_CJV->GetBinContent(2) << " +/- " << hWEl_EstC_CJV->GetBinError(2) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_CJV->GetBinContent(2) << " +/- " << hWEl_VBF_CJV->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "  60 < CJV < 100 " << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_CJV->GetBinContent(3) << " +/- " << hWEl_RVBF_CJV->GetBinError(3) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_CJV->GetBinContent(3) << " +/- " << hWEl_R_CJV->GetBinError(3) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_CJV->GetBinContent(3) << " +/- " << hWMu_DataC_CJV->GetBinError(3) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_CJV->GetBinContent(3) << " +/- " << hWMu_BGC_CJV->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_CJV->GetBinContent(3) << " +/- " << hWEl_EstS_CJV->GetBinError(3) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_CJV->GetBinContent(3) << " +/- " << hWEl_EstC_CJV->GetBinError(3) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_CJV->GetBinContent(3) << " +/- " << hWEl_VBF_CJV->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  100 < CJV < 150 " << std::endl;
  std::cout << std::endl;
  std::cout << "  VBF ratio by histogram : " << hWEl_RVBF_CJV->GetBinContent(4) << " +/- " << hWEl_RVBF_CJV->GetBinError(4) << std::endl;
  std::cout << "  Total ratio            : " << hWEl_R_CJV->GetBinContent(4) << " +/- " << hWEl_R_CJV->GetBinError(4) << std::endl;
  std::cout << "  Data WMu ctrl region   : " << hWMu_DataC_CJV->GetBinContent(4) << " +/- " << hWMu_DataC_CJV->GetBinError(4) << std::endl;
  std::cout << "  Background WMu ctrl    : " << hWMu_BGC_CJV->GetBinContent(4) << " +/- " << hWMu_BGC_CJV->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "  Predicted WEl from WMu : " << hWEl_EstS_CJV->GetBinContent(4) << " +/- " << hWEl_EstS_CJV->GetBinError(4) << std::endl;
  std::cout << "  Observed WEl           : " << hWEl_EstC_CJV->GetBinContent(4) << " +/- " << hWEl_EstC_CJV->GetBinError(4) << std::endl;
  std::cout << "  MC Prediction          : " << hWEl_VBF_CJV->GetBinContent(4) << " +/- " << hWEl_VBF_CJV->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "###############################################################################################################" << std::endl;
  std::cout << std::endl;

  // draw control plots
  std::string pdfName;

  //gStyle->SetOptStat(0);
  //gStyle->SetOptFit(111111111);

  double x_dPhi[4]  = {0.5, 1.4, 2.2, 2.6 + (TMath::Pi()-2.6)/2};
  double ex_dPhi[4] = {0.5, 0.4, 0.4, (TMath::Pi()-2.6)/2};
  double x_eta[4]   = {0.25, 0.75, 1.25, 1.80};
  double ex_eta[4]  = {0.25, 0.25, 0.25, 0.30};
  //double x_eta[4]   = {-1.55, -0.50, +0.50, +1.55};
  //double ex_eta[4]  = {0.55, 0.5, 0.5, 0.55};
  double x_mjj[5]   = {900., 1100., 1400., 1800., 2500.};
  double ex_mjj[5]  = {100, 100, 200, 200, 500};
  double x_met[4]   = {125., 175., 250., 400.};
  double ex_met[4]  = {25, 25, 50, 100};
  double x_cjv[4]   = {20., 45., 80., 125.};
  double ex_cjv[4]  = {10, 15, 20, 25};

  double y_dPhi1[4],ey_dPhi1[4],y_dPhi2[4],ey_dPhi2[4],y_dPhi3[4],ey_dPhi3[4],frac_dPhi[4],efrac_dPhi[4],diff_dPhi[4],ediff_dPhi[4];
  double y_eta1[4],ey_eta1[4],y_eta2[4],ey_eta2[4],y_eta3[4],ey_eta3[4],frac_eta[4],efrac_eta[4],diff_eta[4],ediff_eta[4];
  double y_met1[4],ey_met1[4],y_met2[4],ey_met2[4],y_met3[4],ey_met3[4],frac_met[4],efrac_met[4],diff_met[4],ediff_met[4];
  double y_cjv1[4],ey_cjv1[4],y_cjv2[4],ey_cjv2[4],y_cjv3[4],ey_cjv3[4],frac_cjv[4],efrac_cjv[4],diff_cjv[4],ediff_cjv[4];
  double y_syst1[4],e_syst1[4];

  double y_mjj1[5],ey_mjj1[5],y_mjj2[5],ey_mjj2[5],y_mjj3[5],ey_mjj3[5],frac_mjj[5],efrac_mjj[5],diff_mjj[5],ediff_mjj[5];
  double y_syst2[5],e_syst2[5];

  for(int i=0; i<4; ++i) {
    y_syst1[i] = 0.;
    e_syst1[i] = 0.23;

    y_dPhi1[i]  = hWEl_EstS_DPhi->GetBinContent(i+1);	//Predicted WEl
    ey_dPhi1[i] = hWEl_EstS_DPhi->GetBinError(i+1);	
    y_dPhi2[i]  = hWEl_EstC_DPhi->GetBinContent(i+1);	//Observed WEl
    ey_dPhi2[i] = hWEl_EstC_DPhi->GetBinError(i+1);
    y_dPhi3[i]  = hWEl_VBF_DPhi->GetBinContent(i+1);    //MC Prediction
    ey_dPhi3[i] = hWEl_VBF_DPhi->GetBinError(i+1); 

    if(y_dPhi2[i] > 0) frac_dPhi[i]  = (y_dPhi1[i] - y_dPhi2[i])/y_dPhi2[i];
    efrac_dPhi[i] = (y_dPhi1[i]/y_dPhi2[i]) * sqrt(pow(ey_dPhi1[i]/y_dPhi1[i],2) + pow(ey_dPhi2[i]/y_dPhi2[i],2));
    //diff_dPhi[i]  = y_dPhi3[i]-y_dPhi1[i];
    //ediff_dPhi[i] = 

    y_eta1[i]  = hWEl_EstS_Eta->GetBinContent(i+1);	//Predicted WEl
    ey_eta1[i] = hWEl_EstS_Eta->GetBinError(i+1);	
    y_eta2[i]  = hWEl_EstC_Eta->GetBinContent(i+1);	//Observed WEl
    ey_eta2[i] = hWEl_EstC_Eta->GetBinError(i+1);
    y_eta3[i]  = hWEl_VBF_Eta->GetBinContent(i+1);      //MC Prediction
    ey_eta3[i] = hWEl_VBF_Eta->GetBinError(i+1); 

    if(y_eta2[i] > 0) frac_eta[i]  = (y_eta1[i] - y_eta2[i])/y_eta2[i];
    efrac_eta[i] = (y_eta1[i]/y_eta2[i]) * sqrt(pow(ey_eta1[i]/y_eta1[i],2) + pow(ey_eta2[i]/y_eta2[i],2));

    y_met1[i]  = hWEl_EstS_MET->GetBinContent(i+1);	//Predicted WEl
    ey_met1[i] = hWEl_EstS_MET->GetBinError(i+1);	
    y_met2[i]  = hWEl_EstC_MET->GetBinContent(i+1);	//Observed WEl
    ey_met2[i] = hWEl_EstC_MET->GetBinError(i+1);
    y_met3[i]  = hWEl_VBF_MET->GetBinContent(i+1);      //MC Prediction
    ey_met3[i] = hWEl_VBF_MET->GetBinError(i+1); 

    if(y_met2[i] > 0) frac_met[i]  = (y_met1[i] - y_met2[i])/y_met2[i];
    efrac_met[i] = (y_met1[i]/y_met2[i]) * sqrt(pow(ey_met1[i]/y_met1[i],2) + pow(ey_met2[i]/y_met2[i],2));

    y_cjv1[i]  = hWEl_EstS_CJV->GetBinContent(i+1);	//Predicted WEl
    ey_cjv1[i] = hWEl_EstS_CJV->GetBinError(i+1);	
    y_cjv2[i]  = hWEl_EstC_CJV->GetBinContent(i+1);	//Observed WEl
    ey_cjv2[i] = hWEl_EstC_CJV->GetBinError(i+1);
    y_cjv3[i]  = hWEl_VBF_CJV->GetBinContent(i+1);      //MC Prediction
    ey_cjv3[i] = hWEl_VBF_CJV->GetBinError(i+1); 

    if(y_cjv2[i] > 0) frac_cjv[i]  = (y_cjv1[i] - y_cjv2[i])/y_cjv2[i];
    efrac_cjv[i] = (y_cjv1[i]/y_cjv2[i]) * sqrt(pow(ey_cjv1[i]/y_cjv1[i],2) + pow(ey_cjv2[i]/y_cjv2[i],2));
  }

  for(int i=0; i<5; ++i) {
    y_syst2[i] = 0.;
    e_syst2[i] = 0.23;

    y_mjj1[i]  = hWEl_EstS_Mjj->GetBinContent(i+1);	//Predicted WEl
    ey_mjj1[i] = hWEl_EstS_Mjj->GetBinError(i+1);	
    y_mjj2[i]  = hWEl_EstC_Mjj->GetBinContent(i+1);	//Observed WEl
    ey_mjj2[i] = hWEl_EstC_Mjj->GetBinError(i+1);
    y_mjj3[i]  = hWEl_VBF_Mjj->GetBinContent(i+1);      //MC Prediction
    ey_mjj3[i] = hWEl_VBF_Mjj->GetBinError(i+1); 

    if(y_mjj2[i] > 0) frac_mjj[i]  = (y_mjj1[i] - y_mjj2[i])/y_mjj2[i];
    efrac_mjj[i] = (y_mjj1[i]/y_mjj2[i]) * sqrt(pow(ey_mjj1[i]/y_mjj1[i],2) + pow(ey_mjj2[i]/y_mjj2[i],2));
  }

  TH1D *h1 = new TH1D("h1", "", 1, 0, TMath::Pi());
  TH1D *h2 = new TH1D("h2", "", 1, 0., +2.1);
  //TH1D *h2 = new TH1D("h2", "", 1, -2.1, +2.1);
  TH1D *h3 = new TH1D("h3", "", 1, 800., 3000.);
  TH1D *h4 = new TH1D("h4", "", 1, 100., 500.);
  TH1D *h5 = new TH1D("h5", "", 1, 10., 150.);

  TF1 *f1 = new TF1("f1","pol0",0,TMath::Pi());  	// fit 4 bins
  //TF1 *f1 = new TF1("f1","pol0",0, 2.6); 		// fit 3 bins
  TF1 *f2 = new TF1("f2","pol0",0., +2.1);
  //TF1 *f2 = new TF1("f2","pol0",-2.1, +2.1); 
  TF1 *f3 = new TF1("f3","pol0",800., 3000.); 
  TF1 *f4 = new TF1("f4","pol0",100., 500.); 
  TF1 *f5 = new TF1("f5","pol0",10., 150.); 

  TGraphErrors *gp_dPhi1  = new TGraphErrors(4,x_dPhi,y_dPhi1,ex_dPhi,ey_dPhi1);
  TGraphErrors *gp_dPhi2  = new TGraphErrors(4,x_dPhi,y_dPhi2,ex_dPhi,ey_dPhi2);
  TGraphErrors *gp_dPhi3  = new TGraphErrors(4,x_dPhi,y_dPhi3,ex_dPhi,ey_dPhi3);
  TGraphErrors *gp_dPhiS  = new TGraphErrors(4,x_dPhi,y_syst1,ex_dPhi,e_syst1);
  TGraphErrors *gp_dPhiF  = new TGraphErrors(4,x_dPhi,frac_dPhi,ex_dPhi,efrac_dPhi);

  TGraphErrors *gp_eta1  = new TGraphErrors(4,x_eta,y_eta1,ex_eta,ey_eta1);
  TGraphErrors *gp_eta2  = new TGraphErrors(4,x_eta,y_eta2,ex_eta,ey_eta2);
  TGraphErrors *gp_eta3  = new TGraphErrors(4,x_eta,y_eta3,ex_eta,ey_eta3);
  TGraphErrors *gp_etaS  = new TGraphErrors(4,x_eta,y_syst1,ex_eta,e_syst1);
  TGraphErrors *gp_etaF  = new TGraphErrors(4,x_eta,frac_eta,ex_eta,efrac_eta);

  TGraphErrors *gp_mjj1  = new TGraphErrors(5,x_mjj,y_mjj1,ex_mjj,ey_mjj1);
  TGraphErrors *gp_mjj2  = new TGraphErrors(5,x_mjj,y_mjj2,ex_mjj,ey_mjj2);
  TGraphErrors *gp_mjj3  = new TGraphErrors(5,x_mjj,y_mjj3,ex_mjj,ey_mjj3);
  TGraphErrors *gp_mjjS  = new TGraphErrors(5,x_mjj,y_syst2,ex_mjj,e_syst2);
  TGraphErrors *gp_mjjF  = new TGraphErrors(5,x_mjj,frac_mjj,ex_mjj,efrac_mjj);

  TGraphErrors *gp_met1  = new TGraphErrors(4,x_met,y_met1,ex_met,ey_met1);
  TGraphErrors *gp_met2  = new TGraphErrors(4,x_met,y_met2,ex_met,ey_met2);
  TGraphErrors *gp_met3  = new TGraphErrors(4,x_met,y_met3,ex_met,ey_met3);
  TGraphErrors *gp_metS  = new TGraphErrors(4,x_met,y_syst1,ex_met,e_syst1);
  TGraphErrors *gp_metF  = new TGraphErrors(4,x_met,frac_met,ex_met,efrac_met);

  TGraphErrors *gp_cjv1  = new TGraphErrors(4,x_cjv,y_cjv1,ex_cjv,ey_cjv1);
  TGraphErrors *gp_cjv2  = new TGraphErrors(4,x_cjv,y_cjv2,ex_cjv,ey_cjv2);
  TGraphErrors *gp_cjv3  = new TGraphErrors(4,x_cjv,y_cjv3,ex_cjv,ey_cjv3);
  TGraphErrors *gp_cjvS  = new TGraphErrors(4,x_cjv,y_syst1,ex_cjv,e_syst1);
  TGraphErrors *gp_cjvF  = new TGraphErrors(4,x_cjv,frac_cjv,ex_cjv,efrac_cjv);

  TPaveText *cms = new TPaveText(0.12, 0.68, 0.58 , 0.88, "NDC");
  cms->SetFillColor(0);
  cms->SetFillStyle(4000);
  cms->SetBorderSize(0);
  cms->SetLineColor(0);
  cms->SetTextAlign(12);
  cms->AddText("CMS Preliminary");
  cms->AddText("");
  cms->AddText("#sqrt{s} = 8 TeV L = 19.6 fb^{-1}");
  cms->AddText("");

  TCanvas canvas; 
  canvas.SetCanvasSize(canvas.GetWindowWidth(), 1.2*canvas.GetWindowHeight());
  TPad *pad = (TPad *)canvas.cd(1);
  pad->SetPad("pad","pad",0.05,0.05,.97,.97,kWhite,0,0);
  
  gp_dPhi1->SetTitle("");
  gp_dPhi1->SetMarkerStyle(20);
  gp_dPhi1->SetMarkerSize(0.9);
  gp_dPhi1->SetLineColor(kRed);
  gp_dPhi1->SetMarkerColor(kRed);
  gp_dPhi1->GetXaxis()->SetTitle("#Delta #phi_{jj}");
  gp_dPhi1->GetXaxis()->SetRangeUser(0,TMath::Pi());
  gp_dPhi1->GetYaxis()->SetTitle("N(W#rightarrow e#nu)");
  gp_dPhi1->GetYaxis()->SetRangeUser(0,150);
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

  TLegend leg(0.12,0.67,0.37,0.88);
  leg.SetBorderSize(0);
  leg.SetFillColor(0);
  leg.AddEntry(gp_dPhi1,"predicted (data)","P");
  leg.AddEntry(gp_dPhi2,"observed (data)","P");
  leg.AddEntry(gp_dPhi3,"predicted (MC)","P");
  leg.Draw();

  pdfName= oDir + std::string("/DPhi_Welnu_num.pdf");
  canvas.Print(pdfName.c_str());
 
  h1->GetXaxis()->SetTitle("#Delta #phi_{jj}");
  h1->GetYaxis()->SetTitle("#frac{Predicted - Observed}{Observed}");
  h1->GetYaxis()->SetRangeUser(-2,2);
  h1->SetStats(kFALSE);
  h1->SetLineColor(kBlue);
  h1->SetLineWidth(2);
  h1->Draw();
  gp_dPhiS->SetLineColor(kGray);
  gp_dPhiS->SetLineWidth(0);
  gp_dPhiS->SetFillColor(kGray);
  gp_dPhiS->SetFillStyle(3001);
  gp_dPhiF->SetMarkerStyle(20);
  gp_dPhiF->SetMarkerSize(1.5);
  gp_dPhiF->SetMarkerColor(kBlue);
  gp_dPhiF->Fit("f1","R");
  h1->Draw();
  gp_dPhiS->Draw("2 same");
  gp_dPhiF->Draw("P same");

  TLegend leg2(0.62,0.67,0.88,0.87);
  leg2.SetBorderSize(0);
  leg2.SetFillColor(0);
  leg2.AddEntry(f1,"Fit","l");
  //leg2.AddEntry(f1,"pol0 fit (0 < #Delta #phi_{jj} < 2.6)","l");
  leg2.AddEntry(gp_dPhiS,"Systematic error","f");
  leg2.Draw();
  cms->Draw();

  pdfName= oDir + std::string("/DPhi_Welnu_frac.pdf");
  canvas.Print(pdfName.c_str());

  gp_eta1->SetTitle("");
  gp_eta1->SetMarkerStyle(20);
  gp_eta1->SetMarkerSize(0.9);
  gp_eta1->SetLineColor(kRed);
  gp_eta1->SetMarkerColor(kRed);
  gp_eta1->GetXaxis()->SetTitle("|#eta^{l}|");
  gp_eta1->GetXaxis()->SetRangeUser(-2.1,+2.1);
  gp_eta1->GetYaxis()->SetTitle("N(W#rightarrow e#nu)");
  gp_eta1->GetYaxis()->SetRangeUser(0,40);
  gp_eta1->Draw("AP");
  gp_eta2->SetMarkerStyle(20);
  gp_eta2->SetMarkerSize(0.9);
  gp_eta2->SetLineColor(kBlue);
  gp_eta2->SetMarkerColor(kBlue);
  gp_eta2->Draw("P same");
  gp_eta3->SetMarkerStyle(20);
  gp_eta3->SetMarkerSize(0.9);
  gp_eta3->SetLineColor(kViolet);
  gp_eta3->SetMarkerColor(kViolet);
  gp_eta3->Draw("P same");
  leg.Draw();

  pdfName= oDir + std::string("/Eta_Welnu_num.pdf");
  canvas.Print(pdfName.c_str());
 
  h2->GetXaxis()->SetTitle("|#eta^{l}|");
  h2->GetYaxis()->SetTitle("#frac{Predicted - Observed}{Observed}");
  h2->GetYaxis()->SetRangeUser(-1.5,3.0);
  h2->SetStats(kFALSE);
  h2->SetLineColor(kBlue);
  h2->SetLineWidth(2);
  h2->Draw();
  gp_etaS->SetLineColor(kGray);
  gp_etaS->SetLineWidth(0);
  gp_etaS->SetFillColor(kGray);
  gp_etaS->SetFillStyle(3001);
  gp_etaF->SetMarkerStyle(20);
  gp_etaF->SetMarkerSize(1.5);
  gp_etaF->SetMarkerColor(kBlue);
  gp_etaF->Fit("f2","R");
  h2->Draw();
  gp_etaS->Draw("2 same");
  gp_etaF->Draw("P same");

  TLegend leg3(0.62,0.67,0.88,0.87);
  leg3.SetBorderSize(0);
  leg3.SetFillColor(0);
  leg3.AddEntry(f2,"Fit","l");
  leg3.AddEntry(gp_etaS,"Systematic error","f");
  leg3.Draw();
  cms->Draw();

  pdfName= oDir + std::string("/Eta_Welnu_frac.pdf");
  canvas.Print(pdfName.c_str());

  gp_mjj1->SetTitle("");
  gp_mjj1->SetMarkerStyle(20);
  gp_mjj1->SetMarkerSize(0.9);
  gp_mjj1->SetLineColor(kRed);
  gp_mjj1->SetMarkerColor(kRed);
  gp_mjj1->GetXaxis()->SetTitle("M_{jj} [GeV]");
  gp_mjj1->GetXaxis()->SetRangeUser(800.,3000.);
  gp_mjj1->GetYaxis()->SetTitle("N(W#rightarrow e#nu)");
  gp_mjj1->GetYaxis()->SetRangeUser(0,90);
  gp_mjj1->Draw("AP");
  gp_mjj2->SetMarkerStyle(20);
  gp_mjj2->SetMarkerSize(0.9);
  gp_mjj2->SetLineColor(kBlue);
  gp_mjj2->SetMarkerColor(kBlue);
  gp_mjj2->Draw("P same");
  gp_mjj3->SetMarkerStyle(20);
  gp_mjj3->SetMarkerSize(0.9);
  gp_mjj3->SetLineColor(kViolet);
  gp_mjj3->SetMarkerColor(kViolet);
  gp_mjj3->Draw("P same");
  leg.Draw();

  pdfName= oDir + std::string("/MJJ_Welnu_num.pdf");
  canvas.Print(pdfName.c_str());
  

  h3->GetXaxis()->SetTitle("M_{jj} [GeV]");
  h3->GetXaxis()->SetTitleSize(0.053);
  h3->GetXaxis()->SetTitleOffset(0.85);
  h3->GetYaxis()->SetTitle("(Predicted - Observed)/Observed");
  h3->GetYaxis()->SetTitleSize(0.050);
  h3->GetYaxis()->SetTitleOffset(0.85);
  h3->GetYaxis()->SetRangeUser(-1.0,2.0);
  h3->SetStats(kFALSE);
  h3->SetLineColor(kBlack);
  h3->SetLineStyle(7);
  h3->SetLineWidth(2);
  h3->Draw();
  gp_mjjS->SetLineColor(kGray);
  gp_mjjS->SetLineWidth(0);
  gp_mjjS->SetFillColor(kGray);
  gp_mjjS->SetFillStyle(3001);
  gp_mjjF->SetMarkerStyle(20);
  gp_mjjF->SetMarkerSize(1.4);
  gp_mjjF->SetMarkerColor(kBlue);
  gp_mjjF->SetLineWidth(2);
  gp_mjjF->Fit("f3","R");
  h3->Draw();
  gp_mjjS->Draw("2 same");
  gp_mjjF->Draw("P same");

  TLegend leg4(0.65,0.67,0.88,0.87);
  leg4.SetBorderSize(0);
  leg4.SetFillColor(0);
  leg4.AddEntry(f3,"Fit","l");
  leg4.AddEntry(gp_mjjS,"Syst. error","f");
  leg4.Draw();
  cms->Draw();
  pdfName= oDir + std::string("/MJJ_Welnu_frac.pdf");
  canvas.Print(pdfName.c_str());

  gp_met1->SetTitle("");
  gp_met1->SetMarkerStyle(20);
  gp_met1->SetMarkerSize(0.9);
  gp_met1->SetLineColor(kRed);
  gp_met1->SetMarkerColor(kRed);
  gp_met1->GetXaxis()->SetTitle("E_{T}^{miss} [GeV]");
  gp_met1->GetXaxis()->SetRangeUser(100.,500.);
  gp_met1->GetYaxis()->SetTitle("N(W#rightarrow e#nu)");
  gp_met1->GetYaxis()->SetRangeUser(0,70);
  gp_met1->Draw("AP");
  gp_met2->SetMarkerStyle(20);
  gp_met2->SetMarkerSize(0.9);
  gp_met2->SetLineColor(kBlue);
  gp_met2->SetMarkerColor(kBlue);
  gp_met2->Draw("P same");
  gp_met3->SetMarkerStyle(20);
  gp_met3->SetMarkerSize(0.9);
  gp_met3->SetLineColor(kViolet);
  gp_met3->SetMarkerColor(kViolet);
  gp_met3->Draw("P same");
  leg.Draw();

  pdfName= oDir + std::string("/MET_Welnu_num.pdf");
  canvas.Print(pdfName.c_str());

  h4->GetXaxis()->SetTitle("E_{T}^{miss} [GeV]");
  h4->GetXaxis()->SetTitleSize(0.053);
  h4->GetXaxis()->SetTitleOffset(0.85);
  h4->GetYaxis()->SetTitle("(Predicted - Observed)/Observed");
  h4->GetYaxis()->SetTitleSize(0.050);
  h4->GetYaxis()->SetTitleOffset(0.85);
  h4->GetYaxis()->SetRangeUser(-1.0,2.0);
  h4->SetStats(kFALSE);
  h4->SetLineColor(kBlack);
  h4->SetLineStyle(7);
  h4->SetLineWidth(2);
  h4->Draw();
  gp_metS->SetLineColor(kGray);
  gp_metS->SetLineWidth(0);
  gp_metS->SetFillColor(kGray);
  gp_metS->SetFillStyle(3001);
  gp_metF->SetMarkerStyle(20);
  gp_metF->SetMarkerSize(1.4);
  gp_metF->SetMarkerColor(kBlue);
  gp_metF->SetLineWidth(2);
  gp_metF->Fit("f4","R");
  h4->Draw();
  gp_metS->Draw("2 same");
  gp_metF->Draw("P same");

  TLegend leg5(0.65,0.67,0.88,0.87);
  leg5.SetBorderSize(0);
  leg5.SetFillColor(0);
  leg5.AddEntry(f4,"Fit","l");
  leg5.AddEntry(gp_metS,"Syst. error","f");
  leg5.Draw();
  cms->Draw();
  pdfName= oDir + std::string("/MET_Welnu_frac.pdf");
  canvas.Print(pdfName.c_str());

  
  TLegend leg7(0.52,0.57,0.77,0.78);
  leg7.SetBorderSize(0);
  leg7.SetFillColor(0);
  leg7.AddEntry(gp_dPhi1,"predicted (data)","P");
  leg7.AddEntry(gp_dPhi2,"observed (data)","P");
  leg7.AddEntry(gp_dPhi3,"predicted (MC)","P");
  leg7.Draw();

  gp_cjv1->SetTitle("");
  gp_cjv1->SetMarkerStyle(20);
  gp_cjv1->SetMarkerSize(0.9);
  gp_cjv1->SetLineColor(kRed);
  gp_cjv1->SetMarkerColor(kRed);
  gp_cjv1->GetXaxis()->SetTitle("Central Jet E_{T} [GeV]");
  gp_cjv1->GetXaxis()->SetRangeUser(10.,150.);
  gp_cjv1->GetYaxis()->SetTitle("N(W#rightarrow e#nu)");
  gp_cjv1->GetYaxis()->SetRangeUser(0,90);
  gp_cjv1->Draw("AP");
  gp_cjv2->SetMarkerStyle(20);
  gp_cjv2->SetMarkerSize(0.9);
  gp_cjv2->SetLineColor(kBlue);
  gp_cjv2->SetMarkerColor(kBlue);
  gp_cjv2->Draw("P same");
  gp_cjv3->SetMarkerStyle(20);
  gp_cjv3->SetMarkerSize(0.9);
  gp_cjv3->SetLineColor(kViolet);
  gp_cjv3->SetMarkerColor(kViolet);
  gp_cjv3->Draw("P same");
  leg7.Draw();

  pdfName= oDir + std::string("/CJV_Welnu_num.pdf");
  canvas.Print(pdfName.c_str());

  h5->GetXaxis()->SetTitle("Central Jet E_{T} [GeV]");
  h5->GetYaxis()->SetTitle("#frac{Predicted - Observed}{Observed}");
  h5->GetYaxis()->SetRangeUser(-2.0,3.0);
  h5->SetStats(kFALSE);
  h5->SetLineColor(kBlue);
  h5->SetLineWidth(2);
  h5->Draw();
  gp_cjvS->SetLineColor(kGray);
  gp_cjvS->SetLineWidth(0);
  gp_cjvS->SetFillColor(kGray);
  gp_cjvS->SetFillStyle(3001);
  gp_cjvF->SetMarkerStyle(20);
  gp_cjvF->SetMarkerSize(1.5);
  gp_cjvF->SetMarkerColor(kBlue);
  gp_cjvF->Fit("f5","R");
  h5->Draw();
  gp_cjvS->Draw("2 same");
  gp_cjvF->Draw("P same");

  TLegend leg6(0.62,0.67,0.88,0.87);
  leg6.SetBorderSize(0);
  leg6.SetFillColor(0);
  leg6.AddEntry(f5,"Fit","l");
  leg6.AddEntry(gp_cjvS,"Systematic error","f");
  leg6.Draw();
  cms->Draw();
  pdfName= oDir + std::string("/CJV_Welnu_frac.pdf");
  canvas.Print(pdfName.c_str());
  

}
