//make signal shapes
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>                  // access to gROOT, entry point to ROOT system
#include <TSystem.h>                // interface to OS
#include <TFile.h>                  // file handle class
#include <TTree.h>                  // class to access ntuples
#include <TClonesArray.h>           // ROOT array class
#include <TBenchmark.h>             // class to track macro running statistics
#include <TVector2.h>               // 2D vector class
#include <TMath.h>                  // ROOT math library
#include <vector>                   // STL vector class
#include <iostream>                 // standard I/O
#include <iomanip>                  // functions to format standard I/O
#include <fstream>                  // functions for file I/O
#include "TLorentzVector.h"         // 4-vector class
#include <THStack.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TF1.h>
#include <TEfficiency.h>
#include "TGraphAsymmErrors.h"
#include "TH1D.h"
#include "TRandom.h"
#include <algorithm>
#include <map>
#include <RooMsgService.h>
#include <RooRandom.h>
#include <RooRealVar.h>
#include <RooArgSet.h>
#include <RooArgList.h>
#include <RooCBShape.h>
#include <RooGaussian.h>
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooChi2Var.h>
#include <TLatex.h>
#include <RooPlot.h>
#include <RooHist.h>
#include <RooPlotable.h>
#include <RooWorkspace.h>
#include <RooAddPdf.h>
#include "/afs/cern.ch/work/x/xuyan/work5/PROD17/AN/AN-19-280/utils/general/tdrstyle.C"
#include "/afs/cern.ch/work/x/xuyan/work5/PROD17/AN/AN-19-280/utils/general/CMS_lumi.C"
#endif

std::string to_str_trim(const float a_value, const int n = 2)
{
    return std::to_string(a_value).substr(0,std::to_string(a_value).find(".") + n + 1);
}

