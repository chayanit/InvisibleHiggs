
#include "InvisibleHiggs/Analysis/interface/ProgramOptions.h"
#include "InvisibleHiggs/Analysis/interface/Cuts.h"
#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"
#include "InvisibleHiggs/Analysis/interface/StackPlot.h"
#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"
#include "InvisibleHiggs/Analysis/interface/Datasets.h"
#include "InvisibleHiggs/Analysis/interface/Constants.h"


#include "TTree.h"
#include "TMath.h"
#include "TH1D.h"

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
  TFile* ofile = TFile::Open( (options.oDir+std::string("/WBackground.root")).c_str(), "RECREATE");

  // cuts
  Cuts cuts;
  unsigned nCutsWMu = cuts.nCutsWMu();
  unsigned nCutsWEl = cuts.nCutsWEl();

  TCut puWeight("puWeight");
  TCut trigCorrWeight("trigCorrWeight");
  TCut wWeight = cuts.wWeight();

  TCut cutSignalNoMETNoDPhi = cuts.HLTandMETFilters() + cuts.leptonVeto() + cuts.vbf();
  TCut met35("met>35.");
  TCut metCtrl35("metNoWLepton>35.");
  TCut met70("met>70.");
  TCut metCtrl70("metNoWLepton>70.");

  // histograms
  double dphiEdges[4] = { 0., 1.0, 2.6, TMath::Pi() };

  TH1D* hWMu_MCC_DPhi = new TH1D("hWMu_MCC_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
  TH1D* hWMu_MCS_DPhi = new TH1D("hWMu_MCS_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region
  TH1D* hWMu_BGC_DPhi = new TH1D("hWMu_BGC_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hWMu_DataC_DPhi = new TH1D("hWMu_DataC_DPhi", "", 3, dphiEdges);  // Data ctrl region

  TH1D* hWEl_MCC_DPhi = new TH1D("hWEl_MCC_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
  TH1D* hWEl_MCS_DPhi = new TH1D("hWEl_MCS_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region
  TH1D* hWEl_BGC_DPhi = new TH1D("hWEl_BGC_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hWEl_DataC_DPhi = new TH1D("hWEl_DataC_DPhi", "", 3, dphiEdges);  // Data ctrl region

  TH1D* hWMu_MC_NoMETC_DPhi = new TH1D("hWMu_MC_NoMETC_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
  TH1D* hWMu_MC_NoMETS_DPhi = new TH1D("hWMu_MC_NoMETS_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region
  TH1D* hWMu_BG_NoMETC_DPhi = new TH1D("hWMu_BG_NoMETC_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hWMu_Data_NoMETC_DPhi = new TH1D("hWMu_Data_NoMETC_DPhi", "", 3, dphiEdges);  // Data ctrl region

  TH1D* hWEl_MC_NoMETC_DPhi = new TH1D("hWEl_MC_NoMETC_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
  TH1D* hWEl_MC_NoMETS_DPhi = new TH1D("hWEl_MC_NoMETS_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region
  TH1D* hWEl_BG_NoMETC_DPhi = new TH1D("hWEl_BG_NoMETC_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hWEl_Data_NoMETC_DPhi = new TH1D("hWEl_Data_NoMETC_DPhi", "", 3, dphiEdges);  // Data ctrl region

  TH1D* hWMu_MC_Loose2C_DPhi = new TH1D("hWMu_MC_Loose2C_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
  TH1D* hWMu_MC_Loose2S_DPhi = new TH1D("hWMu_MC_Loose2S_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region
  TH1D* hWMu_BG_Loose2C_DPhi = new TH1D("hWMu_BG_Loose2C_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hWMu_Data_Loose2C_DPhi = new TH1D("hWMu_Data_Loose2C_DPhi", "", 3, dphiEdges);  // Data ctrl region

  TH1D* hWEl_MC_Loose2C_DPhi = new TH1D("hWEl_MC_Loose2C_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
  TH1D* hWEl_MC_Loose2S_DPhi = new TH1D("hWEl_MC_Loose2S_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region
  TH1D* hWEl_BG_Loose2C_DPhi = new TH1D("hWEl_BG_Loose2C_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hWEl_Data_Loose2C_DPhi = new TH1D("hWEl_Data_Loose2C_DPhi", "", 3, dphiEdges);  // Data ctrl region

  TH1D* hWMu_MC_LooseC_DPhi = new TH1D("hWMu_MCC_LooseC_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
  TH1D* hWMu_MC_LooseS_DPhi = new TH1D("hWMu_MCC_LooseS_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region
  TH1D* hWMu_BG_LooseC_DPhi = new TH1D("hWMu_BGC_LooseC_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hWMu_Data_LooseC_DPhi = new TH1D("hWMu_DataC_LooseC_DPhi", "", 3, dphiEdges);  // Data ctrl region

  TH1D* hWEl_MC_LooseC_DPhi = new TH1D("hWEl_MCC_LooseC_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
  TH1D* hWEl_MC_LooseS_DPhi = new TH1D("hWEl_MCC_LooseS_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region
  TH1D* hWEl_BG_LooseC_DPhi = new TH1D("hWEl_BGC_LooseC_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hWEl_Data_LooseC_DPhi = new TH1D("hWEl_DataC_LooseC_DPhi", "", 3, dphiEdges);  // Data ctrl region

  // cutflow histograms
  TH1D* hDataWMu    = new TH1D("hWMu_CutFlow_Data", "", nCutsWMu, 0., nCutsWMu);
  TH1D* hWLNuWMu    = new TH1D("hWMu_CutFlow_WToLNu", "", nCutsWMu, 0., nCutsWMu);
  TH1D* hQCDWMu     = new TH1D("hWMu_CutFlow_QCD", "", nCutsWMu, 0., nCutsWMu);
  TH1D* hDYWMu      = new TH1D("hWMu_CutFlow_DY", "", nCutsWMu, 0., nCutsWMu);
  TH1D* hSingleTWMu = new TH1D("hWMu_CutFlow_SingleTSum", "", nCutsWMu, 0., nCutsWMu);
  TH1D* hDibosonWMu = new TH1D("hWMu_CutFlow_Diboson", "", nCutsWMu, 0., nCutsWMu);

  TH1D* hDataWEl    = new TH1D("hWEl_CutFlow_Data", "", nCutsWEl, 0., nCutsWEl);
  TH1D* hWLNuWEl    = new TH1D("hWEl_CutFlow_WToLNu", "", nCutsWEl, 0., nCutsWEl);
  TH1D* hQCDWEl     = new TH1D("hWEl_CutFlow_QCD", "", nCutsWEl, 0., nCutsWEl);
  TH1D* hDYWEl      = new TH1D("hWEl_CutFlow_DY", "", nCutsWEl, 0., nCutsWEl);
  TH1D* hSingleTWEl = new TH1D("hWEl_CutFlow_SingleTSum", "", nCutsWEl, 0., nCutsWEl);
  TH1D* hDibosonWEl = new TH1D("hWEl_CutFlow_Diboson", "", nCutsWEl, 0., nCutsWEl);
  

  // loop over MC datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);
    
    // check it's  W+Jets
    bool isWJets = false;
    if (dataset.name == "WJets" ||
	dataset.name == "W1Jets" || 
	dataset.name == "W2Jets" || 
	dataset.name == "W3Jets" || 
	dataset.name == "W4Jets") {
      isWJets = true;
      std::cout << "Analysing W MC     : " << dataset.name << std::endl;
    }
    else if (dataset.isData) {
      std::cout << "Analysing Data     : " << dataset.name << std::endl;
    }
    else {
      std::cout << "Analysing BG MC    : " << dataset.name << std::endl;
    }

    // setup cuts
    TCut cutD = cuts.cutDataset(dataset.name);

    TCut cutWMu_C(""), cutWMu_S(""), cutWEl_C(""), cutWEl_S("");
    TCut cutWMu_NoMETC(""), cutWMu_NoMETS(""), cutWEl_NoMETC(""), cutWEl_NoMETS("");
    TCut cutWMu_Loose2C(""), cutWMu_Loose2S(""), cutWEl_Loose2C(""), cutWEl_Loose2S("");
    TCut cutWMu_LooseC(""), cutWMu_LooseS(""), cutWEl_LooseC(""), cutWEl_LooseS("");
    

    // different cuts for W MC
    if (isWJets) {
      cutWMu_C = puWeight * trigCorrWeight * wWeight * (cutD + cuts.wMuGen() + cuts.wMuVBF() + cuts.cutWMu("MET"));
      cutWMu_S = puWeight * trigCorrWeight * wWeight * (cutD + cuts.wMuGen() + cuts.allCutsNoDPhi());
      cutWEl_C = puWeight * trigCorrWeight * wWeight * (cutD + cuts.wElGen() + cuts.wElVBF() + cuts.cutWEl("MET"));
      cutWEl_S = puWeight * trigCorrWeight * wWeight * (cutD + cuts.wElGen() + cuts.allCutsNoDPhi());

      cutWMu_NoMETC = puWeight * trigCorrWeight * wWeight * (cutD + cuts.wMuGen() + cuts.wMuVBF() );
      cutWMu_NoMETS = puWeight * trigCorrWeight * wWeight * (cutD + cuts.wMuGen() + cutSignalNoMETNoDPhi);
      cutWEl_NoMETC = puWeight * trigCorrWeight * wWeight * (cutD + cuts.wElGen() + cuts.wElVBF() );
      cutWEl_NoMETS = puWeight * trigCorrWeight * wWeight * (cutD + cuts.wElGen() + cutSignalNoMETNoDPhi);

      cutWMu_Loose2C = puWeight * trigCorrWeight * wWeight * (cutD + cuts.wMuGen() + cuts.wMuVBF() + metCtrl35);
      cutWMu_Loose2S = puWeight * trigCorrWeight * wWeight * (cutD + cuts.wMuGen() + cutSignalNoMETNoDPhi + met35);
      cutWEl_Loose2C = puWeight * trigCorrWeight * wWeight * (cutD + cuts.wElGen() + cuts.wElVBF() + metCtrl35);
      cutWEl_Loose2S = puWeight * trigCorrWeight * wWeight * (cutD + cuts.wElGen() + cutSignalNoMETNoDPhi + met35);

      cutWMu_LooseC = puWeight * trigCorrWeight * wWeight * (cutD + cuts.wMuGen() + cuts.wMuVBF() + metCtrl70);
      cutWMu_LooseS = puWeight * trigCorrWeight * wWeight * (cutD + cuts.wMuGen() + cutSignalNoMETNoDPhi + met70);
      cutWEl_LooseC = puWeight * trigCorrWeight * wWeight * (cutD + cuts.wElGen() + cuts.wElVBF() + metCtrl70);
      cutWEl_LooseS = puWeight * trigCorrWeight * wWeight * (cutD + cuts.wElGen() + cutSignalNoMETNoDPhi + met70);
    }
    else {
      cutWMu_C = puWeight * trigCorrWeight * (cutD + cuts.wMuVBF() + cuts.cutWMu("MET"));
      cutWMu_S = puWeight * trigCorrWeight * (cutD + cuts.allCutsNoDPhi());
      cutWEl_C = puWeight * trigCorrWeight * (cutD + cuts.wElVBF() + cuts.cutWEl("MET"));
      cutWEl_S = puWeight * trigCorrWeight * (cutD + cuts.allCutsNoDPhi());

      cutWMu_NoMETC = puWeight * trigCorrWeight * (cutD + cuts.wMuVBF() );
      cutWMu_NoMETS = puWeight * trigCorrWeight * (cutD + cutSignalNoMETNoDPhi);
      cutWEl_NoMETC = puWeight * trigCorrWeight * (cutD + cuts.wElVBF() );
      cutWEl_NoMETS = puWeight * trigCorrWeight * (cutD + cutSignalNoMETNoDPhi);

      cutWMu_Loose2C = puWeight * trigCorrWeight * (cutD + cuts.wMuVBF() + metCtrl35);
      cutWMu_Loose2S = puWeight * trigCorrWeight * (cutD + cutSignalNoMETNoDPhi + met35);
      cutWEl_Loose2C = puWeight * trigCorrWeight * (cutD + cuts.wElVBF() + metCtrl35);
      cutWEl_Loose2S = puWeight * trigCorrWeight * (cutD + cutSignalNoMETNoDPhi + met35);

      cutWMu_LooseC = puWeight * trigCorrWeight * (cutD + cuts.wMuVBF() + metCtrl70);
      cutWMu_LooseS = puWeight * trigCorrWeight * (cutD + cutSignalNoMETNoDPhi + met70);
      cutWEl_LooseC = puWeight * trigCorrWeight * (cutD + cuts.wElVBF() + metCtrl70);
      cutWEl_LooseS = puWeight * trigCorrWeight * (cutD + cutSignalNoMETNoDPhi + met70);
    }

    //    std::cout << "  cutWMu_C   " << cutWEl_C << std::endl;
    //    std::cout << "  cutflowWEl " << (puWeight * wWeight * (cuts.cutflowWEl(nCutsWEl-1))) << std::endl;

    TFile* file = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

    // tmp histograms
    TH1D* hWMu_C_DPhi = new TH1D("hWMu_C_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
    TH1D* hWMu_S_DPhi = new TH1D("hWMu_S_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region
    TH1D* hWEl_C_DPhi = new TH1D("hWEl_C_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
    TH1D* hWEl_S_DPhi = new TH1D("hWEl_S_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region

    TH1D* hWMu_NoMETC_DPhi = new TH1D("hWMu_NoMETC_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
    TH1D* hWMu_NoMETS_DPhi = new TH1D("hWMu_NoMETS_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region
    TH1D* hWEl_NoMETC_DPhi = new TH1D("hWEl_NoMETC_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
    TH1D* hWEl_NoMETS_DPhi = new TH1D("hWEl_NoMETS_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region

    TH1D* hWMu_Loose2C_DPhi = new TH1D("hWMu_Loose2C_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
    TH1D* hWMu_Loose2S_DPhi = new TH1D("hWMu_Loose2S_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region
    TH1D* hWEl_Loose2C_DPhi = new TH1D("hWEl_Loose2C_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
    TH1D* hWEl_Loose2S_DPhi = new TH1D("hWEl_Loose2S_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region

    TH1D* hWMu_LooseC_DPhi = new TH1D("hWMu_LooseC_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
    TH1D* hWMu_LooseS_DPhi = new TH1D("hWMu_LooseS_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region
    TH1D* hWEl_LooseC_DPhi = new TH1D("hWEl_LooseC_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
    TH1D* hWEl_LooseS_DPhi = new TH1D("hWEl_LooseS_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region

    // weight  to lumi
    double weight = (dataset.isData ? 1. : lumi * dataset.sigma / dataset.nEvents);
    std::cout << "  weight : " << weight << std::endl;

    tree->Draw("vbfDPhi>>hWMu_C_DPhi", cutWMu_C);
    tree->Draw("vbfDPhi>>hWMu_S_DPhi", cutWMu_S);
    tree->Draw("vbfDPhi>>hWEl_C_DPhi", cutWEl_C);
    tree->Draw("vbfDPhi>>hWEl_S_DPhi", cutWEl_S);
    hWMu_C_DPhi->Scale(weight);
    hWMu_S_DPhi->Scale(weight);
    hWEl_C_DPhi->Scale(weight);
    hWEl_S_DPhi->Scale(weight);
    
    tree->Draw("vbfDPhi>>hWMu_NoMETC_DPhi", cutWMu_NoMETC);
    tree->Draw("vbfDPhi>>hWMu_NoMETS_DPhi", cutWMu_NoMETS);
    tree->Draw("vbfDPhi>>hWEl_NoMETC_DPhi", cutWEl_NoMETC);
    tree->Draw("vbfDPhi>>hWEl_NoMETS_DPhi", cutWEl_NoMETS);
    hWMu_NoMETC_DPhi->Scale(weight);
    hWMu_NoMETS_DPhi->Scale(weight);
    hWEl_NoMETC_DPhi->Scale(weight);
    hWEl_NoMETS_DPhi->Scale(weight);

    tree->Draw("vbfDPhi>>hWMu_Loose2C_DPhi", cutWMu_Loose2C);
    tree->Draw("vbfDPhi>>hWMu_Loose2S_DPhi", cutWMu_Loose2S);
    tree->Draw("vbfDPhi>>hWEl_Loose2C_DPhi", cutWEl_Loose2C);
    tree->Draw("vbfDPhi>>hWEl_Loose2S_DPhi", cutWEl_Loose2S);
    hWMu_Loose2C_DPhi->Scale(weight);
    hWMu_Loose2S_DPhi->Scale(weight);
    hWEl_Loose2C_DPhi->Scale(weight);
    hWEl_Loose2S_DPhi->Scale(weight);

    tree->Draw("vbfDPhi>>hWMu_LooseC_DPhi", cutWMu_LooseC);
    tree->Draw("vbfDPhi>>hWMu_LooseS_DPhi", cutWMu_LooseS);
    tree->Draw("vbfDPhi>>hWEl_LooseC_DPhi", cutWEl_LooseC);
    tree->Draw("vbfDPhi>>hWEl_LooseS_DPhi", cutWEl_LooseS);
    hWMu_LooseC_DPhi->Scale(weight);
    hWMu_LooseS_DPhi->Scale(weight);
    hWEl_LooseC_DPhi->Scale(weight);
    hWEl_LooseS_DPhi->Scale(weight);

    // debug output
    std::cout << "  N ctrl region (dphi<1) : " << hWMu_C_DPhi->GetBinContent(1) << " +/- " << hWMu_C_DPhi->GetBinError(1) << std::endl;
    
    if (isWJets) {
      //      std::cout << "  adding to W MC histogram" << std::endl;
      hWMu_MCC_DPhi->Add(hWMu_C_DPhi);
      hWMu_MCS_DPhi->Add(hWMu_S_DPhi);
      hWEl_MCC_DPhi->Add(hWEl_C_DPhi);
      hWEl_MCS_DPhi->Add(hWEl_S_DPhi);

      hWMu_MC_NoMETC_DPhi->Add(hWMu_NoMETC_DPhi);
      hWMu_MC_NoMETS_DPhi->Add(hWMu_NoMETS_DPhi);
      hWEl_MC_NoMETC_DPhi->Add(hWEl_NoMETC_DPhi);
      hWEl_MC_NoMETS_DPhi->Add(hWEl_NoMETS_DPhi);

      hWMu_MC_LooseC_DPhi->Add(hWMu_LooseC_DPhi);
      hWMu_MC_LooseS_DPhi->Add(hWMu_LooseS_DPhi);
      hWEl_MC_LooseC_DPhi->Add(hWEl_LooseC_DPhi);
      hWEl_MC_LooseS_DPhi->Add(hWEl_LooseS_DPhi);

      hWMu_MC_Loose2C_DPhi->Add(hWMu_Loose2C_DPhi);
      hWMu_MC_Loose2S_DPhi->Add(hWMu_Loose2S_DPhi);
      hWEl_MC_Loose2C_DPhi->Add(hWEl_Loose2C_DPhi);
      hWEl_MC_Loose2S_DPhi->Add(hWEl_Loose2S_DPhi);
    }
    else if (dataset.isData) {
      //      std::cout << "  adding to data histogram" << std::endl;
      hWMu_DataC_DPhi->Add(hWMu_C_DPhi);
      hWEl_DataC_DPhi->Add(hWEl_C_DPhi);

      hWMu_Data_NoMETC_DPhi->Add(hWMu_NoMETC_DPhi);
      hWEl_Data_NoMETC_DPhi->Add(hWEl_NoMETC_DPhi);

      hWMu_Data_Loose2C_DPhi->Add(hWMu_Loose2C_DPhi);
      hWEl_Data_Loose2C_DPhi->Add(hWEl_Loose2C_DPhi);

      hWMu_Data_LooseC_DPhi->Add(hWMu_LooseC_DPhi);
      hWEl_Data_LooseC_DPhi->Add(hWEl_LooseC_DPhi);
    }
    else {  // must be a BG dataset
      //      std::cout << "  adding to BG histogram" << std::endl;
      hWMu_BGC_DPhi->Add(hWMu_C_DPhi);
      hWEl_BGC_DPhi->Add(hWEl_C_DPhi);

      hWMu_BG_NoMETC_DPhi->Add(hWMu_NoMETC_DPhi);
      hWEl_BG_NoMETC_DPhi->Add(hWEl_NoMETC_DPhi);

      hWMu_BG_Loose2C_DPhi->Add(hWMu_Loose2C_DPhi);
      hWEl_BG_Loose2C_DPhi->Add(hWEl_Loose2C_DPhi);

      hWMu_BG_LooseC_DPhi->Add(hWMu_LooseC_DPhi);
      hWEl_BG_LooseC_DPhi->Add(hWEl_LooseC_DPhi);
    }

    delete hWMu_C_DPhi;
    delete hWMu_S_DPhi;
    delete hWEl_C_DPhi;
    delete hWEl_S_DPhi;

    // per-dataset control plots (just an example, add more later)
    ofile->cd();

    std::string hname = std::string("hWMu_WmT_")+dataset.name;
    TH1D* hWMu_WmT = new TH1D(hname.c_str(), "", 40, 0., 120.);
    std::string str = std::string("wMt>>")+hname;
    tree->Draw(str.c_str(), cutWMu_C);
    hWMu_WmT->Write("",TObject::kOverwrite);

    hname = std::string("hWEl_WmT_")+dataset.name;
    TH1D* hWEl_WmT = new TH1D(hname.c_str(), "", 40, 0., 120.);
    str = std::string("wMt>>")+hname;
    tree->Draw(str.c_str(), cutWEl_C);
    hWEl_WmT->Write("",TObject::kOverwrite);

    // per-dataset cutflow hists
    std::string hnameWMu = std::string("hWMu_CutFlow_")+dataset.name;
    std::string hnameWEl = std::string("hWEl_CutFlow_")+dataset.name;
    TH1D* hCutFlowWMu = new TH1D(hnameWMu.c_str(), "", nCutsWMu, 0., nCutsWMu);
    TH1D* hCutFlowWEl = new TH1D(hnameWEl.c_str(), "", nCutsWEl, 0., nCutsWEl);

    for (unsigned c=0; c<nCutsWMu; ++c) {

      TCut cut = puWeight * (cutD + cuts.cutflowWMu(c));
//      if (dataset.name.compare(0,1,"W")==0) cut = puWeight * wWeight * (cuts.cutflowWMu(c));
      if(isWJets) cut = puWeight * wWeight * (cuts.cutflowWMu(c));

      TH1D* h = new TH1D("h","", 1, 0., 1.);
      tree->Draw("0.5>>h", cut);

      hCutFlowWMu->SetBinContent(c+1, h->GetBinContent(1));
      hCutFlowWMu->SetBinError(c+1, h->GetBinError(1));

      delete h;
    }

    for (unsigned c=0; c<nCutsWEl; ++c) {

      TCut cut = puWeight * (cutD + cuts.cutflowWEl(c));
//      if (dataset.name.compare(0,1,"W")==0) cut = puWeight * wWeight * (cuts.cutflowWEl(c));
      if(isWJets) cut = puWeight * wWeight * (cuts.cutflowWEl(c));

      TH1D* h = new TH1D("h","", 1, 0., 1.);
      tree->Draw("0.5>>h", cut);

      hCutFlowWEl->SetBinContent(c+1, h->GetBinContent(1));
      hCutFlowWEl->SetBinError(c+1, h->GetBinError(1));

      delete h;
    }

    hCutFlowWMu->Scale(weight);
    hCutFlowWEl->Scale(weight);

    // sum histograms
    if (dataset.isData) {
      hDataWMu->Add(hCutFlowWMu);
      hDataWEl->Add(hCutFlowWEl);
    }
    if (isWJets) {
      hWLNuWMu->Add(hCutFlowWMu);
      hWLNuWEl->Add(hCutFlowWEl);
    }
    if (dataset.name.compare(0,3,"QCD")==0) {
      hQCDWMu->Add(hCutFlowWMu);
      hQCDWEl->Add(hCutFlowWEl);
    }
    if (dataset.name.compare(0,2,"DY")==0) {
      hDYWMu->Add(hCutFlowWMu);
      hDYWEl->Add(hCutFlowWEl);
    }
    if (dataset.name.compare(0,7,"SingleT")==0) {
      hSingleTWMu->Add(hCutFlowWMu);
      hSingleTWEl->Add(hCutFlowWEl);
    }
    if (dataset.name.compare(0,2,"WW")==0 ||
	dataset.name.compare(0,2,"WZ")==0 ||
	dataset.name.compare(0,2,"ZZ")==0 ) {
      hDibosonWMu->Add(hCutFlowWMu);
      hDibosonWEl->Add(hCutFlowWEl);
    }

    hCutFlowWMu->Write("",TObject::kOverwrite);
    hCutFlowWEl->Write("",TObject::kOverwrite);

    delete tree;
    file->Close();
   
  }

  // write out summed cutflow histograms
  hDataWMu->Write("",TObject::kOverwrite);  
  hWLNuWMu->Write("",TObject::kOverwrite);  
  hQCDWMu->Write("",TObject::kOverwrite);  
  hDYWMu->Write("",TObject::kOverwrite);  
  hSingleTWMu->Write("",TObject::kOverwrite);  
  hDibosonWMu->Write("",TObject::kOverwrite);  

  hDataWEl->Write("",TObject::kOverwrite);  
  hWLNuWEl->Write("",TObject::kOverwrite);  
  hQCDWEl->Write("",TObject::kOverwrite);  
  hDYWEl->Write("",TObject::kOverwrite);  
  hSingleTWEl->Write("",TObject::kOverwrite);  
  hDibosonWEl->Write("",TObject::kOverwrite);  


  // create histograms with the background estimate
  TH1D* hWMu_R_DPhi    = new TH1D("hWMu_R_DPhi", "", 3, dphiEdges);  // ratio of sngl/ctrl
  TH1D* hWMu_EstC_DPhi = new TH1D("hWMu_EstC_DPhi", "", 3, dphiEdges); // estimated W in ctrl region
  TH1D* hWMu_EstS_DPhi = new TH1D("hWMu_EstS_DPhi", "", 3, dphiEdges); // estimated W in signal region

  TH1D* hWEl_R_DPhi    = new TH1D("hWEl_R_DPhi", "", 3, dphiEdges);
  TH1D* hWEl_EstC_DPhi = new TH1D("hWEl_EstC_DPhi", "", 3, dphiEdges);
  TH1D* hWEl_EstS_DPhi = new TH1D("hWEl_EstS_DPhi", "", 3, dphiEdges);

  hWMu_R_DPhi->Divide(hWMu_MCS_DPhi, hWMu_MCC_DPhi, 1., 1.);
  hWMu_EstC_DPhi->Add(hWMu_DataC_DPhi, hWMu_BGC_DPhi, 1., -1.);
  hWMu_EstS_DPhi->Multiply(hWMu_EstC_DPhi, hWMu_R_DPhi, 1., 1.);

  // apply MC/data correction for electron ID
  //  hWEl_BGC_DPhi->Scale(constants::electronIdCorrection);

  hWEl_R_DPhi->Divide(hWEl_MCS_DPhi, hWEl_MCC_DPhi, 1., 1.);
  hWEl_EstC_DPhi->Add(hWEl_DataC_DPhi, hWEl_BGC_DPhi, 1., -1.);
  hWEl_EstS_DPhi->Multiply(hWEl_EstC_DPhi, hWEl_R_DPhi, 1., 1.);

  TH1D* hW_Est_S_DPhi = new TH1D("hW_Est_S_DPhi", "", 3, dphiEdges); 
  hW_Est_S_DPhi->Add(hWMu_EstS_DPhi, hWEl_EstS_DPhi, 1., 1.);

  // repeat for loose MET regions
  TH1D* hWMu_R_NoMET_DPhi    = new TH1D("hWMu_R_NoMET_DPhi", "", 3, dphiEdges);  // ratio of sngl/ctrl
  TH1D* hWMu_Est_NoMETC_DPhi = new TH1D("hWMu_Est_NoMETC_DPhi", "", 3, dphiEdges); // estimated W in ctrl region
  TH1D* hWMu_Est_NoMETS_DPhi = new TH1D("hWMu_Est_NoMETS_DPhi", "", 3, dphiEdges); // estimated W in signal region

  TH1D* hWEl_R_NoMET_DPhi    = new TH1D("hWEl_R_NoMET_DPhi", "", 3, dphiEdges);
  TH1D* hWEl_Est_NoMETC_DPhi = new TH1D("hWEl_Est_NoMETC_DPhi", "", 3, dphiEdges);
  TH1D* hWEl_Est_NoMETS_DPhi = new TH1D("hWEl_Est_NoMETS_DPhi", "", 3, dphiEdges);

  hWMu_R_NoMET_DPhi->Divide(hWMu_MC_NoMETS_DPhi, hWMu_MC_NoMETC_DPhi, 1., 1.);
  hWMu_Est_NoMETC_DPhi->Add(hWMu_Data_NoMETC_DPhi, hWMu_BG_NoMETC_DPhi, 1., -1.);
  hWMu_Est_NoMETS_DPhi->Multiply(hWMu_Est_NoMETC_DPhi, hWMu_R_NoMET_DPhi, 1., 1.);

  hWEl_R_NoMET_DPhi->Divide(hWEl_MC_NoMETS_DPhi, hWEl_MC_NoMETC_DPhi, 1., 1.);
  hWEl_Est_NoMETC_DPhi->Add(hWEl_Data_NoMETC_DPhi, hWEl_BG_NoMETC_DPhi, 1., -1.);
  hWEl_Est_NoMETS_DPhi->Multiply(hWEl_Est_NoMETC_DPhi, hWEl_R_NoMET_DPhi, 1., 1.);

  TH1D* hW_Est_NoMETS_DPhi = new TH1D("hW_Est_NoMETS_DPhi", "", 3, dphiEdges); 
  hW_Est_NoMETS_DPhi->Add(hWMu_Est_NoMETS_DPhi, hWEl_Est_NoMETS_DPhi, 1., 1.);

  TH1D* hWMu_R_Loose2_DPhi    = new TH1D("hWMu_R_Loose2_DPhi", "", 3, dphiEdges);  // ratio of sngl/ctrl
  TH1D* hWMu_Est_Loose2C_DPhi = new TH1D("hWMu_Est_Loose2C_DPhi", "", 3, dphiEdges); // estimated W in ctrl region
  TH1D* hWMu_Est_Loose2S_DPhi = new TH1D("hWMu_Est_Loose2S_DPhi", "", 3, dphiEdges); // estimated W in signal region

  TH1D* hWEl_R_Loose2_DPhi    = new TH1D("hWEl_R_Loose2_DPhi", "", 3, dphiEdges);
  TH1D* hWEl_Est_Loose2C_DPhi = new TH1D("hWEl_Est_Loose2C_DPhi", "", 3, dphiEdges);
  TH1D* hWEl_Est_Loose2S_DPhi = new TH1D("hWEl_Est_Loose2S_DPhi", "", 3, dphiEdges);

  hWMu_R_Loose2_DPhi->Divide(hWMu_MC_Loose2S_DPhi, hWMu_MC_Loose2C_DPhi, 1., 1.);
  hWMu_Est_Loose2C_DPhi->Add(hWMu_Data_Loose2C_DPhi, hWMu_BG_Loose2C_DPhi, 1., -1.);
  hWMu_Est_Loose2S_DPhi->Multiply(hWMu_Est_Loose2C_DPhi, hWMu_R_Loose2_DPhi, 1., 1.);

  hWEl_R_Loose2_DPhi->Divide(hWEl_MC_Loose2S_DPhi, hWEl_MC_Loose2C_DPhi, 1., 1.);
  hWEl_Est_Loose2C_DPhi->Add(hWEl_Data_Loose2C_DPhi, hWEl_BG_Loose2C_DPhi, 1., -1.);
  hWEl_Est_Loose2S_DPhi->Multiply(hWEl_Est_Loose2C_DPhi, hWEl_R_Loose2_DPhi, 1., 1.);

  TH1D* hW_Est_Loose2S_DPhi = new TH1D("hW_Est_Loose2S_DPhi", "", 3, dphiEdges); 
  hW_Est_Loose2S_DPhi->Add(hWMu_Est_Loose2S_DPhi, hWEl_Est_Loose2S_DPhi, 1., 1.);

  TH1D* hWMu_R_Loose_DPhi    = new TH1D("hWMu_R_Loose_DPhi", "", 3, dphiEdges);  // ratio of sngl/ctrl
  TH1D* hWMu_Est_LooseC_DPhi = new TH1D("hWMu_Est_LooseC_DPhi", "", 3, dphiEdges); // estimated W in ctrl region
  TH1D* hWMu_Est_LooseS_DPhi = new TH1D("hWMu_Est_LooseS_DPhi", "", 3, dphiEdges); // estimated W in signal region

  TH1D* hWEl_R_Loose_DPhi    = new TH1D("hWEl_R_Loose_DPhi", "", 3, dphiEdges);
  TH1D* hWEl_Est_LooseC_DPhi = new TH1D("hWEl_Est_LooseC_DPhi", "", 3, dphiEdges);
  TH1D* hWEl_Est_LooseS_DPhi = new TH1D("hWEl_Est_LooseS_DPhi", "", 3, dphiEdges);

  hWMu_R_Loose_DPhi->Divide(hWMu_MC_LooseS_DPhi, hWMu_MC_LooseC_DPhi, 1., 1.);
  hWMu_Est_LooseC_DPhi->Add(hWMu_Data_LooseC_DPhi, hWMu_BG_LooseC_DPhi, 1., -1.);
  hWMu_Est_LooseS_DPhi->Multiply(hWMu_Est_LooseC_DPhi, hWMu_R_Loose_DPhi, 1., 1.);

  hWEl_R_Loose_DPhi->Divide(hWEl_MC_LooseS_DPhi, hWEl_MC_LooseC_DPhi, 1., 1.);
  hWEl_Est_LooseC_DPhi->Add(hWEl_Data_LooseC_DPhi, hWEl_BG_LooseC_DPhi, 1., -1.);
  hWEl_Est_LooseS_DPhi->Multiply(hWEl_Est_LooseC_DPhi, hWEl_R_Loose_DPhi, 1., 1.);

  TH1D* hW_Est_LooseS_DPhi = new TH1D("hW_Est_LooseS_DPhi", "", 3, dphiEdges); 
  hW_Est_LooseS_DPhi->Add(hWMu_Est_LooseS_DPhi, hWEl_Est_LooseS_DPhi, 1., 1.);


  std::cout << std::endl;
  std::cout << "W->mu channel (dphi>2.6)" << std::endl;
  std::cout << "  W+jets MC  ctrl region : " << hWMu_MCC_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  W+jets MC  sgnl region : " << hWMu_MCS_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  Background ctrl region : " << hWMu_BGC_DPhi->GetBinContent(3) << std::endl;
  //  std::cout << "  Background sgnl region : " << hWMu_BGS_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  Data ctrl region       : " << hWMu_DataC_DPhi->GetBinContent(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  W in ctrl region       : " << hWMu_EstC_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  N_S(MC)/N_C(MC)        : " << hWMu_R_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  W in sgnl region       : " << hWMu_EstS_DPhi->GetBinContent(3) << std::endl;
  std::cout << std::endl << std::endl;
  std::cout << "W->el channel (dphi>2.6)" << std::endl;
  std::cout << "  W+jets MC  ctrl region : " << hWEl_MCC_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  W+jets MC  sgnl region : " << hWEl_MCS_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  Background ctrl region : " << hWEl_BGC_DPhi->GetBinContent(3) << std::endl;
  //  std::cout << "  Background sgnl region : " << hWEl_BGS_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  Data ctrl region       : " << hWEl_DataC_DPhi->GetBinContent(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  W in ctrl region       : " << hWEl_EstC_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  N_S(MC)/N_C(MC)        : " << hWEl_R_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  W in sgnl region       : " << hWEl_EstS_DPhi->GetBinContent(3) << std::endl;
  std::cout << std::endl << std::endl;
  std::cout << "W->mu channel (dphi<1.0)" << std::endl;
  std::cout << "  W+jets MC  ctrl region : " << hWMu_MCC_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  W+jets MC  sgnl region : " << hWMu_MCS_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  Background ctrl region : " << hWMu_BGC_DPhi->GetBinContent(1) << std::endl;
  //  std::cout << "  Background sgnl region : " << hWMu_BGS_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  Data ctrl region       : " << hWMu_DataC_DPhi->GetBinContent(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  W in ctrl region       : " << hWMu_EstC_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  N_S(MC)/N_C(MC)        : " << hWMu_R_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  W in sgnl region       : " << hWMu_EstS_DPhi->GetBinContent(1) << std::endl;
  std::cout << std::endl << std::endl;
  std::cout << "W->el channel (dphi<1.0)" << std::endl;
  std::cout << "  W+jets MC  ctrl region : " << hWEl_MCC_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  W+jets MC  sgnl region : " << hWEl_MCS_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  Background ctrl region : " << hWEl_BGC_DPhi->GetBinContent(1) << std::endl;
  //  std::cout << "  Background sgnl region : " << hWEl_BGS_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  Data ctrl region       : " << hWEl_DataC_DPhi->GetBinContent(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  W in ctrl region       : " << hWEl_EstC_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  N_S(MC)/N_C(MC)        : " << hWEl_R_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  W in sgnl region       : " << hWEl_EstS_DPhi->GetBinContent(1) << std::endl;
  std::cout << std::endl;
  std::cout << "Total W (dphi<1.0)" << std::endl;
  std::cout << "  W in sgnl region       : " << hW_Est_S_DPhi->GetBinContent(1) << std::endl;

  // store histograms
  ofile->cd();
    
  hWMu_MCC_DPhi->Write("",TObject::kOverwrite);
  hWMu_MCS_DPhi->Write("",TObject::kOverwrite);
  hWMu_BGC_DPhi->Write("",TObject::kOverwrite);
  //  hWMu_BGS_DPhi->Write("",TObject::kOverwrite);
  hWMu_DataC_DPhi->Write("",TObject::kOverwrite);
  hWMu_R_DPhi->Write("",TObject::kOverwrite);
  hWMu_EstC_DPhi->Write("",TObject::kOverwrite);
  hWMu_EstS_DPhi->Write("",TObject::kOverwrite);

  hWEl_MCC_DPhi->Write("",TObject::kOverwrite);
  hWEl_MCS_DPhi->Write("",TObject::kOverwrite);
  hWEl_BGC_DPhi->Write("",TObject::kOverwrite);
  //  hWEl_BGS_DPhi->Write("",TObject::kOverwrite);
  hWEl_DataC_DPhi->Write("",TObject::kOverwrite);
  hWEl_R_DPhi->Write("",TObject::kOverwrite);
  hWEl_EstC_DPhi->Write("",TObject::kOverwrite);
  hWEl_EstS_DPhi->Write("",TObject::kOverwrite);
  hW_Est_S_DPhi->Write("",TObject::kOverwrite);

  hWMu_MC_NoMETC_DPhi->Write("",TObject::kOverwrite);
  hWMu_MC_NoMETS_DPhi->Write("",TObject::kOverwrite);
  hWMu_BG_NoMETC_DPhi->Write("",TObject::kOverwrite);
  hWMu_Data_NoMETC_DPhi->Write("",TObject::kOverwrite);
  hWMu_R_NoMET_DPhi->Write("",TObject::kOverwrite);
  hWMu_Est_NoMETC_DPhi->Write("",TObject::kOverwrite);
  hWMu_Est_NoMETS_DPhi->Write("",TObject::kOverwrite);

  hWEl_MC_NoMETC_DPhi->Write("",TObject::kOverwrite);
  hWEl_MC_NoMETS_DPhi->Write("",TObject::kOverwrite);
  hWEl_BG_NoMETC_DPhi->Write("",TObject::kOverwrite);
  hWEl_Data_NoMETC_DPhi->Write("",TObject::kOverwrite);
  hWEl_R_NoMET_DPhi->Write("",TObject::kOverwrite);
  hWEl_Est_NoMETC_DPhi->Write("",TObject::kOverwrite);
  hWEl_Est_NoMETS_DPhi->Write("",TObject::kOverwrite);
  hW_Est_NoMETS_DPhi->Write("",TObject::kOverwrite);

  hWMu_MC_Loose2C_DPhi->Write("",TObject::kOverwrite);
  hWMu_MC_Loose2S_DPhi->Write("",TObject::kOverwrite);
  hWMu_BG_Loose2C_DPhi->Write("",TObject::kOverwrite);
  hWMu_Data_Loose2C_DPhi->Write("",TObject::kOverwrite);
  hWMu_R_Loose2_DPhi->Write("",TObject::kOverwrite);
  hWMu_Est_Loose2C_DPhi->Write("",TObject::kOverwrite);
  hWMu_Est_Loose2S_DPhi->Write("",TObject::kOverwrite);

  hWEl_MC_Loose2C_DPhi->Write("",TObject::kOverwrite);
  hWEl_MC_Loose2S_DPhi->Write("",TObject::kOverwrite);
  hWEl_BG_Loose2C_DPhi->Write("",TObject::kOverwrite);
  hWEl_Data_Loose2C_DPhi->Write("",TObject::kOverwrite);
  hWEl_R_Loose2_DPhi->Write("",TObject::kOverwrite);
  hWEl_Est_Loose2C_DPhi->Write("",TObject::kOverwrite);
  hWEl_Est_Loose2S_DPhi->Write("",TObject::kOverwrite);
  hW_Est_Loose2S_DPhi->Write("",TObject::kOverwrite);

  hWMu_MC_LooseC_DPhi->Write("",TObject::kOverwrite);
  hWMu_MC_LooseS_DPhi->Write("",TObject::kOverwrite);
  hWMu_BG_LooseC_DPhi->Write("",TObject::kOverwrite);
  hWMu_Data_LooseC_DPhi->Write("",TObject::kOverwrite);
  hWMu_R_Loose_DPhi->Write("",TObject::kOverwrite);
  hWMu_Est_LooseC_DPhi->Write("",TObject::kOverwrite);
  hWMu_Est_LooseS_DPhi->Write("",TObject::kOverwrite);

  hWEl_MC_LooseC_DPhi->Write("",TObject::kOverwrite);
  hWEl_MC_LooseS_DPhi->Write("",TObject::kOverwrite);
  hWEl_BG_LooseC_DPhi->Write("",TObject::kOverwrite);
  hWEl_Data_LooseC_DPhi->Write("",TObject::kOverwrite);
  hWEl_R_Loose_DPhi->Write("",TObject::kOverwrite);
  hWEl_Est_LooseC_DPhi->Write("",TObject::kOverwrite);
  hWEl_Est_LooseS_DPhi->Write("",TObject::kOverwrite);
  hW_Est_LooseS_DPhi->Write("",TObject::kOverwrite);

  // write the cutflow table
  std::cout << "Writing cut flow TeX file" << std::endl;

  ofstream effFile;
  effFile.open(options.oDir+std::string("/cutflowWMu.tex"));

  effFile << "Cut & N(data) & N($W\\rightarrow l\\nu$) & N(DY) & N(QCD) & N($t\\bar{t}$) & N(single $t$) & N(diboson) \\\\" << std::endl;

  TH1D* hTTbarWMu = (TH1D*) ofile->Get("hWMu_CutFlow_TTBar");

  // cutflow table
  for (unsigned i=0; i<nCutsWMu; ++i) {

    effFile << cuts.cutNameWMu(i) << " & ";
    effFile << "$" << hDataWMu->GetBinContent(i+1) << " \\pm " << hDataWMu->GetBinError(i+1) << "$ & ";
    effFile << "$" << hWLNuWMu->GetBinContent(i+1) << " \\pm " << hWLNuWMu->GetBinError(i+1) << "$ & ";
    effFile << "$" << hDYWMu->GetBinContent(i+1) << " \\pm " << hDYWMu->GetBinError(i+1) << "$ & ";
    effFile << "$" << hQCDWMu->GetBinContent(i+1) << " \\pm " << hQCDWMu->GetBinError(i+1) << "$ & ";
    effFile << "$" << hTTbarWMu->GetBinContent(i+1) << " \\pm " << hTTbarWMu->GetBinError(i+1) << "$ & ";
    effFile << "$" << hSingleTWMu->GetBinContent(i+1) << " \\pm " << hSingleTWMu->GetBinError(i+1) << "$ & ";
    effFile << "$" << hDibosonWMu->GetBinContent(i+1) << " \\pm " << hDibosonWMu->GetBinError(i+1) << "$ \\\\ ";
    effFile << std::endl;

  }

  effFile << std::endl << std::endl;
  effFile.close();

  //WEl cutflow
  effFile.open(options.oDir+std::string("/cutflowWEl.tex"));

  effFile << "Cut & N(data) & N($W\\rightarrow l\\nu$) & N(DY) & N(QCD) & N($t\\bar{t}$) & N(single $t$) & N(diboson) \\\\" << std::endl;

  TH1D* hTTbarWEl = (TH1D*) ofile->Get("hWEl_CutFlow_TTBar");

  // cutflow table
  for (unsigned i=0; i<nCutsWEl; ++i) {

    effFile << cuts.cutNameWEl(i) << " & ";
    effFile << "$" << hDataWEl->GetBinContent(i+1) << " \\pm " << hDataWEl->GetBinError(i+1) << "$ & ";
    effFile << "$" << hWLNuWEl->GetBinContent(i+1) << " \\pm " << hWLNuWEl->GetBinError(i+1) << "$ & ";
    effFile << "$" << hDYWEl->GetBinContent(i+1) << " \\pm " << hDYWEl->GetBinError(i+1) << "$ & ";
    effFile << "$" << hQCDWEl->GetBinContent(i+1) << " \\pm " << hQCDWEl->GetBinError(i+1) << "$ & ";
    effFile << "$" << hTTbarWEl->GetBinContent(i+1) << " \\pm " << hTTbarWEl->GetBinError(i+1) << "$ & ";
    effFile << "$" << hSingleTWEl->GetBinContent(i+1) << " \\pm " << hSingleTWEl->GetBinError(i+1) << "$ & ";
    effFile << "$" << hDibosonWEl->GetBinContent(i+1) << " \\pm " << hDibosonWEl->GetBinError(i+1) << "$ \\\\ ";
    effFile << std::endl;
  }

  effFile << std::endl << std::endl;
  effFile.close();



  ofile->Close();    

}
