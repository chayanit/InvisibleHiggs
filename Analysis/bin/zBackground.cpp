
#include "InvisibleHiggs/Analysis/interface/ProgramOptions.h"
#include "InvisibleHiggs/Analysis/interface/Cuts.h"
#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"
#include "InvisibleHiggs/Analysis/interface/StackPlot.h"
#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"
#include "InvisibleHiggs/Analysis/interface/Datasets.h"

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

  // input datasets
  Datasets datasets(options.iDir);
  datasets.readFile(options.datasetFile);
 
  std::string oDir_Plot = options.oDir+std::string("/ZBackground");

  boost::filesystem::path opath(oDir_Plot);
  if (!exists(opath)) {
    std::cout << "Creating output directory : " << oDir_Plot << std::endl;
    boost::filesystem::create_directory(opath);
  }

  // output file
  TFile* ofile = TFile::Open( (options.oDir+std::string("/ZBackground.root")).c_str(), "RECREATE");

  // cuts
  Cuts cuts;
  unsigned nCutsZMuMu = cuts.nCutsZMuMu();

  TCut puWeight("puWeight");
  TCut trigCorrWeight("trigCorrWeight");
  TCut trigCorr( "( (trigCorrWeight>0)*trigCorrWeight + (trigCorrWeight<=0)*1 )" );
  TCut METNoMuon130("metNoMuon>130.");	// add here later for VBF efficiency when MET>35, MET>70 (QCD estimation)
  TCut METNoMuon100("metNoMuon>100.");
  TCut METNoMuon90("metNoMuon>90.");
  TCut METNoMuon80("metNoMuon>80.");	
  TCut METNo2Muon130("metNo2Muon>130.");
  TCut METNo2Muon100("metNo2Muon>100.");
  TCut METNo2Muon90("metNo2Muon>90.");
  TCut METNo2Muon80("metNo2Muon>80.");

  // histograms
  double dphiEdges[4] = { 0., 1.0, 2.6, TMath::Pi() };

  // signal MET>130
  TH1D* hZ_DY_C_DPhi = new TH1D("hZ_DY_C_DPhi", "", 3, dphiEdges);  // Z+jets MC ctrl region
  TH1D* hZ_BG_C_DPhi = new TH1D("hZ_BG_C_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hZ_Data_C_DPhi = new TH1D("hZ_Data_C_DPhi", "", 3, dphiEdges);  // Data ctrl region

  TH1D* hZ_DY_EffMuMu_D = new TH1D("hZ_DY_EffMuMu_D", "", 1, 0., 1.);	// denominator of MuMu efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
  TH1D* hZ_DY_EffMuMu_N = new TH1D("hZ_DY_EffMuMu_N", "", 1, 0., 1.);	// numerator of MuMu efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
 
  TH1D* hZ_DY_EffVBFS_D = new TH1D("hZ_DY_EffVBFS_D", "", 1, 0., 1.);   // denominator of VBF(S) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
  TH1D* hZ_DY_EffVBFS_N = new TH1D("hZ_DY_EffVBFS_N", "", 1, 0., 1.);   // numerator of VBF(S) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 
 
  TH1D* hZ_DY_EffVBFC_D = new TH1D("hZ_DY_EffVBFC_D", "", 1, 0., 1.);   // denominator of VBF(C) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
  TH1D* hZ_DY_EffVBFC_N = new TH1D("hZ_DY_EffVBFC_N", "", 1, 0., 1.);   // numerator of VBF(C) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 

  // MET > 100
  TH1D* hZ_DY_LooseC_DPhi = new TH1D("hZ_DY_LooseC_DPhi", "", 3, dphiEdges);  // Z+jets MC ctrl region
  TH1D* hZ_BG_LooseC_DPhi = new TH1D("hZ_BG_LooseC_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hZ_Data_LooseC_DPhi = new TH1D("hZ_Data_LooseC_DPhi", "", 3, dphiEdges);  // Data ctrl region

  TH1D* hZ_DY_Loose_EffVBFS_N = new TH1D("hZ_DY_Loose_EffVBFS_N", "", 1, 0., 1.);   // numerator of VBF(S) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples  
  TH1D* hZ_DY_Loose_EffVBFC_N = new TH1D("hZ_DY_Loose_EffVBFC_N", "", 1, 0., 1.);   // numerator of VBF(C) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 

  // MET > 90
  TH1D* hZ_DY_Loose2C_DPhi = new TH1D("hZ_DY_Loose2C_DPhi", "", 3, dphiEdges);  // Z+jets MC ctrl region
  TH1D* hZ_BG_Loose2C_DPhi = new TH1D("hZ_BG_Loose2C_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hZ_Data_Loose2C_DPhi = new TH1D("hZ_Data_Loose2C_DPhi", "", 3, dphiEdges);  // Data ctrl region
 
  TH1D* hZ_DY_Loose2_EffVBFS_N = new TH1D("hZ_DY_Loose2_EffVBFS_N", "", 1, 0., 1.);   // numerator of VBF(S) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 
  TH1D* hZ_DY_Loose2_EffVBFC_N = new TH1D("hZ_DY_Loose2_EffVBFC_N", "", 1, 0., 1.);   // numerator of VBF(C) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 

  // MET > 80
  TH1D* hZ_DY_NoMETC_DPhi = new TH1D("hZ_DY_NoMETC_DPhi", "", 3, dphiEdges);  // Z+jets MC ctrl region
  TH1D* hZ_BG_NoMETC_DPhi = new TH1D("hZ_BG_NoMETC_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hZ_Data_NoMETC_DPhi = new TH1D("hZ_Data_NoMETC_DPhi", "", 3, dphiEdges);  // Data ctrl region

  TH1D* hZ_DY_NoMET_EffVBFS_N = new TH1D("hZ_DY_NoMET_EffVBFS_N", "", 1, 0., 1.);   // numerator of VBF(S) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 
  TH1D* hZ_DY_NoMET_EffVBFC_N = new TH1D("hZ_DY_NoMET_EffVBFC_N", "", 1, 0., 1.);   // numerator of VBF(C) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 

  // cutflow histograms
  TH1D* hZ_CutFlow_Data       = new TH1D("hZ_CutFlow_Data", "", nCutsZMuMu, 0., nCutsZMuMu);
  TH1D* hZ_CutFlow_DY         = new TH1D("hZ_CutFlow_DY", "", nCutsZMuMu, 0., nCutsZMuMu);
  TH1D* hZ_CutFlow_SingleTSum = new TH1D("hZ_CutFlow_SingleTSum", "", nCutsZMuMu, 0., nCutsZMuMu);
  TH1D* hZ_CutFlow_Diboson    = new TH1D("hZ_CutFlow_Diboson", "", nCutsZMuMu, 0., nCutsZMuMu);


  // loop over MC datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);
    
    TCut cutD = cuts.cutDataset(dataset.name);

    // check if it's DYJets
    bool isDY = false;
    if (dataset.name.compare(0,2,"DY")==0) {
      isDY = true;
      std::cout << "Analysing DY->ll MC : " << dataset.name << std::endl;
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
    TCut cutZMuMu_C80     = puWeight * trigCorrWeight * (cutD + cuts.zMuMuVBF() + METNo2Muon80);
    TCut cutZMuMu_C90     = puWeight * trigCorrWeight * (cutD + cuts.zMuMuVBF() + METNo2Muon90);
    TCut cutZMuMu_C100     = puWeight * trigCorrWeight * (cutD + cuts.zMuMuVBF() + METNo2Muon100);
    TCut cutZMuMu_C130    = puWeight * trigCorrWeight * (cutD + cuts.zMuMuVBF() + METNo2Muon130);
    
    TCut cutEfficiencyMuMu_D    = puWeight * (cutD + cuts.zMuMuGen());
    TCut cutEfficiencyMuMu_N    = puWeight * (cutD + cuts.zMuMuGen() + cuts.zMuMuReco());
 
    TCut cutEfficiencyVBFS_D    = puWeight * (cutD + cuts.zMuMuGenMass());
    TCut cutEfficiencyVBFS_N80  = puWeight * trigCorrWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon80);
    TCut cutEfficiencyVBFS_N90  = puWeight * trigCorrWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon90);
    TCut cutEfficiencyVBFS_N100  = puWeight * trigCorrWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon100);
    TCut cutEfficiencyVBFS_N130 = puWeight * trigCorrWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon130);
    
    TCut cutEfficiencyVBFC_D    = puWeight * (cutD + cuts.zMuMuGen() + cuts.zMuMuReco());
    TCut cutEfficiencyVBFC_N80  = puWeight * trigCorrWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon80);
    TCut cutEfficiencyVBFC_N90  = puWeight * trigCorrWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon90);
    TCut cutEfficiencyVBFC_N100  = puWeight * trigCorrWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon100);
    TCut cutEfficiencyVBFC_N130 = puWeight * trigCorrWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon130);
    
    // fill tmp histograms for BG estimation
    TH1D* hZ_C80_DPhi   = new TH1D("hZ_C80_DPhi", "", 3, dphiEdges);  // this is for the actual BG estimation
    TH1D* hZ_C90_DPhi   = new TH1D("hZ_C90_DPhi", "", 3, dphiEdges);  // this is for the actual BG estimation
    TH1D* hZ_C100_DPhi   = new TH1D("hZ_C100_DPhi", "", 3, dphiEdges);  // this is for the actual BG estimation
    TH1D* hZ_C130_DPhi  = new TH1D("hZ_C130_DPhi", "", 3, dphiEdges);  // this is for the actual BG estimation
    // fill tmp histograms for efficiency calculation
    TH1D* hZ_EffMuMu_D  = new TH1D("hZ_EffMuMu_D", "", 1, 0., 1.);
    TH1D* hZ_EffMuMu_N  = new TH1D("hZ_EffMuMu_N", "", 1, 0., 1.);

    TH1D* hZ_EffVBFS_D    = new TH1D("hZ_EffVBFS_D", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_N80  = new TH1D("hZ_EffVBFS_N80", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_N90  = new TH1D("hZ_EffVBFS_N90", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_N100  = new TH1D("hZ_EffVBFS_N100", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_N130 = new TH1D("hZ_EffVBFS_N130", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_D    = new TH1D("hZ_EffVBFC_D", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_N80  = new TH1D("hZ_EffVBFC_N80", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_N90  = new TH1D("hZ_EffVBFC_N90", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_N100  = new TH1D("hZ_EffVBFC_N100", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_N130 = new TH1D("hZ_EffVBFC_N130", "", 1, 0., 1.);

    if (isDY) {
      tree->Draw("vbfDPhi>>hZ_C80_DPhi",  cutZMuMu_C80);
      tree->Draw("vbfDPhi>>hZ_C90_DPhi",  cutZMuMu_C90);
      tree->Draw("vbfDPhi>>hZ_C100_DPhi",  cutZMuMu_C100);
      tree->Draw("vbfDPhi>>hZ_C130_DPhi", cutZMuMu_C130);
      tree->Draw("0.5>>hZ_EffMuMu_D",     cutEfficiencyMuMu_D);		
      tree->Draw("0.5>>hZ_EffMuMu_N",     cutEfficiencyMuMu_N);	
      tree->Draw("0.5>>hZ_EffVBFS_D",     cutEfficiencyVBFS_D);
      tree->Draw("0.5>>hZ_EffVBFS_N80",   cutEfficiencyVBFS_N80);
      tree->Draw("0.5>>hZ_EffVBFS_N90",   cutEfficiencyVBFS_N90);
      tree->Draw("0.5>>hZ_EffVBFS_N100",   cutEfficiencyVBFS_N100);
      tree->Draw("0.5>>hZ_EffVBFS_N130",  cutEfficiencyVBFS_N130);
      tree->Draw("0.5>>hZ_EffVBFC_D",     cutEfficiencyVBFC_D);
      tree->Draw("0.5>>hZ_EffVBFC_N80",   cutEfficiencyVBFC_N80);
      tree->Draw("0.5>>hZ_EffVBFC_N90",   cutEfficiencyVBFC_N90);
      tree->Draw("0.5>>hZ_EffVBFC_N100",   cutEfficiencyVBFC_N100);
      tree->Draw("0.5>>hZ_EffVBFC_N130",  cutEfficiencyVBFC_N130);
    }
    else {
      tree->Draw("vbfDPhi>>hZ_C80_DPhi",  cutZMuMu_C80);
      tree->Draw("vbfDPhi>>hZ_C90_DPhi",  cutZMuMu_C90);
      tree->Draw("vbfDPhi>>hZ_C100_DPhi",  cutZMuMu_C100);
      tree->Draw("vbfDPhi>>hZ_C130_DPhi", cutZMuMu_C130);
    }

    // weight  to lumi
    double weight = (dataset.isData ? 1. : lumi * dataset.sigma / dataset.nEvents);
    hZ_C80_DPhi->Scale(weight);
    hZ_C90_DPhi->Scale(weight);
    hZ_C100_DPhi->Scale(weight);
    hZ_C130_DPhi->Scale(weight);
    hZ_EffVBFS_D->Scale(weight);
    hZ_EffVBFS_N80->Scale(weight);
    hZ_EffVBFS_N90->Scale(weight);
    hZ_EffVBFS_N100->Scale(weight);
    hZ_EffVBFS_N130->Scale(weight);
    hZ_EffVBFC_D->Scale(weight);
    hZ_EffVBFC_N80->Scale(weight);
    hZ_EffVBFC_N90->Scale(weight);
    hZ_EffVBFC_N100->Scale(weight);
    hZ_EffVBFC_N130->Scale(weight);
    hZ_EffMuMu_D->Scale(weight);
    hZ_EffMuMu_N->Scale(weight);

    // add to output histograms
    if (dataset.isData) {
      hZ_Data_NoMETC_DPhi->Add(hZ_C80_DPhi);
      hZ_Data_Loose2C_DPhi->Add(hZ_C90_DPhi);
      hZ_Data_LooseC_DPhi->Add(hZ_C100_DPhi);
      hZ_Data_C_DPhi->Add(hZ_C130_DPhi);
    }
    else if (isDY) {
      hZ_DY_NoMETC_DPhi->Add(hZ_C80_DPhi);
      hZ_DY_Loose2C_DPhi->Add(hZ_C90_DPhi);
      hZ_DY_LooseC_DPhi->Add(hZ_C100_DPhi);
      hZ_DY_C_DPhi->Add(hZ_C130_DPhi);
      hZ_DY_EffVBFS_D->Add(hZ_EffVBFS_D);
      hZ_DY_NoMET_EffVBFS_N->Add(hZ_EffVBFS_N80);
      hZ_DY_Loose2_EffVBFS_N->Add(hZ_EffVBFS_N90);
      hZ_DY_Loose_EffVBFS_N->Add(hZ_EffVBFS_N100);
      hZ_DY_EffVBFS_N->Add(hZ_EffVBFS_N130);
      hZ_DY_EffVBFC_D->Add(hZ_EffVBFC_D);
      hZ_DY_NoMET_EffVBFC_N->Add(hZ_EffVBFC_N80);
      hZ_DY_Loose2_EffVBFC_N->Add(hZ_EffVBFC_N90);
      hZ_DY_Loose_EffVBFC_N->Add(hZ_EffVBFC_N100);
      hZ_DY_EffVBFC_N->Add(hZ_EffVBFC_N130);
      hZ_DY_EffMuMu_D->Add(hZ_EffMuMu_D);
      hZ_DY_EffMuMu_N->Add(hZ_EffMuMu_N);
    }
    else {
      hZ_BG_NoMETC_DPhi->Add(hZ_C80_DPhi);
      hZ_BG_Loose2C_DPhi->Add(hZ_C90_DPhi);
      hZ_BG_LooseC_DPhi->Add(hZ_C100_DPhi);
      hZ_BG_C_DPhi->Add(hZ_C130_DPhi);
    }

    std::cout << "  N ctrl (dphi<1.0) : " << hZ_C130_DPhi->GetBinContent(1) << " +/- " << hZ_C130_DPhi->GetBinError(1) << std::endl;	
    std::cout << "  N ctrl (dphi>2.6) : " << hZ_C130_DPhi->GetBinContent(3) << " +/- " << hZ_C130_DPhi->GetBinError(3) << std::endl;
    
    delete hZ_C80_DPhi;
    delete hZ_C90_DPhi;
    delete hZ_C100_DPhi;
    delete hZ_C130_DPhi;
    delete hZ_EffMuMu_D;
    delete hZ_EffMuMu_N;
    delete hZ_EffVBFS_D;
    delete hZ_EffVBFS_N80;
    delete hZ_EffVBFS_N90;
    delete hZ_EffVBFS_N100;
    delete hZ_EffVBFS_N130;
    delete hZ_EffVBFC_D;
    delete hZ_EffVBFC_N80;
    delete hZ_EffVBFC_N90;
    delete hZ_EffVBFC_N100;
    delete hZ_EffVBFC_N130;

    ofile->cd();

    // cut flow histograms
    std::string hname = std::string("hZ_CutFlow_")+dataset.name;
    TH1D* hZ_CutFlow = new TH1D(hname.c_str(), "", nCutsZMuMu, 0., nCutsZMuMu);

    for (unsigned c=0; c<nCutsZMuMu; ++c) {

      TCut cut;

      if(c == nCutsZMuMu-1) cut = puWeight * trigCorrWeight * (cutD + cuts.cutflowZMuMu(c));
      else cut = puWeight * (cutD + cuts.cutflowZMuMu(c));

      TH1D* h = new TH1D("h","", 1, 0., 1.);
      tree->Draw("0.5>>h", cut);

      hZ_CutFlow->SetBinContent(c+1, h->GetBinContent(1));
      hZ_CutFlow->SetBinError(c+1, h->GetBinError(1));

      delete h;
    }

    hZ_CutFlow->Scale(weight);

    if (dataset.isData) {
      hZ_CutFlow_Data->Add(hZ_CutFlow);
    }
    if (dataset.name.compare(0,2,"DY")==0) {
      hZ_CutFlow_DY->Add(hZ_CutFlow);
    }
    if (dataset.name.compare(0,7,"SingleT")==0) {
      hZ_CutFlow_SingleTSum->Add(hZ_CutFlow);
    }
    if (dataset.name.compare(0,2,"WW")==0 ||
	dataset.name.compare(0,2,"WZ")==0 ||
	dataset.name.compare(0,2,"ZZ")==0 ) {
      hZ_CutFlow_Diboson->Add(hZ_CutFlow);
    }

    hZ_CutFlow->Write("",TObject::kOverwrite);

    delete hZ_CutFlow;

    // Z control plots
    TCut cutPlots = puWeight * trigCorr * (cutD + cuts.zMuMuVBFLoose());
 
    TFile* ofile_Plot = TFile::Open( (oDir_Plot+std::string("/")+dataset.name+std::string(".root")).c_str(), "RECREATE");

    TH1D* ZCtrlZMass	    = new TH1D("ZCtrlZMass",	 "", 30, 60., 120.);
    TH1D* ZCtrlZpT          = new TH1D("ZCtrlZpT",       "", 50, 0.,  1000.);
    TH1D* ZCtrlJet1pT       = new TH1D("ZCtrlJet1pT",    "", 50, 0.,  1000.);
    TH1D* ZCtrlJet1Eta      = new TH1D("ZCtrlJet1Eta",   "", 50, -5., 5.);
    TH1D* ZCtrlJet2pT       = new TH1D("ZCtrlJet2pT",    "", 50, 0.,  1000.);
    TH1D* ZCtrlJet2Eta      = new TH1D("ZCtrlJet2Eta",   "", 50, -5., 5.);
    TH1D* ZCtrlCenJetpT     = new TH1D("ZCtrlCenJetpT",  "", 50, 0.,  400.);
    TH1D* ZCtrlDEtajj       = new TH1D("ZCtrlDEtajj",    "", 50, 0.,  8.);
    TH1D* ZCtrlMjj          = new TH1D("ZCtrlMjj",       "", 50, 0.,  4000.);
    TH1D* ZCtrlMET          = new TH1D("ZCtrlMET",       "", 50, 0.,  500.);
    TH1D* ZCtrlDPhijj       = new TH1D("ZCtrlDPhijj",    "", 50, 0.,  TMath::Pi());

    tree->Draw("zMass>>ZCtrlZMass"	, cutPlots);
    tree->Draw("zPt>>ZCtrlZpT"		, cutPlots);
    tree->Draw("jet1Pt>>ZCtrlJet1pT"	, cutPlots);
    tree->Draw("jet1Eta>>ZCtrlJet1Eta"	, cutPlots);
    tree->Draw("jet2Pt>>ZCtrlJet2pT"	, cutPlots);
    tree->Draw("jet2Eta>>ZCtrlJet2Eta"	, cutPlots);
    tree->Draw("cenJetEt>>ZCtrlCenJetpT", cutPlots);
    tree->Draw("vbfDEta>>ZCtrlDEtajj"	, cutPlots);
    tree->Draw("vbfM>>ZCtrlMjj"		, cutPlots);
    tree->Draw("metNo2Muon>>ZCtrlMET"	, cutPlots);
    tree->Draw("vbfDPhi>>ZCtrlDPhijj"	, cutPlots);

    if (!dataset.isData) {
    ZCtrlZMass->Scale(weight);
    ZCtrlZpT->Scale(weight);
    ZCtrlJet1pT->Scale(weight);
    ZCtrlJet1Eta->Scale(weight);
    ZCtrlJet2pT->Scale(weight);
    ZCtrlJet2Eta->Scale(weight);
    ZCtrlCenJetpT->Scale(weight);
    ZCtrlDEtajj->Scale(weight);
    ZCtrlMjj->Scale(weight);
    ZCtrlMET->Scale(weight);
    ZCtrlDPhijj->Scale(weight);
    }

    ofile_Plot->cd();
    ZCtrlZMass->Write("",TObject::kOverwrite);
    ZCtrlZpT->Write("",TObject::kOverwrite);
    ZCtrlJet1pT->Write("",TObject::kOverwrite);
    ZCtrlJet1Eta->Write("",TObject::kOverwrite);
    ZCtrlJet2pT->Write("",TObject::kOverwrite);
    ZCtrlJet2Eta->Write("",TObject::kOverwrite);
    ZCtrlCenJetpT->Write("",TObject::kOverwrite);
    ZCtrlDEtajj->Write("",TObject::kOverwrite);
    ZCtrlMjj->Write("",TObject::kOverwrite);
    ZCtrlMET->Write("",TObject::kOverwrite);
    ZCtrlDPhijj->Write("",TObject::kOverwrite);

    ofile_Plot->Close();

    // clean up
    delete tree;
    file->Close();
   
  }

  // numbers - calculate these from MC in this program later!
  double ratioBF = 5.626;  //  MCFM + NLO
  //double eps_mumu = 0.290;
  //double eps_s_vbf = 0.0194;
  //double eps_c_vbf = 0.0315;
  //double f = (ratioBF * eps_s_vbf) / (eps_mumu * eps_c_vbf);

  TH1D* hZ_Est_C_DPhi = new TH1D("hZ_Est_C_DPhi", "", 3, dphiEdges); // estimated Z in ctrl region
  TH1D* hZ_Est_S_DPhi = new TH1D("hZ_Est_S_DPhi", "", 3, dphiEdges); // estimated Z in bkgrnd region  
  TH1D* hZ_Eff_S_DPhi = new TH1D("hZ_Eff_S_DPhi", "", 3, dphiEdges);

  TH1D* hZ_DY_EffMuMu = new TH1D("hZ_DY_EffMuMu", "", 1, 0., 1.);     	// epsilon mumu
  TH1D* hZ_DY_EffVBFS = new TH1D("hZ_DY_EffVBFS", "", 1, 0., 1.);  	// epsilon_s_vbf
  TH1D* hZ_DY_EffVBFC = new TH1D("hZ_DY_EffVBFC", "", 1, 0., 1.);       // epsilon_c_vbf
  TH1D* hZ_DY_RatioVBF = new TH1D("hZ_DY_RatioVBF", "", 1, 0., 1.);	// epsilon_s_vbf/epsilon_c_vbf
  TH1D* hZ_DY_TotalEff = new TH1D("hZ_DY_TotalEff", "", 1, 0., 1.); 

  hZ_DY_EffMuMu->Add(hZ_DY_EffMuMu_N);
  hZ_DY_EffMuMu->Divide(hZ_DY_EffMuMu_D);
  double mu_syst = 0.025*hZ_DY_EffMuMu->GetBinContent(1);      //2.5% Muon ID/Iso efficiency uncertainty from EWK-10-002
  hZ_DY_EffMuMu->SetBinError(1,TMath::Sqrt(hZ_DY_EffMuMu->GetBinError(1)*hZ_DY_EffMuMu->GetBinError(1) + mu_syst*mu_syst));
 
  hZ_DY_EffVBFS->Add(hZ_DY_EffVBFS_N);
  hZ_DY_EffVBFS->Divide(hZ_DY_EffVBFS_D);

  hZ_DY_EffVBFC->Add(hZ_DY_EffVBFC_N);
  hZ_DY_EffVBFC->Divide(hZ_DY_EffVBFC_D);

  hZ_DY_RatioVBF->Add(hZ_DY_EffVBFS);
  hZ_DY_RatioVBF->Divide(hZ_DY_EffVBFC);
  //hZ_DY_RatioVBF->SetBinError(1,TMath::Sqrt(hZ_DY_RatioVBF->GetBinError(1)*hZ_DY_RatioVBF->GetBinError(1) + eps_vbf_syst*eps_vbf_syst));

  hZ_DY_TotalEff->Add(hZ_DY_RatioVBF);
  hZ_DY_TotalEff->Divide(hZ_DY_EffMuMu);

  for(int ibin = 1; ibin <= hZ_Eff_S_DPhi->GetNbinsX(); ++ibin) {
    hZ_Eff_S_DPhi->SetBinContent(ibin,hZ_DY_TotalEff->GetBinContent(1));
    hZ_Eff_S_DPhi->SetBinError  (ibin,hZ_DY_TotalEff->GetBinError(1));
  }

  hZ_Est_C_DPhi->Add(hZ_Data_C_DPhi, hZ_BG_C_DPhi, 1., -1.);
  //hZ_Est_S_DPhi->Add(hZ_Est_C_DPhi, f);
  hZ_Est_S_DPhi->Add(hZ_Est_C_DPhi,ratioBF);
  hZ_Est_S_DPhi->Multiply(hZ_Eff_S_DPhi);


  // MET>80
  TH1D* hZ_Est_NoMETC_DPhi = new TH1D("hZ_Est_NoMETC_DPhi", "", 3, dphiEdges); // estimated Z in ctrl region
  TH1D* hZ_Est_NoMETS_DPhi = new TH1D("hZ_Est_NoMETS_DPhi", "", 3, dphiEdges); // estimated Z in bkgrnd region  
  TH1D* hZ_Eff_NoMETS_DPhi = new TH1D("hZ_Eff_NoMETS_DPhi", "", 3, dphiEdges);

  TH1D* hZ_DY_NoMET_EffVBFS = new TH1D("hZ_DY_NoMET_EffVBFS", "", 1, 0., 1.);  	 // epsilon_s_vbf
  TH1D* hZ_DY_NoMET_EffVBFC = new TH1D("hZ_DY_NoMET_EffVBFC", "", 1, 0., 1.);    // epsilon_c_vbf
  TH1D* hZ_DY_NoMET_RatioVBF = new TH1D("hZ_DY_NoMET_RatioVBF", "", 1, 0., 1.);	 // epsilon_s_vbf/epsilon_c_vbf
  TH1D* hZ_DY_NoMET_TotalEff = new TH1D("hZ_DY_NoMET_TotalEff", "", 1, 0., 1.); 

  hZ_DY_NoMET_EffVBFS->Add(hZ_DY_NoMET_EffVBFS_N);
  hZ_DY_NoMET_EffVBFS->Divide(hZ_DY_EffVBFS_D);

  hZ_DY_NoMET_EffVBFC->Add(hZ_DY_NoMET_EffVBFC_N);
  hZ_DY_NoMET_EffVBFC->Divide(hZ_DY_EffVBFC_D);

  hZ_DY_NoMET_RatioVBF->Add(hZ_DY_NoMET_EffVBFS);
  hZ_DY_NoMET_RatioVBF->Divide(hZ_DY_NoMET_EffVBFC);
  //hZ_DY_NoMET_RatioVBF->SetBinError(1,TMath::Sqrt(hZ_DY_NoMET_RatioVBF->GetBinError(1)*hZ_DY_NoMET_RatioVBF->GetBinError(1) + eps_vbf_syst*eps_vbf_syst));

  hZ_DY_NoMET_TotalEff->Add(hZ_DY_NoMET_RatioVBF);
  hZ_DY_NoMET_TotalEff->Divide(hZ_DY_EffMuMu);

  for(int ibin = 1; ibin <= hZ_Eff_NoMETS_DPhi->GetNbinsX(); ++ibin) {
    hZ_Eff_NoMETS_DPhi->SetBinContent(ibin,hZ_DY_NoMET_TotalEff->GetBinContent(1));
    hZ_Eff_NoMETS_DPhi->SetBinError  (ibin,hZ_DY_NoMET_TotalEff->GetBinError(1));
  }

  hZ_Est_NoMETC_DPhi->Add(hZ_Data_NoMETC_DPhi, hZ_BG_NoMETC_DPhi, 1., -1.);
  hZ_Est_NoMETS_DPhi->Add(hZ_Est_NoMETC_DPhi,ratioBF);
  hZ_Est_NoMETS_DPhi->Multiply(hZ_Eff_NoMETS_DPhi);


  // MET>90
  TH1D* hZ_Est_Loose2C_DPhi = new TH1D("hZ_Est_Loose2C_DPhi", "", 3, dphiEdges); // estimated Z in ctrl region
  TH1D* hZ_Est_Loose2S_DPhi = new TH1D("hZ_Est_Loose2S_DPhi", "", 3, dphiEdges); // estimated Z in bkgrnd region  
  TH1D* hZ_Eff_Loose2S_DPhi = new TH1D("hZ_Eff_Loose2S_DPhi", "", 3, dphiEdges);

  TH1D* hZ_DY_Loose2_EffVBFS = new TH1D("hZ_DY_Loose2_EffVBFS", "", 1, 0., 1.);  	// epsilon_s_vbf
  TH1D* hZ_DY_Loose2_EffVBFC = new TH1D("hZ_DY_Loose2_EffVBFC", "", 1, 0., 1.);       // epsilon_c_vbf
  TH1D* hZ_DY_Loose2_RatioVBF = new TH1D("hZ_DY_Loose2_RatioVBF", "", 1, 0., 1.);	// epsilon_s_vbf/epsilon_c_vbf
  TH1D* hZ_DY_Loose2_TotalEff = new TH1D("hZ_DY_Loose2_TotalEff", "", 1, 0., 1.); 

  hZ_DY_Loose2_EffVBFS->Add(hZ_DY_Loose2_EffVBFS_N);
  hZ_DY_Loose2_EffVBFS->Divide(hZ_DY_EffVBFS_D);

  hZ_DY_Loose2_EffVBFC->Add(hZ_DY_Loose2_EffVBFC_N);
  hZ_DY_Loose2_EffVBFC->Divide(hZ_DY_EffVBFC_D);

  hZ_DY_Loose2_RatioVBF->Add(hZ_DY_Loose2_EffVBFS);
  hZ_DY_Loose2_RatioVBF->Divide(hZ_DY_Loose2_EffVBFC);
  //hZ_DY_Loose2_RatioVBF->SetBinError(1,TMath::Sqrt(hZ_DY_Loose2_RatioVBF->GetBinError(1)*hZ_DY_Loose2_RatioVBF->GetBinError(1) + eps_vbf_syst*eps_vbf_syst));

  hZ_DY_Loose2_TotalEff->Add(hZ_DY_Loose2_RatioVBF);
  hZ_DY_Loose2_TotalEff->Divide(hZ_DY_EffMuMu);

  for(int ibin = 1; ibin <= hZ_Eff_Loose2S_DPhi->GetNbinsX(); ++ibin) {
    hZ_Eff_Loose2S_DPhi->SetBinContent(ibin,hZ_DY_Loose2_TotalEff->GetBinContent(1));
    hZ_Eff_Loose2S_DPhi->SetBinError  (ibin,hZ_DY_Loose2_TotalEff->GetBinError(1));
  }

  hZ_Est_Loose2C_DPhi->Add(hZ_Data_Loose2C_DPhi, hZ_BG_Loose2C_DPhi, 1., -1.);
  hZ_Est_Loose2S_DPhi->Add(hZ_Est_Loose2C_DPhi,ratioBF);
  hZ_Est_Loose2S_DPhi->Multiply(hZ_Eff_Loose2S_DPhi);

  // MET>100
  TH1D* hZ_Est_LooseC_DPhi = new TH1D("hZ_Est_LooseC_DPhi", "", 3, dphiEdges); // estimated Z in ctrl region
  TH1D* hZ_Est_LooseS_DPhi = new TH1D("hZ_Est_LooseS_DPhi", "", 3, dphiEdges); // estimated Z in bkgrnd region  
  TH1D* hZ_Eff_LooseS_DPhi = new TH1D("hZ_Eff_LooseS_DPhi", "", 3, dphiEdges);

  TH1D* hZ_DY_Loose_EffVBFS = new TH1D("hZ_DY_Loose_EffVBFS", "", 1, 0., 1.);  	// epsilon_s_vbf
  TH1D* hZ_DY_Loose_EffVBFC = new TH1D("hZ_DY_Loose_EffVBFC", "", 1, 0., 1.);       // epsilon_c_vbf
  TH1D* hZ_DY_Loose_RatioVBF = new TH1D("hZ_DY_Loose_RatioVBF", "", 1, 0., 1.);	// epsilon_s_vbf/epsilon_c_vbf
  TH1D* hZ_DY_Loose_TotalEff = new TH1D("hZ_DY_Loose_TotalEff", "", 1, 0., 1.); 

  hZ_DY_Loose_EffVBFS->Add(hZ_DY_Loose_EffVBFS_N);
  hZ_DY_Loose_EffVBFS->Divide(hZ_DY_EffVBFS_D);

  hZ_DY_Loose_EffVBFC->Add(hZ_DY_Loose_EffVBFC_N);
  hZ_DY_Loose_EffVBFC->Divide(hZ_DY_EffVBFC_D);

  hZ_DY_Loose_RatioVBF->Add(hZ_DY_Loose_EffVBFS);
  hZ_DY_Loose_RatioVBF->Divide(hZ_DY_Loose_EffVBFC);
  //hZ_DY_Loose_RatioVBF->SetBinError(1,TMath::Sqrt(hZ_DY_Loose_RatioVBF->GetBinError(1)*hZ_DY_Loose_RatioVBF->GetBinError(1) + eps_vbf_syst*eps_vbf_syst));

  hZ_DY_Loose_TotalEff->Add(hZ_DY_Loose_RatioVBF);
  hZ_DY_Loose_TotalEff->Divide(hZ_DY_EffMuMu);

  for(int ibin = 1; ibin <= hZ_Eff_LooseS_DPhi->GetNbinsX(); ++ibin) {
    hZ_Eff_LooseS_DPhi->SetBinContent(ibin,hZ_DY_Loose_TotalEff->GetBinContent(1));
    hZ_Eff_LooseS_DPhi->SetBinError  (ibin,hZ_DY_Loose_TotalEff->GetBinError(1));
  }

  hZ_Est_LooseC_DPhi->Add(hZ_Data_LooseC_DPhi, hZ_BG_LooseC_DPhi, 1., -1.);
  hZ_Est_LooseS_DPhi->Add(hZ_Est_LooseC_DPhi,ratioBF);
  hZ_Est_LooseS_DPhi->Multiply(hZ_Eff_LooseS_DPhi);



  // print out

  std::cout << std::endl;
  std::cout << "##################################### MET > 130 #####################################" << std::endl;
  std::cout << "  eps_mumu by histogram  : " << hZ_DY_EffMuMu->GetBinContent(1) << " +/- " << hZ_DY_EffMuMu->GetBinError(1) << std::endl;
  std::cout << "  eps_s_vbf by histogram  : " << hZ_DY_EffVBFS->GetBinContent(1) << " +/- " << hZ_DY_EffVBFS->GetBinError(1) << std::endl;
  std::cout << "  eps_c_vbf by histogram  : " << hZ_DY_EffVBFC->GetBinContent(1) << " +/- " << hZ_DY_EffVBFC->GetBinError(1) << std::endl;
  std::cout << "  ratio_vbf by histogram : " << hZ_DY_RatioVBF->GetBinContent(1) << " +/- " << hZ_DY_RatioVBF->GetBinError(1) << "(stat.) + " << 0.01039 * hZ_DY_RatioVBF->GetBinContent(1) << ", - " << 0.0136 * hZ_DY_RatioVBF->GetBinContent(1) << "(syst.)" << std::endl;
  std::cout << "  total eff by histogram : " << hZ_DY_TotalEff->GetBinContent(1) << " +/- " << hZ_DY_TotalEff->GetBinError(1) << std::endl;
  std::cout << std::endl;

  std::cout << std::endl;
  std::cout << "dphi>2.6" << std::endl;
  std::cout << "  DY+jets MC ctrl region : " << hZ_DY_C_DPhi->GetBinContent(3) << " +/- " << hZ_DY_C_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_C_DPhi->GetBinContent(3) << " +/- " << hZ_BG_C_DPhi->GetBinError(3) << "(stat.) + " << 0.035 * hZ_BG_C_DPhi->GetBinContent(3) << ", - " << 0.0 * hZ_BG_C_DPhi->GetBinContent(3) << "(syst.)" << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_C_DPhi->GetBinContent(3) << " +/- " << hZ_Data_C_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  Z in ctrl region       : " << hZ_Est_C_DPhi->GetBinContent(3) << " +/- " << hZ_Est_C_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Z in sgnl region       : " << hZ_Est_S_DPhi->GetBinContent(3) << " +/- " << hZ_Est_S_DPhi->GetBinError(3) << "(stat.) + " << 0.0104 * hZ_Est_S_DPhi->GetBinContent(3) << ", - " << 0.0161 * hZ_Est_S_DPhi->GetBinContent(3) << "(syst.)" << std::endl;
  std::cout << std::endl << std::endl;
  std::cout << "dphi<1.0" << std::endl;
  std::cout << "  DY+jets MC ctrl region : " << hZ_DY_C_DPhi->GetBinContent(1) << " +/- " << hZ_DY_C_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_C_DPhi->GetBinContent(1) << " +/- " << hZ_BG_C_DPhi->GetBinError(1) << "(stat.) + " << 0.174 * hZ_BG_C_DPhi->GetBinContent(1) << ", - " << 0.09 * hZ_BG_C_DPhi->GetBinContent(1) << "(syst.)" << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_C_DPhi->GetBinContent(1) << " +/- " << hZ_Data_C_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  Z in ctrl region       : " << hZ_Est_C_DPhi->GetBinContent(1) << " +/- " << hZ_Est_C_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Z in sgnl region       : " << hZ_Est_S_DPhi->GetBinContent(1) << " +/- " << hZ_Est_S_DPhi->GetBinError(1) << "(stat.) + " << 0.0169 * hZ_Est_S_DPhi->GetBinContent(1) << ", - " << 0.0311 * hZ_Est_S_DPhi->GetBinContent(1) << "(syst.)" << std::endl;
  std::cout << "#####################################################################################" << std::endl;
  std::cout << std::endl << std::endl;

  // write the cutflow table
  std::cout << "Writing cut flow TeX file" << std::endl;

  ofstream effFile;
  effFile.open(options.oDir+std::string("/cutflowZMuMu.tex"));

  effFile << "Cut & N(data) & N(DY\\rightarrow\\ell\\ell) & N($t\\bar{t}$) & N(single $t$) & N(diboson) \\\\" << std::endl;

  TH1D* hZ_CutFlow_TTBar = (TH1D*) ofile->Get("hZ_CutFlow_TTBar");

  // cutflow table
  for (unsigned i=0; i<nCutsZMuMu; ++i) {

    effFile << cuts.cutNameZMuMu(i) << " & ";
    effFile << "$" << hZ_CutFlow_Data->GetBinContent(i+1) << " \\pm " << hZ_CutFlow_Data->GetBinError(i+1) << "$ & ";
    effFile << "$" << hZ_CutFlow_DY->GetBinContent(i+1) << " \\pm " << hZ_CutFlow_DY->GetBinError(i+1) << "$ & ";
    effFile << "$" << hZ_CutFlow_TTBar->GetBinContent(i+1) << " \\pm " << hZ_CutFlow_TTBar->GetBinError(i+1) << "$ & ";
    effFile << "$" << hZ_CutFlow_SingleTSum->GetBinContent(i+1) << " \\pm " << hZ_CutFlow_SingleTSum->GetBinError(i+1) << "$ & ";
    effFile << "$" << hZ_CutFlow_Diboson->GetBinContent(i+1) << " \\pm " << hZ_CutFlow_Diboson->GetBinError(i+1) << "$ & ";
    effFile << std::endl;

  }

  effFile << std::endl << std::endl;
  effFile.close();

  // list histograms for dataset summing
  std::vector<std::string> hists;
  hists.push_back("ZCtrlZMass");
  hists.push_back("ZCtrlZpT");
  hists.push_back("ZCtrlJet1pT");
  hists.push_back("ZCtrlJet1Eta");
  hists.push_back("ZCtrlJet2pT");
  hists.push_back("ZCtrlJet2Eta");
  hists.push_back("ZCtrlCenJetpT");
  hists.push_back("ZCtrlDEtajj");
  hists.push_back("ZCtrlMjj");
  hists.push_back("ZCtrlMET");
  hists.push_back("ZCtrlDPhijj");

  // sum DY datasets
  std::vector<std::string> DYDatasets;
  DYDatasets.push_back(std::string("DYJetsToLL_NoTrig"));
  DYDatasets.push_back(std::string("DYJetsToLL_PtZ-100_NoTrig"));
  DYDatasets.push_back(std::string("DYJetsToLL_EWK_NoTrig"));
  SumDatasets(oDir_Plot, DYDatasets, hists, "DYJets");

  // sum single top datasets
  std::vector<std::string> topDatasets;
  topDatasets.push_back(std::string("SingleT_t"));
  topDatasets.push_back(std::string("SingleTbar_t"));
  topDatasets.push_back(std::string("SingleT_s"));
  topDatasets.push_back(std::string("SingleTbar_s"));
  topDatasets.push_back(std::string("SingleT_tW"));
  topDatasets.push_back(std::string("SingleTbar_tW"));
  topDatasets.push_back(std::string("TTBar"));
  SumDatasets(oDir_Plot, topDatasets, hists, "SingleT+TTbar"); 

  // sum single top datasets
  std::vector<std::string> dibDatasets;
  dibDatasets.push_back(std::string("WW"));
  dibDatasets.push_back(std::string("WZ"));
  dibDatasets.push_back(std::string("ZZ"));
  SumDatasets(oDir_Plot, dibDatasets, hists, "DiBoson");

  // make plots
  std::cout << "Making plots" << std::endl;
  StackPlot plots(oDir_Plot);
  plots.setLegPos(0.69,0.69,0.89,0.89);

  plots.addDataset("DiBoson", kViolet-6, 0);
  plots.addDataset("SingleT+TTbar", kAzure-2, 0);
  plots.addDataset("DYJets", kPink-4,0);
  plots.addDataset("METABCD", kBlack, 1);

  plots.draw("ZCtrlZpT",	"Z_p_{T} [GeV]",		"N_{events}"	,1,1);
  plots.draw("ZCtrlJet1pT", 	"Leading jet p_{T} [GeV]", 	"N_{events}"	,1,1);
  plots.draw("ZCtrlJet1Eta", 	"Leading jet #eta", 		"N_{events}"	,1,1);
  plots.draw("ZCtrlJet2pT",     "Sub-leading jet p_{T} [GeV]",  "N_{events}"	,1,1);
  plots.draw("ZCtrlJet2Eta",    "Sub-leading jet #eta",         "N_{events}"	,1,1);
  plots.draw("ZCtrlCenJetpT",	"Central jet p_{T} [GeV]",	"N_{events}"    ,1,1);
  plots.draw("ZCtrlDEtajj",	"#Delta #eta_{jj}",		"N_{events}"	,1,1);
  plots.draw("ZCtrlMjj", 	"M_{jj} [GeV]",			"N_{events}"	,1,1);
  plots.draw("ZCtrlMET", 	"E_{T}^{miss} [GeV]",		"N_{events}"	,1,1);
  plots.draw("ZCtrlDPhijj", 	"#Delta #phi_{jj} [GeV]",	"N_{events}"	,1,1);
  plots.setYMax(90.);
  plots.setYMin(0.);
  plots.draw("ZCtrlZMass",      "M_{#mu#mu}  [GeV/c^{2}]",      "N_{events}"    ,0,1);

  //store histograms
  ofile->cd();
  hZ_DY_C_DPhi->Write("",TObject::kOverwrite);
  hZ_BG_C_DPhi->Write("",TObject::kOverwrite);
  hZ_Data_C_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_C_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_S_DPhi->Write("",TObject::kOverwrite);
  hZ_Eff_S_DPhi->Write("",TObject::kOverwrite);
  hZ_DY_EffMuMu_D->Write("",TObject::kOverwrite);
  hZ_DY_EffMuMu_N->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFS_D->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFS_N->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFC_D->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFC_N->Write("",TObject::kOverwrite);
  hZ_DY_EffMuMu->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFS->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFC->Write("",TObject::kOverwrite);
  hZ_DY_RatioVBF->Write("",TObject::kOverwrite);
  hZ_DY_TotalEff->Write("",TObject::kOverwrite);

  hZ_DY_NoMETC_DPhi->Write("",TObject::kOverwrite);
  hZ_BG_NoMETC_DPhi->Write("",TObject::kOverwrite);
  hZ_Data_NoMETC_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_NoMETC_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_NoMETS_DPhi->Write("",TObject::kOverwrite);

  hZ_DY_Loose2C_DPhi->Write("",TObject::kOverwrite);
  hZ_BG_Loose2C_DPhi->Write("",TObject::kOverwrite);
  hZ_Data_Loose2C_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_Loose2C_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_Loose2S_DPhi->Write("",TObject::kOverwrite);

  hZ_DY_LooseC_DPhi->Write("",TObject::kOverwrite);
  hZ_BG_LooseC_DPhi->Write("",TObject::kOverwrite);
  hZ_Data_LooseC_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_LooseC_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_LooseS_DPhi->Write("",TObject::kOverwrite);

  hZ_CutFlow_Data->Write("",TObject::kOverwrite);
  hZ_CutFlow_DY->Write("",TObject::kOverwrite);
  hZ_CutFlow_SingleTSum->Write("",TObject::kOverwrite);
  hZ_CutFlow_Diboson->Write("",TObject::kOverwrite);

  ofile->Close();    

}
