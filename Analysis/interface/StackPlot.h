
#include "TFile.h"

#include <vector>

// class for drawing lots of stack plots of different quantities in a common style

class StackPlot {

public:

  StackPlot(std::string dir);
  ~StackPlot();

  // set plots up
  void addDataset(std::string label, short int col, short int style)
  {
    files_.push_back(TFile::Open((dir_+std::string("/")+label+std::string(".root")).c_str()));
    labels_.push_back(label);
    cols_.push_back(col);
    styles_.push_back(style);
  }

  //  void setLineColours(std::vector<short int> cols) { lcols_ = cols; }
  //  void setFillColours(std::vector<short int> cols) { fcols_ = cols; }

  // optional set up
  void setYMin(float min) { yMin_ = min; }
  void setYMax(float max) { yMax_ = max; }
  void setXTitle(std::string title) { xTitle_ = title; }
  void setYTitle(std::string title) { yTitle_ = title; }
  void setLegPos(float x1, float y1, float x2, float y2);

  // draw a histogram
  void draw(std::string hname, std::string xTitle, std::string yTitle);

  void dumpInfo(std::ostream &o);

private:

  // file directory
  std::string dir_;

  // the files
  std::vector<TFile*> files_;
  std::vector<std::string> labels_;
  std::vector<short int> cols_;
  std::vector<short int> styles_;

  // temporary items
  std::string title_;
  std::string xTitle_;
  std::string yTitle_;

  float yMin_;
  float yMax_;
  float legX1_;
  float legX2_;
  float legY1_;
  float legY2_;


};


