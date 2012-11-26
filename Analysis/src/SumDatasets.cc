#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"

#include <string>
#include <vector>
#include "TFile.h"
#include "TH1D.h"

#include <iostream>

void SumDatasets::sumTH1D(TH1D* h, std::string label, std::vector<TFile*> files) {
  std::vector<TFile*>::iterator file;
  for (file=files.begin(); file!=files.end(); ++file) {
    h->Add( (TH1D*) (*file)->Get(label.c_str()) );
  }
}

SumDatasets::SumDatasets(std::string dir, 
			 std::vector<std::string> datasets, 
			 std::vector<std::string> hists, 
			 std::string outfile) {
  
  // open files
  std::vector<TFile*> files;
  std::vector<std::string>::const_iterator dataset;
  for (dataset=datasets.begin(); dataset!=datasets.end(); ++dataset) {

    TFile* file = TFile::Open( (dir+std::string("/")+(*dataset)+std::string(".root")).c_str(), "READ");

    if (file!=0) files.push_back( file );

  }

  TFile* ofile = TFile::Open( (dir+std::string("/")+outfile+std::string(".root")).c_str(), "RECREATE");

  // sum histograms
  std::vector<std::string>::const_iterator hist;
  for (hist=hists.begin(); hist!=hists.end(); ++hist) {
    TH1D* h( (TH1D*) files.at(0)->Get(hist->c_str()) );
    if (h==0) {
      std::cerr << "Could not find histogram " << (*hist) << " in " << ofile->GetName() << std::endl;
      continue;
    }
    sumTH1D(h, (*hist), files);
    h->Write();
  }

  // close files
  ofile->Close();

  std::vector<TFile*>::iterator file;
  for (file=files.begin(); file!=files.end(); ++file) {
    (*file)->Close();
  }


}


SumDatasets::~SumDatasets() {

}

