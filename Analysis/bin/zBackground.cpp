
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

int main(int argc, char* argv[]) {

  TH1::SetDefaultSumw2();

  ProgramOptions options(argc, argv);

  double lumi = options.lumi;

  std::cout << "Integrated luminosity : " << lumi << " pb-1" << std::endl;

  // input datasets
  Datasets datasets(options.iDir);
  datasets.readFile(options.datasetFile);

  // output file
  TFile* ofile = TFile::Open( (options.oDir+std::string("/ZBackground.root")).c_str(), "UPDATE");

  // cuts
  Cuts cuts;
  TCut puWeight("puWeight");
  TCut METNoMuon("metNoMuon>130.");	// add here later for VBF efficiency when MET>0, MET>70 (QCD estimation)
  TCut METNo2Muon("metNo2Muon>130.");

  TCut cutZMuMu_C = puWeight * cuts.zMuMuVBF();
  TCut cutZMuMuGenPt_C = puWeight * cuts.zMuMuGenPt100VBF(); 	// For inclusive sample add ZgenpT < 100
	
  TCut cutEfficiencyMuMu_D = puWeight * (cuts.HLTandMETFilters() + cuts.zMuMuGen());
  TCut cutEfficiencyMuMu_N = puWeight * (cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco());

  TCut cutEfficiencyVBFS_D = puWeight * (cuts.HLTandMETFilters() + cuts.zMuMuGenMass());
  TCut cutEfficiencyVBFS_N = puWeight * (cuts.HLTandMETFilters() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon);
  TCut cutEfficiencyVBFS_Pt100_D = puWeight * (cuts.HLTandMETFilters() + cuts.zMuMuGenPt100() + cuts.zMuMuGenMass());	// For inclusive sample add ZgenpT < 100
  TCut cutEfficiencyVBFS_Pt100_N = puWeight * (cuts.HLTandMETFilters() + cuts.zMuMuGenPt100() + cuts.zMuMuGenMass() + cuts.vbf() + METNoMuon);

  TCut cutEfficiencyVBFC_D = puWeight * (cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco());
  TCut cutEfficiencyVBFC_N = puWeight * (cuts.HLTandMETFilters() + cuts.zMuMuGen() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon);
  TCut cutEfficiencyVBFC_Pt100_D = puWeight * (cuts.HLTandMETFilters() + cuts.zMuMuGenPt100() + cuts.zMuMuReco());	// For inclusive sample add ZgenpT < 100
  TCut cutEfficiencyVBFC_Pt100_N = puWeight * (cuts.HLTandMETFilters() + cuts.zMuMuGenPt100() + cuts.zMuMuReco() + cuts.vbf() + METNo2Muon);

  // histograms
  double dphiEdges[4] = { 0., 1.0, 2.6, TMath::Pi() };

  TH1D* hZ_DY_C_DPhi = new TH1D("hZ_DY_C_DPhi", "", 3, dphiEdges);  // Z+jets MC ctrl region
  TH1D* hZ_BG_C_DPhi = new TH1D("hZ_BG_C_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hZ_Data_C_DPhi = new TH1D("hZ_Data_C_DPhi", "", 3, dphiEdges);  // Data ctrl region

  TH1D* hZ_DY_EffMuMu_D = new TH1D("hZ_DY_EffMuMu_D", "", 1, 0., 1.);	// denominator of MuMu efficiency from DY + DY_EWK samples
  TH1D* hZ_DY_EffMuMu_N = new TH1D("hZ_DY_EffMuMu_N", "", 1, 0., 1.);	// numerator of MuMu efficiency from DY + DY_EWK samples 
 
  TH1D* hZ_DY_EffVBFS_D = new TH1D("hZ_DY_EffVBFS_D", "", 1, 0., 1.);   // denominator of VBF(S) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
  TH1D* hZ_DY_EffVBFS_N = new TH1D("hZ_DY_EffVBFS_N", "", 1, 0., 1.);   // numerator of VBF(S) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 
 
  TH1D* hZ_DY_EffVBFC_D = new TH1D("hZ_DY_EffVBFC_D", "", 1, 0., 1.);   // denominator of VBF(C) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples
  TH1D* hZ_DY_EffVBFC_N = new TH1D("hZ_DY_EffVBFC_N", "", 1, 0., 1.);   // numerator of VBF(C) efficiency from DY(pT<100) + DY(pT>100) + DY_EWK samples 

  // loop over MC datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);
    
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

    // fill tmp histograms for BG estimation
    TH1D* hZ_C_DPhi = new TH1D("hZ_C_DPhi", "", 3, dphiEdges);  // this is for the actual BG estimation
    // fill tmp histograms for efficiency calculation
    TH1D* hZ_EffMuMu_D = new TH1D("hZ_EffMuMu_D", "", 1, 0., 1.);
    TH1D* hZ_EffMuMu_N = new TH1D("hZ_EffMuMu_N", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_D = new TH1D("hZ_EffVBFS_D", "", 1, 0., 1.);
    TH1D* hZ_EffVBFS_N = new TH1D("hZ_EffVBFS_N", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_D = new TH1D("hZ_EffVBFC_D", "", 1, 0., 1.);
    TH1D* hZ_EffVBFC_N = new TH1D("hZ_EffVBFC_N", "", 1, 0., 1.);

    if (isDY) {
    	if (dataset.name == "DYJetsToLL") {	
		tree->Draw("vbfDPhi>>hZ_C_DPhi", cutZMuMuGenPt_C);
		tree->Draw("0.5>>hZ_EffMuMu_D", cutEfficiencyMuMu_D);
                tree->Draw("0.5>>hZ_EffMuMu_N", cutEfficiencyMuMu_N);
                tree->Draw("0.5>>hZ_EffVBFS_D", cutEfficiencyVBFS_Pt100_D);
                tree->Draw("0.5>>hZ_EffVBFS_N", cutEfficiencyVBFS_Pt100_N);
                tree->Draw("0.5>>hZ_EffVBFC_D", cutEfficiencyVBFC_Pt100_D);
                tree->Draw("0.5>>hZ_EffVBFC_N", cutEfficiencyVBFC_Pt100_N);
	}
	if (dataset.name == "DYJetsToLL_EWK") {
		tree->Draw("vbfDPhi>>hZ_C_DPhi", cutZMuMu_C);
		tree->Draw("0.5>>hZ_EffMuMu_D", cutEfficiencyMuMu_D);
                tree->Draw("0.5>>hZ_EffMuMu_N", cutEfficiencyMuMu_N);
                tree->Draw("0.5>>hZ_EffVBFS_D", cutEfficiencyVBFS_D);
                tree->Draw("0.5>>hZ_EffVBFS_N", cutEfficiencyVBFS_N);
                tree->Draw("0.5>>hZ_EffVBFC_D", cutEfficiencyVBFC_D);
                tree->Draw("0.5>>hZ_EffVBFC_N", cutEfficiencyVBFC_N);
	}
	if (dataset.name == "DYJetsToLL_PtZ-100") {
		tree->Draw("vbfDPhi>>hZ_C_DPhi", cutZMuMu_C);
                tree->Draw("0.5>>hZ_EffVBFS_D", cutEfficiencyVBFS_D);
                tree->Draw("0.5>>hZ_EffVBFS_N", cutEfficiencyVBFS_N);
                tree->Draw("0.5>>hZ_EffVBFC_D", cutEfficiencyVBFC_D);
                tree->Draw("0.5>>hZ_EffVBFC_N", cutEfficiencyVBFC_N);
	}
    }
    else	tree->Draw("vbfDPhi>>hZ_C_DPhi", cutZMuMu_C);

    // weight  to lumi
    double weight = (dataset.isData ? 1. : lumi * dataset.sigma / dataset.nEvents);

    // add to output histograms with lumi weight
    if (dataset.isData) {
      hZ_Data_C_DPhi->Add(hZ_C_DPhi);
    }
    else if (isDY) {
      hZ_DY_C_DPhi->Add(hZ_C_DPhi, weight);
      hZ_DY_EffVBFS_D->Add(hZ_EffVBFS_D, weight);
      hZ_DY_EffVBFS_N->Add(hZ_EffVBFS_N, weight);
      hZ_DY_EffVBFC_D->Add(hZ_EffVBFC_D, weight);
      hZ_DY_EffVBFC_N->Add(hZ_EffVBFC_N, weight);
      if (dataset.name != "DYJetsToLL_PtZ-100") {
	hZ_DY_EffMuMu_D->Add(hZ_EffMuMu_D, weight);
	hZ_DY_EffMuMu_N->Add(hZ_EffMuMu_N, weight);
      }
    }
    else {
      hZ_BG_C_DPhi->Add(hZ_C_DPhi, weight);
    }

    std::cout << "  N ctrl (dphi<1.0) : " << weight * hZ_C_DPhi->GetBinContent(1) << std::endl;	
    std::cout << "  N ctrl (dphi>2.6) : " << weight * hZ_C_DPhi->GetBinContent(3) << std::endl;
    
    delete hZ_C_DPhi;
    delete hZ_EffMuMu_D;
    delete hZ_EffMuMu_N;
    delete hZ_EffVBFS_D;
    delete hZ_EffVBFS_N;
    delete hZ_EffVBFC_D;
    delete hZ_EffVBFC_N;

    // per-dataset control plots (just an example, add more later)
    ofile->cd();

    std::string hname = std::string("hZ_mZ_")+dataset.name;
    TH1D* hZ_mZ = new TH1D(hname.c_str(), "", 30, 60., 120.);
    std::string str = std::string("zMass>>")+hname;
    tree->Draw(str.c_str(), cutZMuMu_C);
    hZ_mZ->Scale(weight);
    hZ_mZ->Write("",TObject::kOverwrite);

    // clean up
    delete tree;
    file->Close();
   
  }

  // numbers - calculate these from MC in this program later!
  double ratioBF = 5.626;  //  MCFM + NLO
  double eps_mumu = 0.290;
  double eps_s_vbf = 0.0194;
  double eps_c_vbf = 0.0315;

  double f = (ratioBF * eps_s_vbf) / (eps_mumu * eps_c_vbf);

  std::cout << "Efficiencies" << std::endl;
  std::cout << "  eps_mumu   : " << eps_mumu << std::endl;
  std::cout << "  eps_s_vbf   : " << eps_s_vbf << std::endl;
  std::cout << "  eps_c_vbf   : " << eps_c_vbf << std::endl;
  std::cout << "  ratio       : " << eps_s_vbf/eps_c_vbf << std::endl;
  std::cout << std::endl <<std::endl;

  TH1D* hZ_Est_C_DPhi = new TH1D("hZ_Est_C_DPhi", "", 3, dphiEdges); // estimated Z in ctrl region
  TH1D* hZ_Est_S_DPhi = new TH1D("hZ_Est_S_DPhi", "", 3, dphiEdges); // estimated Z in bkgrnd region
  
  TH1D* hZ_DY_EffMuMu = new TH1D("hZ_DY_EffMuMu", "", 1, 0., 1.);     	// epsilon mumu
  TH1D* hZ_DY_EffVBFS = new TH1D("hZ_DY_EffVBFS", "", 1, 0., 1.);  	// epsilon_s_vbf
  TH1D* hZ_DY_EffVBFC = new TH1D("hZ_DY_EffVBFC", "", 1, 0., 1.);       // epsilon_c_vbf

  hZ_DY_EffMuMu->Add(hZ_DY_EffMuMu_N);
  hZ_DY_EffMuMu->Divide(hZ_DY_EffMuMu_D);

  hZ_DY_EffVBFS->Add(hZ_DY_EffVBFS_N);
  hZ_DY_EffVBFS->Divide(hZ_DY_EffVBFS_D);

  hZ_DY_EffVBFC->Add(hZ_DY_EffVBFC_N);
  hZ_DY_EffVBFC->Divide(hZ_DY_EffVBFC_D);

  std::cout << std::endl;
  std::cout << "  eps_mumu by histogram  : " << hZ_DY_EffMuMu->GetBinContent(1) << " +/- " << hZ_DY_EffMuMu->GetBinError(1) << std::endl;
  std::cout << "  eps_s_vbf by histogram  : " << hZ_DY_EffVBFS->GetBinContent(1) << " +/- " << hZ_DY_EffVBFS->GetBinError(1) << std::endl;
  std::cout << "  eps_c_vbf by histogram  : " << hZ_DY_EffVBFC->GetBinContent(1) << " +/- " << hZ_DY_EffVBFC->GetBinError(1) << std::endl;
  std::cout << std::endl;

  hZ_Est_C_DPhi->Add(hZ_Data_C_DPhi, hZ_BG_C_DPhi, 1., -1.);
  hZ_Est_S_DPhi->Add(hZ_Est_C_DPhi, f);

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


  //store histograms
  ofile->cd();
  hZ_DY_C_DPhi->Write("",TObject::kOverwrite);
  hZ_BG_C_DPhi->Write("",TObject::kOverwrite);
  hZ_Data_C_DPhi->Write("",TObject::kOverwrite);
//   hZ_R_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_C_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_S_DPhi->Write("",TObject::kOverwrite);
  hZ_DY_EffMuMu_D->Write("",TObject::kOverwrite);
  hZ_DY_EffMuMu_N->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFS_D->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFS_N->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFC_D->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFC_N->Write("",TObject::kOverwrite);
  hZ_DY_EffMuMu->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFS->Write("",TObject::kOverwrite);
  hZ_DY_EffVBFC->Write("",TObject::kOverwrite);
 
  ofile->Close();    

}
