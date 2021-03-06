#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooPolynomial.h"
#include "RooBukinPdf.h"
#include "RooIntegralMorph.h"
#include "RooNLLVar.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "TH1.h"
#include "/afs/cern.ch/work/x/xuyan/work5/PROD17/AN/AN-19-280/utils/general/tdrstyle.C"
#include "/afs/cern.ch/work/x/xuyan/work5/PROD17/AN/AN-19-280/utils/general/CMS_lumi.C"

std::string to_str_trim(const float a_value, const int n = 2)
{
    return std::to_string(a_value).substr(0,std::to_string(a_value).find(".") + n + 1);
}

void plotSignalPDF(){
    
  //gErrorIgnoreLevel = kInfo;
  using namespace std;
  using namespace RooFit;
  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);

  gROOT->SetBatch(1);
  lumi_13TeV = "";
  writeExtraText = 1;
  lumiTextOffset = 0.15;
  bool plot_CMS = true;
  extraText = "Simulation";
  lumiTextSize = 0.35;
  cmsTextSize = 0.45;
  int iPeriod = 5;
  int iPos = 0;
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetTitleSize(0.05,"XYZ");
  gStyle->SetLabelSize(0.05,"XYZ");
  gStyle->SetFrameLineWidth(2);
  gStyle->SetLegendTextSize(0.025);
  gStyle->SetBarWidth(2);
  gStyle->SetHistLineWidth(2);
  
  RooRealVar *m = new RooRealVar("m","m",0,8800);
  RooPlot *frame = m->frame();
  RooAbsPdf *model1 = NULL;
  RooAbsPdf *model2 = NULL;
  RooAbsPdf *model3 = NULL;
  
  int mass[18]={700,800,900,1000,1200,1400,1600,1800,2000,2200,2400,2600,2800,3500,4000,5000,6000,7000};
  TFile *f_1 = NULL;
  
  for(int i=700; i<7001; i+=50){
  
  std::string mass_str = std::to_string(i);
  
  TFile *file = TFile::Open(("signal_pdfs_"+mass_str+"W.root").c_str());
  RooWorkspace *w = (RooWorkspace*)file->Get("signals");
  w->Print();
  //RooPlot *frame = w->var("m")->frame();
  
  // TLegend* leg = new TLegend(0.7,0.2,0.9,0.8);
  bool isanchor = 0;
  for(int j=0; j<18; j++){
      if(i == mass[j]) isanchor = 1;
  }
  
  RooAbsPdf *model = w->pdf("CB2Gaus");
  //model->plotOn(frame,Range(0.75*mass[i],1.25*mass[i],kTRUE));
  if(isanchor)
    model->plotOn(frame,LineWidth(2),LineColor(2));
  else
    model->plotOn(frame,LineWidth(2),LineColor(4));
  
  // TCanvas *c = new TCanvas("","",2400,1800);
  // c->cd();
  // c->Clear();
  // c->SetLeftMargin(0.11);
  // c->SetBottomMargin(0.105);
  // frame->SetTitle("Signal Interpolation Fitted (Wide)");
  // TAxis *xaxis = frame->GetXaxis();
  // TAxis *yaxis = frame->GetYaxis();
  // xaxis->SetTitle("m_{W#gamma}");
  // yaxis->SetTitle("Events (a.u.)");
  // yaxis->SetTitleOffset(1.1);
  // xaxis->SetLimits(500,4000);
  // c->SetLogy();
  // yaxis->SetRangeUser(0.0000001,1);
  // frame->Draw();
  // TString outname = "signals"+mass_str+".png";
  // c->Print(outname);
  
  // delete c;
  // for(int i=0; i<10; i++){
	// rho2->setVal(0.1-i*0.02);
	// rho1->setVal(-0.1);
	// RooAbsPdf *model = w->pdf("Bukin");
	// TString name = "rho2"+to_str_trim(0.1-i*0.02);
	// model->plotOn(frame,Range(0,4000,kTRUE),RooFit::Name(name),LineColor(i));
	// leg->AddEntry(frame->findObject(name),name,"l");
  // }
  }
  TCanvas *c = new TCanvas("","",2400,1800);
  c->cd();
  c->Clear();
  c->SetLeftMargin(0.11);
  c->SetBottomMargin(0.105);
  c->SetGrid(1);
  frame->SetTitle("Signal Interpolation (Wide)");
  TAxis *xaxis = frame->GetXaxis();
  TAxis *yaxis = frame->GetYaxis();
  xaxis->SetTitle("M_{W#gamma}");
  yaxis->SetTitle("Events (a.u.)");
  yaxis->SetTitleOffset(1);
  xaxis->SetLimits(600,8800);
  //c->SetLogy();
  yaxis->SetRangeUser(0,0.9);
  frame->Draw();
  CMS_lumi(c,iPeriod,iPos);
  c->Print("SignalInterpolationW.pdf");
  c->Print("SignalInterpolationW.png");
  c->Print("SignalInterpolationW.svg");
  
  /*
  TFile *file1 = TFile::Open("/afs/cern.ch/work/x/xuyan/work5/PROD17/CMSSW_10_2_13/src/HiggsAnalysis/CombinedLimit/data/tutorials/Combine/Bias/combine_workspace/SR_pdfs_noNormSet_ATLAS2.root");
  RooWorkspace *w1= (RooWorkspace*)file1->Get("backgrounds");
  w1->Print();
  
  RooAbsData *data = w1->data("data_SR_binned");
  data->plotOn(frame,LineColor(2));
  
  model1 = w1->pdf("ATLAS2");
  model1->plotOn(frame,LineColor(2));
  
  TFile *file2 = TFile::Open("/afs/cern.ch/work/x/xuyan/work5/PROD17/CMSSW_10_2_13/src/HiggsAnalysis/CombinedLimit/data/tutorials/Combine/Bias/combine_workspace/SR_pdfs_noNormSet_VVdijet2.root");
  RooWorkspace *w2= (RooWorkspace*)file2->Get("backgrounds");
  w2->Print();
  model2 = w2->pdf("VVdijet2");
  model2->plotOn(frame,LineColor(4));
  
  TFile *file3 = TFile::Open("/afs/cern.ch/work/x/xuyan/work5/PROD17/CMSSW_10_2_13/src/HiggsAnalysis/CombinedLimit/data/tutorials/Combine/Bias/combine_workspace/SR_pdfs_noNormSet_dijet3.root");
  RooWorkspace *w3= (RooWorkspace*)file3->Get("backgrounds");
  w3->Print();
  model3 = w3->pdf("dijet3");
  model3->plotOn(frame,LineColor(8));
  
  
  TCanvas *c = new TCanvas("","",1200,900);
  c->cd();
  c->SetLeftMargin(0.2);
  c->SetLogy();
  TAxis* yaxis = frame->GetYaxis();
  yaxis->SetRangeUser(0.01,10000);
  TAxis* xaxis = frame->GetXaxis();
  xaxis->SetRangeUser(600,1500);
  frame->Draw();
  c->SetGrid();
  //leg->Draw();
  
  c->Print("pdf.svg");
  */
}