void make_signal_narrow_shapes_CBGaus(int signalmass = 6000, int yhi = 600)
{
  //gErrorIgnoreLevel = kInfo;
  using namespace std;
  using namespace RooFit;
  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
  RooRandom::randomGenerator()->SetSeed(37);
  
  gROOT->SetBatch(1);
  lumi_13TeV = "";
  writeExtraText = 1;
  lumiTextOffset = 0.15;
  bool plot_CMS = true;
  extraText = "Simulation";
  lumiTextSize = 0.35;
  cmsTextSize = 0.45;
  int iPeriod = 5;
  int iPos = 11;
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetTitleSize(0.045,"XYZ");
  gStyle->SetLabelSize(0.045,"XYZ");
  gStyle->SetFrameLineWidth(2);
  gStyle->SetLegendTextSize(0.03);
  gStyle->SetBarWidth(1.03);
  gStyle->SetHistLineWidth(2);
  TString signalmass_str = std::to_string(signalmass);

  // --- Create obervable --- 
  RooRealVar *m = new RooRealVar("m","m",600,8800,""); //the name "m" will be used by RooDataSet to import data

  //--- signal PDF ---
  TString fun_name = "CBGaus";
  RooRealVar* CB_mean = new RooRealVar("CB_mean","CB_mean",signalmass,signalmass-100,signalmass+100,"");
  RooRealVar* CB_sigma = new RooRealVar("CB_sigma","CB_sigma",170,130,250,"");
  RooRealVar* CB_alpha = new RooRealVar("CB_alpha","CB_alpha",2,0.5,3,"");
  RooRealVar* CB_n = new RooRealVar("CB_n","CB_n",1,0.5,3,"");
  RooCBShape* CB_model = new RooCBShape("CBShape","Cystal Ball Function",*m,*CB_mean,*CB_sigma,*CB_alpha,*CB_n);
  
  RooRealVar* Gaus_mean = new RooRealVar("Gaus_mean","Gaus_mean",signalmass-40,signalmass-150,signalmass+50,"");
  RooRealVar* Gaus_sigma = new RooRealVar("Gaus_sigma","Gaus_sigma",300,250,350,"");
  RooGaussian* Gaus_model = new RooGaussian("Gaussian","Gaussian Function",*m,*Gaus_mean,*Gaus_sigma);
  RooRealVar* frac = new RooRealVar("frac","frac",0.75,0.7,0.85);
  RooAddPdf* com_model = new RooAddPdf("CBGaus","CBGaus",RooArgList(*CB_model,*Gaus_model),RooArgList(*frac));

  // --- Import Binned dataset ---
  int bin = 410; //20GeV
  if(signalmass > 1999)//40GeV
	  bin = 205;
  if(signalmass > 4999) //80GeV
	  bin = 103;
  if(signalmass > 6999) //100GeV
	  bin = 82;
  float s_mass, xsec_puweight, xsec_sf;
  TH1F MChist("MC","MC",bin,600,8800);
									
  TFile file("/afs/cern.ch/work/x/xuyan/work5/PROD17/DATA/Run2/fullcut/SignalMC"+signalmass_str+"N_postproc_WGamma17_SR_sigrange_fullcut_jmcorr_May22.root");
  TTree* tree = (TTree*)file.Get("Events");
  tree->SetBranchAddress("m", &s_mass);
  tree->SetBranchAddress("xsec_puweight", &xsec_puweight);
  tree->SetBranchAddress("xsec_sf", &xsec_sf);
  for (int ievt = 0; ievt<tree->GetEntries();ievt++) {
    tree->GetEntry(ievt);
    MChist.Fill(s_mass, xsec_puweight*xsec_sf);
  }
  RooDataHist datah("Signal MC (W band)","Signal MC (W band)",RooArgSet(*m),&MChist);
  cout<<"number of weighted entries: "<<datah.sum(false)<<endl;
  
  RooFitResult *r = com_model->fitTo(datah,Range(signalmass*0.75,signalmass*1.25),RooFit::Minimizer("Minuit2"),SumW2Error(false),Save()); //SumW2Error(false) for weighted data, see how to choose this with same calling without SumW2Error(false)

  
  // --- Plot ---
  gStyle->SetOptStat(111111);
  RooPlot *frame = m->frame(Title("Signal"+signalmass_str));
  datah.plotOn(frame,RooFit::Name("data"));
  frame->getAttMarker()->SetMarkerSize(2);
  com_model->plotOn(frame,LineColor(2),RooFit::Name("fit"),Range(signalmass*0.75,signalmass*1.25));
  //com_model->plotOn(frame,VisualizeError(*r,2,kFALSE),FillColor(kYellow),LineColor(0),RooFit::Name("err2"));
  com_model->plotOn(frame,VisualizeError(*r,1,kFALSE),FillColor(kGreen),LineColor(0),RooFit::Name("err1"),Range(signalmass*0.75,signalmass*1.25));
  com_model->plotOn(frame,Components("CBShape"),LineStyle(kDashed),LineColor(1),RooFit::Name("CB"),Range(signalmass*0.75,signalmass*1.25));
  com_model->plotOn(frame,Components("Gaussian"),LineStyle(kDashed),RooFit::Name("Gaus"),Range(signalmass*0.75,signalmass*1.25));
  com_model->plotOn(frame,LineColor(2),RooFit::Name("fit"),Range(signalmass*0.75,signalmass*1.25));
  datah.plotOn(frame);
  frame->getAttMarker()->SetMarkerSize(2);
  
  TString chi2txt = "#chi^{2}/ndf: "+to_str_trim(frame->chiSquare("fit","data", 7));
  TString NLLtxt = "NLL: "+to_str_trim(com_model->createNLL(datah)->getVal());
  TString CBmean = "#mu_{CB}: "+to_str_trim(CB_mean->getVal())+" #pm "+to_str_trim(CB_mean->getError())+" (GeV)";
  TString CBsigma = "#sigma_{CB}: "+to_str_trim(CB_sigma->getVal())+" #pm "+to_str_trim(CB_sigma->getError())+" (GeV)";
  TString CBalpha = "#alpha_{CB}: "+to_str_trim(CB_alpha->getVal())+" #pm "+to_str_trim(CB_alpha->getError());
  TString CBn = "n_{CB}: "+to_str_trim(CB_n->getVal())+" #pm "+to_str_trim(CB_n->getError());
  TString Gaussmean = "#mu_{Gaus}: "+to_str_trim(Gaus_mean->getVal())+" #pm "+to_str_trim(Gaus_mean->getError())+" (GeV)";
  TString Gaussigma = "#sigma_{Gaus}: "+to_str_trim(Gaus_sigma->getVal())+" #pm "+to_str_trim(Gaus_sigma->getError())+" (GeV)";
  TString Fraction = "CB frac: "+to_str_trim(frac->getVal())+" #pm "+to_str_trim(frac->getError());
  TLatex *chi2lax = new TLatex(0.15,0.7-0*0.04,chi2txt);
  //TLatex *NLLlax = new TLatex(0.15,0.8-0.06,NLLtxt);
  TLatex *CBmeanlax = new TLatex(0.15,0.7-1*0.04,CBmean);
  TLatex *CBsigmalax = new TLatex(0.15,0.7-2*0.04,CBsigma);
  TLatex *CBalphalax = new TLatex(0.15,0.7-3*0.04,CBalpha);
  TLatex *CBnlax = new TLatex(0.15,0.7-4*0.04,CBn);
  TLatex *Gaussigmalax = new TLatex(0.15,0.7-5*0.04,Gaussigma);
  TLatex *Gaussmeanlax = new TLatex(0.15,0.7-6*0.04,Gaussmean);
  TLatex *Fraclax = new TLatex(0.15,0.7-7*0.04,Fraction);

  CBmeanlax->SetNDC();
  CBsigmalax->SetNDC();
  CBalphalax->SetNDC();
  CBnlax->SetNDC();
  Gaussigmalax->SetNDC();
  Gaussmeanlax->SetNDC();
  Fraclax->SetNDC();

  CBmeanlax->SetTextSize(0.026);
  CBsigmalax->SetTextSize(0.026);
  CBalphalax->SetTextSize(0.026);
  CBnlax->SetTextSize(0.026);
  Gaussigmalax->SetTextSize(0.026);
  Gaussmeanlax->SetTextSize(0.026);
  Fraclax->SetTextSize(0.026);
  
  frame->addObject(CBmeanlax);
  frame->addObject(CBsigmalax);
  frame->addObject(CBalphalax);
  frame->addObject(CBnlax);
  frame->addObject(Gaussigmalax);
  frame->addObject(Gaussmeanlax);
  frame->addObject(Fraclax);
  
  TCanvas *c01 = new TCanvas("c01","c01",2100,2000);
  TPad *p01a = new TPad("p01a","p01a",0.05,0.27,0.95,1.0);
  TPad *p01b = new TPad("p01b","p01b",0.05,0.10,0.95,0.315);
  p01a->Draw();
  p01b->Draw();
  p01a->cd();
  p01a->SetLeftMargin(0.11);
  p01a->SetRightMargin(0.1);
  p01a->SetBottomMargin(0.11);
  //axis,log scale and range setting functions must be called after all plotOn functions being called
  TAxis* xaxis = frame->GetXaxis();
  TAxis* yaxis = frame->GetYaxis();
  xaxis->SetTitle("");
  xaxis->SetTitleOffset(1.2);
  yaxis->SetTitle("Events / 100 GeV");
  yaxis->SetTitleOffset(1.35);
  yaxis->SetRangeUser(0,yhi);
  xaxis->SetRangeUser(signalmass*0.75,signalmass*1.25);
  frame->Draw();
  CMS_lumi(p01a,iPeriod,iPos);
  TLegend *l =  new TLegend(0.60,0.75,0.88,0.88);
  l->AddEntry(frame->findObject("data"),"2017 Signal MC","lep");
  l->AddEntry(frame->findObject("fit"),"Signal fit "+fun_name,"l");
  l->AddEntry(frame->findObject("err1"),"Fit Error 1 #sigma","f");
  //l->AddEntry(frame->findObject("err2"),"Fit Error 2 #sigma","f");
  l->Draw("same");
  
  p01b->cd();
  p01b->SetLeftMargin(0.11);
  p01b->SetRightMargin(0.1);
  p01b->SetTopMargin(0.2);
  p01b->SetBottomMargin(0.25);
  RooHist* hpull = frame->pullHist();
  RooPlot* pull_frame = m->frame();
  pull_frame->addPlotable(hpull,"B") ;
  hpull->SetMarkerStyle(8);
  hpull->SetMarkerSize(0);
  hpull->SetLineWidth(0);
  hpull->SetFillColor(kAzure+1);
  hpull->SetFillColorAlpha(kAzure+1,0.7);
  xaxis = pull_frame->GetXaxis();
  yaxis = pull_frame->GetYaxis();
  xaxis->SetTitle("M_{j#gamma} (GeV)");
  yaxis->SetTitle("#frac{MC-fit}{#sigma_{stat}}");
  yaxis->SetTitleOffset(0.4);
  yaxis->SetRangeUser(-5,5);
  xaxis->SetLabelSize(0.12);
  xaxis->SetTitleSize(0.12);
  yaxis->SetLabelSize(0.12);
  yaxis->SetTitleSize(0.12);
  yaxis->SetNdivisions(5);
  xaxis->SetRangeUser(signalmass*0.75,signalmass*1.25);
  p01b->SetGrid();
  pull_frame->Draw();
  p01b->Update();
  c01->Print("CBGaus"+signalmass_str+".png");
  c01->Print("CBGaus"+signalmass_str+".pdf");
  c01->Print("CBGaus"+signalmass_str+".svg");
  
  

/*
  TCanvas *c01 = new TCanvas("c01","c01",1200,900);
  //axis,log scale and range setting functions must be called after all plotOn functions being called
  TAxis* xaxis = frame->GetXaxis();
  TAxis* yaxis = frame->GetYaxis();
  xaxis->SetTitle("m_{j#gamma} (GeV)");
  xaxis->SetTitleOffset(1.2);
  yaxis->SetTitle("Events / 20 GeV");
  yaxis->SetTitleOffset(1.2);
  yaxis->SetRangeUser(0,yhi);
  xaxis->SetRangeUser(signalmass*0.75,signalmass*1.25);
  //xaxis->SetRangeUser(600,875);
  //frame->SetMaximum(600);
  //frame->SetMinimum(0);
  //c01->SetLogy();
  frame->Draw();
  TLegend *l =  new TLegend(0.7,0.75,0.85,0.85);
  l->AddEntry(frame->findObject("fit"),"SignalMC fit","l");
  //l->AddEntry(frame->findObject("bkgfun"),"Background Fit","l");
  //l->AddEntry(frame->findObject("err1"),"Fit Error 1 #sigma","f");
  //l->AddEntry(frame->findObject("err2"),"Fit Error 2 #sigma","f");
  l->Draw("same");
  c01->Print(signalmass_str+".png");
  c01->Print(signalmass_str+".pdf");

  TCanvas *c02 = new TCanvas("c02","c02",1200,300);
  //axis,log scale and range setting functions must be called after all plotOn functions being called
  c02->cd();
  RooHist* hpull = frame->pullHist();
  RooPlot* pull_frame = m->frame();
  pull_frame->addPlotable(hpull,"P") ;
  hpull->SetMarkerStyle(8);
  xaxis = pull_frame->GetXaxis();
  yaxis = pull_frame->GetYaxis();
  xaxis->SetTitle("M_{j#gamma}");
  yaxis->SetTitle("#frac{MC-fit}{#sigma_{stat}}");
  yaxis->SetTitleOffset(0.5);
  yaxis->SetRangeUser(-5,5);
  xaxis->SetLabelSize(0.08);
  xaxis->SetTitleSize(0.08);
  yaxis->SetLabelSize(0.08);
  yaxis->SetTitleSize(0.08);
  yaxis->SetNdivisions(5);
  xaxis->SetRangeUser(signalmass*0.75,signalmass*1.25);
  //xaxis->SetRangeUser(600,875);
  c02->SetBottomMargin(0.18);
  c02->SetTopMargin(0.18);
  c02->SetGrid();
  pull_frame->Draw();
  c02->Update();
  c02->Print("pull"+signalmass_str+".png");
  c02->Print("pull"+signalmass_str+".pdf");

  TCanvas *c03 = new TCanvas("c03","c03",1200,900);
  //axis,log scale and range setting functions must be called after all plotOn functions being called
  xaxis = frame->GetXaxis();
  yaxis = frame->GetYaxis();
  xaxis->SetTitle("m_{j#gamma} (GeV)");
  xaxis->SetTitleOffset(1.2);
  yaxis->SetTitle("Events / 20 GeV");
  yaxis->SetTitleOffset(1.2);
  yaxis->SetRangeUser(0.00001,0.01);
  xaxis->SetRangeUser(signalmass*0.75,signalmass*1.25);
  //xaxis->SetRangeUser(600,875);
  //xaxis->SetRangeUser(600,signalmass*1.3);
  //frame->SetMaximum(600);
  //frame->SetMinimum(0);
  c03->SetLogy();
  frame->Draw();
  l =  new TLegend(0.7,0.75,0.85,0.85);
  l->AddEntry(frame->findObject("fit"),"SignalMC fit","l");
  //l->AddEntry(frame->findObject("bkgfun"),"Background Fit","l");
  //l->AddEntry(frame->findObject("err1"),"Fit Error 1 #sigma","f");
  //l->AddEntry(frame->findObject("err2"),"Fit Error 2 #sigma","f");
  l->Draw("same");
  c03->Print(signalmass_str+"log.png");
  */
  
  // --- Output root file ---
  RooWorkspace *w = new RooWorkspace("w","w");
  w->import(*m);
  w->import(datah,Rename("signal_MC"));
  w->import(*com_model);
  w->writeToFile(signalmass_str+"N-shapes-Unbinned-CBGaus"+".root");
  
  
}
