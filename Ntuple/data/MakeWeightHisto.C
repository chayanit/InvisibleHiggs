{
#include <TFile.h>
#include <TList.h>
#include <TH1.h>

  TH1F *METL1  = new TH1F("METL1",  "METL1",  20, 0., 400.);
  TH1F *METHLT = new TH1F("METHLT", "METHLT", 20, 0., 400.);
  TH1F *JetHLT = new TH1F("JetHLT", "JetHLT", 20, 0., 100.);
  TH1F *MjjHLT = new TH1F("MjjHLT", "MjjHLT", 12, 800., 1400.);

  TFile *file = new TFile("DataMCWeight_53X_v1.root","RECREATE");

  TList* hList = new TList();
  hList->Add(METL1);
  hList->Add(METHLT); 
  hList->Add(MjjHLT);
  hList->Add(JetHLT);

  METL1->SetBinContent(0,0.);
  METL1->SetBinContent(1,0.7133);
  METL1->SetBinContent(2,0.7648);
  METL1->SetBinContent(3,0.6543);
  METL1->SetBinContent(4,0.6163);
  METL1->SetBinContent(5,0.6957);
  METL1->SetBinContent(6,0.8161);
  METL1->SetBinContent(7,0.9082);
  METL1->SetBinContent(8,0.9622);
  METL1->SetBinContent(9,0.9889);
  METL1->SetBinContent(10,1.0000);
  METL1->SetBinContent(11,1.0000);
  METL1->SetBinContent(12,1.0000);
  METL1->SetBinContent(13,1.0000);
  METL1->SetBinContent(14,1.0000);
  METL1->SetBinContent(15,1.0000);
  METL1->SetBinContent(16,1.0000);
  METL1->SetBinContent(17,1.0000);
  METL1->SetBinContent(18,1.0000);
  METL1->SetBinContent(19,1.0000);
  METL1->SetBinContent(20,1.0000);
  METL1->SetBinContent(21,1.0000);

  METHLT->SetBinContent(0,0.);
  METHLT->SetBinContent(1,0.);
  METHLT->SetBinContent(2,0.);
  METHLT->SetBinContent(3,0.);
  METHLT->SetBinContent(4,0.);
  METHLT->SetBinContent(5,0.6439);
  METHLT->SetBinContent(6,0.8177);
  METHLT->SetBinContent(7,1.0000);
  METHLT->SetBinContent(8,1.0000);
  METHLT->SetBinContent(9,1.0000);
  METHLT->SetBinContent(10,1.0000);
  METHLT->SetBinContent(11,1.0000);
  METHLT->SetBinContent(12,1.0000);
  METHLT->SetBinContent(13,1.0000);
  METHLT->SetBinContent(14,1.0000);
  METHLT->SetBinContent(15,1.0000);
  METHLT->SetBinContent(16,1.0000);
  METHLT->SetBinContent(17,1.0000);
  METHLT->SetBinContent(18,1.0000);
  METHLT->SetBinContent(19,1.0000);
  METHLT->SetBinContent(20,1.0000);
  METHLT->SetBinContent(21,1.0000);  

  JetHLT->SetBinContent(0,0.);
  JetHLT->SetBinContent(1,0.);
  JetHLT->SetBinContent(2,0.);
  JetHLT->SetBinContent(3,0.);
  JetHLT->SetBinContent(4,0.);
  JetHLT->SetBinContent(5,0.);
  JetHLT->SetBinContent(6,0.);
  JetHLT->SetBinContent(7,0.);
  JetHLT->SetBinContent(8,0.0826);
  JetHLT->SetBinContent(9,0.4312);
  JetHLT->SetBinContent(10,0.7436);
  JetHLT->SetBinContent(11,0.8938);
  JetHLT->SetBinContent(12,1.0000);
  JetHLT->SetBinContent(13,1.0000);
  JetHLT->SetBinContent(14,1.0000);
  JetHLT->SetBinContent(15,1.0000);
  JetHLT->SetBinContent(16,1.0000);
  JetHLT->SetBinContent(17,1.0000);
  JetHLT->SetBinContent(18,1.0000);
  JetHLT->SetBinContent(19,1.0000);
  JetHLT->SetBinContent(20,1.0000);
  JetHLT->SetBinContent(21,1.0000);

  MjjHLT->SetBinContent(0,0.);
  MjjHLT->SetBinContent(1,0.4555);
  MjjHLT->SetBinContent(2,0.5378);
  MjjHLT->SetBinContent(3,0.8148);
  MjjHLT->SetBinContent(4,0.9057);
  MjjHLT->SetBinContent(5,1.0000);
  MjjHLT->SetBinContent(6,1.0000);
  MjjHLT->SetBinContent(7,1.0000);
  MjjHLT->SetBinContent(8,1.0000);
  MjjHLT->SetBinContent(9,1.0000);
  MjjHLT->SetBinContent(10,1.0000);
  MjjHLT->SetBinContent(11,1.0000);
  MjjHLT->SetBinContent(12,1.0000);
  MjjHLT->SetBinContent(13,1.0000);

  hList->Write();
  file->Write();
  file->Close();
}
