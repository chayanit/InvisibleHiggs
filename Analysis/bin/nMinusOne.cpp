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

namespace po = boost::program_options;

int main(int argc, char* argv[]) {

  // some variables
  double lumi = 697.;  //pb-1
  std::string iDir("/storage/phjjb/invisibleHiggs/InvHiggsInfo_v8");
  std::string oDir("analysis_v8");
  std::string datasetFile("InvisibleHiggs/Ntuple/data/datasets_v8.txt");

  // program options
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "Display this message")
    ("outdir,o", po::value<std::string>(), "Output directory")
    ("indir,d", po::value<std::string>(), "Input directory")
    ("datasets,f", po::value<std::string>(), "Datasets file")
    ("lumi,l", po::value<double>(), "Integrated luminosity");

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);  
  po::notify(vm);

  // help
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    std::exit(1);
  }
  
  if (vm.count("outdir"))   oDir = vm["outdir"].as<std::string>();
  if (vm.count("indir"))    iDir=vm["datadir"].as<std::string>();
  if (vm.count("datasets")) datasetFile=vm["datasets"].as<std::string>();
  if (vm.count("lumi"))     lumi=vm["lumi"].as<double>();

  // create output directory if it doesn't exist already
  boost::filesystem::path opath(oDir);
  if (!exists(opath)) {
    std::cout << "Creating output directory : " << oDir << std::endl;
    boost::filesystem::create_directory(opath);
  }
  else std::cout << "Writing results to " << oDir << std::endl;

  std::cout << "Integrated luminosity : " << lumi << " pb-1" << std::endl;

  Datasets datasets;
  datasets.readFile(datasetFile);

  // cuts
  Cuts cuts;
  TCut puWeight("puWeight");

  // loop over datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);

    TFile* ifile = TFile::Open( (iDir+std::string("/")+dataset.name+std::string(".root")).c_str(), "READ");

    if (ifile==0) {
      //      std::cerr << "No file for " << dataset.name << std::endl;
      continue;
    }

    std::cout << "Making histograms for " << dataset.name << std::endl;
    
    TTree* tree = (TTree*) ifile->Get("invHiggsInfo/InvHiggsInfo");

    TFile* ofile = TFile::Open( (oDir+std::string("/")+dataset.name+std::string(".root")).c_str(), "UPDATE");

    // create histograms
    TH1D* hTrig     = new TH1D("hTrigNM1",     "", 2,   0.,  2.);
    TH1D* hMETfilt  = new TH1D("hMETFiltNM1",  "", 2,   0.,  2.);
    TH1D* hDijet    = new TH1D("hDijetNM1",    "", 100, 0.,  200.);
    TH1D* hSgnEtaJJ = new TH1D("hSgnEtaJJNM1", "", 2,   -1., 1.);
    TH1D* hDEtaJJ   = new TH1D("hDEtaJJNM1",   "", 50,  0.,  10.);
    TH1D* hMjj      = new TH1D("hMjjNM1",      "", 100, 0.,  3000.);
    TH1D* hMET      = new TH1D("hMETNM1",      "", 100, 0.,  200.);
    TH1D* hDPhiJM   = new TH1D("hDPhiJMNM1",   "", 50,  0.,  TMath::Pi());
    TH1D* hEVeto    = new TH1D("hEVetoNM1",    "", 50,  0.,  50.);
    TH1D* hMuVeto   = new TH1D("hMuVetoNM1",   "", 50,  0.,  50.);

    // fill histograms
    TCut trig = puWeight * cuts.nMinusOneCuts(0);
    int n = tree->Draw("hltResult2>>hTrigNM1", trig);
    std::cout << "   " << n << " events passing trigger" << std::endl;

    TCut metFilt = puWeight * cuts.nMinusOneCuts(1);
    tree->Draw("(metflag1 && metflag2 && metflag3 && metflag4 && metflag5 && metflag6)>>hMETFiltNM1", metFilt);

    TCut dijet = puWeight * cuts.nMinusOneCuts(2);
    tree->Draw("jet2Pt>>hDijetNM1", dijet);

    TCut sgnEtaJJ = puWeight * cuts.nMinusOneCuts(3);
    tree->Draw("TMath::Sign(1., jet1Eta*jet2Eta)>>hSgnEtaJJNM1", sgnEtaJJ);

    TCut dEtaJJ = puWeight * cuts.nMinusOneCuts(4);
    tree->Draw("abs(jet1Eta-jet2Eta)>>hDEtaJJNM1", dEtaJJ);

    TCut mJJ = puWeight * cuts.nMinusOneCuts(5);
    tree->Draw("vbfM>>hMjjNM1", mJJ);

    TCut met = puWeight * cuts.nMinusOneCuts(6);
    tree->Draw("met>>hMETNM1", met);

    TCut dPhiJM = puWeight * cuts.nMinusOneCuts(7);
    tree->Draw("min(abs(abs(abs(metPhi-jet1Phi)-TMath::Pi())-TMath::Pi()), abs(abs(abs(metPhi-jet2Phi)-TMath::Pi())-TMath::Pi()))>>hDPhiJMNM1", dPhiJM);

    TCut eVeto = puWeight * cuts.nMinusOneCuts(8);
    tree->Draw("ele1Pt>>hEVetoNM1", eVeto);

    TCut muVeto = puWeight * cuts.nMinusOneCuts(9);
    tree->Draw("mu1Pt>>hMuVetoNM1", muVeto);

    // write histograms
    hTrig->Write("",TObject::kOverwrite);
    hMETfilt->Write("",TObject::kOverwrite);
    hDijet->Write("",TObject::kOverwrite);
    hSgnEtaJJ->Write("",TObject::kOverwrite);
    hDEtaJJ->Write("",TObject::kOverwrite);
    hMjj->Write("",TObject::kOverwrite);
    hMET->Write("",TObject::kOverwrite);
    hDPhiJM->Write("",TObject::kOverwrite);
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
  hists.push_back("hDPhiJMNM1");
  hists.push_back("hEVetoNM1");
  hists.push_back("hMuVetoNM1");

  // sum Z+jets
  std::cout << "Summing histograms for Z+Jets" << std::endl;
  std::vector<std::string> zJets;
  zJets.push_back("ZJetsToNuNu_50_HT_100_MET");
  zJets.push_back("ZJetsToNuNu_100_HT_200_MET");
  zJets.push_back("ZJetsToNuNu_200_HT_400_MET");
  zJets.push_back("ZJetsToNuNu_400_HT_inf_MET");
  
  SumDatasets(oDir,
	      zJets,
	      hists,
	      "ZJetsToLL_MET");

  // sum QCD
  std::cout << "Summing histograms for QCD" << std::endl;
  std::vector<std::string> qcd;
  qcd.push_back("QCD_Pt-30to50_MET");
  qcd.push_back("QCD_Pt-50to80_MET");
  qcd.push_back("QCD_Pt-80to120_MET");
  qcd.push_back("QCD_Pt-120to170_MET");
  qcd.push_back("QCD_Pt-170to300_MET");
  qcd.push_back("QCD_Pt-300to470_MET");
  qcd.push_back("QCD_Pt-470to600_MET");
  qcd.push_back("QCD_Pt-600to800_MET");
  qcd.push_back("QCD_Pt-800to1000_MET");
  qcd.push_back("QCD_Pt-1000to1400_MET");
  qcd.push_back("QCD_Pt-1400to1800_MET");
  qcd.push_back("QCD_Pt-1800_MET");
 
  SumDatasets(oDir,
	      qcd,
	      hists,
	      "QCD_MET");

  // make plots
  std::cout << "Making plots" << std::endl;
  StackPlot plots(oDir);
  plots.addDataset("QCD_MET",                    kBlue,   0);
  plots.addDataset("WJetsToLNu_MET",             kGreen,  0);
  plots.addDataset("ZJetsToLL_MET",              kOrange, 0);
  plots.addDataset("VBF_HToZZTo4Nu_M120_NoTrig", kRed,    2);

  plots.draw("hTrigNM1", "", "");
  plots.draw("hMETFiltNM1", "", "");
  plots.draw("hDijetNM1", "", "");
  plots.draw("hSgnEtaNM1", "", "");
  plots.draw("hDEtaJJNM1", "", "");
  plots.draw("hMjjNM1", "", "");
  plots.draw("hMETNM1", "", "");
  plots.draw("hDPhiJMNM1", "", "");

  plots.setYMin(1e-1);
  plots.setYMax(1e2);
  plots.draw("hEVetoNM1", "E_{T} [GeV]", "N_{evt}");
  plots.draw("hMuVetoNM1", "p_{T} [GeV]", "N_{evt}");

}
