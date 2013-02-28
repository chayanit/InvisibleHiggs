
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
  std::string iDir("");
  std::string oDir("");
  std::string datasetFile(" ");

  // program options
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "Display this message")
    ("outdir,o", po::value<std::string>(), "Output directory")
    ("indir,i", po::value<std::string>(), "Input directory")
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
  if (vm.count("indir"))    iDir=vm["indir"].as<std::string>();
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

  // datasets
  Datasets datasets;
  datasets.readFile(datasetFile);

  // cuts
  Cuts cuts;
  TCut ctrl("");
  ctrl += cuts.cut(0);  // trigger
  ctrl += cuts.cut(1);  // met filters
  ctrl += cuts.cut(2);  // dijets
  ctrl += cuts.cut(3);  // fwd/bkwd
  ctrl += cuts.cut(7);  // lepton vetoes
  ctrl += cuts.cut(8);

  TCut ctrl1 = TCut("puWeight") * (ctrl+cuts.cut(5));  // Mjj
  TCut ctrl2 = TCut("puWeight") * (ctrl+cuts.cut(6));  // MET

  // loop over datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);

    std::cout << "Making histograms for " << dataset.name << " " << (dataset.isData?"data":"MC") << std::endl;

    TFile* ifile = TFile::Open( (iDir+std::string("/")+dataset.name+std::string(".root")).c_str(), "READ");

    if (ifile==0) {
      //      std::cerr << "No file for " << dataset.name << std::endl;
      continue;
    }
    
    TTree* tree = (TTree*) ifile->Get("invHiggsInfo/InvHiggsInfo");

    TFile* ofile = TFile::Open( (oDir+std::string("/")+dataset.name+std::string(".root")).c_str(), "UPDATE");

    // create histograms
    TH1D* hjet1pt    = new TH1D("hjet1pt",    "", 100, 0.,  200.);
    TH1D* hjet2pt    = new TH1D("hjet2pt",    "", 100, 0.,  200.);
    TH1D* hjet1eta   = new TH1D("hjet1eta",   "", 50, -5.,  5.);
    TH1D* hjet2eta   = new TH1D("hjet2eta",   "", 50, -5.,  5.);
    TH1D* hjetdeta   = new TH1D("hjetdeta",   "", 50,  0.,  10.);
    TH1D* hjetdphi   = new TH1D("hjetdphi",   "", 72,  0.,  TMath::Pi());
    TH1D* hmjj       = new TH1D("hmjj",       "", 100, 0.,  3000.);
    TH1D* hmet       = new TH1D("hmet",       "", 100, 0.,  200.);

    // fill histograms
    tree->Draw("jet1Pt>>hjet1pt", ctrl2);
    tree->Draw("jet2Pt>>hjet2pt", ctrl2);
    tree->Draw("jet1Eta>>hjet1eta", ctrl2);
    tree->Draw("jet2Eta>>hjet2eta", ctrl2);
    tree->Draw("vbfDEta>>hjetdeta", ctrl2);
    tree->Draw("vbfDPhi>>hjetdphi", ctrl2);
    tree->Draw("vbfM>>hmjj", ctrl2);
    tree->Draw("met>>hmet", ctrl1);

    // scale MC to lumi
    if (!dataset.isData) {
      double weight = lumi * dataset.sigma / dataset.nEvents;
      hjet1pt->Scale(weight);
      hjet2pt->Scale(weight);
      hjet1eta->Scale(weight);
      hjet2eta->Scale(weight);
      hjetdeta->Scale(weight);
      hjetdphi->Scale(weight);
      hmjj->Scale(weight);
      hmet->Scale(weight);  
    }

    // write histograms
    hjet1pt->Write("",TObject::kOverwrite);
    hjet2pt->Write("",TObject::kOverwrite);
    hjet1eta->Write("",TObject::kOverwrite);
    hjet2eta->Write("",TObject::kOverwrite);
    hjetdeta->Write("",TObject::kOverwrite);
    hjetdphi->Write("",TObject::kOverwrite);
    hmjj->Write("",TObject::kOverwrite);
    hmet->Write("",TObject::kOverwrite);

    ofile->Close();    
    ifile->Close();

  }

  // list histograms for dataset summing
  std::vector<std::string> hists;
  hists.push_back("hjet1pt");
  hists.push_back("hjet2pt");
  hists.push_back("hjetdeta");
  hists.push_back("hjetdphi");
  hists.push_back("hmjj");
  hists.push_back("hmet");


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
  SumDatasets(oDir, qcdDatasets, hists, "QCD");

  // sum Z+jets datasets
  std::vector<std::string> zjetsDatasets;
  zjetsDatasets.push_back(std::string("Zvv_50to100"));
  zjetsDatasets.push_back(std::string("Zvv_100to200"));
  zjetsDatasets.push_back(std::string("Zvv_200to400"));
  zjetsDatasets.push_back(std::string("Zvv_400toinf"));
  SumDatasets(oDir, zjetsDatasets, hists, "ZJets");

  // sum W+jets datasets
  std::vector<std::string> wjetsDatasets;
  wjetsDatasets.push_back(std::string("W1Jets"));
  wjetsDatasets.push_back(std::string("W2Jets"));
  wjetsDatasets.push_back(std::string("W3Jets"));
  wjetsDatasets.push_back(std::string("W4Jets"));
  SumDatasets(oDir, wjetsDatasets, hists, "WNJets");

  // sum single top datasets
  std::vector<std::string> topDatasets;
  topDatasets.push_back(std::string("SingleT_t"));
  topDatasets.push_back(std::string("SingleTbar_t"));
  topDatasets.push_back(std::string("SingleT_s"));
  topDatasets.push_back(std::string("SingleTbar_s"));
  topDatasets.push_back(std::string("SingleT_tW"));
  topDatasets.push_back(std::string("SingleTbar_tW"));
  SumDatasets(oDir, topDatasets, hists, "SingleT");

  // sum single top datasets
  std::vector<std::string> dibDatasets;
  dibDatasets.push_back(std::string("WW"));
  dibDatasets.push_back(std::string("WZ"));
  dibDatasets.push_back(std::string("ZZ"));
  SumDatasets(oDir, dibDatasets, hists, "DiBoson");

  // make plots
  std::cout << "Making plots" << std::endl;
  StackPlot plots(oDir);
  plots.setLabel("CMS Preliminary 2012 #int L = 19.56 fb^{-1}");

  plots.addDataset("QCD",        kBlue, 0);
  plots.addDataset("WNJets",      kGreen, 0);
  plots.addDataset("ZJets",      kOrange, 0);
  plots.addDataset("TTbar",      kMagenta, 0);
  plots.addDataset("SingleT",  kMagenta-2, 0);
  plots.addDataset("DiBoson",    kRed-2, 0);

  plots.addDataset("signalM120", kRed, 2);

  plots.addDataset("METABCD",    kBlack, 1);


  // draw plots
  plots.dumpInfo(std::cout);

  plots.draw("hjet1pt", "E_{T} [GeV]", "N_{events}");
  plots.draw("hjet2pt", "E_{T} [GeV]", "N_{events}");
  plots.draw("hjetdeta", "#Delta #eta_{jj}", "N_{events}");
  plots.draw("hjetdphi", "#Delta #phi_{jj} [GeV]", "N_{events}");
  plots.draw("hmjj", "M_{jj} [GeV]", "N_{events}");
  plots.draw("hmet", "E_{T}^{miss} [GeV]", "N_{events}");

}
