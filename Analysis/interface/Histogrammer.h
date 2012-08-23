#ifndef Histogrammer_h
#define Histogrammer_h

#include "InvisibleHiggs/Analysis/interface/Cuts.h"

#include <string>

class Histogrammer {
 public:

  Histogrammer(std::string odir,
	       std::string idir,
	       Cuts cuts);

  ~Histogrammer();

  void addDataset(std::string name, 
		  double weight, 
		  std::string idir="");

  void loop();

  void makeHistograms(std::string dataset, 
		      double weight, 
		      std::string idir);

 private:

  std::string odir_;
  std::string idir_;

  Cuts cuts_;

  std::vector<std::string> names_;
  std::vector<double> weights_;
  std::vector<std::string> idirs_;

};

#endif
