
#include "InvisibleHiggs/Analysis/interface/Cuts.h"
#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"
#include "InvisibleHiggs/Analysis/interface/StackPlot.h"
#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"
#include "InvisibleHiggs/Analysis/interface/Datasets.h"

#include "TTree.h"
#include "TH1D.h"
#include "TMath.h"
#include "TCanvas.h"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <fstream>

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

  Datasets datasets;
  datasets.readFile(datasetFile);

  // get the data
  TFile* ifile = TFile::Open( (iDir+std::string("/METABCD.root")).c_str(), "READ");
  TTree* tree = (TTree*) ifile->Get("invHiggsInfo/InvHiggsInfo");

  TFile* ofile = TFile::Open( (oDir+std::string("/QCDBackground.root")).c_str(), "UPDATE");


  // cuts
  Cuts cuts;
  TCut puWeight("puWeight");
  TCut allCuts = puWeight * cuts.allCuts();

  // plot for data dphi in loose region
  std::cout << "QCD Loose region" << std::endl;

  std::cout << "Plotting delta-phi for these cuts :" << std::endl;
  std::cout << cuts.qcdLoose() << std::endl << std::endl;

  TH1D* hDPhiControl = new TH1D("hDPhiControl", "", 72,  0.,  TMath::Pi());
  tree->Draw("vbfDPhi>>hDPhiControl", cuts.qcdLoose());

  // count events in high and low DPhi in contorl region
  int nLooseHiDPhi = tree->Draw("", cuts.qcdLooseHiDPhi());
  int nLooseLoDPhi = tree->Draw("", cuts.qcdLooseLoDPhi());

  std::cout << "N events (DPhi > 2.6)         : " << nLooseHiDPhi << std::endl;
  std::cout << "N events (DPhi < 1.0)         : " << nLooseLoDPhi << std::endl;

  double ratio = double(nLooseLoDPhi) / double(nLooseHiDPhi);
  double err_ratio = ratio*sqrt(1/sqrt(nLooseLoDPhi) + 1/sqrt(nLooseHiDPhi));

  std::cout << "Ratio N(DPhi<1.0)/N(DPhi>2.6) : " << ratio << " +\- " << err_ratio << std::endl;
  std::cout << std::endl;

  // count events in high DPhi in signal region
  std::cout << "QCD Tight region" << std::endl;
  int nTightHiDPhi = tree->Draw("", cuts.qcdTightHiDPhi());

  std::cout << "N events (DPhi > 2.6)         : " << nTightHiDPhi << std::endl;
  std::cout << std::endl;

  // read W/Z background estimates from file
  std::cout << "Reading Z+jets, W+jets backgrounds for DPhi>2.6 from file" << std::endl;
  double bgZjets(0.), err_bgZjets(0.);
  double bgWjets(0.), err_bgWjets(0.);

  ifstream file;
  double n, err_n;
  std::string name;

  file.open((oDir+std::string("/zjets.txt")).c_str());
  while (file >> name >> n >> err_n) {
    if (name == "QCD") {
      bgZjets = n;
      err_bgZjets = err_n;
    }
  }
  file.close();

  std::cout << "Z+jets : " << bgZjets << " +/- " << err_bgZjets << std::endl;

  file.open((oDir+std::string("/wjets.txt")).c_str());
  while (file >> name >> n >> err_n) {
    if (name == "QCD") {
      bgWjets = n;
      err_bgWjets = err_n;
    }
  }
  file.close();

  std::cout << "W+jets : " << bgWjets << " +/- " << err_bgWjets << std::endl;
  std::cout << std::endl;

  // calculate QCD background estimate
  double bgQCD(0.), err_bgQCD(0.);

  double tmp = double(nTightHiDPhi) - bgZjets - bgWjets;
  double err_tmp = sqrt(nTightHiDPhi + pow(err_bgZjets,2) + pow(err_bgWjets,2));
  std::cout << tmp << " +/- " << err_tmp << std::endl;
  bgQCD = tmp * ratio;
  err_bgQCD = bgQCD * sqrt( pow(err_tmp/tmp, 2) + pow(err_ratio/ratio,2) );

  std::cout << "N expected (DPhi < 1.0)       : " << bgQCD << " +/- " << err_bgQCD << std::endl;

  // write output file
  ofstream txtFile;
  txtFile.open(oDir+std::string("/qcd.txt"));
  txtFile << "Signal\t" << bgQCD << "\t" << err_bgQCD << std::endl;
  txtFile.close();

  // make plot
  TCanvas canvas;
  canvas.SetLogy();

  hDPhiControl->SetLineColor(kRed);
  hDPhiControl->SetLineStyle(1);
  hDPhiControl->GetXaxis()->SetTitle("#Delta #phi");
  hDPhiControl->GetYaxis()->SetTitle("N_{events}");
  hDPhiControl->Draw();

  canvas.Print((oDir+std::string("/QCDControl.pdf")).c_str());


  ofile->Close();

}
