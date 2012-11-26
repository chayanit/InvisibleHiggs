#include "InvisibleHiggs/Analysis/interface/Cuts.h"
#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"
#include "InvisibleHiggs/Analysis/interface/StackPlot.h"
#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"
#include "InvisibleHiggs/Analysis/interface/Datasets.h"

#include "TTree.h"

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
  TCut allCuts = puWeight * cuts.allCuts();

  ofstream effFile;
  effFile.open(oDir+std::string("/efficiency.txt"));

  effFile << "Dataset\tsigma\tN_pass(MC)\tN_tot(MC)\teff\teff_err\tN_pass\tN_err" << std::endl;

  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);

    TFile* ifile = TFile::Open( (iDir+std::string("/")+dataset.name+std::string(".root")).c_str(), "READ");

    if (ifile==0) {
      std::cerr << "No file for " << dataset.name << std::endl;
      continue;
    }
    
    TTree* tree = (TTree*) ifile->Get("invHiggsInfo/InvHiggsInfo");
    
    TH1D* weight = new TH1D("weight","", 1, 0., 1.);
    tree->Draw("0.5>>weight", allCuts);
    double nMCPas = weight->GetBinContent(1);
    double nMCTot = double(dataset.nEvents);

    double eff    = double(nMCPas)/dataset.nEvents;
    double effErr = sqrt( nMCPas * (1-nMCPas/nMCTot) ) / nMCTot;  // binomial error

    double n      = lumi * dataset.sigma * eff;
    double nErr   = lumi * dataset.sigma * effErr;


    effFile << dataset.name << "\t"
	    << dataset.sigma << "\t"
	    << nMCPas << "\t"
	    << nMCTot << "\t"
	    << 100.*eff << "\t"
	    << 100.*effErr << "\t" 
	    << n << "\t"
	    << nErr << std::endl;
    
    ifile->Close();

  }

  effFile.close();


}
