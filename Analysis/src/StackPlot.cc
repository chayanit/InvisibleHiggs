#include "InvisibleHiggs/Analysis/interface/StackPlot.h"

#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TLegendEntry.h"
#include "TH1.h"
#include "TGraphErrors.h"
#include <TStyle.h>
#include <TROOT.h>

#include <iostream>
#include <algorithm>    // std::reverse

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
  legY2_(0.8),
  label_("")
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

void StackPlot::draw(std::string hname, std::string xTitle, std::string yTitle, bool logy, bool drawRatioPlot) {

  std::cout << "Drawing " << hname << " " << std::endl;

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat("");

  TH1::SetDefaultSumw2();

  TCanvas canvas;
  // canvas.SetLogy();
  unsigned int ww = canvas.GetWindowWidth();
  unsigned int wh = canvas.GetWindowHeight();

  //std::cout << "canvas weight = " << ww << std::endl;
  //std::cout << "canvas height = " << wh << std::endl;

  TPad *pad1; // Use TPad to allow us to draw ratio plot below main plot
  if (drawRatioPlot) {
  canvas.SetCanvasSize(ww, 1.2*wh);
  canvas.Divide(1, 2);
    // pad1 = (TPad *) canvas.cd(1);
    pad1 = new TPad("pad1","",0,0.30,1,1);
     // pad1->SetBottomMargin(2);
    pad1->SetBottomMargin(0.01);
  } else
    pad1 = new TPad("pad1","",0,0,1,1);
  pad1->Draw();
  pad1->cd();
  if (logy) pad1->SetLogy();
  THStack stack("hs", (hname+std::string(" stack")).c_str());

  TLegend leg(legX1_, legY1_, legX2_, legY2_);
  leg.SetFillColor(0);

  bool drawStack=false;

  std::vector<TLegendEntry*> entries; // To hold legend entries, so can draw in proper order

  std::vector<TFile*>::iterator file = files_.begin();
  
  // Some method of scanning all input files and determining the min y value needed for y axis
  // BIT BROKEN - if you get a seg fault when drawing, you prob have no events in the first file and it gets angry
  int i = 0;
  double ymin=0;
  for (; file!=files_.end(); ++file, ++i) {

    if (*file==0) {
      std::cerr << "No file for " << labels_.at(i) << std::endl;
      continue;
    }

    TH1D* h = (TH1D*) (*file)->Get(hname.c_str());

    if (h==0) {
      std::cout << "No histogram " << hname << " in file " << labels_.at(i) << std::endl;
      continue;
    }

    if (styles_.at(i) == 0) {
      double min = h->GetMinimum(0);
      if (min != 0) ymin = min;
    }
    if (ymin != 0) break;
  }

  /////////////////////////////////
  // make the stacked histograms //
  /////////////////////////////////
  i=0;
  file = files_.begin();
  for (; file!=files_.end(); ++file, ++i) {

    if (*file==0) {
      std::cerr << "No file for " << labels_.at(i) << std::endl;
      continue;
    }

    TH1D* h = (TH1D*) (*file)->Get(hname.c_str());

    if (h==0) {
      std::cout << "No histogram " << hname << " in file " << labels_.at(i) << std::endl;
      break;
    }

    if (styles_.at(i) == 0) {
      if (h->GetEntries()==0) {
      	std::cout << "No entries in histogram " << labels_.at(i) << std::endl;
      	continue;
      }
      
      drawStack=true;
      // h->SetLineColor(cols_.at(i));
      // h->SetLineWidth(0);
      h->SetLineColor(kBlack);
      h->SetFillColor(cols_.at(i));

      stack.Add(h);
      TLegendEntry *legE = new TLegendEntry(h, labels_.at(i).c_str(), "F");
      entries.push_back(legE);
      
    }

  }
  

  // Little hack here to get sum of hists in the THStack
  // ROOT doesn't have a method to convert a THStack to a TH1 for some stupid reason
  // Good ol' Rene...

  TList *histList = stack.GetHists();
  TIter next(histList); // Get list of hists in Stack
  TH1 *hMC  = (TH1*) histList->First()->Clone();
  TObject *obj;
  while ((obj = next())) // Loop through all hists in stack
  {
    // skip first object since it's used by creating the histogram                               
    if(obj == histList->First()) continue;
    hMC -> Add((TH1*)obj);
  }


  if (drawStack) {
    stack.Draw("HIST");

    // Some auto-axis ranging, doesn't work brilliantly, try tweaking the 5 and 0.1
    // assumes log axes!!!
    if (logy){
      stack.SetMaximum(stack.GetMaximum()*100);
      // stack.SetMinimum(ymin*0.1);
      stack.SetMinimum(ymin);
    }


    // If user has set axis range, use that preferentially
    if (yMax_ > 0.) stack.SetMaximum(yMax_);
    if (yMin_ > 0.) stack.SetMinimum(yMin_);
    stack.Draw("HIST"); //redraw to update axes

    // Make axis labels nice
    // Change size depending on if ratio plot or not (ratio plot shrinks actual sizes, so need to account for this)
    if (drawRatioPlot){
      // stack.GetXaxis()->SetTitleSize(0.07);
      stack.GetXaxis()->SetTitleOffset(999); //Effectively turn off x axis title on main plot
      // stack.GetXaxis()->SetLabelSize(0.07);
      stack.GetXaxis()->SetLabelOffset(999); //Effectively turn off x axis label on main plot

      stack.GetYaxis()->SetTitleSize(0.07);
      stack.GetYaxis()->SetTitleOffset(0.7);
      stack.GetYaxis()->SetLabelSize(0.05);
    } else{
      stack.GetXaxis()->SetTitleSize(0.045);
      stack.GetXaxis()->SetTitleOffset(0.9);
      stack.GetXaxis()->SetTitle(xTitle.c_str());

      stack.GetYaxis()->SetTitleSize(0.045);
      // stack.GetYaxis()->SetTitleOffset(1.2);    
    }
    stack.GetYaxis()->SetTitle(yTitle.c_str());

    if (!drawRatioPlot){
    }


  }

  //////////////////////////
  // draw errors on stack //
  //////////////////////////

  TH1D* ErrComp = (TH1D *) hMC->Clone();
  ErrComp->SetMarkerStyle(0);
  ErrComp->SetMarkerSize(0);
  ErrComp->SetLineColor(kGray+1);
  ErrComp->SetLineWidth(0);
  ErrComp->SetFillColor(kGray+1);
  ErrComp->SetFillStyle(3001);
  ErrComp->Draw("E2same");

  /////////////////////////////////
  // draw non-stacked histograms //
  /////////////////////////////////
  TH1D* hData;
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
      h->SetLineColor(kBlack);
      h->SetMarkerColor(cols_.at(i));
      // leg.AddEntry(h, "Data", "P");
      TLegendEntry *legE = new TLegendEntry(h, "Data", "PL");
      entries.push_back(legE);
      h->Draw("PE SAME");
      hData = (TH1D*)h->Clone("hData");
    }

    // signal
    if (styles_.at(i) == 2) {
      h->SetLineStyle(1);
      h->SetLineWidth(3);
      h->SetLineColor(cols_.at(i));
      TLegendEntry *legE = new TLegendEntry(h, labels_.at(i).c_str(), "L");
      entries.push_back(legE);
      h->Draw("HISTE SAME");
    }

  }

  // draw the other stuff
  canvas.RedrawAxis();  

  // Draw line on plot if N-1
  // Note, depends on hists defined in nMinusOne.cpp
  double cutVal = 0.;
  if (!hname.compare("hDijetNM1")) cutVal = 50.;
  else if (!hname.compare("hDEtaJJNM1")) cutVal = 4.2;
  else if (!hname.compare("hMjjNM1")) cutVal = 1200.;
  else if (!hname.compare("hMETNM1")) cutVal = 130.;
  else if (!hname.compare("hDPhiJJNM1")) cutVal = 1.0;
  else if (!hname.compare("hCenEtNM1")) cutVal = 30.0;

  if (cutVal != 0) {
    TLine *cutLine = new TLine(cutVal,0,cutVal,stack.GetMaximum()*1.5);
    cutLine->SetLineColor(kBlack);
    cutLine->SetLineWidth(3);
    cutLine->SetLineStyle(7);
    cutLine->Draw();
  }

  /////////////////
  // draw labels //
  /////////////////
  // First, CMS text in top left of plot
  TPaveText cms(0.12, 0.68, 0.45, 0.9, "NDC");
  cms.SetFillColor(0);
  cms.SetFillStyle(4000);
  cms.SetBorderSize(0);
  cms.SetLineColor(0);
  cms.SetTextAlign(12);
  cms.AddText("CMS Preliminary 2012");
  cms.AddText("");
  cms.AddText("#int L = 19.6 fb^{-1}");
  cms.AddText("");
  // any other text user has specified
  cms.AddText(label_.c_str());
  cms.Draw();

  //////////////////////////////////////////////
  // draw legend, with entries in right order //
  //////////////////////////////////////////////
  std::reverse(entries.begin(),entries.end());
  for (unsigned int n = 0;n < entries.size(); n++)
  {
    leg.AddEntry(entries.at(n)->GetObject(), entries.at(n)->GetLabel(), entries.at(n)->GetOption());
  }
  if (drawRatioPlot) leg.SetBorderSize(0);
  leg.Draw();

  canvas.cd();

  //////////////////////
  // Draw ratio plot  //
  //////////////////////
  if (drawRatioPlot){
    TPad *pad2 = new TPad("pad2","",0,0.0,1,0.30);
    // TPad *pad2 = (TPad *) canvas.cd(2);
    pad2->SetTopMargin(1);
    pad2->SetBottomMargin(0.33);
    // pad2->SetTopMargin(0);
    pad2->Draw();
    pad2->cd();

     
    hData->Add(hMC,-1);
    hData->Divide(hMC);
    hData->SetMarkerStyle(8);
    // hData->Draw("ep");
    hData->SetXTitle(xTitle.c_str());
    hData->SetYTitle("#frac{Data - MC}{MC}");
    hData->GetXaxis()->SetTitleSize(0.15);
    hData->GetXaxis()->SetTitleOffset(0.85);
    hData->GetXaxis()->SetLabelSize(0.12);
    hData->GetXaxis()->SetLabelOffset(0.008);

    hData->GetYaxis()->SetTitleSize(0.12);
    hData->GetYaxis()->SetTitleOffset(0.28);
    hData->GetYaxis()->SetNdivisions(3,5,0);
    hData->GetYaxis()->SetLabelSize(0.12);
    hData->GetYaxis()->SetLabelOffset(0.008);
    hData->Draw("ep");
    hData->SetMaximum(2.);
    hData->SetMinimum(-2.);

    int NBins = hMC->GetNbinsX();
    TGraphErrors * Erreff = new TGraphErrors(NBins);
    for(int iBin = 1; iBin <= NBins; ++iBin) {   
      double x = hMC->GetBinCenter(iBin);
      double y_smSum = hMC->GetBinContent(iBin); 
      if(!(y_smSum > 0.)) continue;
      double relerr = hMC->GetBinError(iBin)/hMC->GetBinContent(iBin);
      Erreff->SetPoint(iBin-1, x, 0);
      Erreff->SetPointError(iBin-1, 0.5*hMC->GetBinWidth(iBin),relerr);
    }
    Erreff->SetMarkerStyle(0);
    Erreff->SetMarkerSize(0);
    Erreff->SetLineColor(kGray+1);
    Erreff->SetLineWidth(0);
    Erreff->SetFillColor(kGray+1);
    Erreff->SetFillStyle(3001);
    Erreff->Draw("2");

    hData->Draw("ep same");

    double lineMin = hData->GetXaxis()->GetXmin();
    double lineMax = hData->GetXaxis()->GetXmax();
    TLine *line = new TLine(lineMin,0,lineMax,0);
    line->SetLineColor(kBlack);
    line->SetLineWidth(2);
    line->SetLineStyle(2);
    line->Draw();
    canvas.cd();
  }


  std::string filename = dir_+std::string("/")+hname+".pdf";
  std::cout << "Writing pdf file " << filename << std::endl;

  canvas.Print( filename.c_str() ,"pdf");
}


void StackPlot::dumpInfo(std::ostream &o) {

  o << "StackPlot" << std::endl;
  for (unsigned i=0; i<files_.size(); ++i) {
    o << "  dataset : " << labels_.at(i) << ", " << cols_.at(i) << ", " << styles_.at(i) << std::endl;
  }
  o << std::endl;

}
