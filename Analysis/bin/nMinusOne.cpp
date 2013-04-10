#include "InvisibleHiggs/Analysis/interface/ProgramOptions.h"
#include "InvisibleHiggs/Analysis/interface/Cuts.h"
#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"
#include "InvisibleHiggs/Analysis/interface/StackPlot.h"
#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"
#include "InvisibleHiggs/Analysis/interface/Datasets.h"

#include "TTree.h"
#include "TMath.h"
#include "TCanvas.h"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>

int main(int argc, char* argv[]) {

  TH1::SetDefaultSumw2();

  ProgramOptions options(argc, argv);

  double lumi = options.lumi;
  std::string iDir = options.iDir;
  std::string oDir = options.oDir+std::string("/NMinusOne");

  boost::filesystem::path opath(oDir);
  if (!exists(opath)) {
    std::cout << "Creating output directory : " << oDir << std::endl;
    boost::filesystem::create_directory(opath);
  }
  else std::cout << "Writing results to " << oDir << std::endl;

  Datasets datasets;
  datasets.readFile(options.datasetFile);

  // cuts
  Cuts cuts;
  TCut puWeight("puWeight");
  TCut trigCorr("trigCorrWeight");
  TCut trig      = cuts.nMinusOneCuts("trigger");
  TCut metFilt   = cuts.nMinusOneCuts("metFilter");
  TCut eVeto     = cuts.nMinusOneCuts("EVeto");
  TCut muVeto    = cuts.nMinusOneCuts("MuVeto");
  TCut dijet     = cuts.nMinusOneCuts("dijet");
  TCut sgnEtaJJ  = cuts.nMinusOneCuts("sgnEtaJJ");
  TCut dEtaJJ    = cuts.nMinusOneCuts("dEtaJJ");
  TCut met       = cuts.nMinusOneCuts("MET");
  TCut mJJ       = cuts.nMinusOneCuts("Mjj");
  TCut dPhiJJ    = cuts.nMinusOneCuts("dPhiJJ");


  // loop over datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);

    if (dataset.isData) continue;

    TFile* ifile = TFile::Open( (iDir+std::string("/")+dataset.name+std::string(".root")).c_str(), "READ");

    if (ifile==0) {
      std::cerr << "No file for " << dataset.name << std::endl;
      continue;
    }

    std::cout << "Making histograms for " << dataset.name << std::endl;
    
    TTree* tree = (TTree*) ifile->Get("invHiggsInfo/InvHiggsInfo");

    TFile* ofile = TFile::Open( (oDir+std::string("/")+dataset.name+std::string(".root")).c_str(), "UPDATE");

    // create histograms
    TH1D* hTrig     = new TH1D("hTrigNM1",     "", 2,   0.,  2.);
    TH1D* hMETfilt  = new TH1D("hMETFiltNM1",  "", 2,   0.,  2.);
    TH1D* hDijet    = new TH1D("hDijetNM1",    "", 50,  0.,  250.);
    TH1D* hSgnEtaJJ = new TH1D("hSgnEtaJJNM1", "", 2,   -1., 1.);
    TH1D* hDEtaJJ   = new TH1D("hDEtaJJNM1",   "", 50,  0.,  8.);
    TH1D* hMjj      = new TH1D("hMjjNM1",      "", 50,  0.,  4000.);
    TH1D* hMET      = new TH1D("hMETNM1",      "", 50,  0.,  500.);
    TH1D* hDPhiJMet = new TH1D("hDPhiJMetNM1", "", 50,  0.,  TMath::Pi());
    TH1D* hDPhiJJ   = new TH1D("hDPhiJJNM1",   "", 50,  0.,  TMath::Pi());
    TH1D* hEVeto    = new TH1D("hEVetoNM1",    "", 50,  0.,  50.);
    TH1D* hMuVeto   = new TH1D("hMuVetoNM1",   "", 50,  0.,  50.);

    // Additional cuts specific to DYJetsToLL (cut on Zpt <100 to avoid double counting with the PtZ-100 sample)
    TCut cutD = cuts.cutDataset(dataset.name);

    // Additional WJets corrections (so can use inclusive & exclusive samples)
    TCut wWeight("");
    if (dataset.name.compare(0,1,"W") == 0) { 
      wWeight =  cuts.wWeight();
    } 

    TCut otherCuts = puWeight * trigCorr * wWeight;

    // fill histograms
    int n = tree->Draw("hltResult2>>hTrigNM1", (trig + cutD) * otherCuts);
    std::cout << "   " << n << " events passing trigger" << std::endl;

    tree->Draw("(metflag1 && metflag2 && metflag3 && metflag4 && metflag5 && metflag6)>>hMETFiltNM1", (metFilt + cutD) * otherCuts);
    tree->Draw("jet2Pt>>hDijetNM1", (dijet + cutD) * otherCuts);
    tree->Draw("TMath::Sign(1., jet1Eta*jet2Eta)>>hSgnEtaJJNM1", (sgnEtaJJ + cutD) * otherCuts);
    tree->Draw("abs(jet1Eta-jet2Eta)>>hDEtaJJNM1", (dEtaJJ + cutD) * otherCuts);
    tree->Draw("vbfM>>hMjjNM1", (mJJ  + cutD) * otherCuts);
    tree->Draw("met>>hMETNM1", (met + cutD) * otherCuts);
    tree->Draw("min(abs(abs(abs(metPhi-jet1Phi)-TMath::Pi())-TMath::Pi()), abs(abs(abs(metPhi-jet2Phi)-TMath::Pi())-TMath::Pi()))>>hDPhiJMetNM1", (dPhiJJ + cutD) * otherCuts);
    tree->Draw("vbfDPhi>>hDPhiJJNM1", (dPhiJJ + cutD) * otherCuts);
    tree->Draw("ele1Pt>>hEVetoNM1", (eVeto + cutD) * otherCuts);
    tree->Draw("mu1Pt>>hMuVetoNM1", (muVeto + cutD) * otherCuts);

    // Scale MC according to luminosity
    if (!dataset.isData) {
      double weight = lumi * dataset.sigma / dataset.nEvents;
      hTrig->Scale(weight);
      hMETfilt->Scale(weight);
      hDijet->Scale(weight);
      hSgnEtaJJ->Scale(weight);
      hDEtaJJ->Scale(weight);
      hMjj->Scale(weight);
      hMET->Scale(weight);
      hDPhiJMet->Scale(weight);
      hDPhiJJ->Scale(weight);
      hEVeto->Scale(weight);
      hMuVeto->Scale(weight);
    }    

    // write histograms
    hTrig->Write("",TObject::kOverwrite);
    hMETfilt->Write("",TObject::kOverwrite);
    hDijet->Write("",TObject::kOverwrite);
    hSgnEtaJJ->Write("",TObject::kOverwrite);
    hDEtaJJ->Write("",TObject::kOverwrite);
    hMjj->Write("",TObject::kOverwrite);
    hMET->Write("",TObject::kOverwrite);
    hDPhiJMet->Write("",TObject::kOverwrite);
    hDPhiJJ->Write("",TObject::kOverwrite);
    hEVeto->Write("",TObject::kOverwrite);
    hMuVeto->Write("",TObject::kOverwrite);
    
    ofile->Close();
    
    ifile->Close();

  }

  // list of histograms for summing
  std::vector<std::string> hists;
  hists.push_back("hTrigNM1");
  hists.push_back("hMETFiltNM1");
  hists.push_back("hDijetNM1");
  hists.push_back("hSgnEtaJJNM1");
  hists.push_back("hDEtaJJNM1");
  hists.push_back("hMjjNM1");
  hists.push_back("hMETNM1");
  hists.push_back("hDPhiJMetNM1");
  hists.push_back("hDPhiJJNM1");
  hists.push_back("hEVetoNM1");
  hists.push_back("hMuVetoNM1");

  // sum Z+jets
  std::cout << "Summing histograms for Z+Jets" << std::endl;
  std::vector<std::string> zJets;
  zJets.push_back("Zvv_50to100");
  zJets.push_back("Zvv_100to200");
  zJets.push_back("Zvv_200to400");
  zJets.push_back("Zvv_400toinf");
  
  SumDatasets(oDir,
	      zJets,
	      hists,
	      "ZJets");

  // sum W+jets datasets
  std::cout << "Summing histograms for W+Jets" << std::endl;
  std::vector<std::string> wJets;
  wJets.push_back(std::string("WJets"));
  wJets.push_back(std::string("W1Jets"));
  wJets.push_back(std::string("W2Jets"));
  wJets.push_back(std::string("W3Jets"));
  wJets.push_back(std::string("W4Jets"));
  SumDatasets(oDir,
	      wJets,
	      hists,
	      "WNJets");

  // sum QCD
  std::cout << "Summing histograms for QCD" << std::endl;
  std::vector<std::string> qcd;
  qcd.push_back("QCD_Pt30to50");
  qcd.push_back("QCD_Pt50to80");
  qcd.push_back("QCD_Pt80to120");
  qcd.push_back("QCD_Pt120to170");
  qcd.push_back("QCD_Pt170to300");
  qcd.push_back("QCD_Pt300to470");
  qcd.push_back("QCD_Pt470to600");
  qcd.push_back("QCD_Pt600to800");
  qcd.push_back("QCD_Pt800to1000");
  qcd.push_back("QCD_Pt1000to1400");
  qcd.push_back("QCD_Pt1400to1800");
  qcd.push_back("QCD_Pt1800");
 
  SumDatasets(oDir,
	      qcd,
	      hists,
	      "QCD");

  // Sum Single t and tbar
  std::cout << "Summing histograms for Single T and TTbar" << std::endl;
  std::vector<std::string> singleT;
  singleT.push_back("TTBar");
  singleT.push_back("SingleT_t");
  singleT.push_back("SingleT_s");
  singleT.push_back("SingleT_tW");
  singleT.push_back("SingleTbar_t");
  singleT.push_back("SingleTbar_s");
  singleT.push_back("SingleTbar_tW");
  SumDatasets(oDir,singleT,hists,"SingleT+TTbar");

  // Sum diboson
  std::cout << "Summing histograms for Diboson" << std::endl;
  std::vector<std::string> diboson;
  diboson.push_back("WW");
  diboson.push_back("WZ");
  diboson.push_back("ZZ");
  SumDatasets(oDir,diboson,hists,"Diboson");

  //Sum DYJetsToLL
  std::cout << "Summing histograms for DYJetsToLL" << std::endl;
  std::vector<std::string> dyjets;
  dyjets.push_back("DYJetsToLL");
  dyjets.push_back("DYJetsToLL_PtZ-100");
  dyjets.push_back("DYJetsToLL_EWK");
  SumDatasets(oDir,dyjets,hists,"DYJets");

  // make plots
  std::cout << "Making plots" << std::endl;
  StackPlot plots(oDir);
  plots.setLegPos(0.69,0.67,0.98,0.97);
  // plots.setLabel("CMS Preliminary 2012 #int L = 19.56 fb^{-1}");

  plots.addDataset("Diboson", kViolet-6, 0);
  plots.addDataset("DYJets", kPink-4,0);
  plots.addDataset("QCD", kGreen+3, 0);
  plots.addDataset("SingleT+TTbar", kAzure-2, 0);
  plots.addDataset("ZJets", kOrange-2, 0);
  plots.addDataset("WNJets", kBlue+1, 0);
  plots.addDataset("SignalM120_PYTHIA", kRed, 2);

  plots.draw("hTrigNM1", "", "");
  plots.draw("hMETFiltNM1", "", "");
  plots.draw("hDijetNM1", "Sub-leading jet p_{T} [GeV]", "Entries per bin");
  plots.draw("hSgnEtaNM1", "#eta_{1}#times#eta_{2}", "Entries per bin");
  plots.draw("hDEtaJJNM1", "#Delta #eta_{jj}", "Entries per bin");
  plots.draw("hMjjNM1", "M_{jj} [GeV]", "Entries per bin");
  plots.draw("hMETNM1", "#slash{E}_{T} [GeV]", "Entries per bin");
  plots.draw("hDPhiJMetNM1", "#Delta #phi_{j-#slash{E}_{T}}", "Entries per bin");
  plots.draw("hDPhiJJNM1", "#Delta #phi_{jj}", "Entries per bin");

  plots.setYMin(1e-1);
  plots.setYMax(1e2);
  plots.draw("hEVetoNM1", "E_{T} [GeV]", "N_{evt}");
  plots.draw("hMuVetoNM1", "p_{T} [GeV]", "N_{evt}");

}
