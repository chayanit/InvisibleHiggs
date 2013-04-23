
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

  // output file
  TFile* ofile = TFile::Open( (options.oDir+std::string("/ZBackground.root")).c_str(), "RECREATE");

  // cuts
  Cuts cuts;
  unsigned nCutsZMuMu = cuts.nCutsZMuMu();

  TCut puWeight("puWeight");
  TCut METNoMuon130("metNoMuon>130.");	// add here later for VBF efficiency when MET>0, MET>70 (QCD estimation)
  TCut METNoMuon70("metNoMuon>70.");	
  TCut METNoMuon35("metNoMuon>35.");	
  TCut METNo2Muon130("metNo2Muon>130.");
  TCut METNo2Muon70("metNo2Muon>70.");
  TCut METNo2Muon35("metNo2Muon>35.");

  // histograms
  double dphiEdges[4] = { 0., 1.0, 2.6, TMath::Pi() };

  // signal MET>130
  TH1D* hZ_DY_C_DPhi = new TH1D("hZ_DY_C_DPhi", "", 3, dphiEdges);  // Z+jets MC ctrl region
  TH1D* hZ_BG_C_DPhi = new TH1D("hZ_BG_C_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hZ_Data_C_DPhi = new TH1D("hZ_Data_C_DPhi", "", 3, dphiEdges);  // Data ctrl region

  TH1D* hZ_DY_EffMuMu_D = new TH1D("hZ_DY_EffMuMu_D", "", 1, 0., 1.);	// denominator of MuMu efficiency from DY + DY_EWK samples
  TH1D* hZ_DY_EffMuMu_N = new TH1D("hZ_DY_EffMuMu_N", "", 1, 0., 1.);	// numerator of MuMu efficiency from DY + DY_EWK samples 
 
  TH1D* hZ_DY_EffVBFS_D = new TH1D("hZ_DY_EffVBFS_D", "", 1, 0., 1.);   // denominator of VBF(S) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
  TH1D* hZ_DY_EffVBFS_N = new TH1D("hZ_DY_EffVBFS_N", "", 1, 0., 1.);   // numerator of VBF(S) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 
 
  TH1D* hZ_DY_EffVBFC_D = new TH1D("hZ_DY_EffVBFC_D", "", 1, 0., 1.);   // denominator of VBF(C) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
  TH1D* hZ_DY_EffVBFC_N = new TH1D("hZ_DY_EffVBFC_N", "", 1, 0., 1.);   // numerator of VBF(C) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 

  // MET > 70
  TH1D* hZ_DY_LooseC_DPhi = new TH1D("hZ_DY_C_DPhi", "", 3, dphiEdges);  // Z+jets MC ctrl region
  TH1D* hZ_BG_LooseC_DPhi = new TH1D("hZ_BG_C_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hZ_Data_LooseC_DPhi = new TH1D("hZ_Data_C_DPhi", "", 3, dphiEdges);  // Data ctrl region

  TH1D* hZ_DY_Loose_EffVBFS_N = new TH1D("hZ_DY_EffVBFS_N", "", 1, 0., 1.);   // numerator of VBF(S) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples  
  TH1D* hZ_DY_Loose_EffVBFC_N = new TH1D("hZ_DY_EffVBFC_N", "", 1, 0., 1.);   // numerator of VBF(C) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 

  // MET > 35
  TH1D* hZ_DY_Loose2C_DPhi = new TH1D("hZ_DY_C_DPhi", "", 3, dphiEdges);  // Z+jets MC ctrl region
  TH1D* hZ_BG_Loose2C_DPhi = new TH1D("hZ_BG_C_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hZ_Data_Loose2C_DPhi = new TH1D("hZ_Data_C_DPhi", "", 3, dphiEdges);  // Data ctrl region

  TH1D* hZ_DY_Loose2_EffVBFS_N = new TH1D("hZ_DY_EffVBFS_N", "", 1, 0., 1.);   // numerator of VBF(S) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 
   TH1D* hZ_DY_Loose2_EffVBFC_N = new TH1D("hZ_DY_EffVBFC_N", "", 1, 0., 1.);   // numerator of VBF(C) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 

  // MET > 0
  TH1D* hZ_DY_NoMETC_DPhi = new TH1D("hZ_DY_C_DPhi", "", 3, dphiEdges);  // Z+jets MC ctrl region
  TH1D* hZ_BG_NoMETC_DPhi = new TH1D("hZ_BG_C_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hZ_Data_NoMETC_DPhi = new TH1D("hZ_Data_C_DPhi", "", 3, dphiEdges);  // Data ctrl region

  TH1D* hZ_DY_NoMET_EffVBFS_N = new TH1D("hZ_DY_EffVBFS_N", "", 1, 0., 1.);   // numerator of VBF(S) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 
   TH1D* hZ_DY_NoMET_EffVBFC_N = new TH1D("hZ_DY_EffVBFC_N", "", 1, 0., 1.);   // numerator of VBF(C) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 

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
    TCut cutZMuMu_C0   = puWeight * (cutD + cuts.zMuMuVBF());
    TCut cutZMuMu_C35  = puWeight * (cutD + cuts.zMuMuVBF() + METNo2Muon35);
    TCut cutZMuMu_C70  = puWeight * (cutD + cuts.zMuMuVBF() + METNo2Muon70);
    TCut cutZMuMu_C130 = puWeight * (cutD + cuts.zMuMuVBF() + METNo2Muon130);
    
    TCut cutEfficiencyMuMu_D    = puWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen());
    TCut cutEfficiencyMuMu_N    = puWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco());
    
    TCut cutEfficiencyVBFS_D    = puWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass());
    TCut cutEfficiencyVBFS_N0   = puWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf());
    TCut cutEfficiencyVBFS_N35  = puWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon130);
    TCut cutEfficiencyVBFS_N70  = puWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon130);
    TCut cutEfficiencyVBFS_N130 = puWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon130);
    
    TCut cutEfficiencyVBFC_D    = puWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco());
    TCut cutEfficiencyVBFC_N0   = puWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf());
    TCut cutEfficiencyVBFC_N35  = puWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon35);
    TCut cutEfficiencyVBFC_N70  = puWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon70);
    TCut cutEfficiencyVBFC_N130 = puWeight * (cutD + cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon130);
    

    // fill tmp histograms for BG estimation
    TH1D* hZ_C0_DPhi   = new TH1D("hZ_C0_DPhi", "", 3, dphiEdges);  // this is for the actual BG estimation
    TH1D* hZ_C35_DPhi  = new TH1D("hZ_C35_DPhi", "", 3, dphiEdges);  // this is for the actual BG estimation
    TH1D* hZ_C70_DPhi  = new TH1D("hZ_C70_DPhi", "", 3, dphiEdges);  // this is for the actual BG estimation
    TH1D* hZ_C130_DPhi = new TH1D("hZ_C130_DPhi", "", 3, dphiEdges);  // this is for the actual BG estimation
    // fill tmp histograms for efficiency calculation
    TH1D* hZ_EffMuMu_D = new TH1D("hZ_EffMuMu_D", "", 1, 0., 1.);
    TH1D* hZ_EffMuMu_N = new TH1D("hZ_EffMuMu_N", "", 1, 0., 1.);

    TH1D* hZ_EffVBFS_D    = new TH1D("hZ_EffVBFS_D", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_N0   = new TH1D("hZ_EffVBFS_N0", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_N35  = new TH1D("hZ_EffVBFS_N35", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_N70  = new TH1D("hZ_EffVBFS_N70", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_N130 = new TH1D("hZ_EffVBFS_N130", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_D    = new TH1D("hZ_EffVBFC_D", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_N0   = new TH1D("hZ_EffVBFC_N0", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_N35  = new TH1D("hZ_EffVBFC_N35", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_N70  = new TH1D("hZ_EffVBFC_N70", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_N130 = new TH1D("hZ_EffVBFC_N130", "", 1, 0., 1.);

    if (isDY) {
      tree->Draw("vbfDPhi>>hZ_C0_DPhi",   cutZMuMu_C0);
      tree->Draw("vbfDPhi>>hZ_C35_DPhi",  cutZMuMu_C35);
      tree->Draw("vbfDPhi>>hZ_C70_DPhi",  cutZMuMu_C70);
      tree->Draw("vbfDPhi>>hZ_C130_DPhi", cutZMuMu_C130);
      tree->Draw("0.5>>hZ_EffMuMu_D",     cutEfficiencyMuMu_D);		
      tree->Draw("0.5>>hZ_EffMuMu_N",     cutEfficiencyMuMu_N);		
      tree->Draw("0.5>>hZ_EffVBFS_D",     cutEfficiencyVBFS_D);
      tree->Draw("0.5>>hZ_EffVBFS_N0",    cutEfficiencyVBFS_N0);
      tree->Draw("0.5>>hZ_EffVBFS_N35",   cutEfficiencyVBFS_N35);
      tree->Draw("0.5>>hZ_EffVBFS_N70",   cutEfficiencyVBFS_N70);
      tree->Draw("0.5>>hZ_EffVBFS_N130",  cutEfficiencyVBFS_N130);
      tree->Draw("0.5>>hZ_EffVBFC_D",     cutEfficiencyVBFC_D);
      tree->Draw("0.5>>hZ_EffVBFC_N0",    cutEfficiencyVBFC_N0);
      tree->Draw("0.5>>hZ_EffVBFC_N35",   cutEfficiencyVBFC_N35);
      tree->Draw("0.5>>hZ_EffVBFC_N70",   cutEfficiencyVBFC_N70);
      tree->Draw("0.5>>hZ_EffVBFC_N130",  cutEfficiencyVBFC_N130);
    }
    else {
      tree->Draw("vbfDPhi>>hZ_C0_DPhi",   cutZMuMu_C0);
      tree->Draw("vbfDPhi>>hZ_C35_DPhi",  cutZMuMu_C35);
      tree->Draw("vbfDPhi>>hZ_C70_DPhi",  cutZMuMu_C70);
      tree->Draw("vbfDPhi>>hZ_C130_DPhi", cutZMuMu_C130);
    }

    // weight  to lumi
    double weight = (dataset.isData ? 1. : lumi * dataset.sigma / dataset.nEvents);
    hZ_C0_DPhi->Scale(weight);
    hZ_C35_DPhi->Scale(weight);
    hZ_C70_DPhi->Scale(weight);
    hZ_C130_DPhi->Scale(weight);
    hZ_EffVBFS_D->Scale(weight);
    hZ_EffVBFS_N0->Scale(weight);
    hZ_EffVBFS_N35->Scale(weight);
    hZ_EffVBFS_N70->Scale(weight);
    hZ_EffVBFS_N130->Scale(weight);
    hZ_EffVBFC_D->Scale(weight);
    hZ_EffVBFC_N0->Scale(weight);
    hZ_EffVBFC_N35->Scale(weight);
    hZ_EffVBFC_N70->Scale(weight);
    hZ_EffVBFC_N130->Scale(weight);
    hZ_EffMuMu_D->Scale(weight);
    hZ_EffMuMu_N->Scale(weight);

    // add to output histograms
    if (dataset.isData) {
      hZ_Data_NoMETC_DPhi->Add(hZ_C0_DPhi);
      hZ_Data_Loose2C_DPhi->Add(hZ_C35_DPhi);
      hZ_Data_LooseC_DPhi->Add(hZ_C70_DPhi);
      hZ_Data_C_DPhi->Add(hZ_C130_DPhi);
    }
    else if (isDY) {
      hZ_DY_NoMETC_DPhi->Add(hZ_C0_DPhi);
      hZ_DY_Loose2C_DPhi->Add(hZ_C35_DPhi);
      hZ_DY_LooseC_DPhi->Add(hZ_C70_DPhi);
      hZ_DY_C_DPhi->Add(hZ_C130_DPhi);
      hZ_DY_EffVBFS_D->Add(hZ_EffVBFS_D);
      hZ_DY_NoMET_EffVBFS_N->Add(hZ_EffVBFS_N0);
      hZ_DY_Loose2_EffVBFS_N->Add(hZ_EffVBFS_N35);
      hZ_DY_Loose_EffVBFS_N->Add(hZ_EffVBFS_N70);
      hZ_DY_EffVBFS_N->Add(hZ_EffVBFS_N130);
      hZ_DY_EffVBFC_D->Add(hZ_EffVBFC_D);
      hZ_DY_NoMET_EffVBFC_N->Add(hZ_EffVBFC_N0);
      hZ_DY_Loose2_EffVBFC_N->Add(hZ_EffVBFC_N35);
      hZ_DY_Loose_EffVBFC_N->Add(hZ_EffVBFC_N70);
      hZ_DY_EffVBFC_N->Add(hZ_EffVBFC_N130);
      if (dataset.name != "DYJetsToLL_PtZ-100") {
      hZ_DY_EffMuMu_D->Add(hZ_EffMuMu_D);
      hZ_DY_EffMuMu_N->Add(hZ_EffMuMu_N);
      }
    }
    else {
      hZ_BG_NoMETC_DPhi->Add(hZ_C0_DPhi);
      hZ_BG_Loose2C_DPhi->Add(hZ_C35_DPhi);
      hZ_BG_LooseC_DPhi->Add(hZ_C70_DPhi);
      hZ_BG_C_DPhi->Add(hZ_C130_DPhi);
    }

    std::cout << "  N ctrl (dphi<1.0) : " << hZ_C130_DPhi->GetBinContent(1) << " +/- " << hZ_C130_DPhi->GetBinError(1) << std::endl;	
    std::cout << "  N ctrl (dphi>2.6) : " << hZ_C130_DPhi->GetBinContent(3) << " +/- " << hZ_C130_DPhi->GetBinError(3) << std::endl;
    
    delete hZ_C0_DPhi;
    delete hZ_C35_DPhi;
    delete hZ_C70_DPhi;
    delete hZ_C130_DPhi;
    delete hZ_EffMuMu_D;
    delete hZ_EffMuMu_N;
    delete hZ_EffVBFS_D;
    delete hZ_EffVBFS_N0;
    delete hZ_EffVBFS_N35;
    delete hZ_EffVBFS_N70;
    delete hZ_EffVBFS_N130;
    delete hZ_EffVBFC_D;
    delete hZ_EffVBFC_N0;
    delete hZ_EffVBFC_N35;
    delete hZ_EffVBFC_N70;
    delete hZ_EffVBFC_N130;

    ofile->cd();

    // cut flow histograms
    std::string hname = std::string("hZ_CutFlow_")+dataset.name;
    TH1D* hZ_CutFlow = new TH1D(hname.c_str(), "", nCutsZMuMu, 0., nCutsZMuMu);

    for (unsigned c=0; c<nCutsZMuMu; ++c) {

      TCut cut = puWeight * (cutD + cuts.cutflowZMuMu(c));
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

    // control plots
    TCut cutPlots = puWeight * cuts.zMuMuVBF();

    hname = std::string("hZ_mZ_")+dataset.name;
    TH1D* hZ_mZ = new TH1D(hname.c_str(), "", 30, 60., 120.);
    std::string str = std::string("zMass>>")+hname;
    tree->Draw(str.c_str(), cutPlots);
    hZ_mZ->Scale(weight);
    hZ_mZ->Write("",TObject::kOverwrite);


    // clean up
    delete tree;
    file->Close();
   
  }

  // numbers - calculate these from MC in this program later!
  double ratioBF = 5.626;  //  MCFM + NLO
  //double eps_mumu = 0.290;
  //double eps_s_vbf = 0.0194;
  //double eps_c_vbf = 0.0315;
  double eps_vbf_syst = 0.008; 		//JES+MET uncertainty

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
  hZ_DY_RatioVBF->SetBinError(1,TMath::Sqrt(hZ_DY_RatioVBF->GetBinError(1)*hZ_DY_RatioVBF->GetBinError(1) + eps_vbf_syst*eps_vbf_syst));

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


  // MET>0
  TH1D* hZ_Est_NoMETC_DPhi = new TH1D("hZ_Est_NoMETC_DPhi", "", 3, dphiEdges); // estimated Z in ctrl region
  TH1D* hZ_Est_NoMETS_DPhi = new TH1D("hZ_Est_NoMETS_DPhi", "", 3, dphiEdges); // estimated Z in bkgrnd region  
  TH1D* hZ_Eff_NoMETS_DPhi = new TH1D("hZ_Eff_NoMETS_DPhi", "", 3, dphiEdges);

  TH1D* hZ_DY_NoMET_EffMuMu = new TH1D("hZ_DY_NoMET_EffMuMu", "", 1, 0., 1.);     	// epsilon mumu
  TH1D* hZ_DY_NoMET_EffVBFS = new TH1D("hZ_DY_NoMET_EffVBFS", "", 1, 0., 1.);  	// epsilon_s_vbf
  TH1D* hZ_DY_NoMET_EffVBFC = new TH1D("hZ_DY_NoMET_EffVBFC", "", 1, 0., 1.);       // epsilon_c_vbf
  TH1D* hZ_DY_NoMET_RatioVBF = new TH1D("hZ_DY_NoMET_RatioVBF", "", 1, 0., 1.);	// epsilon_s_vbf/epsilon_c_vbf
  TH1D* hZ_DY_NoMET_TotalEff = new TH1D("hZ_DY_NoMET_TotalEff", "", 1, 0., 1.); 

  hZ_DY_NoMET_EffVBFS->Add(hZ_DY_NoMET_EffVBFS_N);
  hZ_DY_NoMET_EffVBFS->Divide(hZ_DY_EffVBFS_D);

  hZ_DY_NoMET_EffVBFC->Add(hZ_DY_NoMET_EffVBFC_N);
  hZ_DY_NoMET_EffVBFC->Divide(hZ_DY_EffVBFC_D);

  hZ_DY_NoMET_RatioVBF->Add(hZ_DY_NoMET_EffVBFS);
  hZ_DY_NoMET_RatioVBF->Divide(hZ_DY_NoMET_EffVBFC);
  hZ_DY_NoMET_RatioVBF->SetBinError(1,TMath::Sqrt(hZ_DY_NoMET_RatioVBF->GetBinError(1)*hZ_DY_NoMET_RatioVBF->GetBinError(1) + eps_vbf_syst*eps_vbf_syst));

  hZ_DY_NoMET_TotalEff->Add(hZ_DY_NoMET_RatioVBF);
  hZ_DY_NoMET_TotalEff->Divide(hZ_DY_EffMuMu);

  for(int ibin = 1; ibin <= hZ_Eff_NoMETS_DPhi->GetNbinsX(); ++ibin) {
    hZ_Eff_NoMETS_DPhi->SetBinContent(ibin,hZ_DY_NoMET_TotalEff->GetBinContent(1));
    hZ_Eff_NoMETS_DPhi->SetBinError  (ibin,hZ_DY_NoMET_TotalEff->GetBinError(1));
  }

  hZ_Est_NoMETC_DPhi->Add(hZ_Data_NoMETC_DPhi, hZ_BG_NoMETC_DPhi, 1., -1.);
  hZ_Est_NoMETS_DPhi->Add(hZ_Est_NoMETC_DPhi,ratioBF);
  hZ_Est_NoMETS_DPhi->Multiply(hZ_Eff_NoMETS_DPhi);


  // MET>35
  TH1D* hZ_Est_Loose2C_DPhi = new TH1D("hZ_Est_Loose2C_DPhi", "", 3, dphiEdges); // estimated Z in ctrl region
  TH1D* hZ_Est_Loose2S_DPhi = new TH1D("hZ_Est_Loose2S_DPhi", "", 3, dphiEdges); // estimated Z in bkgrnd region  
  TH1D* hZ_Eff_Loose2S_DPhi = new TH1D("hZ_Eff_Loose2S_DPhi", "", 3, dphiEdges);

  TH1D* hZ_DY_Loose2_EffMuMu = new TH1D("hZ_DY_Loose2_EffMuMu", "", 1, 0., 1.);     	// epsilon mumu
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
  hZ_DY_Loose2_RatioVBF->SetBinError(1,TMath::Sqrt(hZ_DY_Loose2_RatioVBF->GetBinError(1)*hZ_DY_Loose2_RatioVBF->GetBinError(1) + eps_vbf_syst*eps_vbf_syst));

  hZ_DY_Loose2_TotalEff->Add(hZ_DY_Loose2_RatioVBF);
  hZ_DY_Loose2_TotalEff->Divide(hZ_DY_EffMuMu);

  for(int ibin = 1; ibin <= hZ_Eff_Loose2S_DPhi->GetNbinsX(); ++ibin) {
    hZ_Eff_Loose2S_DPhi->SetBinContent(ibin,hZ_DY_Loose2_TotalEff->GetBinContent(1));
    hZ_Eff_Loose2S_DPhi->SetBinError  (ibin,hZ_DY_Loose2_TotalEff->GetBinError(1));
  }

  hZ_Est_Loose2C_DPhi->Add(hZ_Data_Loose2C_DPhi, hZ_BG_Loose2C_DPhi, 1., -1.);
  hZ_Est_Loose2S_DPhi->Add(hZ_Est_Loose2C_DPhi,ratioBF);
  hZ_Est_Loose2S_DPhi->Multiply(hZ_Eff_Loose2S_DPhi);

  // MET>70
  TH1D* hZ_Est_LooseC_DPhi = new TH1D("hZ_Est_LooseC_DPhi", "", 3, dphiEdges); // estimated Z in ctrl region
  TH1D* hZ_Est_LooseS_DPhi = new TH1D("hZ_Est_LooseS_DPhi", "", 3, dphiEdges); // estimated Z in bkgrnd region  
  TH1D* hZ_Eff_LooseS_DPhi = new TH1D("hZ_Eff_LooseS_DPhi", "", 3, dphiEdges);

  TH1D* hZ_DY_Loose_EffMuMu = new TH1D("hZ_DY_Loose_EffMuMu", "", 1, 0., 1.);     	// epsilon mumu
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
  hZ_DY_Loose_RatioVBF->SetBinError(1,TMath::Sqrt(hZ_DY_Loose_RatioVBF->GetBinError(1)*hZ_DY_Loose_RatioVBF->GetBinError(1) + eps_vbf_syst*eps_vbf_syst));

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
  std::cout << "  eps_mumu by histogram  : " << hZ_DY_EffMuMu->GetBinContent(1) << " +/- " << hZ_DY_EffMuMu->GetBinError(1) << std::endl;
  std::cout << "  eps_s_vbf by histogram  : " << hZ_DY_EffVBFS->GetBinContent(1) << " +/- " << hZ_DY_EffVBFS->GetBinError(1) << std::endl;
  std::cout << "  eps_c_vbf by histogram  : " << hZ_DY_EffVBFC->GetBinContent(1) << " +/- " << hZ_DY_EffVBFC->GetBinError(1) << std::endl;
  std::cout << "  ratio_vbf by histogram : " << hZ_DY_RatioVBF->GetBinContent(1) << " +/- " << hZ_DY_RatioVBF->GetBinError(1) << std::endl;
  std::cout << "  total eff by histogram : " << hZ_DY_TotalEff->GetBinContent(1) << " +/- " << hZ_DY_TotalEff->GetBinError(1) << std::endl;
  std::cout << std::endl;

  std::cout << std::endl;
  std::cout << "dphi>2.6" << std::endl;
  std::cout << "  DY+jets MC ctrl region : " << hZ_DY_C_DPhi->GetBinContent(3) << " +/- " << hZ_DY_C_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_C_DPhi->GetBinContent(3) << " +/- " << hZ_BG_C_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_C_DPhi->GetBinContent(3) << " +/- " << hZ_Data_C_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  Z in ctrl region       : " << hZ_Est_C_DPhi->GetBinContent(3) << " +/- " << hZ_Est_C_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Z in sgnl region       : " << hZ_Est_S_DPhi->GetBinContent(3) << " +/- " << hZ_Est_S_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl << std::endl;
  std::cout << "dphi<1.0" << std::endl;
  std::cout << "  DY+jets MC ctrl region : " << hZ_DY_C_DPhi->GetBinContent(1) << " +/- " << hZ_DY_C_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_C_DPhi->GetBinContent(1) << " +/- " << hZ_BG_C_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_C_DPhi->GetBinContent(1) << " +/- " << hZ_Data_C_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  Z in ctrl region       : " << hZ_Est_C_DPhi->GetBinContent(1) << " +/- " << hZ_Est_C_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Z in sgnl region       : " << hZ_Est_S_DPhi->GetBinContent(1) << " +/- " << hZ_Est_S_DPhi->GetBinError(1) << std::endl;
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
