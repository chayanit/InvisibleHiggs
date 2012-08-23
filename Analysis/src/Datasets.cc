#include "InvisibleHiggs/Analysis/interface/Datasets.h"

#include <iostream>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


Datasets::Datasets() {


}


Datasets::~Datasets() {

}


void Datasets::readFile(std::string filename) {

  std::string line;
  std::ifstream myfile(filename);

  if (myfile.is_open()) {

    while ( myfile.good() ) {
      getline(myfile,line);

      // split line by whitespace
      std::vector<std::string> splitVec;
      boost::split(splitVec, line, boost::algorithm::is_any_of("\t\v\f\r "));
      splitVec.erase(std::remove(splitVec.begin(), 
				 splitVec.end(), 
				 ""), 
		     splitVec.end());

      if (splitVec.size() >= 3) {
	//std::cout << splitVec.at(0) << ":" << splitVec.at(1) << ":" << splitVec.at(2) << std::endl;
	addDataset(splitVec.at(0), 
		   boost::lexical_cast<int>( splitVec.at(1) ), 
		   boost::lexical_cast<double>( splitVec.at(2) ));
      }
    }

    myfile.close();

  }
  else std::cerr << "Unable to open " << filename << std::endl; 

}


void Datasets::addDataset(std::string name, int nevts, double sigma) {
  names_.push_back(name);
  nevts_.push_back(nevts);
  sigmas_.push_back(sigma);
}


Dataset Datasets::getDataset(unsigned i) {
  Dataset tmp;
  tmp.name = names_.at(i);
  tmp.nEvents = nevts_.at(i);
  tmp.sigma = sigmas_.at(i);
  return tmp;
}
