#define fun_type 3
#define isNorm 1 //1: Norm 2: Orig
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
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooChi2Var.h>
#include <TLatex.h>
#include <RooPlot.h>
#include <RooHist.h>
#include <RooPlotable.h>
#include <RooWorkspace.h>
#include <RooAddPdf.h>
#include <RooBukinPdf.h>
#include "/afs/cern.ch/work/x/xuyan/work5/PROD17/AN/AN-19-280/utils/general/tdrstyle.C"
#include "/afs/cern.ch/work/x/xuyan/work5/PROD17/AN/AN-19-280/utils/general/CMS_lumi.C"
#endif

std::string to_str_trim(const float a_value, const int n = 2)
{
    return std::to_string(a_value).substr(0,std::to_string(a_value).find(".") + n + 1);
}

void make_sideband_shapes(int seed=37)
{
  gErrorIgnoreLevel = kInfo;
  gROOT->SetBatch(1);
  using namespace RooFit;
  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
  RooRandom::randomGenerator()->SetSeed(37); 
  
  gROOT->SetBatch(1);
  lumi_13TeV = "";
  writeExtraText = 1;
  lumiTextOffset = 0.15;
  bool plot_CMS = true;
  extraText = "Preliminary";
  lumiTextSize = 0.35;
  cmsTextSize = 0.45;
  int iPeriod = 12;
  int iPos = 11;
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetTitleSize(0.045,"XYZ");
  gStyle->SetLabelSize(0.045,"XYZ");
  gStyle->SetFrameLineWidth(2);
  gStyle->SetLegendTextSize(0.03);
  gStyle->SetBarWidth(1.03);
  gStyle->SetHistLineWidth(2);

  // --- Create obervable --- 
  RooRealVar *x = new RooRealVar("m","m",600,4000,""); //the name "m" will be used by RooDataSet to import data, normalization range is 600-3500 but plot range can be defined to like 600-3000

  //--- background PDF ---
#if fun_type == 1
  //-----------------------------dijet 2-----------------------------------
  TString fun_name = "dijet2";
  RooRealVar *dijet2_p0 = new RooRealVar("dijet2_p0","dijet2_p_0",-11,-20,-5,""); //-10.5471 +- 3.06516
  RooRealVar *dijet2_p1 = new RooRealVar("dijet2_p1","dijet2_p_1",-1,-5,-0.01,""); //-0.799413 +- 0.560435
  RooGenericPdf *model = new RooGenericPdf(fun_name,"(pow(m/13000,dijet2_p0+dijet2_p1*log(m/13000)))",RooArgList(*x,*dijet2_p0,*dijet2_p1));
  RooRealVar *bkg_norm = new RooRealVar("bkg_norm","bkg_norm",5000,0,100000,""); 
  RooAddPdf *ex_model = new RooAddPdf(fun_name+"extended",fun_name+"extended",RooArgList(*model),RooArgList(*bkg_norm));
#elif fun_type == 2
  //-----------------------------ATLAS1-----------------------------------
  TString fun_name = "ATLAS1";
  RooRealVar *ATLAS1_p0 = new RooRealVar("ATLAS1_p0","ATLAS1_p_0",0,-1000,1000,""); //-10.5471 +- 3.06516
  RooRealVar *ATLAS1_p1 = new RooRealVar("ATLAS1_p1","ATLAS1_p_1",0,-1000,1000,""); //-0.799413 +- 0.560435
  RooGenericPdf *model = new RooGenericPdf(fun_name,"(pow(1-pow(m/13000,1/3),ATLAS1_p0)/pow(m/13000,ATLAS1_p1))",RooArgList(*x,*ATLAS1_p0,*ATLAS1_p1));
  RooRealVar *bkg_norm = new RooRealVar("bkg_norm","bkg_norm",5000,0,100000,""); 
  RooAddPdf *ex_model = new RooAddPdf(fun_name+"extended",fun_name+"extended",RooArgList(*model),RooArgList(*bkg_norm));
#elif fun_type == 3
  //-----------------------------VVdijet2-----------------------------------
  TString fun_name = "VVdijet-2";
  /*
  //Normalized fit
  RooRealVar *VVdijet2_p0 = new RooRealVar("VVdijet2_p0","VVdijet2_p_0",-15,-20,20,""); //-10.5471 +- 3.06516
  RooRealVar *VVdijet2_p1 = new RooRealVar("VVdijet2_p1","VVdijet2_p_1",15,-20,20,""); //-0.799413 +- 0.560435
  RooRealVar *VVdijet2_p2 = new RooRealVar("VVdijet2_p2","VVdijet2_p_2",0,-20,20,""); //-0.799413 +- 0.560435
  */
  RooRealVar *VVdijet2_p0 = new RooRealVar("VVdijet2_p0","VVdijet2_p_0",0,-1000,1000,""); //-10.5471 +- 3.06516
  RooRealVar *VVdijet2_p1 = new RooRealVar("VVdijet2_p1","VVdijet2_p_1",0,-1000,1000,""); //-0.799413 +- 0.560435
  RooRealVar *VVdijet2_p2 = new RooRealVar("VVdijet2_p2","VVdijet2_p_2",0,-1000,1000,""); //-0.799413 +- 0.560435
  RooGenericPdf *model = new RooGenericPdf(fun_name,"(pow(1-m/13000,VVdijet2_p0)/pow(m/13000,VVdijet2_p1+VVdijet2_p2*log(m/13000)))",RooArgList(*x,*VVdijet2_p0,*VVdijet2_p1,*VVdijet2_p2));
  RooRealVar *bkg_norm = new RooRealVar("bkg_norm","bkg_norm",5000,0,100000,""); 
  RooAddPdf *ex_model = new RooAddPdf(fun_name+"extended",fun_name+"extended",RooArgList(*model),RooArgList(*bkg_norm));
#elif fun_type == 4
  //-----------------------------exp-----------------------------------
  TString fun_name = "exp";
  RooRealVar *exp_p0 = new RooRealVar("exp_p0","exp_p_0",0,-1000,1000,""); //-10.5471 +- 3.06516
  RooRealVar *exp_p1 = new RooRealVar("exp_p1","exp_p_1",0,-1000,1000,""); //-0.799413 +- 0.560435
  RooGenericPdf *model = new RooGenericPdf(fun_name,"exp(exp_p0*(m/13000)+exp_p1*pow(m/13000,2))",RooArgList(*x,*exp_p0,*exp_p1));
  RooRealVar *bkg_norm = new RooRealVar("bkg_norm","bkg_norm",5000,0,100000,""); 
  RooAddPdf *ex_model = new RooAddPdf(fun_name+"extended",fun_name+"extended",RooArgList(*model),RooArgList(*bkg_norm));
#elif fun_type == 5
  //-----------------------------dijet 3-----------------------------------
  TString fun_name = "dijet3";
  RooRealVar *dijet3_p0 = new RooRealVar("dijet3_p0","dijet3_p_0",0,-1000,1000,""); //-10.5471 +- 3.06516
  RooRealVar *dijet3_p1 = new RooRealVar("dijet3_p1","dijet3_p_1",0,-1000,1000,""); //-0.799413 +- 0.560435
  RooRealVar *dijet3_p2 = new RooRealVar("dijet3_p2","dijet3_p_2",0,-1000,1000,""); //-0.799413 +- 0.560435
  RooGenericPdf *model = new RooGenericPdf(fun_name,"(pow(m/13000,dijet3_p0+dijet3_p1*log(m/13000)+dijet3_p2*pow(log(m/13000),2)))",RooArgList(*x,*dijet3_p0,*dijet3_p1,*dijet3_p2));
  RooRealVar *bkg_norm = new RooRealVar("bkg_norm","bkg_norm",5000,0,100000,""); 
  RooAddPdf *ex_model = new RooAddPdf(fun_name+"extended",fun_name+"extended",RooArgList(*model),RooArgList(*bkg_norm));
#elif fun_type == 6
  //-----------------------------ATLAS2-----------------------------------
  TString fun_name = "ATLAS2";
  RooRealVar *ATLAS2_p0 = new RooRealVar("ATLAS2_p0","ATLAS2_p_0",0,-1000,1000,""); //-10.5471 +- 3.06516
  RooRealVar *ATLAS2_p1 = new RooRealVar("ATLAS2_p1","ATLAS2_p_1",0,-1000,1000,""); //-0.799413 +- 0.560435
  RooRealVar *ATLAS2_p2 = new RooRealVar("ATLAS2_p2","ATLAS2_p_2",0,-1000,1000,""); //-0.799413 +- 0.560435
  RooGenericPdf *model = new RooGenericPdf(fun_name,"(pow(1-pow(m/13000,1/3),ATLAS2_p0)/pow(m/13000,ATLAS2_p1+ATLAS2_p2*log(m/13000)))",RooArgList(*x,*ATLAS2_p0,*ATLAS2_p1,*ATLAS2_p2));
  RooRealVar *bkg_norm = new RooRealVar("bkg_norm","bkg_norm",5000,0,100000,""); 
  RooAddPdf *ex_model = new RooAddPdf(fun_name+"extended",fun_name+"extended",RooArgList(*model),RooArgList(*bkg_norm));
#elif fun_type == 7
  //-----------------------------VVdijet3-----------------------------------
  TString fun_name = "VVdijet3";
  /*
  //Normalized fit
  RooRealVar *VVdijet3_p0 = new RooRealVar("VVdijet3_p0","VVdijet3_p_0",-15,-20,20,""); //-10.5471 +- 3.06516
  RooRealVar *VVdijet3_p1 = new RooRealVar("VVdijet3_p1","VVdijet3_p_1",15,-20,20,""); //-0.799413 +- 0.560435
  RooRealVar *VVdijet3_p2 = new RooRealVar("VVdijet3_p2","VVdijet3_p_2",0,-20,20,""); //-0.799413 +- 0.560435
  */
  RooRealVar *VVdijet3_p0 = new RooRealVar("VVdijet3_p0","VVdijet3_p_0",0,-100,100,""); //-10.5471 +- 3.06516
  RooRealVar *VVdijet3_p1 = new RooRealVar("VVdijet3_p1","VVdijet3_p_1",0,-10,10,""); //-0.799413 +- 0.560435
  RooRealVar *VVdijet3_p2 = new RooRealVar("VVdijet3_p2","VVdijet3_p_2",0,-1,1,""); //-0.799413 +- 0.560435
  RooRealVar *VVdijet3_p3 = new RooRealVar("VVdijet3_p3","VVdijet3_p_3",0,-1,1,""); //-0.799413 +- 0.560435
  RooGenericPdf *model = new RooGenericPdf(fun_name,"(pow(1-m/13000,VVdijet3_p0)/pow(m/13000,VVdijet3_p1+VVdijet3_p2*log(m/13000)+VVdijet3_p3*pow(log(m/13000),2)))",RooArgList(*x,*VVdijet3_p0,*VVdijet3_p1,*VVdijet3_p2,*VVdijet3_p3));
  RooRealVar *bkg_norm = new RooRealVar("bkg_norm","bkg_norm",5000,0,100000,""); 
  RooAddPdf *ex_model = new RooAddPdf(fun_name+"extended",fun_name+"extended",RooArgList(*model),RooArgList(*bkg_norm));
#endif

  // --- Import unBinned dataset ---
  TFile file("/afs/cern.ch/work/x/xuyan/work5/PROD17/DATA/Run2/fullcut/Run2Data_postproc_WGammaRun2_SB_sigrange_fullcut_jmcorr_May22.root");
  TTree* tree = (TTree*)file.Get("Events");
  RooDataSet data("Data sideband","Data sideband",RooArgSet(*x),Import(*tree));//import branches with names match the "variable name" (not variable) listed in imargset
  RooRealVar weight("weight","weight",2.4826);
  RooDataSet data_norm("Data sideband norm.","Data sideband (nomalized to yield in W band)",&data,RooArgSet(*x,weight),"",weight.GetName());
  cout<<"number of data: "<<data.numEntries()<<endl;
  
  // --- Perform extended ML fit of composite PDF to toy data ---
#if isNorm == 1
    RooFitResult *r = model->fitTo(data_norm,Range(600,4000),RooFit::Minimizer("Minuit2"),SumW2Error(false),Save()); //SumW2Error(false) for weighted data, see how to choose this with same calling without SumW2Error(false)
#else
    RooFitResult *r = model->fitTo(data,Range(600,4000),RooFit::Minimizer("Minuit2"),Save()); //SumW2Error(false) for weighted data, see how to choose this with same calling without SumW2Error(false)
#endif
    
  // --- plot for chi2 calculation and visualization ---
  x->setBins(170); //fit is unbinned but chi2 is calculated by binning data with this value
  RooPlot *frame = x->frame();
#if isNorm == 1
    frame->SetTitle("Data Sideband nomalized to W band");
    RooDataHist datah("dh","binned data",RooArgSet(*x),data_norm);
    datah.plotOn(frame,RooFit::Name("datah"),Binning(170,600,4000),DataError(RooAbsData::SumW2)); //for weighted data
#else
    frame->SetTitle("Data Sideband");
    RooDataHist datah("dh","binned data",RooArgSet(*x),data);
    datah.plotOn(frame,RooFit::Name("datah"),Binning(170,600,4000),DataError(RooAbsData::Poisson)); //for unweighted data
#endif
  model->plotOn(frame,LineStyle(kDashed),RooFit::Name(fun_name));
  model->plotOn(frame,VisualizeError(*r,2,kFALSE),FillColor(kYellow),LineColor(0),RooFit::Name("err2"));
  model->plotOn(frame,VisualizeError(*r,1,kFALSE),FillColor(kGreen),LineColor(0),RooFit::Name("err1"));
  model->plotOn(frame,LineStyle(kDashed),RooFit::Name(fun_name));
#if isNorm == 1
    datah.plotOn(frame,RooFit::Name("datah"),Binning(170,600,4000),DataError(RooAbsData::SumW2)); //for weighted data
#else
    datah.plotOn(frame,RooFit::Name("datah"),Binning(170,600,4000),DataError(RooAbsData::Poisson)); //for unweighted data
#endif

    frame->Print("V");
  RooAbsReal* nll = NULL;
#if isNorm == 1
    nll = model->createNLL(data_norm);
#else
    nll = model->createNLL(data);
#endif
  
  cout<<"NLL is: "<<nll->getVal()<<endl;

  cout<<datah<<endl;

  cout<<"data bins: "<<datah.numEntries()<<endl;
  int n_0 = 0;
  for(int i=0; i<datah.numEntries(); i++){
    datah.get(i) ;
    if(datah.weight() == 0)
      n_0++;
  }
  cout<<"Number of empty bins: "<<n_0<<endl;
  
  RooArgSet *flparams = model->getParameters(*x);
  int nfloparam = (flparams->selectByAttrib("Constant",kFALSE))->getSize();
  cout<<"# of floating params: "<<nfloparam<<endl;

  // Chi2 problem solved:
  // Unweighted data:
  // RooChi2Var uses RooAbsData::Possion to obtain Possion interval (which can treat 0 entry bins), only skip bins with both PDF and data = 0
  // chi2square() obtains chi2 by skipping 0 entry bins, divide by numbr of non-0 entry bins
  // Weighted data:
  // RooChi2Var uses RooAbsData::SumW2, fail when arrive 0-entry bins
  // chi2square() obtains chi2 by skipping 0 entry bins, divide by numbr of non-0 entry bins
  RooChi2Var chi2 ("chi2", "chi2", *model,datah,DataError(RooAbsData::SumW2));//Default: SumW2
  TString chi2txt = "Chi2/Ndof: "+to_str_trim(frame->chiSquare(fun_name,"datah",nfloparam));
  
  cout<<"chi2 from RooRealVar with possion is: "<<chi2.getVal()<<endl;
  cout<<"reduced chi2/ndof is: "<<frame->chiSquare(fun_name,"datah", nfloparam)<<endl;
  cout<<"chi2/ndof is: "<<frame->chiSquare(fun_name,"datah")<<endl;
  double n_non0 = frame->chiSquare(fun_name,"datah", nfloparam)*2/(frame->chiSquare(fun_name,"datah", nfloparam)-frame->chiSquare(fun_name,"datah"));
  cout<<"Estimated number of bins from chi2/ndof: "<<n_non0<<endl;
  cout<<"compare: reduced chi2: "<<frame->chiSquare(fun_name,"datah", nfloparam)*(n_non0-nfloparam)<<" non-reduced chi2: "<<frame->chiSquare(fun_name,"datah")*n_non0<<endl;
  frame->Print("V");
  
  TString NLLtxt = "NLL: "+to_str_trim(nll->getVal());
  TLatex *NLLlax = new TLatex(2500,70,NLLtxt);
  NLLlax->SetTextSize(0.025);
  NLLlax->SetTextColor(kBlack);
  frame->addObject(NLLlax);

  // access residuals
  double SSR = 0;
  RooCurve* curve = (RooCurve*)frame->findObject(fun_name,RooCurve::Class());
  RooHist* hist = (RooHist*)frame->findObject("datah",RooHist::Class());
  Double_t xstart,xstop,y;
  curve->GetPoint(0,xstart,y);
  curve->GetPoint(curve->GetN()-1,xstop,y);
  for(Int_t i=0 ; i<hist->GetN() ; i++) {
    Double_t x,point;
    hist->GetPoint(i,x,point);
    // Only calculate pull for bins inside curve range
    if (x<xstart || x>xstop) continue;
    Double_t yy;
    yy = point - curve->interpolate(x);
    SSR += pow(yy,2);
  }
  cout<<"SSR is: "<<SSR<<" NdoF is: "<<hist->GetN()<<endl;
  TString SSRtxt = "SSR: "+to_str_trim(SSR);
  TLatex *SSRlax = new TLatex(2500,50,SSRtxt);
  SSRlax->SetTextSize(0.025);
  SSRlax->SetTextColor(kBlack);
  frame->addObject(SSRlax);

  // --- Visualization ---
  gStyle->SetOptStat(111111);
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
  xaxis->SetTitle("m_{j#gamma} (GeV)");
  xaxis->SetTitleOffset(1.2);
  yaxis->SetTitle("Events / 20 GeV");
  yaxis->SetTitleOffset(1.2);
  yaxis->SetRangeUser(0.2,10000);
  xaxis->SetRangeUser(600,4000);
  p01a->SetLogy();
  frame->Draw();
  CMS_lumi(p01a,iPeriod,iPos);
  TLegend *l =  new TLegend(0.5,0.7,0.85,0.85);
#if isNorm == 1
    l->AddEntry(frame->findObject(fun_name),"Data SB norm. fit "+fun_name,"l");
#else
    l->AddEntry(frame->findObject(fun_name),"Data SB fit "+fun_name,"l");
#endif
  //l->AddEntry(frame->findObject("bkgfun"),"Background Fit","l");
    l->AddEntry(frame->findObject("err1"),"Fit Error 1 #sigma","f");
    l->AddEntry(frame->findObject("err2"),"Fit Error 2 #sigma","f");
    l->Draw("same");
  
    p01b->cd();
	p01b->SetLeftMargin(0.11);
    p01b->SetRightMargin(0.1);
    p01b->SetTopMargin(0.2);
    p01b->SetBottomMargin(0.25);
	RooHist* hpull = frame->pullHist();
    RooPlot* pull_frame = x->frame();
    pull_frame->addPlotable(hpull,"P") ;
    hpull->SetMarkerStyle(8);
    xaxis = pull_frame->GetXaxis();
    yaxis = pull_frame->GetYaxis();
    xaxis->SetTitle("M_{j#gamma}");
    yaxis->SetTitle("#frac{data_norm-fit}{#sigma_{stat}}");
    yaxis->SetTitleOffset(0.5);
    yaxis->SetRangeUser(-5,5);
    xaxis->SetLabelSize(0.08);
    xaxis->SetTitleSize(0.08);
    xaxis->SetRangeUser(600,3000);
    yaxis->SetLabelSize(0.08);
    yaxis->SetTitleSize(0.08);
    yaxis->SetNdivisions(5);
    p01b->SetGrid();
    pull_frame->Draw();
    p01b->Update();
	
#if isNorm == 1
    c01->Print(fun_name+"_norm.png");
    c01->Print(fun_name+"_norm.pdf");
	c01->Print(fun_name+"_norm.svg");
#else
    c01->Print(fun_name+".png");
    c01->Print(fun_name+".pdf");
	c01->Print(fun_name+".svg");
#endif
  
  // --- Output root file ---
#if isNorm == 1
    RooWorkspace *w = new RooWorkspace("w","w");
    w->import(*x);
    w->import(data_norm,Rename("data_norm_sideband"));
    w->import(*model);
    w->writeToFile("sideband_norm-shapes-Unbinned-"+fun_name+".root");
#else
    RooWorkspace *w = new RooWorkspace("w","w");
    w->import(*x);
    w->import(data,Rename("data_sideband"));
    w->import(*model);
    w->writeToFile("sideband-shapes-Unbinned-"+fun_name+".root");
#endif
    
  

  /*
  // --- Perform extended ML fit of composite PDF to toy data ---
  RooFitResult *ex_r = NULL;
  if(isNorm)
    ex_r = ex_model->fitTo(data_norm,Range(600,3000),RooFit::Minimizer("Minuit2"),Extended(true),SumW2Error(false),Save());
  else
    ex_r = ex_model->fitTo(data,Range(600,3000),RooFit::Minimizer("Minuit2"),Extended(true),Save());
  cout<<"Normalization is: "<<bkg_norm->getVal()<<endl;
  */
  
}
