
#ifndef Datasets_h
#define Datasets_h

#include <string>
#include <vector>

struct Dataset {
  std::string name;
  int nEvents;
  double sigma;
  bool isData;
};


class Datasets {

 public:

  Datasets();
  ~Datasets();

  void readFile(std::string filename);

  void addDataset(std::string name, 
		  int nevt,
		  double sigma,
		  int isData);

  unsigned size() { return names_.size(); }

  Dataset getDataset(unsigned i);

 private:

  std::vector<std::string> names_;
  std::vector<int> nevts_;
  std::vector<double> sigmas_;
  std::vector<bool> isData_;

};

#endif

