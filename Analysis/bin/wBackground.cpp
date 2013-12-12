
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

  // For control plots
  std::string oDirPlots = options.oDir+std::string("/WControlPlots");
  boost::filesystem::path opath(oDirPlots);
  if (!exists(opath)) {
    std::cout << "Creating output directory : " << oDirPlots << std::endl;
    boost::filesystem::create_directory(opath);
  }
  else std::cout << "Writing plots results to " << oDirPlots << std::endl;
  std::vector<std::string> hnames; // to hold hist names for control plots

  if (options.doMCFMWeights) std::cout << "Going to apply MCFM weights" << std::endl;

  // cuts
  Cuts cuts;
  unsigned nCutsWMu = cuts.nCutsWMu();
  unsigned nCutsWEl = cuts.nCutsWEl();

  TCut puWeight("puWeight");
  TCut elWeight("");//el1Weight");
  TCut muWeight("");//mu1Weight");
  //TCut wWeight = cuts.wWeight();
  TCut trigCorr( "(trigCorrWeight>0) ? trigCorrWeight : 1." );

  TCut cutSignalNoMETNoDPhi = cuts.HLTandMETFilters() + cuts.leptonVeto() + cuts.vbf();

  // For lepton weights
  TCut lVetoWeight   = cuts.elVetoWeight(options.leptCorr) * cuts.muVetoWeight(options.leptCorr);
  TCut muTightWeight = cuts.muTightWeight(options.leptCorr);
  TCut elTightWeight = cuts.elTightWeight(options.leptCorr);
  
  // histograms
  double dphiEdges[4] = { 0., 1.0, 2.6, TMath::Pi() };
  double metEdges[13] = { 0., 10., 20., 30., 40., 50., 60., 70., 80., 90., 100., 110., 120. };

  TH1D* hWMu_MCC_DPhi = new TH1D("hWMu_MCC_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
  TH1D* hWMu_MCS_DPhi = new TH1D("hWMu_MCS_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region
  TH1D* hWMu_BGC_DPhi = new TH1D("hWMu_BGC_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hWMu_DataC_DPhi = new TH1D("hWMu_DataC_DPhi", "", 3, dphiEdges);  // Data ctrl region

  TH1D* hWEl_MCC_DPhi = new TH1D("hWEl_MCC_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
  TH1D* hWEl_MCS_DPhi = new TH1D("hWEl_MCS_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region
  TH1D* hWEl_BGC_DPhi = new TH1D("hWEl_BGC_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hWEl_DataC_DPhi = new TH1D("hWEl_DataC_DPhi", "", 3, dphiEdges);  // Data ctrl region

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

    TFile* file = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

//     std::string lwFile = options.iDir + std::string("/") + dataset.name + std::string("_LeptonWeights.root");   
//     lw.addFriend(tree, lwFile);

    // setup cuts
    TCut cutD = cuts.cutDataset(dataset.name);
    TCut otherCuts = puWeight * trigCorr;
    // special cut for QCD W+jets and DY
    TCut wWeight("");
    TCut yStarWeight("");
    TCut mjjWeight("");

    TCut cutWMu_C(""), cutWMu_S(""), cutWEl_C(""), cutWEl_S("");
    TCut cutWMuControlPlot("");
    TCut cutWElControlPlot("");

    // tmp histograms
    TH1D* hWMu_C_DPhi = new TH1D("hWMu_C_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
    TH1D* hWMu_S_DPhi = new TH1D("hWMu_S_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region
    TH1D* hWEl_C_DPhi = new TH1D("hWEl_C_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
    TH1D* hWEl_S_DPhi = new TH1D("hWEl_S_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region

    double weight = 1.;

    // check it's  W+Jets
    bool isWJets = false;
    bool isEwkW  = false;
    bool isQCD   = false;
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

      cutWMu_C = otherCuts * muTightWeight * wWeight * (cuts.wMuVBF() + cuts.cutWMu("MET"));
      cutWMu_S = otherCuts * lVetoWeight * wWeight * (cuts.wMuGen() + cuts.allCutsNoDPhi());
      cutWEl_C = otherCuts * elTightWeight * wWeight * (cuts.wElVBF() + cuts.cutWEl("MET"));
      cutWEl_S = otherCuts * lVetoWeight * wWeight * (cuts.wElGen() + cuts.allCutsNoDPhi());

      cutWMuControlPlot = otherCuts * muTightWeight * wWeight * (cuts.HLTandMETFilters() + cuts.vbfloose() + cuts.cutWMu("MET") + cuts.cutWMu("wMu") );
      cutWElControlPlot = otherCuts * elTightWeight * wWeight * (cuts.HLTandMETFilters() + cuts.vbfloose() + cuts.cutWEl("MET") + cuts.cutWEl("wEl") );

      // weight  to lumi
      weight = lumi * dataset.sigma / dataset.nEvents;
      std::cout << "  weight : " << weight << std::endl;
      
      tree->Draw("vbfDPhi>>hWMu_C_DPhi", cutWMu_C);
      tree->Draw("vbfDPhi>>hWMu_S_DPhi", cutWMu_S);
      tree->Draw("vbfDPhi>>hWEl_C_DPhi", cutWEl_C);
      tree->Draw("vbfDPhi>>hWEl_S_DPhi", cutWEl_S);
      hWMu_C_DPhi->Scale(weight);
      hWMu_S_DPhi->Scale(weight);
      hWEl_C_DPhi->Scale(weight);
      hWEl_S_DPhi->Scale(weight);
      
      // add to final histogram
      hWMu_MCC_DPhi->Add(hWMu_C_DPhi);
      hWMu_MCS_DPhi->Add(hWMu_S_DPhi);
      hWEl_MCC_DPhi->Add(hWEl_C_DPhi);
      hWEl_MCS_DPhi->Add(hWEl_S_DPhi);
    }
    else if (dataset.isData) {
      std::cout << "Analysing Data     : " << dataset.name << std::endl;

      cutWMu_C = (cuts.wMuVBF() + cuts.cutWMu("MET"));
      cutWMu_S = cuts.allCutsNoDPhi();
      cutWEl_C = (cuts.wElVBF() + cuts.cutWEl("MET"));
      cutWEl_S = cuts.allCutsNoDPhi();

      tree->Draw("vbfDPhi>>hWMu_C_DPhi", cutWMu_C);
      tree->Draw("vbfDPhi>>hWMu_S_DPhi", cutWMu_S);
      tree->Draw("vbfDPhi>>hWEl_C_DPhi", cutWEl_C);
      tree->Draw("vbfDPhi>>hWEl_S_DPhi", cutWEl_S);
      
      hWMu_DataC_DPhi->Add(hWMu_C_DPhi);
      hWEl_DataC_DPhi->Add(hWEl_C_DPhi);

      cutWMuControlPlot = (cuts.HLTandMETFilters() + cuts.vbfloose() + cuts.cutWMu("MET") + cuts.cutWMu("wMu") );
      cutWElControlPlot = (cuts.HLTandMETFilters() + cuts.vbfloose() + cuts.cutWEl("MET") + cuts.cutWEl("wEl") );

    }
    else {
      std::cout << "Analysing BG MC    : " << dataset.name << std::endl;

      if (dataset.name.compare(0,3,"QCD") == 0) isQCD = true;

      if (dataset.name == "DYJetsToLL_PtZ-100" || dataset.name == "DYJetsToLL") {
  if (options.doMCFMWeights) {
    yStarWeight = TCut("8.49667e-01 + (1.49687e-01*abs((log((sqrt(zgenmass*zgenmass + zgenpt*zgenpt*cosh(zgeneta)*cosh(zgeneta)) + zgenpt*sinh(zgeneta))/(sqrt(zgenmass*zgenmass + zgenpt*zgenpt)))) - 0.5*(genJet1Eta + genJet2Eta)))");
    mjjWeight   = TCut("3.92568e-01 + (1.20734e-01*log(genVBFM)) - (2.55622e-04*genVBFM)");
  }
      }

      otherCuts  *= yStarWeight * mjjWeight;

      cutWMu_C = otherCuts * muTightWeight * (cutD + cuts.wMuVBF() + cuts.cutWMu("MET"));
      cutWMu_S = otherCuts * lVetoWeight * (cutD + cuts.allCutsNoDPhi());
      cutWEl_C = otherCuts * elTightWeight * (cutD + cuts.wElVBF() + cuts.cutWEl("MET"));
      cutWEl_S = otherCuts * lVetoWeight * (cutD + cuts.allCutsNoDPhi());

      cutWMuControlPlot = otherCuts * muTightWeight * (cutD + cuts.HLTandMETFilters() + cuts.vbfloose() + cuts.cutWMu("MET") + cuts.cutWMu("wMu") );
      cutWElControlPlot = otherCuts * elTightWeight * (cutD + cuts.HLTandMETFilters() + cuts.vbfloose() + cuts.cutWEl("MET") + cuts.cutWEl("wEl") );

      // weight  to lumi
      weight = lumi * dataset.sigma / dataset.nEvents;
      std::cout << "  weight : " << weight << std::endl;

      tree->Draw("vbfDPhi>>hWMu_C_DPhi", cutWMu_C);
      tree->Draw("vbfDPhi>>hWEl_C_DPhi", cutWEl_C);
      hWMu_C_DPhi->Scale(weight);
      hWEl_C_DPhi->Scale(weight);
      
      if (!isQCD) {      
      hWMu_BGC_DPhi->Add(hWMu_C_DPhi);
      hWEl_BGC_DPhi->Add(hWEl_C_DPhi);
      }
      
    }
        
    // debug output
    std::cout << "  N ctrl (mu, dphi<1) : " << hWMu_C_DPhi->GetBinContent(1) << " +/- " << hWMu_C_DPhi->GetBinError(1) << std::endl;
    std::cout << "  N sgnl (mu, dphi<1) : " << hWMu_S_DPhi->GetBinContent(1) << " +/- " << hWMu_S_DPhi->GetBinError(1) << std::endl;
    std::cout << "  N ctrl (el, dphi<1) : " << hWEl_C_DPhi->GetBinContent(1) << " +/- " << hWEl_C_DPhi->GetBinError(1) << std::endl;
    std::cout << "  N sgnl (el, dphi<1) : " << hWEl_S_DPhi->GetBinContent(1) << " +/- " << hWEl_S_DPhi->GetBinError(1) << std::endl;
    
    delete hWMu_C_DPhi;
    delete hWMu_S_DPhi;
    delete hWEl_C_DPhi;
    delete hWEl_S_DPhi;
    
    // per-dataset control plots (just an example, add more later)
    ofile->cd();
    
    // per-dataset cutflow hists
    std::string hnameWMu = std::string("hWMu_CutFlow_")+dataset.name;
    std::string hnameWEl = std::string("hWEl_CutFlow_")+dataset.name;
    TH1D* hCutFlowWMu = new TH1D(hnameWMu.c_str(), "", nCutsWMu, 0., nCutsWMu);
    TH1D* hCutFlowWEl = new TH1D(hnameWEl.c_str(), "", nCutsWEl, 0., nCutsWEl);

    for (unsigned c=0; c<nCutsWMu; ++c) {

      TCut cut;
      if(c == nCutsWMu-1) //apply trigger correction
        {
    cut = otherCuts * (cutD + cuts.cutflowWMu(c));
          if(isWJets) cut = otherCuts * wWeight * (cuts.cutflowWMu(c));

    if(!(dataset.isData)) cut *= muTightWeight;
        }
      else
        {
    cut = puWeight * yStarWeight * mjjWeight * (cutD + cuts.cutflowWMu(c));
          if(isWJets) cut = puWeight * wWeight * (cuts.cutflowWMu(c));

    if(!(dataset.isData)) cut *= muTightWeight;
        }
      TH1D* h = new TH1D("h","", 1, 0., 1.);
      tree->Draw("0.5>>h", cut);

      hCutFlowWMu->SetBinContent(c+1, h->GetBinContent(1));
      hCutFlowWMu->SetBinError(c+1, h->GetBinError(1));

      delete h;
    }

    for (unsigned c=0; c<nCutsWEl; ++c) {

      TCut cut;
      if(c == nCutsWEl-1) //apply trigger correction
        {
    cut = otherCuts * (cutD + cuts.cutflowWEl(c));
          if(isWJets) cut = otherCuts * wWeight * (cuts.cutflowWEl(c));

    if(!(dataset.isData)) cut *= elTightWeight;
        }
      else
        {
    cut = puWeight * yStarWeight * mjjWeight * (cuts.cutflowWEl(c));
          if(isWJets) cut = puWeight * wWeight * (cuts.cutflowWEl(c));

    if(!(dataset.isData)) cut *= elTightWeight;
        }
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
    if (isWJets || isEwkW) {
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
        dataset.name.compare(0,2,"ZZ")==0 ||
  dataset.name.compare(0,2,"WG")==0) {
      hDibosonWMu->Add(hCutFlowWMu);
      hDibosonWEl->Add(hCutFlowWEl);
    }

    hCutFlowWMu->Write("",TObject::kOverwrite);
    hCutFlowWEl->Write("",TObject::kOverwrite);

    delete hCutFlowWMu;
    delete hCutFlowWEl;

    // For electron and muon channels do control plots:
    // jet1Pt, jet2Pt, jet1Eta, jet2Eta, Mjj, dEtajj, dPhijj, MET and W_mT, W_pT 

    TFile* ofilePlots = TFile::Open( (oDirPlots+std::string("/")+dataset.name+std::string(".root")).c_str(), "RECREATE");

    std::string chan[] = {"Mu", "El"};
    // weight = (dataset.isData) ? 1. : (lumi * dataset.sigma / dataset.nEvents);
    // std::cout << "  weight : " << weight << std::endl;


    for (int c = 0; c < 2; c++){
        TCut Wcut = (c) ? cutWElControlPlot : cutWMuControlPlot; //wWeight included above

        std::string hname = "hW"+chan[c]+"_jet1Pt";
        if (i==0) hnames.push_back(hname);
        TH1D* hJet1Pt = new TH1D(hname.c_str(), "", 50, 0., 1000.);
        std::string str = "jet1Pt>>"+hname;
        tree->Draw(str.c_str(), Wcut);
        hJet1Pt->Scale(weight);
        hJet1Pt->Write("", TObject::kOverwrite); 

        hname = "hW"+chan[c]+"_jet2Pt";
        if (i==0) hnames.push_back(hname);
        TH1D* hJet2Pt = new TH1D(hname.c_str(), "", 50, 0., 1000.);
        str = "jet2Pt>>"+hname;
        tree->Draw(str.c_str(), Wcut);
        hJet2Pt->Scale(weight); 
        hJet2Pt->Write("", TObject::kOverwrite); 

        hname = "hW"+chan[c]+"_jet1Eta";
        if (i==0) hnames.push_back(hname);
        TH1D* hJet1Eta = new TH1D(hname.c_str(), "", 50, -5., 5.);
        str = "jet1Eta>>"+hname;
        tree->Draw(str.c_str(), Wcut);
        hJet1Eta->Scale(weight); 
        hJet1Eta->Write("", TObject::kOverwrite); 

        hname = "hW"+chan[c]+"_jet2Eta";
        if (i==0) hnames.push_back(hname);
        TH1D* hJet2Eta = new TH1D(hname.c_str(), "", 50, -5., 5.);
        str = "jet2Eta>>"+hname;
        tree->Draw(str.c_str(), Wcut);
        hJet2Eta->Scale(weight); 
        hJet2Eta->Write("", TObject::kOverwrite); 

        hname = "hW"+chan[c]+"_Mjj";
        if (i==0) hnames.push_back(hname);
        TH1D* hMjj = new TH1D(hname.c_str(), "", 50, 0., 4000. );
        str = "vbfM>>"+hname;
        tree->Draw(str.c_str(), Wcut);
        hMjj->Scale(weight);
        hMjj->Write("", TObject::kOverwrite);

        hname = "hW"+chan[c]+"_dEtaJJ";
        if (i==0) hnames.push_back(hname);
        TH1D* hDEtaJJ = new TH1D(hname.c_str(), "", 50, 0.,8.);
        str = "vbfDEta>>"+hname;
        tree->Draw(str.c_str(), Wcut);
        hDEtaJJ->Scale(weight); 
        hDEtaJJ->Write("", TObject::kOverwrite); 

        hname = "hW"+chan[c]+"_dPhiJJ";
        if (i==0) hnames.push_back(hname);
        TH1D* hDPhiJJ = new TH1D(hname.c_str(), "", 50, 0., TMath::Pi());
        str = "vbfDPhi>>"+hname;
        tree->Draw(str.c_str(), Wcut);
        hDPhiJJ->Scale(weight); 
        hDPhiJJ->Write("", TObject::kOverwrite); 
        
        hname = "hW"+chan[c]+"_MET";
        if (i==0) hnames.push_back(hname);
        TH1D* hMET = new TH1D(hname.c_str(), "", 50, 0., 500. );
        if (c)
          str = "met>>"+hname;
        else
          str = "metNoWLepton>>"+hname;
        tree->Draw(str.c_str(), Wcut);
        hMET->Scale(weight);
        hMET->Write("", TObject::kOverwrite);

        hname = "hW"+chan[c]+"_WmT";
        if (i==0) hnames.push_back(hname);
        TH1D* hWmT = new TH1D(hname.c_str(), "", 20, 0., 200.);
        str = "wMt>>"+hname;
        tree->Draw(str.c_str(), Wcut);
        hWmT->Scale(weight); 
        hWmT->Write("", TObject::kOverwrite); 

        hname = "hW"+chan[c]+"_WpT";
        if (i==0) hnames.push_back(hname);
        TH1D* hWpT = new TH1D(hname.c_str(), "", 25, 0., 500. );
        str = "wPt>>"+hname;
        tree->Draw(str.c_str(), Wcut);
        hWpT->Scale(weight); 
        hWpT->Write("", TObject::kOverwrite); 

    }   

    delete tree;
    ofilePlots->Close();

    file->Close();
   
  }

  // W->mu
  // stat only histograms
  TH1D* hWMu_R_DPhi          = new TH1D("hWMu_R_DPhi", "", 3, dphiEdges);  // ratio of sngl/ctrl
  TH1D* hWMu_EstC_DPhi       = new TH1D("hWMu_EstC_DPhi", "", 3, dphiEdges); // estimated W in ctrl region
  TH1D* hWMu_EstS_DPhi       = new TH1D("hWMu_EstS_DPhi", "", 3, dphiEdges); // estimated W in signal region

  // syst only histograms
  TH1D* hWMu_BGC_DPhi_Syst   = new TH1D("hWMu_BGC_DPhi_Syst", "", 3, dphiEdges);
  TH1D* hWMu_DataC_DPhi_Syst = new TH1D("hWMu_DataC_DPhi_Syst", "", 3, dphiEdges);
  TH1D* hWMu_R_DPhi_Syst     = new TH1D("hWMu_R_DPhi_Syst", "", 3, dphiEdges);  // ratio of sngl/ctrl
  TH1D* hWMu_EstC_DPhi_Syst  = new TH1D("hWMu_EstC_DPhi_Syst", "", 3, dphiEdges); // estimated W in ctrl region
  TH1D* hWMu_EstS_DPhi_Syst  = new TH1D("hWMu_EstS_DPhi_Syst", "", 3, dphiEdges); // estimated W in signal region

  // stat only calculation
  hWMu_R_DPhi->Divide(hWMu_MCS_DPhi, hWMu_MCC_DPhi, 1., 1.);
  for (int i=1; i<=hWMu_R_DPhi->GetNbinsX(); ++i) hWMu_R_DPhi->SetBinError(i,0.);  // ignore MC stats
  hWMu_BGC_DPhi_Syst->Add(hWMu_BGC_DPhi, 1.);  // take a copy for the syst only
  for (int i=1; i<=hWMu_BGC_DPhi->GetNbinsX(); ++i) hWMu_BGC_DPhi->SetBinError(i,0.);  // ignore MC stats
  hWMu_EstC_DPhi->Add(hWMu_DataC_DPhi, hWMu_BGC_DPhi, 1., -1.);
  hWMu_EstS_DPhi->Multiply(hWMu_EstC_DPhi, hWMu_R_DPhi, 1., 1.);

  // syst only calculation
  hWMu_DataC_DPhi_Syst->Add(hWMu_DataC_DPhi, 1.);
  for (int i=1; i<=hWMu_DataC_DPhi_Syst->GetNbinsX(); ++i) hWMu_DataC_DPhi_Syst->SetBinError(i,0.); // ignore data stats
  hWMu_R_DPhi_Syst->Divide(hWMu_MCS_DPhi, hWMu_MCC_DPhi, 1., 1.);
  hWMu_EstC_DPhi_Syst->Add(hWMu_DataC_DPhi_Syst, hWMu_BGC_DPhi_Syst, 1., -1.);
  hWMu_EstS_DPhi_Syst->Multiply(hWMu_EstC_DPhi_Syst, hWMu_R_DPhi_Syst, 1., 1.);

  // W->e
  // stat only histograms
  TH1D* hWEl_R_DPhi          = new TH1D("hWEl_R_DPhi", "", 3, dphiEdges);
  TH1D* hWEl_EstC_DPhi       = new TH1D("hWEl_EstC_DPhi", "", 3, dphiEdges);
  TH1D* hWEl_EstS_DPhi       = new TH1D("hWEl_EstS_DPhi", "", 3, dphiEdges);

  // syst only histograms
  TH1D* hWEl_BGC_DPhi_Syst   = new TH1D("hWEl_BGC_DPhi_Syst", "", 3, dphiEdges);
  TH1D* hWEl_DataC_DPhi_Syst = new TH1D("hWEl_DataC_DPhi_Syst", "", 3, dphiEdges);
  TH1D* hWEl_R_DPhi_Syst     = new TH1D("hWEl_R_DPhi_Syst", "", 3, dphiEdges);
  TH1D* hWEl_EstC_DPhi_Syst  = new TH1D("hWEl_EstC_DPhi_Syst", "", 3, dphiEdges);
  TH1D* hWEl_EstS_DPhi_Syst  = new TH1D("hWEl_EstS_DPhi_Syst", "", 3, dphiEdges);

  // stat only calculation
  hWEl_R_DPhi->Divide(hWEl_MCS_DPhi, hWEl_MCC_DPhi, 1., 1.);
  for (int i=1; i<=hWEl_R_DPhi->GetNbinsX(); ++i) hWEl_R_DPhi->SetBinError(i,0.);
  hWEl_BGC_DPhi_Syst->Add(hWEl_BGC_DPhi, 1.);  // take a copy for the syst only
  for (int i=1; i<=hWEl_BGC_DPhi->GetNbinsX(); ++i) hWEl_BGC_DPhi->SetBinError(i,0.);  // ignore MC BG uncertainty
  hWEl_EstC_DPhi->Add(hWEl_DataC_DPhi, hWEl_BGC_DPhi, 1., -1.);
  hWEl_EstS_DPhi->Multiply(hWEl_EstC_DPhi, hWEl_R_DPhi, 1., 1.);

  // syst only calculation
  hWEl_DataC_DPhi_Syst->Add(hWEl_DataC_DPhi, 1.);
  for (int i=1; i<=hWEl_DataC_DPhi_Syst->GetNbinsX(); ++i) hWEl_DataC_DPhi_Syst->SetBinError(i,0.); // ignore data stats
  hWEl_R_DPhi_Syst->Divide(hWEl_MCS_DPhi, hWEl_MCC_DPhi, 1., 1.);
  hWEl_EstC_DPhi_Syst->Add(hWEl_DataC_DPhi_Syst, hWEl_BGC_DPhi_Syst, 1., -1.);
  hWEl_EstS_DPhi_Syst->Multiply(hWEl_EstC_DPhi_Syst, hWEl_R_DPhi_Syst, 1., 1.);


  std::cout << std::endl;
  /*
  std::cout << "W->mu channel (dphi>2.6)" << std::endl;
  std::cout << "  W+jets MC  ctrl region : " << hWMu_MCC_DPhi->GetBinContent(3) << " +/- " << hWMu_MCC_DPhi->GetBinError(3) << std::endl;
  std::cout << "  W+jets MC  sgnl region : " << hWMu_MCS_DPhi->GetBinContent(3) << " +/- " << hWMu_MCS_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Background ctrl region : " << hWMu_BGC_DPhi->GetBinContent(3) << " +/- " << hWMu_BGC_DPhi->GetBinError(3) << std::endl;
  //  std::cout << "  Background sgnl region : " << hWMu_BGS_DPhi->GetBinContent(3) << " +/- " << hWMu_BGS_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Data ctrl region       : " << hWMu_DataC_DPhi->GetBinContent(3) << " +/- " << hWMu_DataC_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  W in ctrl region       : " << hWMu_EstC_DPhi->GetBinContent(3) << " +/- " << hWMu_EstC_DPhi->GetBinError(3) << std::endl;
  std::cout << "  N_S(MC)/N_C(MC)        : " << hWMu_R_DPhi->GetBinContent(3) << " +/- " << hWMu_R_DPhi->GetBinError(3) << std::endl;
  std::cout << "  W in sgnl region       : " << hWMu_EstS_DPhi->GetBinContent(3) << " +/- " << hWMu_EstS_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl << std::endl;
  std::cout << "W->el channel (dphi>2.6)" << std::endl;
  std::cout << "  W+jets MC  ctrl region : " << hWEl_MCC_DPhi->GetBinContent(3) << " +/- " << hWEl_MCC_DPhi->GetBinError(3) << std::endl;
  std::cout << "  W+jets MC  sgnl region : " << hWEl_MCS_DPhi->GetBinContent(3) << " +/- " << hWEl_MCS_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Background ctrl region : " << hWEl_BGC_DPhi->GetBinContent(3) << " +/- " << hWEl_BGC_DPhi->GetBinError(3) << std::endl;
  //  std::cout << "  Background sgnl region : " << hWEl_BGS_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  Data ctrl region       : " << hWEl_DataC_DPhi->GetBinContent(3) << " +/- " << hWEl_DataC_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  W in ctrl region       : " << hWEl_EstC_DPhi->GetBinContent(3) << " +/- " << hWEl_EstC_DPhi->GetBinError(3) << std::endl;
  std::cout << "  N_S(MC)/N_C(MC)        : " << hWEl_R_DPhi->GetBinContent(3) << " +/- " << hWEl_R_DPhi->GetBinError(3) << std::endl;
  std::cout << "  W in sgnl region       : " << hWEl_EstS_DPhi->GetBinContent(3) << " +/- " << hWEl_EstS_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl << std::endl;
  */
  std::cout << "W->mu channel (dphi<1.0)" << std::endl;
  std::cout << "  W+jets MC  ctrl region : " << hWMu_MCC_DPhi->GetBinContent(1) << " +/- 0 (stat) +/- " << hWMu_MCC_DPhi->GetBinError(1) << " (syst)" << std::endl;
  std::cout << "  W+jets MC  sgnl region : " << hWMu_MCS_DPhi->GetBinContent(1) << " +/- 0 (stat) +/- " << hWMu_MCS_DPhi->GetBinError(1) << " (syst)" << std::endl;
  std::cout << "  Background ctrl region : " << hWMu_BGC_DPhi->GetBinContent(1) << " +/- " << hWMu_BGC_DPhi->GetBinError(1) << " (stat) +/- " << hWMu_BGC_DPhi_Syst->GetBinError(1) << " (syst)" << std::endl;
  //  std::cout << "  Background sgnl region : " << hWMu_BGS_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  Data ctrl region       : " << hWMu_DataC_DPhi->GetBinContent(1) << " +/- " << hWMu_DataC_DPhi->GetBinError(1) << " (stat) +/- " << hWMu_DataC_DPhi_Syst->GetBinError(1) << " (syst)" << std::endl;
  std::cout << std::endl;
  std::cout << "  W in ctrl region       : " << hWMu_EstC_DPhi->GetBinContent(1) << " +/- " << hWMu_EstC_DPhi->GetBinError(1) << " (stat) +/- " << hWMu_EstC_DPhi_Syst->GetBinError(1) << " (syst)" << std::endl;
  std::cout << "  N_S(MC)/N_C(MC)        : " << hWMu_R_DPhi->GetBinContent(1) << " +/- " << hWMu_R_DPhi->GetBinError(1) << " (stat) +/- " << hWMu_R_DPhi_Syst->GetBinError(1) << " (syst)" << std::endl;
  std::cout << "  W in sgnl region       : " << hWMu_EstS_DPhi->GetBinContent(1) << " +/- " << hWMu_EstS_DPhi->GetBinError(1) << " (stat) +/- " << hWMu_EstS_DPhi_Syst->GetBinError(1) << " (syst)" << std::endl;
  std::cout << std::endl << std::endl;

  std::cout << "W->el channel (dphi<1.0)" << std::endl;
  std::cout << "  W+jets MC  ctrl region : " << hWEl_MCC_DPhi->GetBinContent(1) << " +/- 0 (stat) +/- " << hWEl_MCC_DPhi->GetBinError(1) << " (syst)" << std::endl;
  std::cout << "  W+jets MC  sgnl region : " << hWEl_MCS_DPhi->GetBinContent(1) << " +/- 0 (stat) +/- " << hWEl_MCS_DPhi->GetBinError(1) << " (syst)" << std::endl;
  std::cout << "  Background ctrl region : " << hWEl_BGC_DPhi->GetBinContent(1) << " +/- " << hWEl_BGC_DPhi->GetBinError(1) << " (stat) +/- " << hWEl_BGC_DPhi_Syst->GetBinError(1) << " (syst)" << std::endl;
  //  std::cout << "  Background sgnl region : " << hWEl_BGS_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  Data ctrl region       : " << hWEl_DataC_DPhi->GetBinContent(1) << " +/- " << hWEl_DataC_DPhi->GetBinError(1) << " (stat) +/- " << hWEl_DataC_DPhi_Syst->GetBinError(1) << " (syst)" << std::endl;
  std::cout << std::endl;
  std::cout << "  W in ctrl region       : " << hWEl_EstC_DPhi->GetBinContent(1) << " +/- " << hWEl_EstC_DPhi->GetBinError(1) << " (stat) +/- " << hWEl_EstC_DPhi_Syst->GetBinError(1) << " (syst)" << std::endl;
  std::cout << "  N_S(MC)/N_C(MC)        : " << hWEl_R_DPhi->GetBinContent(1) << " +/- " << hWEl_R_DPhi->GetBinError(1) << " (stat) +/- " << hWEl_R_DPhi_Syst->GetBinError(1) << " (syst)" << std::endl;
  std::cout << "  W in sgnl region       : " << hWEl_EstS_DPhi->GetBinContent(1) << " +/- " << hWEl_EstS_DPhi->GetBinError(1) << " (stat) +/- " << hWEl_EstS_DPhi_Syst->GetBinError(1) << " (syst)" << std::endl;
  std::cout << std::endl << std::endl;



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
  

for (unsigned n = 0; n < hnames.size(); n++)
  std::cout << hnames[n] <<std::endl;

  // Do control plots
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

  // sum single top datasets
  std::vector<std::string> dibDatasets;
  dibDatasets.push_back(std::string("WW"));
  dibDatasets.push_back(std::string("WZ"));
  dibDatasets.push_back(std::string("ZZ"));
  dibDatasets.push_back(std::string("WG"));
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
  plots.setLegPos(0.70,0.60,0.93,0.89);

  plots.addDataset("DiBoson", kViolet-6, 0);
  plots.addDataset("QCD", kGreen+3, 0);
  plots.addDataset("DYJets+EWK", kPink-4,0);
  plots.addDataset("SingleT+TTBar", kAzure-2, 0);
  plots.addDataset("WNJets+EWK", kGreen-3, 0);
  plots.addDataset("METABCD", kBlack, 1);

  std::string chan[] = {"Mu", "El"};
  for (int c = 0; c < 2; c++){
    std::string label = (c) ? "e channel" : "#mu channel";
    plots.setLabel(label.c_str());
    std::string hname = "hW"+chan[c]+"_jet1Pt";
    plots.draw(hname.c_str(), "Leading jet p_{T} [GeV]", "N_{events}", true,"RATIO");

    hname = "hW"+chan[c]+"_jet2Pt";
    plots.draw(hname.c_str(), "Sub-leading jet p_{T} [GeV]", "N_{events}",true,"RATIO");

    hname = "hW"+chan[c]+"_jet1Eta";
    if (c == 0) plots.setYMax(1E5);
    plots.draw(hname.c_str(), "Leading jet #eta", "N_{events}",true,"RATIO");
    // if (c == 0) plots.setYMax(-1);

    if (c == 0) plots.setYMax(1E5); else plots.setYMax(3E4);
    hname = "hW"+chan[c]+"_jet2Eta";
    plots.draw(hname.c_str(), "Sub-leading jet #eta", "N_{events}",true,"RATIO");
    // plots.setYMax(-1);

    hname = "hW"+chan[c]+"_Mjj";
    plots.draw(hname.c_str(), "M_{jj} [GeV]", "N_{events}",true,"RATIO");

    hname = "hW"+chan[c]+"_dEtaJJ";
    plots.draw(hname.c_str(), "#Delta #eta_{jj}", "N_{events}",true,"RATIO");

    hname = "hW"+chan[c]+"_dPhiJJ";
    if (c == 1) plots.setYMax(2E4);
    plots.draw(hname.c_str(), "#Delta #phi_{jj}", "N_{events}",true,"RATIO");
    // plots.setYMax(-1);

    hname = "hW"+chan[c]+"_MET";
    plots.draw(hname.c_str(), "E_{T}^{miss} [GeV]", "N_{events}",true,"RATIO");

    hname = "hW"+chan[c]+"_WmT";
    plots.draw(hname.c_str(), "M^{W}_{T} [GeV]", "N_{events}",true,"RATIO");

    hname = "hW"+chan[c]+"_WpT";
    plots.draw(hname.c_str(), "p^{W}_{T} [GeV]", "N_{events}",true,"RATIO");

  }

  // store histograms
  ofile->cd(); 
    
  hWMu_MCC_DPhi->Write("",TObject::kOverwrite);
  hWMu_MCS_DPhi->Write("",TObject::kOverwrite);
  hWMu_BGC_DPhi->Write("",TObject::kOverwrite); 
  //  hWMu_BGS_DPhi->Write("",TObject::kOverwrite);
  hWMu_DataC_DPhi->Write("",TObject::kOverwrite);
  hWMu_R_DPhi->Write("",TObject::kOverwrite);
  hWMu_R_DPhi_Syst->Write("",TObject::kOverwrite);
  hWMu_EstC_DPhi->Write("",TObject::kOverwrite);
  hWMu_EstC_DPhi_Syst->Write("",TObject::kOverwrite);
  hWMu_EstS_DPhi->Write("",TObject::kOverwrite);
  hWMu_EstS_DPhi_Syst->Write("",TObject::kOverwrite);
  
  hWEl_MCC_DPhi->Write("",TObject::kOverwrite); 
  hWEl_MCS_DPhi->Write("",TObject::kOverwrite); 
  hWEl_BGC_DPhi->Write("",TObject::kOverwrite);
  //  hWEl_BGS_DPhi->Write("",TObject::kOverwrite);
  hWEl_DataC_DPhi->Write("",TObject::kOverwrite);
  hWEl_R_DPhi->Write("",TObject::kOverwrite);
  hWEl_R_DPhi_Syst->Write("",TObject::kOverwrite);
  hWEl_EstC_DPhi->Write("",TObject::kOverwrite);
  hWEl_EstC_DPhi_Syst->Write("",TObject::kOverwrite);
  hWEl_EstS_DPhi->Write("",TObject::kOverwrite);
  hWEl_EstS_DPhi_Syst->Write("",TObject::kOverwrite);


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

  ofile->Close();    

}
