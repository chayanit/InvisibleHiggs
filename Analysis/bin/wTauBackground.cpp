
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

  std::cout << "Integrated luminosity : " << lumi << " pb-1" << std::endl;

  Datasets datasets(options.iDir);
  datasets.readFile(options.datasetFile);

  // output file
  TFile* ofile = TFile::Open( (options.oDir+std::string("/WTauBackground.root")).c_str(), "RECREATE");

  // For control plots
  std::string oDirPlots = options.oDir+std::string("/WTauControlPlots");
  boost::filesystem::path opath(oDirPlots);
  if (!exists(opath)) {
    std::cout << "Creating output directory : " << oDirPlots << std::endl;
    boost::filesystem::create_directory(opath);
  }
  else std::cout << "Writing plots results to " << oDirPlots << std::endl;
  std::vector<std::string> hnames; // to hold hist names for control plots

  if (options.doMCFMWeights) std::cout << "Going to apply MCFM weights" << std::endl;

  /////////////////////////
  // Define general cuts //
  /////////////////////////
  Cuts cuts;
  unsigned nCutsWTau = cuts.nCutsWTau();

  TCut puWeight("puWeight");
  TCut trigCorrWeight( "(trigCorrWeight>0) ? trigCorrWeight : 1." );

  // Cuts for control plots
  // Get puWeight etc added below if necessary
  // standard TightMjj selection - essentially signal selection but no DPhiJJ and no cjv
  TCut cutTightMjj_basic = cuts.cutWTau("trigger") + cuts.cutWTau("lVeto") + cuts.cutWTau("dijet") + cuts.cutWTau("dEtaJJ") + cuts.cutWTau("MET") + cuts.cutWTau("Mjj"); 
  TCut cutTightMjj(""); // used to add in PU, trig corr, wWeight etc

  TCut cutDPhiSignalNoCJV_basic = cutTightMjj_basic + cuts.cutWTau("dPhiJJ"); //standard DPhiSignalNoCJV selection
  TCut cutDPhiSignalNoCJV(""); // used to add in PU, trig corr, wWeight etc
  
  ////////////////////////////////////
  // Define histograms for counting //
  ////////////////////////////////////

  double dphiEdges[4] = { 0., 1.0, 2.6, TMath::Pi() };
  
  // For N_data and N_BG
  TH1D* hWTau_BGC_DPhi       = new TH1D("hWTau_BGC_DPhi",   "", 3, dphiEdges);  // background MC ctrl region (ctrl region = require tau reconstructed)
  TH1D* hWTau_DataC_DPhi     = new TH1D("hWTau_DataC_DPhi", "", 3, dphiEdges);  // Data ctrl region

  TH1D* hWTau_MCEl_DPhi      = new TH1D("hWTau_MCEl_DPhi",  "", 3, dphiEdges);  // Measuring Wenu contamination

  // Hists for eff_tauID
  TH1D* hWTau_MCC_NoCJV_DPhi = new TH1D("hWTau_MCC_NoCJV_DPhi", "", 3, dphiEdges);  // W+jets MC at gen level in ctrl region, no CJV
  TH1D* hWTau_MCS_NoCJV_DPhi = new TH1D("hWTau_MCS_NoCJV_DPhi", "", 3, dphiEdges);  // W+jets MC at gen level in signal region, no CJV
  
  // Hists for eff_CJV
  TH1D* hWTau_MC_CJV_DPhi    = new TH1D("hWTau_MC_CJV_DPhi",   "", 3, dphiEdges); // W+jets MC at gen level with CJV - also provides MC signal estimate
  TH1D* hWTau_MC_NoCJV_DPhi  = new TH1D("hWTau_MC_NoCJV_DPhi", "", 3, dphiEdges); // W+jets MC at gen level without CJV (same as hWTau_MCS_NoCJV_DPhi, so I should probably get rid of this to make it faster...)
  
  TH1D* hWTau_MCC_CJV_DPhi    = new TH1D("hWTau_MCC_CJV_DPhi",   "", 3, dphiEdges); // W+jets MC at gen level with CJV and tau reco
  
  // cutflow histograms
  TH1D* hDataWTau            = new TH1D("hWTau_CutFlow_Data",        "", nCutsWTau, 0., nCutsWTau);
  TH1D* hWLNuWTau            = new TH1D("hWTau_CutFlow_WToLNu",      "", nCutsWTau, 0., nCutsWTau);
  TH1D* hQCDWTau             = new TH1D("hWTau_CutFlow_QCD",         "", nCutsWTau, 0., nCutsWTau);
  TH1D* hDYWTau              = new TH1D("hWTau_CutFlow_DY",          "", nCutsWTau, 0., nCutsWTau);
  TH1D* hZvvWTau             = new TH1D("hWTau_CutFlow_Zvv",         "", nCutsWTau, 0., nCutsWTau);
  TH1D* hSingleTWTau         = new TH1D("hWTau_CutFlow_SingleTSum",  "", nCutsWTau, 0., nCutsWTau);
  TH1D* hDibosonWTau         = new TH1D("hWTau_CutFlow_Diboson",     "", nCutsWTau, 0., nCutsWTau);
  TH1D* hSignalWTau          = new TH1D("hWTau_CutFlow_SignalSum",   "", nCutsWTau, 0., nCutsWTau);

  ///////////////////////////
  // loop over MC datasets //
  ///////////////////////////
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);

    TFile* file = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

    /////////////////////////////
    // setup cuts & temp hists //
    /////////////////////////////

    // For DY samples - ensure we don't double count for pT > 100 (2 DY smaples) 
    TCut cutD = cuts.cutDataset(dataset.name);
    // TCut cutD("");

    TCut cutWTau_C(""); // Cut to calculate numbers of data and BG from MC 
    TCut cutWTau_MCC_NoCJV(""), cutWTau_MCS_NoCJV(""); // Cuts for Tau ID eff
    TCut cutWTau_MC_CJV(""), cutWTau_MC_NoCJV(""); // Cuts for CJV eff
    TCut cutWTau_MCC_CJV("");

    // tmp histograms - numbers of BG & data. you need one for data otherwise it comes out at 0. 
    // I don't know why - I think it's something to do with the fact that hists belong to certain files depending on which one was last opened, 
    // so closing a file below might delete that hist, hence you get out 0
    TH1D* hWTau_BGC_DPhi_tmp       = new TH1D("hWTau_BGC_DPhi_tmp","",3,dphiEdges);
    TH1D* hWTau_DataC_DPhi_tmp     = new TH1D("hWTau_DataC_DPhi_tmp","",3,dphiEdges);
    
    TH1D* hWTau_MCEl_DPhi_tmp      = new TH1D("hWTau_MCEl_DPhi_tmp", "", 3, dphiEdges);  

    // tmp histograms - tau ID eff
    TH1D* hWTau_MCC_NoCJV_DPhi_tmp = new TH1D("hWTau_MCC_NoCJV_DPhi_tmp", "", 3, dphiEdges);  // W+jets MC ctrl region
    TH1D* hWTau_MCS_NoCJV_DPhi_tmp = new TH1D("hWTau_MCS_NoCJV_DPhi_tmp", "", 3, dphiEdges);  // W+jets MC sgnl region
    
    // tmp histogram - CJV eff
    TH1D* hWTau_MC_CJV_DPhi_tmp    = new TH1D("hWTau_MC_CJV_DPhi_tmp",    "", 3, dphiEdges);  // W+jets MC ctrl region
    TH1D* hWTau_MC_NoCJV_DPhi_tmp  = new TH1D("hWTau_MC_NoCJV_DPhi_tmp",  "", 3, dphiEdges);  // W+jets MC sgnl region

    TH1D* hWTau_MCC_CJV_DPhi_tmp    = new TH1D("hWTau_MCC_CJV_DPhi_tmp",    "", 3, dphiEdges);  // W+jets MC ctrl region

    // Weight to lumi
    double weight = (dataset.isData) ? 1. : (lumi * dataset.sigma / dataset.nEvents);
    // double weight = 1.;

    ///////////////////////////////
    // Now do some hist filling! //
    ///////////////////////////////

    // check it's W+Jets as they are used for eff hists
    bool isWJets = false;    
    bool isEwkW  = false;

    TCut wWeight(""); // do inside dataset loop to avoid it affecting EWK samples!
    TCut yStarWeight("");
    TCut mjjWeight("");
    TCut otherCuts = puWeight * trigCorrWeight;

    if (dataset.name == "WJets" ||
        dataset.name == "W1Jets" || 
        dataset.name == "W2Jets" || 
        dataset.name == "W3Jets" || 
        dataset.name == "W4Jets" ||
        dataset.name == "EWK_Wp2Jets" ||
        dataset.name == "EWK_Wm2Jets") {

      if (dataset.name == "EWK_Wp2Jets" || dataset.name == "EWK_Wm2Jets") isEwkW = true;
      else isWJets = true;
    
      if(isWJets) {
	if (options.doMCFMWeights) {
	  yStarWeight = TCut("8.49667e-01 + (1.49687e-01*abs((log((sqrt(wgenmass*wgenmass + wgenpt*wgenpt*cosh(wgeneta)*cosh(wgeneta)) + wgenpt*sinh(wgeneta))/(sqrt(wgenmass*wgenmass + wgenpt*wgenpt)))) - 0.5*(genJet1Eta + genJet2Eta)))");
	  mjjWeight   = TCut("3.92568e-01 + (1.20734e-01*log(genVBFM)) - (2.55622e-04*genVBFM)");
	  wWeight     = yStarWeight * mjjWeight * cuts.wWeight();
	}
	else {
	  wWeight     = cuts.wWeight();
	}
      }

      std::cout << "Analysing W MC     : " << dataset.name << std::endl;
      std::cout << "  weight : " << weight << std::endl;

      // W->enu contamination
      tree->Draw("vbfDPhi>>hWTau_MCEl_DPhi_tmp", otherCuts * wWeight * (cutD + cuts.wElGen() + cuts.cutWTau("wTau") + cutTightMjj_basic));
      hWTau_MCEl_DPhi_tmp->Scale(weight);
      hWTau_MCEl_DPhi->Add(hWTau_MCEl_DPhi_tmp);

      // Do Tau ID eff
      cutWTau_MCC_NoCJV = otherCuts * wWeight * (cuts.wTauGen() + cutDPhiSignalNoCJV_basic + cuts.cutWTau("wTau"));
      cutWTau_MCS_NoCJV = otherCuts * wWeight * (cuts.wTauGen() + cutDPhiSignalNoCJV_basic);

      tree->Draw("vbfDPhi>>hWTau_MCC_NoCJV_DPhi_tmp", cutWTau_MCC_NoCJV);
      tree->Draw("vbfDPhi>>hWTau_MCS_NoCJV_DPhi_tmp", cutWTau_MCS_NoCJV); 
      
      hWTau_MCC_NoCJV_DPhi_tmp->Scale(weight);
      hWTau_MCS_NoCJV_DPhi_tmp->Scale(weight);
      
      hWTau_MCC_NoCJV_DPhi->Add(hWTau_MCC_NoCJV_DPhi_tmp);
      hWTau_MCS_NoCJV_DPhi->Add(hWTau_MCS_NoCJV_DPhi_tmp);

      // Do CJV eff
      cutWTau_MC_CJV   = otherCuts * wWeight * (cuts.wTauGen() + cutDPhiSignalNoCJV_basic + cuts.cutWTau("CJV"));
      cutWTau_MC_NoCJV = otherCuts * wWeight * (cuts.wTauGen() + cutDPhiSignalNoCJV_basic);

      cutWTau_MCC_CJV   = otherCuts * wWeight * (cuts.wTauGen() + cutDPhiSignalNoCJV_basic + cuts.cutWTau("CJV") + cuts.cutWTau("wTau"));

      tree->Draw("vbfDPhi>>hWTau_MC_CJV_DPhi_tmp", cutWTau_MC_CJV);
      tree->Draw("vbfDPhi>>hWTau_MC_NoCJV_DPhi_tmp", cutWTau_MC_NoCJV); 
      tree->Draw("vbfDPhi>>hWTau_MCC_CJV_DPhi_tmp", cutWTau_MCC_CJV);

      hWTau_MC_CJV_DPhi_tmp->Scale(weight);  
      hWTau_MC_NoCJV_DPhi_tmp->Scale(weight);  
      hWTau_MCC_CJV_DPhi_tmp->Scale(weight);  

      hWTau_MC_CJV_DPhi->Add(hWTau_MC_CJV_DPhi_tmp);
      hWTau_MC_NoCJV_DPhi->Add(hWTau_MC_NoCJV_DPhi_tmp);
      hWTau_MCC_CJV_DPhi->Add(hWTau_MCC_CJV_DPhi_tmp);
      
      // Setup control plot cuts
      cutTightMjj        = otherCuts * wWeight * cutTightMjj_basic;
      cutDPhiSignalNoCJV = otherCuts * wWeight * cutDPhiSignalNoCJV_basic;
    
      // Debug output  
      std::cout << "  N ctrl region (dphi<1, standard selection, no CJV, tau reco) : " << hWTau_MCC_NoCJV_DPhi_tmp->GetBinContent(1) << " +/- " << hWTau_MCC_NoCJV_DPhi_tmp->GetBinError(1) << std::endl;// debug output
      std::cout << "  N ctrl region (dphi<1, standard selection, no CJV, no tau reco) : " << hWTau_MCS_NoCJV_DPhi_tmp->GetBinContent(1) << " +/- " << hWTau_MCS_NoCJV_DPhi_tmp->GetBinError(1) << std::endl;// debug output
      std::cout << "  N ctrl region (dphi<1, standard selection, no tau reco, CJV) : " << hWTau_MC_CJV_DPhi_tmp->GetBinContent(1) << " +/- " << hWTau_MC_CJV_DPhi_tmp->GetBinError(1) << std::endl;// debug output
      std::cout << "  N ctrl region (dphi<1, standard selection, tau reco, CJV) : " << hWTau_MCC_CJV_DPhi_tmp->GetBinContent(1) << " +/- " << hWTau_MCC_CJV_DPhi_tmp->GetBinError(1) << std::endl;// debug output
      // cutWTau_MCC_NoCJV.Print();

    } else if (dataset.isData) {

      std::cout << "Analysing Data     : " << dataset.name << std::endl;

      // Count number of tau in control region in data
      // cutWTau_C = (cuts.cutWTau("wTau") + cutDPhiSignalNoCJV_basic);
      cutWTau_C = cutDPhiSignalNoCJV_basic + cuts.cutWTau("wTau");

      tree->Draw("vbfDPhi>>hWTau_DataC_DPhi_tmp",cutWTau_C);
      hWTau_DataC_DPhi->Add(hWTau_DataC_DPhi_tmp);

      // Setup cuts for control plots 
      cutTightMjj        = cutTightMjj_basic;
      cutDPhiSignalNoCJV = cutDPhiSignalNoCJV_basic;

      // debug output
      std::cout << "  N ctrl region (dphi<1) : " << hWTau_DataC_DPhi->GetBinContent(1) << " +/- " << hWTau_DataC_DPhi->GetBinError(1) << std::endl;

    } else { // All MC _BUT_ WJets. Need to go into hWTau_BGC_DPhi
 
      if (dataset.name.compare(0,17,"SignalM125_POWHEG")!=0) {

        std::cout << "Analysing BG MC    : " << dataset.name << std::endl;
        std::cout << "  weight : " << weight << std::endl;

        if (dataset.name == "DYJetsToLL_PtZ-100" || dataset.name == "DYJetsToLL") {
        	if (options.doMCFMWeights) {
          	yStarWeight = TCut("8.49667e-01 + (1.49687e-01*abs((log((sqrt(zgenmass*zgenmass + zgenpt*zgenpt*cosh(zgeneta)*cosh(zgeneta)) + zgenpt*sinh(zgeneta))/(sqrt(zgenmass*zgenmass + zgenpt*zgenpt)))) - 0.5*(genJet1Eta + genJet2Eta)))");
          	mjjWeight   = TCut("3.92568e-01 + (1.20734e-01*log(genVBFM)) - (2.55622e-04*genVBFM)");
		}
        }

	otherCuts *= yStarWeight * mjjWeight;

	// Setup control plot cuts 
      	cutTightMjj        = otherCuts * (cutD + cutTightMjj_basic);
      	cutDPhiSignalNoCJV = otherCuts * (cutD + cutDPhiSignalNoCJV_basic);

        // Count number of tau in control region in bg
        cutWTau_C = otherCuts * (cutD  && cuts.cutWTau("wTau") && cutDPhiSignalNoCJV_basic);
        tree->Draw("vbfDPhi>>hWTau_BGC_DPhi_tmp", cutWTau_C);
        
        hWTau_BGC_DPhi_tmp->Scale(weight);
        
        hWTau_BGC_DPhi->Add(hWTau_BGC_DPhi_tmp);


        // debug output
        std::cout << "  N ctrl region (dphi<1) : " << hWTau_BGC_DPhi_tmp->GetBinContent(1) << " +/- " << hWTau_BGC_DPhi_tmp->GetBinError(1) << std::endl;
      }
    }
        
    
    delete hWTau_BGC_DPhi_tmp;
    delete hWTau_DataC_DPhi_tmp;
    delete hWTau_MCEl_DPhi_tmp;
    delete hWTau_MCC_NoCJV_DPhi_tmp;
    delete hWTau_MCS_NoCJV_DPhi_tmp;
    delete hWTau_MC_CJV_DPhi_tmp;    
    delete hWTau_MCC_CJV_DPhi_tmp;    
    delete hWTau_MC_NoCJV_DPhi_tmp;    
    
    ofile->cd();
    
    ///////////////////////////////
    // per-dataset cutflow hists //
    ///////////////////////////////
    std::string hnameWTau = std::string("hWTau_CutFlow_")+dataset.name;
    TH1D* hCutFlowWTau = new TH1D(hnameWTau.c_str(), "", nCutsWTau, 0., nCutsWTau);

    for (unsigned c=0; c<(nCutsWTau-1); ++c) {

      TCut cut;
      if(c == (nCutsWTau-2)) {
          cut = otherCuts * (cutD + cuts.cutflowWTau(c));
          if(isWJets) cut = otherCuts * wWeight * ( cuts.cutflowWTau(c));
      } else {
          cut = puWeight * yStarWeight * mjjWeight * (cutD + cuts.cutflowWTau(c));
          if(isWJets) cut = puWeight * wWeight * (cuts.cutflowWTau(c));
      }
      TH1D* h = new TH1D("h","", 1, 0., 1.);
      tree->Draw("0.5>>h", cut);
      // cut.Print();
      // std::cout << weight << "\t" << h->GetBinContent(1) *weight<< std::endl;
      hCutFlowWTau->SetBinContent(c+1, h->GetBinContent(1));
      hCutFlowWTau->SetBinError(c+1, h->GetBinError(1));

      delete h;
    }

    hCutFlowWTau->Scale(weight);

    ////////////////////
    // sum histograms //
    ////////////////////
    if (dataset.isData) 
      hDataWTau->Add(hCutFlowWTau);
    
    if (isWJets || isEwkW)
      hWLNuWTau->Add(hCutFlowWTau);
    
    if (dataset.name.compare(0,3,"QCD")==0)
      hQCDWTau->Add(hCutFlowWTau);
    
    if (dataset.name.compare(0,2,"DY")==0) 
      hDYWTau->Add(hCutFlowWTau);

    if (dataset.name.compare(0,3,"Zvv")==0) 
      hZvvWTau->Add(hCutFlowWTau);
    
    if (dataset.name.compare(0,7,"SingleT")==0) 
      hSingleTWTau->Add(hCutFlowWTau);
    
    if (dataset.name.compare(0,17,"SignalM125_POWHEG")==0) 
      hSignalWTau->Add(hCutFlowWTau);
    
    if (dataset.name.compare(0,2,"WW")==0 ||
        dataset.name.compare(0,2,"WZ")==0 ||
        dataset.name.compare(0,2,"ZZ")==0 ) 
      hDibosonWTau->Add(hCutFlowWTau);

    hCutFlowWTau->Write("",TObject::kOverwrite);

    delete hCutFlowWTau;

    ///////////////////////
    // Do control plots: //
    ///////////////////////
    // Tau1Pt, Tau1Eta, DPhiJJ, Tau1mT (post DPhiSIGNALnoCJV), numTaus, MinDR(Tau1,tag jet), NumTauMatched,  Tau1mT (TightMjj)

    TFile* ofilePlots = TFile::Open( (oDirPlots+std::string("/")+dataset.name+std::string(".root")).c_str(), "RECREATE");

    // Ones with DPhiSignalNoCJV cut
    std::string hname = "hWTau_tau1Pt";
    if (i==0) hnames.push_back(hname);
    TH1D* hTau1Pt = new TH1D(hname.c_str(), "", 10, 0., 200.);
    std::string str = "tau1Pt>>"+hname;
    tree->Draw(str.c_str(), cutDPhiSignalNoCJV);
    hTau1Pt->Scale(weight);
    hTau1Pt->Write("", TObject::kOverwrite); 

    hname = "hWTau_tau1Eta";
    if (i==0) hnames.push_back(hname);
    TH1D* hTau1Eta = new TH1D(hname.c_str(), "", 10, -2.5, 2.5);
    str = "tau1Eta>>"+hname;
    tree->Draw(str.c_str(), cutDPhiSignalNoCJV);
    hTau1Eta->Scale(weight); 
    hTau1Eta->Write("", TObject::kOverwrite); 
    
    hname = "hWTau_mT_DPhiSignalNoCJV";
    if (i==0) hnames.push_back(hname);
    TH1D* hWmT_DPhiSignalNoCJV = new TH1D(hname.c_str(), "", 20, 0., 200.);
    str = "tau1mT>>"+hname;
    tree->Draw(str.c_str(), cutDPhiSignalNoCJV);
    hWmT_DPhiSignalNoCJV->Scale(weight); 
    hWmT_DPhiSignalNoCJV->Write("", TObject::kOverwrite); 

    // Plots with TightMjj (+tau)
    hname = "hWTau_dPhiJJ";
    if (i==0) hnames.push_back(hname);
    TH1D* hDPhiJJ = new TH1D(hname.c_str(), "", 25, 0., TMath::Pi());
    str = "vbfDPhi>>"+hname;
    tree->Draw(str.c_str(), cutTightMjj);
    hDPhiJJ->Scale(weight); 
    hDPhiJJ->Write("", TObject::kOverwrite); 
    
    hname = "hNumberTaus";
    if (i==0) hnames.push_back(hname);
    TH1D* hNTaus = new TH1D(hname.c_str(), "", 5, 0., 5.);
    str = "nTaus_pass>>"+hname;
    tree->Draw(str.c_str(), cutTightMjj);
    hNTaus->Scale(weight); 
    hNTaus->Write("", TObject::kOverwrite); 

    hname = "hMinDR";
    if (i==0) hnames.push_back(hname);
    TH1D* hMinDR = new TH1D(hname.c_str(), "", 18, 0., 4.5);
    str = "tau1dR>>"+hname;
    tree->Draw(str.c_str(), cutTightMjj);
    hMinDR->Scale(weight); 
    hMinDR->Write("", TObject::kOverwrite); 

    hname = "hTauMatchJet";
    if (i==0) hnames.push_back(hname);
    TH1D* hTauMatchJet = new TH1D(hname.c_str(), "", 4, 0., 4);
    str = "nTaus_match>>"+hname;
    tree->Draw(str.c_str(), cutTightMjj);
    hTauMatchJet->Scale(weight); 
    hTauMatchJet->Write("", TObject::kOverwrite); 

    hname = "hWTau_mT_TightMjj";
    if (i==0) hnames.push_back(hname);
    TH1D* hWmT_TightMjj = new TH1D(hname.c_str(), "", 20, 0., 200);
    str = "tau1mT>>"+hname;
    tree->Draw(str.c_str(), cutTightMjj);
    hWmT_TightMjj->Scale(weight); 
    hWmT_TightMjj->Write("", TObject::kOverwrite); 

    delete tree;

    ofilePlots->Close();
    file->Close();
   
  } //end of datasets loop

  // create histograms with the background estimate & efficiency calculations
  // Note that the eqn in the AN boils down to (data-bg) * (hWTau_MC_CJV_DPhi/hWTau_MCC_NoCJV_DPhi), and I'll call the latter bracket R (following convention in wBackground code)

  // Stat hists
  TH1D* hWTau_TauIDEff_DPhi        = new TH1D("hWTau_TauIDEff_DPhi",   "", 3, dphiEdges); // tau ID eff
  TH1D* hWTau_CJVEff_DPhi          = new TH1D("hWTau_CJVEff_DPhi",     "", 3, dphiEdges); // CJV eff
  TH1D* hWTau_R_DPhi               = new TH1D("hWTau_R_DPhi",          "", 3, dphiEdges); // ratio of sngl/ctrl
  TH1D* hWTau_EstC_DPhi            = new TH1D("hWTau_EstC_DPhi ",      "", 3, dphiEdges); // n^data - n^BG in Anne Marie's AN
  TH1D* hWTau_EstS_DPhi            = new TH1D("hWTau_EstS_DPhi",       "", 3, dphiEdges); // Final number of tau estimate
  
  // Syst hists
  TH1D* hWTau_DataC_DPhi_Syst      = new TH1D("hWTau_DataC_DPhi_Syst", "", 3, dphiEdges); // Data in control region (syst)
  TH1D* hWTau_BGC_DPhi_Syst        = new TH1D("hWTau_BGC_DPhi_Syst",   "", 3, dphiEdges); // Data in control region (syst)
  TH1D* hWTau_R_DPhi_Syst          = new TH1D("hWTau_R_DPhi_Syst",     "", 3, dphiEdges); // ratio of sngl/ctrl
  TH1D* hWTau_EstC_DPhi_Syst       = new TH1D("hWTau_EstC_DPhi_Syst",  "", 3, dphiEdges); // n^data - n^BG in Anne Marie's AN (syst)
  TH1D* hWTau_EstS_DPhi_Syst       = new TH1D("hWTau_EstS_DPhi_Syst",  "", 3, dphiEdges); // Final number of tau estimate (syst)
  
  // Syst hists - data/mc ID
  TH1D* hWTau_TauIDEff_Dphi_SystID = new TH1D("hWTau_TauIDEff_Dphi_SystID",   "", 3, dphiEdges); // tau ID eff
  TH1D* hWTau_EstC_DPhi_SystID     = new TH1D("hWTau_EstC_DPhi_SystID",  "", 3, dphiEdges); // n^data - n^BG in Anne Marie's AN (syst)
  TH1D* hWTau_EstS_DPhi_SystID     = new TH1D("hWTau_EstS_DPhi_SystID",  "", 3, dphiEdges); // Final number of tau estimate (syst)
  TH1D* hWTau_R_DPhi_SystID        = new TH1D("hWTau_R_DPhi_SystID",     "", 3, dphiEdges); // ratio of sngl/ctrl

  hWTau_TauIDEff_DPhi->Divide(hWTau_MCC_NoCJV_DPhi, hWTau_MCS_NoCJV_DPhi, 1., 1.); // calculate tau ID eff
  hWTau_CJVEff_DPhi->Divide(hWTau_MC_CJV_DPhi, hWTau_MC_NoCJV_DPhi, 1., 1.); // calculate CJV eff

  // Stat only calc
  hWTau_R_DPhi->Divide(hWTau_MC_CJV_DPhi, hWTau_MCC_NoCJV_DPhi, 1., 1.);
  for (int i=1; i<=hWTau_R_DPhi->GetNbinsX(); ++i) hWTau_R_DPhi->SetBinError(i,0.); //ignore MC stats in R
  hWTau_BGC_DPhi_Syst->Add(hWTau_BGC_DPhi, 1.);  // take a copy for the syst only
  for (int i=1; i<=hWTau_BGC_DPhi->GetNbinsX(); ++i) hWTau_BGC_DPhi->SetBinError(i,0.);  // ignore MC stats
  hWTau_EstC_DPhi->Add(hWTau_DataC_DPhi, hWTau_BGC_DPhi, 1., -1.); // n^data - n^BG
  hWTau_EstS_DPhi->Multiply(hWTau_EstC_DPhi, hWTau_R_DPhi,1.,1.); //stat errors from data only, none from ratio

  // Syst only calc
  hWTau_DataC_DPhi_Syst->Add(hWTau_DataC_DPhi, 1.);
  for (int i=1; i<=hWTau_DataC_DPhi_Syst->GetNbinsX(); ++i) hWTau_DataC_DPhi_Syst->SetBinError(i,0.); // no stat error from data
  hWTau_R_DPhi_Syst->Divide(hWTau_MC_CJV_DPhi, hWTau_MCC_NoCJV_DPhi, 1., 1.);  // includes MC stats in R
  hWTau_EstC_DPhi_Syst->Add(hWTau_DataC_DPhi_Syst, hWTau_BGC_DPhi_Syst, 1., -1.); // do includes stat errors from MC bg
  hWTau_EstS_DPhi_Syst->Multiply(hWTau_EstC_DPhi_Syst, hWTau_R_DPhi_Syst, 1., 1.); 

  // Systematics on data/mc tau ID 
  // This could probably be streamlined...
  double scaleFactor = 0.08;
  hWTau_TauIDEff_Dphi_SystID->Add(hWTau_TauIDEff_DPhi,1.);
  for (int i=1; i<=hWTau_TauIDEff_Dphi_SystID->GetNbinsX(); ++i) hWTau_TauIDEff_Dphi_SystID->SetBinError(i,scaleFactor*hWTau_TauIDEff_Dphi_SystID->GetBinContent(i));
  hWTau_R_DPhi_SystID->Add(hWTau_R_DPhi);
  for (int i=1; i<=hWTau_R_DPhi_SystID->GetNbinsX(); ++i) hWTau_R_DPhi_SystID->SetBinError(i,scaleFactor*hWTau_R_DPhi_SystID->GetBinContent(i)); //ignore MC stats in R
  hWTau_EstC_DPhi_SystID->Add(hWTau_DataC_DPhi_Syst, hWTau_BGC_DPhi, 1., -1.); // no stat errors form data or mc
  hWTau_EstS_DPhi_SystID->Multiply(hWTau_EstC_DPhi_SystID, hWTau_R_DPhi_SystID, 1., 1.); // use hWTau_EstC_DPhi_SystID if you only want data/mc scale systs errors, or hWTau_EstC_DPhi_Syst if you want total syst error

  std::cout << std::endl << std::endl;
  std::cout << "W->tau channel (dphi<1.0)" << std::endl;
  std::cout << "  Data ctrl region                                       : " << hWTau_DataC_DPhi->GetBinContent(1) << " +/- " << hWTau_DataC_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Background ctrl region                                 : " << hWTau_BGC_DPhi->GetBinContent(1) << " +/- " << hWTau_BGC_DPhi->GetBinError(1) << std::endl;
  std::cout << "  W+jets MC - gen level tau, standard selection, no CJV, tau reco            : " << hWTau_MCC_NoCJV_DPhi->GetBinContent(1) << " +/- " << hWTau_MCC_NoCJV_DPhi->GetBinError(1) << std::endl;
  std::cout << "  W+jets MC - gen level tau, standard selection, no CJV                      : " << hWTau_MCS_NoCJV_DPhi->GetBinContent(1) << " +/- " << hWTau_MCS_NoCJV_DPhi->GetBinError(1) << std::endl;
  std::cout << "  W+jets MC - gen level tau, standard selection w/CJV (MC estimate in signal region) : " << hWTau_MC_CJV_DPhi->GetBinContent(1) << " +/- " << hWTau_MC_CJV_DPhi->GetBinError(1) << std::endl;
  std::cout << "  W+jets MC - gen level tau, standard selection wout/CJV : " << hWTau_MC_NoCJV_DPhi->GetBinContent(1) << " +/- " << hWTau_MC_NoCJV_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  Number of W->enu that pass VBF + tau selection (no CJV): " << hWTau_MCEl_DPhi->GetBinContent(1) << " +/- " << hWTau_MCEl_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  W in ctrl region                                       : " << hWTau_EstC_DPhi ->GetBinContent(1) << " +/- " << hWTau_EstC_DPhi ->GetBinError(1) << std::endl;
  std::cout << "  eff_tauID                                              : " << hWTau_TauIDEff_DPhi->GetBinContent(1) << " +/- " << hWTau_TauIDEff_DPhi->GetBinError(1) << " +/- " << hWTau_TauIDEff_Dphi_SystID->GetBinError(1) << std::endl;
  std::cout << "  eff_CJV                                                : " << hWTau_CJVEff_DPhi->GetBinContent(1) << " +/- " << hWTau_CJVEff_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl << std::endl;
  std::cout << "  W in sgnl region                                       : " << hWTau_EstS_DPhi->GetBinContent(1) << " +/- " << hWTau_EstS_DPhi->GetBinError(1) << " (stat from data) +/- " << 0.08*hWTau_EstS_DPhi->GetBinContent(1) << " (syst, data/mc ID scale) +/- " << hWTau_EstS_DPhi_Syst->GetBinError(1) << " (syst from MC stats) +/- " << 0.05*hWTau_EstS_DPhi->GetBinContent(1) << " (w->e contamination" << std::endl;


  // write the cutflow table
  std::cout << "Writing cut flow TeX file" << std::endl;

  ofstream effFile;
  effFile.open(options.oDir+std::string("/cutflowWTau.tex"));

  effFile << "Cut & N(data) & N($W\\rightarrow \\tau\\nu$) & N($Z\\rightarrow \\nu \\nu$) & N(DY) & N(QCD) & N($t\\bar{t}$) & N(single $t$) & N(diboson) & N(Signal $m_H = 125$ GeV) \\\\" << std::endl;

  TH1D* hTTbarWTau = (TH1D*) ofile->Get("hWTau_CutFlow_TTBar");
  // cutflow table
  for (unsigned i=0; i<(nCutsWTau-1); ++i) {

    effFile << cuts.cutNameWTau(i) << " & ";
    effFile << "$" << hDataWTau->GetBinContent(i+1) << " \\pm " << hDataWTau->GetBinError(i+1) << "$ & ";
    effFile << "$" << hWLNuWTau->GetBinContent(i+1) << " \\pm " << hWLNuWTau->GetBinError(i+1) << "$ & ";
    effFile << "$" << hZvvWTau->GetBinContent(i+1) << " \\pm " << hZvvWTau->GetBinError(i+1) << "$ & ";
    effFile << "$" << hDYWTau->GetBinContent(i+1) << " \\pm " << hDYWTau->GetBinError(i+1) << "$ & ";
    effFile << "$" << hQCDWTau->GetBinContent(i+1) << " \\pm " << hQCDWTau->GetBinError(i+1) << "$ & ";
    effFile << "$" << hTTbarWTau->GetBinContent(i+1) << " \\pm " << hTTbarWTau->GetBinError(i+1) << "$ & ";
    effFile << "$" << hSingleTWTau->GetBinContent(i+1) << " \\pm " << hSingleTWTau->GetBinError(i+1) << "$ & ";
    effFile << "$" << hDibosonWTau->GetBinContent(i+1) << " \\pm " << hDibosonWTau->GetBinError(i+1) << "$ & ";
    effFile << "$" << hSignalWTau->GetBinContent(i+1) << " \\pm " << hSignalWTau->GetBinError(i+1) << "$ \\\\ ";
    effFile << std::endl;

  }

  effFile << std::endl << std::endl;
  effFile.close();  

  // for (unsigned n = 0; n < hnames.size(); n++)
  //   std::cout << hnames[n] <<std::endl;

  //////////////////////
  // Do control plots //
  //////////////////////
  std::vector<std::string> wjetsDatasets;
  wjetsDatasets.push_back(std::string("WJets"));
  wjetsDatasets.push_back(std::string("W1Jets"));
  wjetsDatasets.push_back(std::string("W2Jets"));
  wjetsDatasets.push_back(std::string("W3Jets"));
  wjetsDatasets.push_back(std::string("W4Jets"));
  wjetsDatasets.push_back(std::string("EWK_Wp2Jets"));
  wjetsDatasets.push_back(std::string("EWK_Wm2Jets"));
  SumDatasets(oDirPlots, wjetsDatasets, hnames, "WNJets+EWK");

  std::vector<std::string> topDatasets;
  topDatasets.push_back(std::string("SingleT_t"));
  topDatasets.push_back(std::string("SingleTbar_t"));
  topDatasets.push_back(std::string("SingleT_s"));
  topDatasets.push_back(std::string("SingleTbar_s"));
  topDatasets.push_back(std::string("SingleT_tW"));
  topDatasets.push_back(std::string("SingleTbar_tW"));
  topDatasets.push_back(std::string("TTBar"));
  SumDatasets(oDirPlots, topDatasets, hnames, "SingleT+TTBar");

  // sum DY contributions
  std::vector<std::string> dyjets;
  dyjets.push_back("DYJetsToLL");
  dyjets.push_back("DYJetsToLL_PtZ-100");
  dyjets.push_back("DYJetsToLL_EWK");
  SumDatasets(oDirPlots,dyjets,hnames,"DYJets+EWK");

  std::vector<std::string> zJets;
  zJets.push_back("Zvv_50to100");
  zJets.push_back("Zvv_100to200");
  zJets.push_back("Zvv_200to400");
  zJets.push_back("Zvv_400toinf");
  SumDatasets(oDirPlots, zJets, hnames, "ZJets");

  // sum single top datasets
  std::vector<std::string> dibDatasets;
  dibDatasets.push_back(std::string("WW"));
  dibDatasets.push_back(std::string("WZ"));
  dibDatasets.push_back(std::string("ZZ"));
  SumDatasets(oDirPlots, dibDatasets, hnames, "DiBoson");

  // sum QCD histograms
  std::vector<std::string> qcdDatasets;
  qcdDatasets.push_back(std::string("QCD_Pt30to50"));
  qcdDatasets.push_back(std::string("QCD_Pt50to80"));
  qcdDatasets.push_back(std::string("QCD_Pt80to120"));
  qcdDatasets.push_back(std::string("QCD_Pt120to170"));
  qcdDatasets.push_back(std::string("QCD_Pt170to300"));
  qcdDatasets.push_back(std::string("QCD_Pt300to470"));
  qcdDatasets.push_back(std::string("QCD_Pt470to600"));
  qcdDatasets.push_back(std::string("QCD_Pt600to800"));
  qcdDatasets.push_back(std::string("QCD_Pt800to1000"));
  qcdDatasets.push_back(std::string("QCD_Pt1000to1400"));
  qcdDatasets.push_back(std::string("QCD_Pt1400to1800"));
  qcdDatasets.push_back(std::string("QCD_Pt1800"));
  SumDatasets(oDirPlots, qcdDatasets, hnames, "QCD");

  // make plots
  std::cout << "Making plots" << std::endl;
  StackPlot plots(oDirPlots);
  plots.setLegPos(0.65,0.60,0.93,0.89);

  plots.addDataset("DiBoson", kViolet-6, 0);
  // plots.addDataset("QCD", kGreen+3, 0);
  plots.addDataset("DYJets+EWK", kPink-4,0);
  plots.addDataset("SingleT+TTBar", kAzure-2, 0);
  plots.addDataset("ZJets", kOrange-2, 0);
  plots.addDataset("WNJets+EWK", kGreen-3, 0);
  plots.addDataset("SignalM125_POWHEG", kRed, 2);
  plots.addDataset("METABCD", kBlack, 1);

  plots.setLabel("#tau channel");
  std::string hname = "hWTau_tau1Pt";
  plots.setYMax(45);
  plots.draw(hname.c_str(), "Leading #tau p_{T} [GeV]", "N_{events}", false, true);

  hname = "hWTau_tau1Eta";
  plots.setYMax(12);
  plots.draw(hname.c_str(), "Leading #tau #eta", "N_{events}",false, true);

  hname = "hWTau_dPhiJJ";
  plots.setYMax(5E5);
  plots.draw(hname.c_str(), "#Delta #phi_{jj}", "N_{events}",true, true);

  hname = "hWTau_mT_DPhiSignalNoCJV";
  plots.setYMax(15);
  plots.draw(hname.c_str(), "m_{T}(#tau#nu) [GeV]", "N_{events}",false, true);

  hname = "hNumberTaus";
  plots.setYMax(-1);
  plots.draw(hname.c_str(), "Number of #tau", "N_{events}",true, true);

  hname = "hMinDR";
  plots.setYMax(5E4);
  plots.draw(hname.c_str(), "#Delta R_{min}(#tau, leading jet)", "N_{events}",true, true);

  hname = "hTauMatchJet";
  plots.setYMax(-1);
  plots.draw(hname.c_str(), "#tau matching with tagging jets", "N_{events}",true, true);
  
  hname = "hWTau_mT_TightMjj";
  plots.setYMax(45);
  plots.draw(hname.c_str(), "m_{T}(#tau#nu) [GeV]", "N_{events}",false, true);

  //////////////////////////////////////
  // store cutflow & other histograms //
  //////////////////////////////////////
  ofile->cd(); 
  
  // Store numbers for data, bg, efficiencies  
  hWTau_BGC_DPhi->Write("",TObject::kOverwrite);
  hWTau_BGC_DPhi_Syst->Write("",TObject::kOverwrite);
  hWTau_DataC_DPhi->Write("",TObject::kOverwrite);
  hWTau_DataC_DPhi_Syst->Write("",TObject::kOverwrite);
  hWTau_MCC_NoCJV_DPhi->Write("",TObject::kOverwrite);
  hWTau_MCS_NoCJV_DPhi->Write("",TObject::kOverwrite);
  hWTau_MC_CJV_DPhi->Write("",TObject::kOverwrite);
  hWTau_MC_NoCJV_DPhi->Write("",TObject::kOverwrite);
  // Store cutflow hists
  hDataWTau->Write("",TObject::kOverwrite);
  hWLNuWTau->Write("",TObject::kOverwrite);
  hQCDWTau->Write("",TObject::kOverwrite);
  hDYWTau->Write("",TObject::kOverwrite);
  hZvvWTau->Write("",TObject::kOverwrite);
  hSingleTWTau->Write("",TObject::kOverwrite);
  hDibosonWTau->Write("",TObject::kOverwrite);
  hSignalWTau->Write("",TObject::kOverwrite);
  // Store final efficiencies, overall numbers, etc
  hWTau_TauIDEff_DPhi->Write("",TObject::kOverwrite);
  hWTau_CJVEff_DPhi->Write("",TObject::kOverwrite);
  hWTau_EstC_DPhi ->Write("",TObject::kOverwrite);
  hWTau_EstC_DPhi_Syst->Write("",TObject::kOverwrite);
  hWTau_EstS_DPhi ->Write("",TObject::kOverwrite);
  hWTau_EstS_DPhi_Syst->Write("",TObject::kOverwrite);
  hWTau_R_DPhi->Write("",TObject::kOverwrite);
  hWTau_R_DPhi_Syst->Write("",TObject::kOverwrite);

  ofile->Close();    

}
