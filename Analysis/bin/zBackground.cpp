
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
  TCut trigCorr( "(trigCorrWeight>0) ? trigCorrWeight : 1." );
  TCut METNoMuon130("metNoMuon>130.");	// add here later for VBF efficiency when MET>35, MET>70 (QCD estimation)
  TCut METNo2Muon130("metNo2Muon>130.");
  TCut cutLoDPhi = cuts.cut("dPhiJJ");

  // histograms
  double dphiEdges[4] = { 0., 1.0, 2.6, TMath::Pi() };
  double metEdges[13] = { 0., 10., 20., 30., 40., 50., 60., 70., 80., 90., 100., 110., 120. };

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


  // 2D mET vs dphi
  TH2D* hZ_DY_C_METDPhi = new TH2D("hZ_DY_C_METDPhi", "", 3, dphiEdges, 12, metEdges);  // Z+jets MC ctrl region
  TH2D* hZ_BG_C_METDPhi = new TH2D("hZ_BG_C_METDPhi", "", 3, dphiEdges, 12, metEdges);  // background MC ctrl region
  TH2D* hZ_Data_C_METDPhi = new TH2D("hZ_Data_C_METDPhi", "", 3, dphiEdges, 12, metEdges);  // Data ctrl region

  TH1D* hZ_DY_EffVBFS_MET0_N = new TH1D("hZ_DY_EffVBFS_MET0_N", "", 12, metEdges);
  TH1D* hZ_DY_EffVBFS_MET0_D = new TH1D("hZ_DY_EffVBFS_MET0_D", "", 12, metEdges);
  TH1D* hZ_DY_EffVBFC_MET0_N = new TH1D("hZ_DY_EffVBFC_MET0_N", "", 12, metEdges);
  TH1D* hZ_DY_EffVBFC_MET0_D = new TH1D("hZ_DY_EffVBFC_MET0_D", "", 12, metEdges);

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
    TCut cutZMuMu_C    = puWeight * trigCorr * (cutD + cuts.zMuMuVBF() + METNo2Muon130);
    
    TCut cutEfficiencyMuMu_D    = puWeight * (cutD + cuts.zMuMuGen());
    TCut cutEfficiencyMuMu_N    = puWeight * (cutD + cuts.zMuMuGen() + cuts.zMuMuReco());
 
    TCut cutEfficiencyVBFS_D    = puWeight * (cutD + cuts.zMuMuGenMass());
    TCut cutEfficiencyVBFS_N    = puWeight * trigCorr * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon130 + cutLoDPhi);
    
    TCut cutEfficiencyVBFC_D    = puWeight * (cutD + cuts.zMuMuGen() + cuts.zMuMuReco());
    TCut cutEfficiencyVBFC_N    = puWeight * trigCorr * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon130 + cutLoDPhi);

    // cuts for 2D
    TCut cutZMuMu_MET0_C    = puWeight * trigCorr * (cutD + cuts.zMuMuVBF());

    TCut cutEfficiencyVBFS_MET0_N  = puWeight * trigCorr * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf());
    TCut cutEfficiencyVBFC_MET0_N  = puWeight * trigCorr * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf());
    
    // fill tmp histograms for BG estimation
    TH1D* hZ_C_DPhi  = new TH1D("hZ_C_DPhi", "", 3, dphiEdges);  // this is for the actual BG estimation
    // fill tmp histograms for efficiency calculation
    TH1D* hZ_EffMuMu_D  = new TH1D("hZ_EffMuMu_D", "", 1, 0., 1.);
    TH1D* hZ_EffMuMu_N  = new TH1D("hZ_EffMuMu_N", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_D  = new TH1D("hZ_EffVBFS_D", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_N  = new TH1D("hZ_EffVBFS_N", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_D  = new TH1D("hZ_EffVBFC_D", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_N  = new TH1D("hZ_EffVBFC_N", "", 1, 0., 1.);

    // 2D calculation
    TH2D* hZ_C_METDPhi       = new TH2D("hZ_C_METDPhi", "", 3, dphiEdges, 12, metEdges);
    TH1D* hZ_EffVBFS_MET0_N  = new TH1D("hZ_EffVBFS_MET0_N", "", 12, metEdges);
    TH1D* hZ_EffVBFS_MET0_D  = new TH1D("hZ_EffVBFS_MET0_D", "", 12, metEdges);
    TH1D* hZ_EffVBFC_MET0_N  = new TH1D("hZ_EffVBFC_MET0_N", "", 12, metEdges);
    TH1D* hZ_EffVBFC_MET0_D  = new TH1D("hZ_EffVBFC_MET0_D", "", 12, metEdges);

    if (isDY) {
      tree->Draw("vbfDPhi>>hZ_C_DPhi", cutZMuMu_C);
      tree->Draw("0.5>>hZ_EffMuMu_D",     cutEfficiencyMuMu_D);		
      tree->Draw("0.5>>hZ_EffMuMu_N",     cutEfficiencyMuMu_N);	
      tree->Draw("0.5>>hZ_EffVBFS_D",     cutEfficiencyVBFS_D);
      tree->Draw("0.5>>hZ_EffVBFS_N",  cutEfficiencyVBFS_N);
      tree->Draw("0.5>>hZ_EffVBFC_D",     cutEfficiencyVBFC_D);
      tree->Draw("0.5>>hZ_EffVBFC_N",  cutEfficiencyVBFC_N);

      tree->Draw("met:vbfDPhi>>hZ_C_METDPhi", cutZMuMu_MET0_C);
      tree->Draw("met>>hZ_EffVBFS_MET0_N",   cutEfficiencyVBFS_MET0_N);
      tree->Draw("met>>hZ_EffVBFS_MET0_D",   cutEfficiencyVBFS_D);
      tree->Draw("met>>hZ_EffVBFC_MET0_N",   cutEfficiencyVBFC_MET0_N);
      tree->Draw("met>>hZ_EffVBFC_MET0_D",   cutEfficiencyVBFC_D);

    }
    else {
      tree->Draw("vbfDPhi>>hZ_C_DPhi", cutZMuMu_C);

      tree->Draw("met:vbfDPhi>>hZ_C_METDPhi",  cutZMuMu_MET0_C);
    }

    // weight  to lumi
    double weight = (dataset.isData) ? 1. : lumi * dataset.sigma / dataset.nEvents;

    hZ_C_DPhi->Scale(weight);
    hZ_EffVBFS_D->Scale(weight);
    hZ_EffVBFS_N->Scale(weight);
    hZ_EffVBFC_D->Scale(weight);
    hZ_EffVBFC_N->Scale(weight);
    hZ_EffMuMu_D->Scale(weight);
    hZ_EffMuMu_N->Scale(weight);

    hZ_C_METDPhi->Scale(weight);
    hZ_EffVBFS_MET0_N->Scale(weight);
    hZ_EffVBFS_MET0_D->Scale(weight);
    hZ_EffVBFC_MET0_N->Scale(weight);
    hZ_EffVBFC_MET0_D->Scale(weight);


    // add to output histograms
    if (dataset.isData) {
      hZ_Data_C_DPhi->Add(hZ_C_DPhi);
      hZ_Data_C_METDPhi->Add(hZ_C_METDPhi);
    }
    else if (isDY) {
      hZ_DY_C_DPhi->Add(hZ_C_DPhi);
      hZ_DY_EffVBFS_D->Add(hZ_EffVBFS_D);
      hZ_DY_EffVBFS_N->Add(hZ_EffVBFS_N);
      hZ_DY_EffVBFC_D->Add(hZ_EffVBFC_D);
      hZ_DY_EffVBFC_N->Add(hZ_EffVBFC_N);
      hZ_DY_EffMuMu_D->Add(hZ_EffMuMu_D);
      hZ_DY_EffMuMu_N->Add(hZ_EffMuMu_N);

      hZ_DY_C_METDPhi->Add(hZ_C_METDPhi);
      hZ_DY_EffVBFS_MET0_N->Add(hZ_EffVBFS_MET0_N);
      hZ_DY_EffVBFS_MET0_D->Add(hZ_EffVBFS_MET0_D);
      hZ_DY_EffVBFC_MET0_N->Add(hZ_EffVBFC_MET0_N);
      hZ_DY_EffVBFC_MET0_D->Add(hZ_EffVBFC_MET0_D);
    }
    else {
      hZ_BG_C_DPhi->Add(hZ_C_DPhi);
      hZ_BG_C_METDPhi->Add(hZ_C_METDPhi);
    }

    std::cout << "  N ctrl (dphi<1.0) : " << hZ_C_DPhi->GetBinContent(1) << " +/- " << hZ_C_DPhi->GetBinError(1) << std::endl;  
    //std::cout << "  N ctrl (dphi>2.6) : " << hZ_C_DPhi->GetBinContent(3) << " +/- " << hZ_C_DPhi->GetBinError(3) << std::endl;
    
    delete hZ_C_DPhi;
    delete hZ_EffMuMu_D;
    delete hZ_EffMuMu_N;
    delete hZ_EffVBFS_D;
    delete hZ_EffVBFS_N;
    delete hZ_EffVBFC_D;
    delete hZ_EffVBFC_N;

    delete hZ_C_METDPhi;
    delete hZ_EffVBFS_MET0_N;
    delete hZ_EffVBFS_MET0_D;
    delete hZ_EffVBFC_MET0_N;
    delete hZ_EffVBFC_MET0_D;

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

    // clean up
    delete tree;
    file->Close();
   
  }

  // numbers - calculate these from MC in this program later!
  double ratioBF = constants::ratioZToNuNuZToLL;

  // efficiencies (these will all have syst uncertainty only)
  TH1D* hZ_DY_EffMuMu       = new TH1D("hZ_DY_EffMuMu",  "", 1, 0., 1.);     	// epsilon mumu
  TH1D* hZ_DY_EffVBFS       = new TH1D("hZ_DY_EffVBFS",  "", 1, 0., 1.);  	// epsilon_s_vbf
  TH1D* hZ_DY_EffVBFC       = new TH1D("hZ_DY_EffVBFC",  "", 1, 0., 1.);       // epsilon_c_vbf
  TH1D* hZ_DY_RatioVBF      = new TH1D("hZ_DY_RatioVBF", "", 1, 0., 1.);	// epsilon_s_vbf/epsilon_c_vbf
  TH1D* hZ_DY_TotalEff      = new TH1D("hZ_DY_TotalEff", "", 1, 0., 1.); 

  // for stat only calculation
  TH1D* hZ_Eff_S_DPhi       = new TH1D("hZ_Eff_S_DPhi",       "", 3, dphiEdges);
  TH1D* hZ_Est_C_DPhi       = new TH1D("hZ_Est_C_DPhi",       "", 3, dphiEdges);
  TH1D* hZ_Est_S_DPhi       = new TH1D("hZ_Est_S_DPhi",       "", 3, dphiEdges);

  // for syst only calculation
  TH1D* hZ_BG_C_DPhi_Syst   = new TH1D("hZ_BG_C_DPhi_Syst",   "", 3, dphiEdges);
  TH1D* hZ_Data_C_DPhi_Syst = new TH1D("hZ_Data_C_DPhi_Syst", "", 3, dphiEdges);
  TH1D* hZ_Eff_S_DPhi_Syst  = new TH1D("hZ_Eff_S_DPhi_Syst",  "", 3, dphiEdges);
  TH1D* hZ_Est_C_DPhi_Syst  = new TH1D("hZ_Est_C_DPhi_Syst",  "", 3, dphiEdges);
  TH1D* hZ_Est_S_DPhi_Syst  = new TH1D("hZ_Est_S_DPhi_Syst",  "", 3, dphiEdges);

  // calculate efficiencies
  hZ_DY_EffMuMu->Add(hZ_DY_EffMuMu_N);
  hZ_DY_EffMuMu->Divide(hZ_DY_EffMuMu_D);
  double syst_EffMuMu = constants::syst_effMu*hZ_DY_EffMuMu->GetBinContent(1);      //2.5% Muon ID/Iso efficiency uncertainty from EWK-10-002
  hZ_DY_EffMuMu->SetBinError(1,TMath::Sqrt(hZ_DY_EffMuMu->GetBinError(1)*hZ_DY_EffMuMu->GetBinError(1) + syst_EffMuMu*syst_EffMuMu));
 
  hZ_DY_EffVBFS->Add(hZ_DY_EffVBFS_N);
  hZ_DY_EffVBFS->Divide(hZ_DY_EffVBFS_D);

  hZ_DY_EffVBFC->Add(hZ_DY_EffVBFC_N);
  hZ_DY_EffVBFC->Divide(hZ_DY_EffVBFC_D);

  hZ_DY_RatioVBF->Add(hZ_DY_EffVBFS);
  hZ_DY_RatioVBF->Divide(hZ_DY_EffVBFC);

  hZ_DY_TotalEff->Add(hZ_DY_RatioVBF);
  hZ_DY_TotalEff->Divide(hZ_DY_EffMuMu);

  for(int ibin = 1; ibin <= hZ_Eff_S_DPhi->GetNbinsX(); ++ibin) {
    hZ_Eff_S_DPhi->SetBinContent(ibin,hZ_DY_TotalEff->GetBinContent(1));
    hZ_Eff_S_DPhi->SetBinError(ibin,0.);
    hZ_Eff_S_DPhi_Syst->SetBinContent(ibin,hZ_DY_TotalEff->GetBinContent(1));
    hZ_Eff_S_DPhi_Syst->SetBinError(ibin,hZ_DY_TotalEff->GetBinError(1));
  }

  // do stat only version
  hZ_BG_C_DPhi_Syst->Add(hZ_BG_C_DPhi, 1.);  // copy MC BG histogram for syst calculation
  for (int i=1; i<=hZ_BG_C_DPhi->GetNbinsX(); ++i) hZ_BG_C_DPhi->SetBinError(i,0.);  // set MC BG errors to zero for stat only
  hZ_Est_C_DPhi->Add(hZ_Data_C_DPhi, hZ_BG_C_DPhi, 1., -1.);
  hZ_Est_S_DPhi->Add(hZ_Est_C_DPhi,ratioBF);
  hZ_Est_S_DPhi->Multiply(hZ_Eff_S_DPhi);

  // do syst only version
  hZ_Data_C_DPhi_Syst->Add(hZ_Data_C_DPhi, 1.);
  for (int i=1; i<=hZ_Data_C_DPhi_Syst->GetNbinsX(); ++i) hZ_Data_C_DPhi_Syst->SetBinError(i,0.);

  hZ_Est_C_DPhi_Syst->Add(hZ_Data_C_DPhi_Syst, hZ_BG_C_DPhi, 1., -1.);
  hZ_Est_S_DPhi_Syst->Add(hZ_Est_C_DPhi_Syst,ratioBF);
  hZ_Est_S_DPhi_Syst->Multiply(hZ_Eff_S_DPhi_Syst);


  // 2D calculation
//   TH2D* hZ_Est_C_METDPhi = new TH2D("hZ_Est_C_METDPhi", "", 3, dphiEdges, 12, metEdges); // estimated Z in ctrl region
//   TH2D* hZ_Est_S_METDPhi = new TH2D("hZ_Est_S_METDPhi", "", 3, dphiEdges, 12, metEdges); // estimated Z in bkgrnd region  
//   TH2D* hZ_Eff_S_METDPhi = new TH2D("hZ_Eff_S_METDPhi", "", 3, dphiEdges, 12, metEdges);

//   TH1D* hZ_DY_EffVBFS_MET0 = new TH1D("hZ_DY_EffVBFS_MET0", "", 12, metEdges);
//   TH1D* hZ_DY_EffVBFC_MET0 = new TH1D("hZ_DY_EffVBFC_MET0", "", 12, metEdges);
//   TH1D* hZ_DY_RatioVBF_MET0 = new TH1D("hZ_DY_RatioVBF_MET0", "", 12, metEdges);
//   TH1D* hZ_DY_TotalEff_MET0 = new TH1D("hZ_DY_TotalEff_MET0", "", 12, metEdges); 

//   hZ_DY_EffVBFS_MET0->Add(hZ_DY_EffVBFS_MET0_N);
//   hZ_DY_EffVBFS_MET0->Divide(hZ_DY_EffVBFS_MET0_D);

//   hZ_DY_EffVBFC_MET0->Add(hZ_DY_EffVBFC_MET0_N);
//   hZ_DY_EffVBFC_MET0->Divide(hZ_DY_EffVBFC_MET0_D);

//   hZ_DY_RatioVBF_MET0->Add(hZ_DY_EffVBFS_MET0);
//   hZ_DY_RatioVBF_MET0->Divide(hZ_DY_EffVBFC_MET0);

//   hZ_DY_TotalEff_MET0->Add(hZ_DY_RatioVBF_MET0);
//   hZ_DY_TotalEff_MET0->Scale(1/hZ_DY_EffMuMu->GetBinContent(1));

//   for(int ibin = 1; ibin <= hZ_Eff_S_METDPhi->GetNbinsX(); ++ibin) {
//     for(int jbin = 1; jbin <= hZ_Eff_S_METDPhi->GetNbinsY(); ++jbin) {
//       hZ_Eff_S_METDPhi->SetBinContent(ibin,jbin,hZ_DY_TotalEff_MET0->GetBinContent(jbin));
//       hZ_Eff_S_METDPhi->SetBinError  (ibin,jbin,hZ_DY_TotalEff_MET0->GetBinError(jbin));
//     }
//   }

//   hZ_Est_C_METDPhi->Add(hZ_Data_C_METDPhi, hZ_BG_C_METDPhi, 1., -1.);
//   //hZ_Est_S_METDPhi->Add(hZ_Est_C_METDPhi, f);
//   hZ_Est_S_METDPhi->Add(hZ_Est_C_METDPhi,ratioBF);
//   hZ_Est_S_METDPhi->Multiply(hZ_Eff_S_METDPhi);


  // print out

  std::cout << std::endl;
  std::cout << "##################################### MET > 130 #####################################" << std::endl;
  std::cout << "dphi<1.0" << std::endl;
  std::cout << std::endl;
  std::cout << "  eps_mumu  : " << hZ_DY_EffMuMu->GetBinContent(1) << " +/- " << hZ_DY_EffMuMu->GetBinError(1) << std::endl;
  std::cout << "  eps_s_vbf : " << hZ_DY_EffVBFS->GetBinContent(1) << " +/- " << hZ_DY_EffVBFS->GetBinError(1) << std::endl;
  std::cout << "  eps_c_vbf : " << hZ_DY_EffVBFC->GetBinContent(1) << " +/- " << hZ_DY_EffVBFC->GetBinError(1) << std::endl;
  std::cout << "  ratio_vbf : " << hZ_DY_RatioVBF->GetBinContent(1) << " +/- " << hZ_DY_RatioVBF->GetBinError(1) << std::endl;
  std::cout << "  total eff : " << hZ_DY_TotalEff->GetBinContent(1) << " +/- " << hZ_DY_TotalEff->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  DY+jets MC ctrl region : " << hZ_DY_C_DPhi->GetBinContent(1) << " +/- " << hZ_DY_C_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_C_DPhi->GetBinContent(1) << " +/- " << hZ_BG_C_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_C_DPhi->GetBinContent(1) << " +/- " << hZ_Data_C_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  Z in ctrl region       : " << hZ_Est_C_DPhi->GetBinContent(1) << " +/- " << hZ_Est_C_DPhi->GetBinError(1) << " (stat.) + " << hZ_Est_C_DPhi_Syst->GetBinError(1) << " (syst)" << std::endl;
  std::cout << "  Z in sgnl region       : " << hZ_Est_S_DPhi->GetBinContent(1) << " +/- " << hZ_Est_S_DPhi->GetBinError(1) << " (stat.) + " << hZ_Est_S_DPhi_Syst->GetBinError(1) << " (syst)" << std::endl;
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
  SumDatasets(oDir_Plot, DYDatasets, hists, "DYJets+EWK");

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
  plots.setLegPos(0.70,0.60,0.93,0.89);
  //plots.setLegPos(0.62,0.62,0.89,0.89);

  plots.addDataset("DiBoson", kViolet-6, 0);
  plots.addDataset("SingleT+TTbar", kAzure-2, 0);
  plots.addDataset("DYJets+EWK", kPink-4,0);
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

  //store histograms
  ofile->cd();
  hZ_DY_C_DPhi->Write("",TObject::kOverwrite);
  hZ_BG_C_DPhi->Write("",TObject::kOverwrite);
  hZ_Data_C_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_C_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_C_DPhi_Syst->Write("",TObject::kOverwrite);
  hZ_Est_S_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_S_DPhi_Syst->Write("",TObject::kOverwrite);

  hZ_Eff_S_DPhi->Write("",TObject::kOverwrite);
  hZ_Eff_S_DPhi_Syst->Write("",TObject::kOverwrite);
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

//   hZ_DY_C_METDPhi->Write("",TObject::kOverwrite);
//   hZ_BG_C_METDPhi->Write("",TObject::kOverwrite);
//   hZ_Data_C_METDPhi->Write("",TObject::kOverwrite);
//   hZ_Est_C_METDPhi->Write("",TObject::kOverwrite);
//   hZ_Est_S_METDPhi->Write("",TObject::kOverwrite);
//   hZ_Eff_S_METDPhi->Write("",TObject::kOverwrite);


  hZ_CutFlow_Data->Write("",TObject::kOverwrite);
  hZ_CutFlow_DY->Write("",TObject::kOverwrite);
  hZ_CutFlow_SingleTSum->Write("",TObject::kOverwrite);
  hZ_CutFlow_Diboson->Write("",TObject::kOverwrite);

  ofile->Close();    

}
