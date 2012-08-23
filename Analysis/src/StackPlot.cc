#include "InvisibleHiggs/Analysis/interface/StackPlot.h"

#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TPaveText.h"

#include <iostream>


StackPlot::StackPlot(std::string dir) :
  dir_(dir),
  files_(0),
  labels_(0),
  cols_(0),
  styles_(0),
  title_(""),
  xTitle_(""),
  yTitle_(""),
  yMin_(0.),
  yMax_(0.),
  legX1_(0.6),
  legX2_(0.8),
  legY1_(0.6),
  legY2_(0.8)
{

}

StackPlot::~StackPlot() {

}

void StackPlot::setLegPos(float x1, float y1, float x2, float y2) {
  legX1_ = x1;
  legY1_ = y1;
  legX2_ = x2;
  legY2_ = y2;
}

void StackPlot::draw(std::string hname, std::string xTitle, std::string yTitle) {

  std::cout << "Drawing " << hname << " " << std::endl;

  gStyle->SetOptTitle(0);

  TCanvas canvas;

  canvas.SetLogy();

  THStack stack("hs", (hname+std::string(" stack")).c_str());

  TLegend leg(legX1_, legY1_, legX2_, legY2_);
  leg.SetFillColor(0);

  // make the stack
  bool drawStack=false;
  std::vector<TFile*>::iterator file = files_.begin();
  int i=0;
  for (; file!=files_.end(); ++file, ++i) {

    if (*file==0) {
      std::cerr << "No file for " << labels_.at(i) << std::endl;
      continue;
    }

    TH1D* h = (TH1D*) (*file)->Get(hname.c_str());

    if (h==0) {
      std::cout << "No histogram " << hname << std::endl;
      break;
    }

    if (styles_.at(i) == 0) {
      if (h->GetEntries()==0) {
	std::cout << "No entries in histogram " << labels_.at(i) << std::endl;
	continue;
      }
      drawStack=true;
      h->SetLineColor(cols_.at(i));
      h->SetFillColor(cols_.at(i));
      stack.Add(h);
      leg.AddEntry(h, labels_.at(i).c_str(), "F");
    }

  }
  
  if (drawStack) stack.Draw("HISTE");

  // draw non-stacked histograms
  file = files_.begin();
  i=0;
  for (; file!=files_.end(); ++file, ++i) {

    if (*file==0) {
      std::cerr << "No file for " << labels_.at(i) << std::endl;
      continue;
    }

    TH1D* h = (TH1D*) (*file)->Get(hname.c_str());

    if (h==0) {
      std::cout << "No histogram " << hname << std::endl;
      break;
    }

    // data
    if (styles_.at(i) == 1) {
      h->SetMarkerStyle(8);
      h->SetMarkerColor(cols_.at(i));
      leg.AddEntry(h, "Data", "P");
      h->Draw("PE SAME");
    }

    // signal
    if (styles_.at(i) == 2) {
      h->SetLineStyle(1);
      h->SetLineColor(cols_.at(i));
      leg.AddEntry(h, labels_.at(i).c_str(), "L");
      h->Draw("HISTE SAME");
    }

  }

  // draw the other stuff
  canvas.RedrawAxis();

  // draw labels
  TPaveText cms(0.1, 0.9, 0.4, 0.95, "NDC");
  cms.SetFillColor(0);
  cms.SetBorderSize(1);
  cms.SetLineColor(0);
  cms.AddText("CMS Preliminary 2012 - 697 pb^{-1}");
  cms.Draw();

  // draw legend
  leg.Draw();

  std::string filename = dir_+std::string("/")+hname+std::string(".pdf");
  std::cout << "Writing file " << filename << std::endl;

  canvas.Print( filename.c_str() );

}


void StackPlot::dumpInfo(std::ostream &o) {

  o << "StackPlot" << std::endl;
  for (unsigned i=0; i<files_.size(); ++i) {
    o << "  dataset : " << labels_.at(i) << ", " << cols_.at(i) << ", " << styles_.at(i) << std::endl;
  }
  o << std::endl;

}
