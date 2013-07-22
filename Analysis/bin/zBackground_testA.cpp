
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
 
  //std::string oDir_Plot = options.oDir+std::string("/ZBackground");

  //boost::filesystem::path opath(oDir_Plot);
  //if (!exists(opath)) {
  //  std::cout << "Creating output directory : " << oDir_Plot << std::endl;
  //  boost::filesystem::create_directory(opath);
  //}

  // output file
  TFile* ofile = TFile::Open( (options.oDir+std::string("/ZBackgroundBins.root")).c_str(), "RECREATE");

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
  //TH1D* hZ_DY_C_noDPhi = new TH1D("hZ_DY_C_noDPhi", "", 1, 0., 1.);  // Z+jets MC ctrl region
  //TH1D* hZ_BG_C_noDPhi = new TH1D("hZ_BG_C_noDPhi", "", 1, 0., 1.);  // background MC ctrl region
  //TH1D* hZ_Data_C_noDPhi = new TH1D("hZ_Data_C_noDPhi", "", 1, 0., 1.);  // Data ctrl region

  TH1D* hZ_DY_C_DPhi = new TH1D("hZ_DY_C_DPhi", "", 4, dphiEdges);  // Z+jets MC ctrl region
  TH1D* hZ_BG_C_DPhi = new TH1D("hZ_BG_C_DPhi", "", 4, dphiEdges);  // background MC ctrl region
  TH1D* hZ_Data_C_DPhi = new TH1D("hZ_Data_C_DPhi", "", 4, dphiEdges);  // Data ctrl region

  TH1D* hZ_DY_EffMuMu_D = new TH1D("hZ_DY_EffMuMu_D", "", 1, 0., 1.);	// denominator of MuMu efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
  TH1D* hZ_DY_EffMuMu_N = new TH1D("hZ_DY_EffMuMu_N", "", 1, 0., 1.);	// numerator of MuMu efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
 
  TH1D* hZ_DY_EffVBFS_D = new TH1D("hZ_DY_EffVBFS_D", "", 1, 0., 1.);   // denominator of VBF(S) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
  //TH1D* hZ_DY_EffVBFS_N = new TH1D("hZ_DY_EffVBFS_N", "", 1, 0., 1.);   // numerator of VBF(S) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples  
  //TH1D* hZ_DY_EffVBFS_NDPhi = new TH1D("hZ_DY_EffVBFS_NDPhi", "", 4, dphiEdges);

  TH1D* hZ_DY_EffVBFS_NLo = new TH1D("hZ_DY_EffVBFS_NLo", "", 1, 0., 1.);   
  TH1D* hZ_DY_EffVBFS_NHi = new TH1D("hZ_DY_EffVBFS_NHi", "", 1, 0., 1.);
  TH1D* hZ_DY_EffVBFS_NMe1 = new TH1D("hZ_DY_EffVBFS_NMe1", "", 1, 0., 1.); 
  TH1D* hZ_DY_EffVBFS_NMe2 = new TH1D("hZ_DY_EffVBFS_NMe2", "", 1, 0., 1.);

  TH1D* hZ_DY_EffVBFC_D = new TH1D("hZ_DY_EffVBFC_D", "", 1, 0., 1.);   // denominator of VBF(C) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
  //TH1D* hZ_DY_EffVBFC_N = new TH1D("hZ_DY_EffVBFC_N", "", 1, 0., 1.);   // numerator of VBF(C) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 
  //TH1D* hZ_DY_EffVBFC_NDPhi = new TH1D("hZ_DY_EffVBFC_NDPhi", "", 4, dphiEdges);

  TH1D* hZ_DY_EffVBFC_NLo = new TH1D("hZ_DY_EffVBFC_NLo", "", 1, 0., 1.);  
  TH1D* hZ_DY_EffVBFC_NHi = new TH1D("hZ_DY_EffVBFC_NHi", "", 1, 0., 1.);
  TH1D* hZ_DY_EffVBFC_NMe1 = new TH1D("hZ_DY_EffVBFC_NMe1", "", 1, 0., 1.);
  TH1D* hZ_DY_EffVBFC_NMe2 = new TH1D("hZ_DY_EffVBFC_NMe2", "", 1, 0., 1.);

  // cutflow histograms
  //TH1D* hZ_CutFlow_Data       = new TH1D("hZ_CutFlow_Data", "", nCutsZMuMu, 0., nCutsZMuMu);
  //TH1D* hZ_CutFlow_DY         = new TH1D("hZ_CutFlow_DY", "", nCutsZMuMu, 0., nCutsZMuMu);
  //TH1D* hZ_CutFlow_SingleTSum = new TH1D("hZ_CutFlow_SingleTSum", "", nCutsZMuMu, 0., nCutsZMuMu);
  //TH1D* hZ_CutFlow_Diboson    = new TH1D("hZ_CutFlow_Diboson", "", nCutsZMuMu, 0., nCutsZMuMu);


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
    TCut cutZMuMu_C    = puWeight * trigCorr * (cutD + cuts.zMuMuVBF() + METNo2Muon130);
    
    TCut cutEfficiencyMuMu_D    = puWeight * (cutD + cuts.zMuMuGen());
    TCut cutEfficiencyMuMu_N    = puWeight * (cutD + cuts.zMuMuGen() + cuts.zMuMuReco());
 
    TCut cutEfficiencyVBFS_D    = puWeight * (cutD + cuts.zMuMuGenMass());
    //TCut cutEfficiencyVBFS_N    = puWeight * trigCorr * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon130);
    TCut cutEfficiencyVBFS_NLoDPhi    = puWeight * trigCorr * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon130 + cutLoDPhi);
    TCut cutEfficiencyVBFS_NHiDPhi    = puWeight * trigCorr * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon130 + cutHiDPhi);
    TCut cutEfficiencyVBFS_NMe1DPhi    = puWeight * trigCorr * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon130 + cutMe1DPhi);
    TCut cutEfficiencyVBFS_NMe2DPhi    = puWeight * trigCorr * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon130 + cutMe2DPhi);
    
    TCut cutEfficiencyVBFC_D    = puWeight * (cutD + cuts.zMuMuGen() + cuts.zMuMuReco());
    //TCut cutEfficiencyVBFC_N    = puWeight * trigCorr * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon130);
    TCut cutEfficiencyVBFC_NLoDPhi    = puWeight * trigCorr * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon130 + cutLoDPhi);
    TCut cutEfficiencyVBFC_NHiDPhi    = puWeight * trigCorr * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon130 + cutHiDPhi);
    TCut cutEfficiencyVBFC_NMe1DPhi    = puWeight * trigCorr * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon130 + cutMe1DPhi);
    TCut cutEfficiencyVBFC_NMe2DPhi    = puWeight * trigCorr * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon130 + cutMe2DPhi);
 
    // fill tmp histograms for BG estimation
    //TH1D* hZ_C_noDPhi  = new TH1D("hZ_C_noDPhi", "", 1, 0., 1.);
    TH1D* hZ_C_DPhi  = new TH1D("hZ_C_DPhi", "", 4, dphiEdges);  // this is for the actual BG estimation
    // fill tmp histograms for efficiency calculation
    TH1D* hZ_EffMuMu_D  = new TH1D("hZ_EffMuMu_D", "", 1, 0., 1.);
    TH1D* hZ_EffMuMu_N  = new TH1D("hZ_EffMuMu_N", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_D  = new TH1D("hZ_EffVBFS_D", "", 1, 0., 1.);
    //TH1D* hZ_EffVBFS_N  = new TH1D("hZ_EffVBFS_N", "", 1, 0., 1.);
    //TH1D* hZ_EffVBFS_NDPhi = new TH1D("hZ_EffVBFS_NDPhi", "", 4, dphiEdges);
  
    TH1D* hZ_EffVBFS_NLo  = new TH1D("hZ_EffVBFS_NLo", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_NMe1  = new TH1D("hZ_EffVBFS_NMe1", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_NMe2  = new TH1D("hZ_EffVBFS_NMe2", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_NHi  = new TH1D("hZ_EffVBFS_NHi", "", 1, 0., 1.);

    TH1D* hZ_EffVBFC_D  = new TH1D("hZ_EffVBFC_D", "", 1, 0., 1.);
    //TH1D* hZ_EffVBFC_N  = new TH1D("hZ_EffVBFC_N", "", 1, 0., 1.);
    //TH1D* hZ_EffVBFC_NDPhi = new TH1D("hZ_EffVBFC_NDPhi", "", 4, dphiEdges);

    TH1D* hZ_EffVBFC_NLo  = new TH1D("hZ_EffVBFC_NLo", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_NMe1  = new TH1D("hZ_EffVBFC_NMe1", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_NMe2  = new TH1D("hZ_EffVBFC_NMe2", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_NHi  = new TH1D("hZ_EffVBFC_NHi", "", 1, 0., 1.);

    if (isDY) {
      //tree->Draw("0.5>>hZ_C_noDPhi", cutZMuMu_C);
      tree->Draw("vbfDPhi>>hZ_C_DPhi", cutZMuMu_C);
      tree->Draw("0.5>>hZ_EffMuMu_D",      cutEfficiencyMuMu_D);		
      tree->Draw("0.5>>hZ_EffMuMu_N",      cutEfficiencyMuMu_N);
	
      tree->Draw("0.5>>hZ_EffVBFS_D",      cutEfficiencyVBFS_D);
      //tree->Draw("0.5>>hZ_EffVBFS_N",     cutEfficiencyVBFS_N);
      //tree->Draw("vbfDPhi>>hZ_EffVBFS_NDPhi",     cutEfficiencyVBFS_N);
      tree->Draw("0.5>>hZ_EffVBFS_NLo",   cutEfficiencyVBFS_NLoDPhi);
      tree->Draw("0.5>>hZ_EffVBFS_NHi",   cutEfficiencyVBFS_NHiDPhi);
      tree->Draw("0.5>>hZ_EffVBFS_NMe1",   cutEfficiencyVBFS_NMe1DPhi);
      tree->Draw("0.5>>hZ_EffVBFS_NMe2",   cutEfficiencyVBFS_NMe2DPhi);

      tree->Draw("0.5>>hZ_EffVBFC_D",    cutEfficiencyVBFC_D);
      //tree->Draw("0.5>>hZ_EffVBFC_N",    cutEfficiencyVBFC_N);
      //tree->Draw("vbfDPhi>>hZ_EffVBFC_NDPhi",     cutEfficiencyVBFC_N);
      tree->Draw("0.5>>hZ_EffVBFC_NLo",  cutEfficiencyVBFC_NLoDPhi);
      tree->Draw("0.5>>hZ_EffVBFC_NHi",  cutEfficiencyVBFC_NHiDPhi);
      tree->Draw("0.5>>hZ_EffVBFC_NMe1",  cutEfficiencyVBFC_NMe1DPhi);
      tree->Draw("0.5>>hZ_EffVBFC_NMe2",  cutEfficiencyVBFC_NMe2DPhi);
    }
    else {
      tree->Draw("0.5>>hZ_C_noDPhi", cutZMuMu_C);
      tree->Draw("vbfDPhi>>hZ_C_DPhi", cutZMuMu_C);
    }

    // weight  to lumi
    double weight = (dataset.isData) ? 1. : lumi * dataset.sigma / dataset.nEvents;

    //hZ_C_noDPhi->Scale(weight);
    hZ_C_DPhi->Scale(weight);
    hZ_EffVBFS_D->Scale(weight);
    //hZ_EffVBFS_N->Scale(weight);
    //hZ_EffVBFS_NDPhi->Scale(weight);
    hZ_EffVBFS_NLo->Scale(weight);
    hZ_EffVBFS_NMe1->Scale(weight);
    hZ_EffVBFS_NMe2->Scale(weight);
    hZ_EffVBFS_NHi->Scale(weight);
    hZ_EffVBFC_D->Scale(weight);
    //hZ_EffVBFC_N->Scale(weight);
    //hZ_EffVBFC_NDPhi->Scale(weight);
    hZ_EffVBFC_NLo->Scale(weight);
    hZ_EffVBFC_NMe1->Scale(weight);
    hZ_EffVBFC_NMe2->Scale(weight);
    hZ_EffVBFC_NHi->Scale(weight);
    hZ_EffMuMu_D->Scale(weight);
    hZ_EffMuMu_N->Scale(weight);

    // add to output histograms
    if (dataset.isData) {
      //hZ_Data_C_noDPhi->Add(hZ_C_noDPhi);
      hZ_Data_C_DPhi->Add(hZ_C_DPhi);
    }
    else if (isDY) {
      //hZ_DY_C_noDPhi->Add(hZ_C_noDPhi);
      hZ_DY_C_DPhi->Add(hZ_C_DPhi);

      hZ_DY_EffVBFS_D->Add(hZ_EffVBFS_D);
      //hZ_DY_EffVBFS_N->Add(hZ_EffVBFS_N);
      //hZ_DY_EffVBFS_NDPhi->Add(hZ_EffVBFS_NDPhi);
      hZ_DY_EffVBFS_NLo->Add(hZ_EffVBFS_NLo);
      hZ_DY_EffVBFS_NMe1->Add(hZ_EffVBFS_NMe1);
      hZ_DY_EffVBFS_NMe2->Add(hZ_EffVBFS_NMe2);
      hZ_DY_EffVBFS_NHi->Add(hZ_EffVBFS_NHi);

      hZ_DY_EffVBFC_D->Add(hZ_EffVBFC_D);
      //hZ_DY_EffVBFC_N->Add(hZ_EffVBFC_N);
      //hZ_DY_EffVBFC_NDPhi->Add(hZ_EffVBFC_NDPhi);
      hZ_DY_EffVBFC_NLo->Add(hZ_EffVBFC_NLo);
      hZ_DY_EffVBFC_NMe1->Add(hZ_EffVBFC_NMe1);
      hZ_DY_EffVBFC_NMe2->Add(hZ_EffVBFC_NMe2);
      hZ_DY_EffVBFC_NHi->Add(hZ_EffVBFC_NHi);

      hZ_DY_EffMuMu_D->Add(hZ_EffMuMu_D);
      hZ_DY_EffMuMu_N->Add(hZ_EffMuMu_N);
    }
    else {
      //hZ_BG_C_noDPhi->Add(hZ_C_noDPhi);
      hZ_BG_C_DPhi->Add(hZ_C_DPhi);
    }

    std::cout << "  N ctrl (dphi<1.0) : " << hZ_C_DPhi->GetBinContent(1) << " +/- " << hZ_C_DPhi->GetBinError(1) << std::endl;  
    //std::cout << "  N ctrl (dphi>2.6) : " << hZ_C_DPhi->GetBinContent(3) << " +/- " << hZ_C_DPhi->GetBinError(3) << std::endl;
    
    //delete hZ_C_noDPhi;
    delete hZ_C_DPhi;
    delete hZ_EffMuMu_D;
    delete hZ_EffMuMu_N;
    delete hZ_EffVBFS_D;
    //delete hZ_EffVBFS_N;
    //delete hZ_EffVBFS_NDPhi;
    delete hZ_EffVBFS_NLo;
    delete hZ_EffVBFS_NMe1;
    delete hZ_EffVBFS_NMe2;
    delete hZ_EffVBFS_NHi;
    delete hZ_EffVBFC_D;
    //delete hZ_EffVBFC_N;
    //delete hZ_EffVBFC_NDPhi;
    delete hZ_EffVBFC_NLo;
    delete hZ_EffVBFC_NMe1;
    delete hZ_EffVBFC_NMe2;
    delete hZ_EffVBFC_NHi;

    /*
    ofile->cd();

    // cut flow histograms
    std::string hname = std::string("hZ_CutFlow_")+dataset.name;
    TH1D* hZ_CutFlow = new TH1D(hname.c_str(), "", nCutsZMuMu, 0., nCutsZMuMu);

    for (unsigned c=0; c<nCutsZMuMu; ++c) {

      TCut cut;

      if(c == nCutsZMuMu-1) cut = puWeight * trigCorr * (cutD + cuts.cutflowZMuMu(c));
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

    TH1D* ZCtrlZMass      = new TH1D("ZCtrlZMass",   "", 30, 60., 120.);
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
    */
    // clean up
    delete tree;
    file->Close();
   
  }

  // numbers - calculate these from MC in this program later!
  double ratioBF = 5.626;  //  MCFM + NLO

  //no dPhi cut
  //TH1D* hZ_Est_C_noDPhi = new TH1D("hZ_Est_C_noDPhi", "", 1, 0., 1.); // estimated Z in ctrl region
  //TH1D* hZ_Est_S_noDPhi = new TH1D("hZ_Est_S_noDPhi", "", 1, 0., 1.); // estimated Z in bkgrnd region  
  //TH1D* hZ_Eff_S_noDPhi = new TH1D("hZ_Eff_S_noDPhi", "", 1, 0., 1.);

  // bins dPhi
  TH1D* hZ_Est_C_DPhi = new TH1D("hZ_Est_C_DPhi", "", 4, dphiEdges); // estimated Z in ctrl region
  TH1D* hZ_Est_S_DPhi = new TH1D("hZ_Est_S_DPhi", "", 4, dphiEdges); // estimated Z in bkgrnd region  
  TH1D* hZ_Eff_S_DPhi = new TH1D("hZ_Eff_S_DPhi", "", 4, dphiEdges);

  TH1D* hZ_DY_EffMuMu = new TH1D("hZ_DY_EffMuMu", "", 1, 0., 1.);     	// epsilon mumu
  hZ_DY_EffMuMu->Add(hZ_DY_EffMuMu_N);
  hZ_DY_EffMuMu->Divide(hZ_DY_EffMuMu_D);
  double mu_syst = 0.025*hZ_DY_EffMuMu->GetBinContent(1);      //2.5% Muon ID/Iso efficiency uncertainty from EWK-10-002
  hZ_DY_EffMuMu->SetBinError(1,TMath::Sqrt(hZ_DY_EffMuMu->GetBinError(1)*hZ_DY_EffMuMu->GetBinError(1) + mu_syst*mu_syst));

  //TH1D* hZ_DY_EffVBFS = new TH1D("hZ_DY_EffVBFS", "", 1, 0., 1.);  	// epsilon_s_vbf
  TH1D* hZ_DY_EffVBFSLo = new TH1D("hZ_DY_EffVBFSLo", "", 1, 0., 1.);
  TH1D* hZ_DY_EffVBFSMe1 = new TH1D("hZ_DY_EffVBFSMe1", "", 1, 0., 1.);
  TH1D* hZ_DY_EffVBFSMe2 = new TH1D("hZ_DY_EffVBFSMe2", "", 1, 0., 1.);
  TH1D* hZ_DY_EffVBFSHi = new TH1D("hZ_DY_EffVBFSHi", "", 1, 0., 1.);
  //TH1D* hZ_DY_EffVBFS_DPhi = new TH1D("hZ_DY_EffVBFS_DPhi", "", 4, dphiEdges);
  //hZ_DY_EffVBFS->Add(hZ_DY_EffVBFS_N);
  //hZ_DY_EffVBFS->Divide(hZ_DY_EffVBFS_D);
  hZ_DY_EffVBFSLo->Add(hZ_DY_EffVBFS_NLo);
  hZ_DY_EffVBFSLo->Divide(hZ_DY_EffVBFS_D);
  hZ_DY_EffVBFSMe1->Add(hZ_DY_EffVBFS_NMe1);
  hZ_DY_EffVBFSMe1->Divide(hZ_DY_EffVBFS_D);
  hZ_DY_EffVBFSMe2->Add(hZ_DY_EffVBFS_NMe2);
  hZ_DY_EffVBFSMe2->Divide(hZ_DY_EffVBFS_D);
  hZ_DY_EffVBFSHi->Add(hZ_DY_EffVBFS_NHi);
  hZ_DY_EffVBFSHi->Divide(hZ_DY_EffVBFS_D);

  //for(int ibin = 1; ibin <= hZ_DY_EffVBFS_DPhi->GetNbinsX(); ++ibin) {
  //  hZ_DY_EffVBFS_DPhi->SetBinContent(ibin,hZ_DY_EffVBFS_NDPhi->GetBinContent(ibin)/hZ_DY_EffVBFS_D->GetBinContent(1));
  //  hZ_DY_EffVBFS_DPhi->SetBinError(ibin,TMath::Sqrt(pow(hZ_DY_EffVBFS_NDPhi->GetBinError(ibin)/hZ_DY_EffVBFS_NDPhi->GetBinContent(ibin),2) + pow(hZ_DY_EffVBFS_D->GetBinError(1)/hZ_DY_EffVBFS_D->GetBinContent(1),2)));
  //}  
 
  //TH1D* hZ_DY_EffVBFC = new TH1D("hZ_DY_EffVBFC", "", 1, 0., 1.);       // epsilon_c_vbf
  TH1D* hZ_DY_EffVBFCLo = new TH1D("hZ_DY_EffVBFCLo", "", 1, 0., 1.);       // epsilon_c_vbf
  TH1D* hZ_DY_EffVBFCMe1 = new TH1D("hZ_DY_EffVBFCMe1", "", 1, 0., 1.);       // epsilon_c_vbf
  TH1D* hZ_DY_EffVBFCMe2 = new TH1D("hZ_DY_EffVBFCMe2", "", 1, 0., 1.);       // epsilon_c_vbf
  TH1D* hZ_DY_EffVBFCHi = new TH1D("hZ_DY_EffVBFCHi", "", 1, 0., 1.);       // epsilon_c_vbf
  //TH1D* hZ_DY_EffVBFC_DPhi = new TH1D("hZ_DY_EffVBFC_DPhi", "", 4, dphiEdges);
  //hZ_DY_EffVBFC->Add(hZ_DY_EffVBFC_N);
  //hZ_DY_EffVBFC->Divide(hZ_DY_EffVBFC_D);
  hZ_DY_EffVBFCLo->Add(hZ_DY_EffVBFC_NLo);
  hZ_DY_EffVBFCLo->Divide(hZ_DY_EffVBFC_D);
  hZ_DY_EffVBFCMe1->Add(hZ_DY_EffVBFC_NMe1);
  hZ_DY_EffVBFCMe1->Divide(hZ_DY_EffVBFC_D);
  hZ_DY_EffVBFCMe2->Add(hZ_DY_EffVBFC_NMe2);
  hZ_DY_EffVBFCMe2->Divide(hZ_DY_EffVBFC_D);
  hZ_DY_EffVBFCHi->Add(hZ_DY_EffVBFC_NHi);
  hZ_DY_EffVBFCHi->Divide(hZ_DY_EffVBFC_D);
  //for(int ibin = 1; ibin <= hZ_DY_EffVBFC_DPhi->GetNbinsX(); ++ibin) {
  //  hZ_DY_EffVBFC_DPhi->SetBinContent(ibin,hZ_DY_EffVBFC_NDPhi->GetBinContent(ibin)/hZ_DY_EffVBFC_D->GetBinContent(1));
  //  hZ_DY_EffVBFC_DPhi->SetBinError  (ibin,TMath::Sqrt(pow(hZ_DY_EffVBFC_NDPhi->GetBinError(ibin)/hZ_DY_EffVBFC_NDPhi->GetBinContent(ibin),2) + pow(hZ_DY_EffVBFC_D->GetBinError(1)/hZ_DY_EffVBFC_D->GetBinContent(1),2)));
  //}
 
  //TH1D* hZ_DY_RatioVBF = new TH1D("hZ_DY_RatioVBF", "", 1, 0., 1.);	// epsilon_s_vbf/epsilon_c_vbf
  //TH1D* hZ_DY_RatioVBF_DPhi = new TH1D("hZ_DY_RatioVBF_DPhi", "", 4, dphiEdges);
  TH1D* hZ_DY_RatioVBFLo = new TH1D("hZ_DY_RatioVBFLo", "", 1, 0., 1.);
  TH1D* hZ_DY_RatioVBFMe1 = new TH1D("hZ_DY_RatioVBFMe1", "", 1, 0., 1.);
  TH1D* hZ_DY_RatioVBFMe2 = new TH1D("hZ_DY_RatioVBFMe2", "", 1, 0., 1.);
  TH1D* hZ_DY_RatioVBFHi = new TH1D("hZ_DY_RatioVBFHi", "", 1, 0., 1.);
  //hZ_DY_RatioVBF->Add(hZ_DY_EffVBFS);
  //hZ_DY_RatioVBF->Divide(hZ_DY_EffVBFC);
  hZ_DY_RatioVBFLo->Add(hZ_DY_EffVBFSLo);
  hZ_DY_RatioVBFLo->Divide(hZ_DY_EffVBFCLo);
  hZ_DY_RatioVBFMe1->Add(hZ_DY_EffVBFSMe1);
  hZ_DY_RatioVBFMe1->Divide(hZ_DY_EffVBFCMe1);
  hZ_DY_RatioVBFMe2->Add(hZ_DY_EffVBFSMe2);
  hZ_DY_RatioVBFMe2->Divide(hZ_DY_EffVBFCMe2);
  hZ_DY_RatioVBFHi->Add(hZ_DY_EffVBFSHi);
  hZ_DY_RatioVBFHi->Divide(hZ_DY_EffVBFCHi);
  //hZ_DY_RatioVBF_DPhi->Add(hZ_DY_EffVBFS_DPhi);
  //hZ_DY_RatioVBF_DPhi->Divide(hZ_DY_EffVBFC_DPhi);
 
  //TH1D* hZ_DY_TotalEff = new TH1D("hZ_DY_TotalEff", "", 1, 0., 1.); 
  //TH1D* hZ_DY_TotalEff_DPhi = new TH1D("hZ_DY_TotalEff_DPhi", "", 4, dphiEdges);
  TH1D* hZ_DY_TotalEffLo = new TH1D("hZ_DY_TotalEffLo", "", 1, 0., 1.);
  TH1D* hZ_DY_TotalEffMe1 = new TH1D("hZ_DY_TotalEffMe1", "", 1, 0., 1.);
  TH1D* hZ_DY_TotalEffMe2 = new TH1D("hZ_DY_TotalEffMe2", "", 1, 0., 1.);
  TH1D* hZ_DY_TotalEffHi = new TH1D("hZ_DY_TotalEffHi", "", 1, 0., 1.);
  hZ_DY_TotalEffLo->Add(hZ_DY_RatioVBFLo);
  hZ_DY_TotalEffLo->Divide(hZ_DY_EffMuMu);
  hZ_DY_TotalEffMe1->Add(hZ_DY_RatioVBFMe1);
  hZ_DY_TotalEffMe1->Divide(hZ_DY_EffMuMu);
  hZ_DY_TotalEffMe2->Add(hZ_DY_RatioVBFMe2);
  hZ_DY_TotalEffMe2->Divide(hZ_DY_EffMuMu);
  hZ_DY_TotalEffHi->Add(hZ_DY_RatioVBFHi);
  hZ_DY_TotalEffHi->Divide(hZ_DY_EffMuMu);

  //hZ_Eff_S_noDPhi->Add(hZ_DY_RatioVBF);
  //hZ_Eff_S_noDPhi->Divide(hZ_DY_EffMuMu);
  hZ_Eff_S_DPhi->SetBinContent(1,hZ_DY_TotalEffLo->GetBinContent(1));
  hZ_Eff_S_DPhi->SetBinError(1,hZ_DY_TotalEffLo->GetBinError(1));
  hZ_Eff_S_DPhi->SetBinContent(2,hZ_DY_TotalEffMe1->GetBinContent(1));
  hZ_Eff_S_DPhi->SetBinError(2,hZ_DY_TotalEffMe1->GetBinError(1));
  hZ_Eff_S_DPhi->SetBinContent(3,hZ_DY_TotalEffMe2->GetBinContent(1));
  hZ_Eff_S_DPhi->SetBinError(3,hZ_DY_TotalEffMe2->GetBinError(1));
  hZ_Eff_S_DPhi->SetBinContent(4,hZ_DY_TotalEffHi->GetBinContent(1));
  hZ_Eff_S_DPhi->SetBinError(4,hZ_DY_TotalEffHi->GetBinError(1));
  //for(int ibin = 1; ibin <= hZ_Eff_S_DPhi->GetNbinsX(); ++ibin) {
  //  hZ_Eff_S_DPhi->SetBinContent(ibin,hZ_DY_RatioVBF_DPhi->GetBinContent(ibin)/hZ_DY_EffMuMu->GetBinContent(1));
  //  hZ_Eff_S_DPhi->SetBinError  (ibin,TMath::Sqrt(pow(hZ_DY_RatioVBF_DPhi->GetBinError(ibin)/hZ_DY_RatioVBF_DPhi->GetBinContent(ibin),2) + pow(hZ_DY_EffMuMu->GetBinError(1)/hZ_DY_EffMuMu->GetBinContent(1),2)));
  //}

  // no dPhi
  //hZ_Est_C_noDPhi->Add(hZ_Data_C_noDPhi, hZ_BG_C_noDPhi, 1., -1.);
  //hZ_Est_S_noDPhi->Add(hZ_Est_C_noDPhi,ratioBF);
  //hZ_Est_S_noDPhi->Multiply(hZ_Eff_S_noDPhi);
  // bin dPhi
  hZ_Est_C_DPhi->Add(hZ_Data_C_DPhi, hZ_BG_C_DPhi, 1., -1.);
  hZ_Est_S_DPhi->Add(hZ_Est_C_DPhi,ratioBF);
  hZ_Est_S_DPhi->Multiply(hZ_Eff_S_DPhi);

  // print out

  std::cout << std::endl;
  std::cout << "##################################### Z BG from Zmumu in bins of dPhijj #####################################" << std::endl;
  //std::cout << "no dphi cut" << std::endl;
  //std::cout << std::endl;
  std::cout << "  eps_mumu by histogram  : " << hZ_DY_EffMuMu->GetBinContent(1) << " +/- " << hZ_DY_EffMuMu->GetBinError(1) << std::endl;
  //std::cout << "  eps_s_vbf by histogram  : " << hZ_DY_EffVBFS->GetBinContent(1) << " +/- " << hZ_DY_EffVBFS->GetBinError(1) << std::endl;
  //std::cout << "  eps_c_vbf by histogram  : " << hZ_DY_EffVBFC->GetBinContent(1) << " +/- " << hZ_DY_EffVBFC->GetBinError(1) << std::endl;
  //std::cout << "  ratio_vbf by histogram : " << hZ_DY_RatioVBF->GetBinContent(1) << " +/- " << hZ_DY_RatioVBF->GetBinError(1) << std::endl;
  //std::cout << "  total eff by histogram : " << hZ_Eff_S_noDPhi->GetBinContent(1) << " +/- " << hZ_Eff_S_noDPhi->GetBinError(1) << std::endl;
  //std::cout << std::endl;
  //std::cout << "  DY+jets MC ctrl region : " << hZ_DY_C_noDPhi->GetBinContent(1) << " +/- " << hZ_DY_C_noDPhi->GetBinError(1) << std::endl;
  //std::cout << "  Background ctrl region : " << hZ_BG_C_noDPhi->GetBinContent(1) << " +/- " << hZ_BG_C_noDPhi->GetBinError(1) << std::endl;
  //std::cout << "  Data ctrl region       : " << hZ_Data_C_noDPhi->GetBinContent(1) << " +/- " << hZ_Data_C_noDPhi->GetBinError(1) << std::endl;
  //std::cout << std::endl;
  //std::cout << "  Z in ctrl region       : " << hZ_Est_C_noDPhi->GetBinContent(1) << " +/- " << hZ_Est_C_noDPhi->GetBinError(1) << std::endl;
  //std::cout << "  Z in sgnl region       : " << hZ_Est_S_noDPhi->GetBinContent(1) << " +/- " << hZ_Est_S_noDPhi->GetBinError(1) << std::endl;
//"(stat.) + " << 0.0169 * hZ_Est_S_DPhi->GetBinContent(1) << ", - " << 0.0311 * hZ_Est_S_DPhi->GetBinContent(1) << "(syst.)" << std::endl;
  std::cout << std::endl;
  std::cout << "dphi < 1.0" << std::endl;
  std::cout << std::endl;
  std::cout << "  eps_s_vbf by histogram : " << hZ_DY_EffVBFSLo->GetBinContent(1) << " +/- " << hZ_DY_EffVBFSLo->GetBinError(1) << std::endl;
  std::cout << "  eps_c_vbf by histogram : " << hZ_DY_EffVBFCLo->GetBinContent(1) << " +/- " << hZ_DY_EffVBFCLo->GetBinError(1) << std::endl;
  std::cout << "  ratio_vbf by histogram : " << hZ_DY_RatioVBFLo->GetBinContent(1) << " +/- " << hZ_DY_RatioVBFLo->GetBinError(1) << std::endl;
  std::cout << "  total eff by histogram : " << hZ_Eff_S_DPhi->GetBinContent(1) << " +/- " << hZ_Eff_S_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  DY+jets MC ctrl region : " << hZ_DY_C_DPhi->GetBinContent(1) << " +/- " << hZ_DY_C_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_C_DPhi->GetBinContent(1) << " +/- " << hZ_BG_C_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_C_DPhi->GetBinContent(1) << " +/- " << hZ_Data_C_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  Z in ctrl region       : " << hZ_Est_C_DPhi->GetBinContent(1) << " +/- " << hZ_Est_C_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Z in sgnl region       : " << hZ_Est_S_DPhi->GetBinContent(1) << " +/- " << hZ_Est_S_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "dphi > 1.0 dphi < 1.8" << std::endl;
  std::cout << std::endl;
  std::cout << "  eps_s_vbf by histogram : " << hZ_DY_EffVBFSMe1->GetBinContent(1) << " +/- " << hZ_DY_EffVBFSMe1->GetBinError(1) << std::endl;
  std::cout << "  eps_c_vbf by histogram : " << hZ_DY_EffVBFCMe1->GetBinContent(1) << " +/- " << hZ_DY_EffVBFCMe1->GetBinError(1) << std::endl;
  std::cout << "  ratio_vbf by histogram : " << hZ_DY_RatioVBFMe1->GetBinContent(1) << " +/- " << hZ_DY_RatioVBFMe1->GetBinError(1) << std::endl;
  std::cout << "  total eff by histogram : " << hZ_Eff_S_DPhi->GetBinContent(2) << " +/- " << hZ_Eff_S_DPhi->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "  DY+jets MC ctrl region : " << hZ_DY_C_DPhi->GetBinContent(2) << " +/- " << hZ_DY_C_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_C_DPhi->GetBinContent(2) << " +/- " << hZ_BG_C_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_C_DPhi->GetBinContent(2) << " +/- " << hZ_Data_C_DPhi->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "  Z in ctrl region       : " << hZ_Est_C_DPhi->GetBinContent(2) << " +/- " << hZ_Est_C_DPhi->GetBinError(2) << std::endl;
  std::cout << "  Z in sgnl region       : " << hZ_Est_S_DPhi->GetBinContent(2) << " +/- " << hZ_Est_S_DPhi->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "dphi > 1.8 dphi < 2.6" << std::endl;
  std::cout << std::endl; 
  std::cout << "  eps_s_vbf by histogram : " << hZ_DY_EffVBFSMe2->GetBinContent(1) << " +/- " << hZ_DY_EffVBFSMe2->GetBinError(1) << std::endl;
  std::cout << "  eps_c_vbf by histogram : " << hZ_DY_EffVBFCMe2->GetBinContent(1) << " +/- " << hZ_DY_EffVBFCMe2->GetBinError(1) << std::endl;
  std::cout << "  ratio_vbf by histogram : " << hZ_DY_RatioVBFMe2->GetBinContent(1) << " +/- " << hZ_DY_RatioVBFMe2->GetBinError(1) << std::endl;
  std::cout << "  total eff by histogram : " << hZ_Eff_S_DPhi->GetBinContent(3) << " +/- " << hZ_Eff_S_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  DY+jets MC ctrl region : " << hZ_DY_C_DPhi->GetBinContent(3) << " +/- " << hZ_DY_C_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_C_DPhi->GetBinContent(3) << " +/- " << hZ_BG_C_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_C_DPhi->GetBinContent(3) << " +/- " << hZ_Data_C_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  Z in ctrl region       : " << hZ_Est_C_DPhi->GetBinContent(3) << " +/- " << hZ_Est_C_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Z in sgnl region       : " << hZ_Est_S_DPhi->GetBinContent(3) << " +/- " << hZ_Est_S_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "dphi > 2.6" << std::endl;
  std::cout << std::endl;
  std::cout << "  eps_s_vbf by histogram : " << hZ_DY_EffVBFSHi->GetBinContent(1) << " +/- " << hZ_DY_EffVBFSHi->GetBinError(1) << std::endl;
  std::cout << "  eps_c_vbf by histogram : " << hZ_DY_EffVBFCHi->GetBinContent(1) << " +/- " << hZ_DY_EffVBFCHi->GetBinError(1) << std::endl;
  std::cout << "  ratio_vbf by histogram : " << hZ_DY_RatioVBFHi->GetBinContent(1) << " +/- " << hZ_DY_RatioVBFHi->GetBinError(1) << std::endl;
  std::cout << "  total eff by histogram : " << hZ_Eff_S_DPhi->GetBinContent(4) << " +/- " << hZ_Eff_S_DPhi->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "  DY+jets MC ctrl region : " << hZ_DY_C_DPhi->GetBinContent(4) << " +/- " << hZ_DY_C_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_C_DPhi->GetBinContent(4) << " +/- " << hZ_BG_C_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_C_DPhi->GetBinContent(4) << " +/- " << hZ_Data_C_DPhi->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "  Z in ctrl region       : " << hZ_Est_C_DPhi->GetBinContent(4) << " +/- " << hZ_Est_C_DPhi->GetBinError(4) << std::endl;
  std::cout << "  Z in sgnl region       : " << hZ_Est_S_DPhi->GetBinContent(4) << " +/- " << hZ_Est_S_DPhi->GetBinError(4) << std::endl;
  std::cout << std::endl;
  std::cout << "#####################################################################################" << std::endl;
  std::cout << std::endl << std::endl;

  /*
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
  plots.setLegPos(0.62,0.62,0.89,0.89);

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
  plots.draw("ZCtrlDPhijj", 	"#Delta #phi_{jj}",		"N_{events}"	,1,1);
  plots.setYMax(90.);
  plots.setYMin(0.);
  plots.draw("ZCtrlZMass",      "M_{#mu#mu}  [GeV/c^{2}]",      "N_{events}"    ,0,1);

  */
  //store histograms
  ofile->cd();
  //hZ_DY_C_noDPhi->Write("",TObject::kOverwrite);
  //hZ_BG_C_noDPhi->Write("",TObject::kOverwrite);
  //hZ_Data_C_noDPhi->Write("",TObject::kOverwrite);
  //hZ_Est_C_noDPhi->Write("",TObject::kOverwrite);
  //hZ_Est_S_noDPhi->Write("",TObject::kOverwrite);
  //hZ_Eff_S_noDPhi->Write("",TObject::kOverwrite);
  hZ_DY_C_DPhi->Write("",TObject::kOverwrite);
  hZ_BG_C_DPhi->Write("",TObject::kOverwrite);
  hZ_Data_C_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_C_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_S_DPhi->Write("",TObject::kOverwrite);
  hZ_Eff_S_DPhi->Write("",TObject::kOverwrite);
  hZ_DY_EffMuMu_D->Write("",TObject::kOverwrite);
  hZ_DY_EffMuMu_N->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFS_D->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFS_NLo->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFS_NMe1->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFS_NMe2->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFS_NHi->Write("",TObject::kOverwrite);
  //hZ_DY_EffVBFS_N->Write("",TObject::kOverwrite);
  //hZ_DY_EffVBFS_NDPhi->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFC_D->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFC_NLo->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFC_NMe1->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFC_NMe2->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFC_NHi->Write("",TObject::kOverwrite);
  //hZ_DY_EffVBFC_N->Write("",TObject::kOverwrite);
  //hZ_DY_EffVBFC_NDPhi->Write("",TObject::kOverwrite);
  hZ_DY_EffMuMu->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFSLo->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFSMe1->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFSMe2->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFSHi->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFCLo->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFCMe1->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFCMe2->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFCHi->Write("",TObject::kOverwrite);
  hZ_DY_RatioVBFLo->Write("",TObject::kOverwrite);
  hZ_DY_RatioVBFMe1->Write("",TObject::kOverwrite);
  hZ_DY_RatioVBFMe2->Write("",TObject::kOverwrite);
  hZ_DY_RatioVBFHi->Write("",TObject::kOverwrite);
  //hZ_DY_EffVBFS_DPhi->Write("",TObject::kOverwrite);
  //hZ_DY_EffVBFC_DPhi->Write("",TObject::kOverwrite);
  //hZ_DY_RatioVBF_DPhi->Write("",TObject::kOverwrite);
  //hZ_DY_TotalEff->Write("",TObject::kOverwrite);
 
  //hZ_CutFlow_Data->Write("",TObject::kOverwrite);
  //hZ_CutFlow_DY->Write("",TObject::kOverwrite);
  //hZ_CutFlow_SingleTSum->Write("",TObject::kOverwrite);
  //hZ_CutFlow_Diboson->Write("",TObject::kOverwrite);

  ofile->Close();    

}
