#define treeplot_cxx
// The class definition in treeplot.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("treeplot.C")
// root> T->Process("treeplot.C","some options")
// root> T->Process("treeplot.C+")
//


#include "treeplot.h"
#include <TROOT.h>
#include <TString.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TEfficiency.h>
#include <TLegend.h>
#include <TAxis.h>


Int_t count1 = 0;
Int_t count2 = 0;
TH1 *hist1 = new TH1F("1","pt_{#gamma}",100,0,2000);
TH1 *hist2 = new TH1F("2","eta_{#gamma}",50,-2,2);
TH1 *hist3 = new TH1F("3","pt_{j}",100,0,2000);
TH1 *hist4 = new TH1F("4","eta_{j}",50,-2,2);
TH1 *hist5 = new TH1F("5","E_{j}",100,0,2000);
TH1 *hist6 = new TH1F("6","masssoftdrop_{j}",60,50,110);
TH1 *hist7 = new TH1F("7","tau21_{j}",50,0,1);
TH1 *hist8 = new TH1F("8","cos(#theta*)_{p}",50,0,1);
TH1 *hist9 = new TH1F("9","pt/M",100,0,2);
TH1 *hist10 = new TH1F("10","invariant mass",100,0,3500);
TH1 *hist11 = new TH1F("11","seperation",50,0,2);
TH1 *hist12 = new TH1F("12","eta_diff",50,0,8);
TH1 *hist13 = new TH1F("13","phi_diff",50,0,8);
TH1 *hist14 = new TH1F("14","mvaID val",50,-1,1);

void treeplot::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).
  gROOT->SetBatch(1);
  cout<<"OK"<<endl;

   TString option = GetOption();
}

void treeplot::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t treeplot::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   fReader.SetEntry(entry);

   if(entry%1000==0)
     cout<<"Processing "<<entry<<endl;
   if(entry > 20000)
     return kTRUE;

   //double signalmass = 2000;
   //if((*ak8puppijet_masssoftdropcorr > 65 && *ak8puppijet_masssoftdropcorr < 105) && abs(*sys_invmass - signalmass) < signalmass*0.05 && *sys_ptoverm > 0.38 && *sys_costhetastar < 0.67 && abs(*photon_eta) < 0.9 && abs(*ak8puppijet_eta) < 1.32){
   if((*ak8puppijet_masssoftdropcorr > 40 && *ak8puppijet_masssoftdropcorr < 65))
     {
       hist1->Fill(*photon_pt);
       hist2->Fill(*photon_eta);
       hist3->Fill(*ak8puppijet_pt);
       hist4->Fill(*ak8puppijet_eta);
       hist5->Fill(*ak8puppijet_e);
       hist6->Fill(*ak8puppijet_masssoftdropcorr);
       hist7->Fill(*ak8puppijet_tau21);
       hist8->Fill(*sys_costhetastar);
       hist9->Fill(*sys_ptoverm);
       hist10->Fill(*sys_invmass);
       hist11->Fill(*sys_seperation);
       hist12->Fill(abs(abs(*photon_eta) - abs(*ak8puppijet_eta)));
       hist13->Fill(abs(*photon_phi - *ak8puppijet_phi));
       hist14->Fill(*photon_mvaval);
     }
   

   return kTRUE;
}

void treeplot::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void treeplot::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

  int color = 4;
  hist1->SetLineColor(color);
  hist2->SetLineColor(color);
  hist3->SetLineColor(color);
  hist4->SetLineColor(color);
  hist5->SetLineColor(color);
  hist6->SetLineColor(color);
  hist7->SetLineColor(color);
  hist8->SetLineColor(color);
  hist9->SetLineColor(color);
  hist10->SetLineColor(color);
  hist11->SetLineColor(color);
  hist12->SetLineColor(color);
  hist13->SetLineColor(color);
  hist14->SetLineColor(color);

  double norm = (double) hist1->GetEntries();
  hist1->Scale(1/norm);
  hist2->Scale(1/norm);
  hist3->Scale(1/norm);
  hist4->Scale(1/norm);
  hist5->Scale(1/norm);
  hist6->Scale(1/norm);
  hist7->Scale(1/norm);
  hist8->Scale(1/norm);
  hist9->Scale(1/norm);
  hist10->Scale(1/norm);
  hist11->Scale(1/norm);
  hist12->Scale(1/norm);
  hist13->Scale(1/norm);
  hist14->Scale(1/norm);
  

  gStyle->SetOptStat(0);
  
  //Non stacked plots

  TLegend *legend = new TLegend(0.65,0.8,0.9,0.9);

  TCanvas *c01 = new TCanvas("c01","pt_{#gamma}",1200,900);
  TAxis *xaxis = hist1->GetXaxis();
  TAxis *yaxis = hist1->GetYaxis();
  xaxis->SetTitle("pt_{#gamma} (GeV)");
  yaxis->SetTitle("Entries");
  yaxis->SetTitleOffset(1.3);
  yaxis->SetRangeUser(0.00001, 1);
  c01->SetLogy();
  c01->cd();
  hist1->SetLineWidth(2);
  hist1->Draw("HIST");
  legend->Clear();
  //legend->AddEntry(hist1,"2017 SinglePhoton C","f");
  //legend->Draw();
  c01->Print("p_pt.png");

  TCanvas *c02 = new TCanvas("c02","eta_{#gamma}",1200,900);
  xaxis = hist2->GetXaxis();
  yaxis = hist2->GetYaxis();
  xaxis->SetTitle("eta_{#gamma}");
  yaxis->SetTitle("Entries");
  yaxis->SetTitleOffset(1.3);
  yaxis->SetRangeUser(0.00001, 1);
  c02->SetLogy();
  c02->cd();
  hist2->SetLineWidth(2);
  hist2->Draw("HIST");
  legend->Clear();
  //legend->AddEntry(hist2,"2017 signal MC , pass EleVeto","f");
  //legend->Draw();
  c02->Print("p_eta.png");

  TCanvas *c03 = new TCanvas("c03","pt AK8Jet",1200,900);
  xaxis = hist3->GetXaxis();
  yaxis = hist3->GetYaxis();
  xaxis->SetTitle("pt AK8Jet (GeV)");
  yaxis->SetTitle("Entries");
  yaxis->SetTitleOffset(1.3);
  yaxis->SetRangeUser(0.00001, 1);
  //yaxis->SetRangeUser(0,2000);
  c03->SetLogy();
  c03->cd();
  hist3->SetLineWidth(2);
  hist3->Draw("HIST");
  legend->Clear();
  //legend->AddEntry(hist5,"2017 signal MC ","f");
  //legend->Draw();
  c03->Print("j_pt.png");

  TCanvas *c04 = new TCanvas("c04","eta AK8Jet",1200,900);
  xaxis = hist4->GetXaxis();
  yaxis = hist4->GetYaxis();
  xaxis->SetTitle("eta AK8Jet");
  yaxis->SetTitle("Entries");
  yaxis->SetTitleOffset(1.3);
  yaxis->SetRangeUser(0.00001, 1);
  c04->SetLogy();
  c04->cd();
  hist4->SetLineWidth(2);
  hist4->Draw("HIST");
  legend->Clear();
  //legend->AddEntry(hist4,"2017 signal MC ","f");
  //legend->Draw();
  c04->Print("j_eta.png");

  TCanvas *c05 = new TCanvas("c05","E AK8Jet",1200,900);
  xaxis = hist5->GetXaxis();
  yaxis = hist5->GetYaxis();
  xaxis->SetTitle("E AK8Jet (GeV)");
  yaxis->SetTitle("Entries");
  yaxis->SetTitleOffset(1.3);
  yaxis->SetRangeUser(0.00001, 1);
  c05->SetLogy();
  c05->cd();
  hist5->SetLineWidth(2);
  hist5->Draw("HIST");
  legend->Clear();
  //legend->AddEntry(hist5,"2017 signal MC ","f");
  //legend->Draw();
  c05->Print("j_e.png");

  TCanvas *c06 = new TCanvas("c06","mass softdrop AK8Jet",1200,900);
  xaxis = hist6->GetXaxis();
  yaxis = hist6->GetYaxis();
  xaxis->SetTitle("mass softdrop AK8Jet (GeV)");
  yaxis->SetTitle("Entries");
  yaxis->SetTitleOffset(1.3);
  yaxis->SetRangeUser(0.00001, 1);
  //yaxis->SetRangeUser(0,400);
  c06->SetLogy();
  c06->cd();
  hist6->SetLineWidth(2);
  hist6->Draw("HIST");
  legend->Clear();
  //legend->AddEntry(hist6,"2017 signal MC ","f");
  //legend->Draw();
  c06->Print("j_masssd.png");

  TCanvas *c07 = new TCanvas("c07","tau21 AK8Jet",1200,900);
  xaxis = hist7->GetXaxis();
  yaxis = hist7->GetYaxis();
  xaxis->SetTitle("tau21 AK8Jet");
  yaxis->SetTitle("Entries");
  yaxis->SetTitleOffset(1.3);
  yaxis->SetRangeUser(0.00001, 1);
  c07->SetLogy();
  c07->cd();
  hist7->SetLineWidth(2);
  hist7->Draw("HIST");
  legend->Clear();
  //legend->AddEntry(hist7,"2017 signal MC ","f");
  //legend->Draw();
  c07->Print("j_tau21.png");

  TCanvas *c08 = new TCanvas("c8","cos AK8Jet",1200,900);
  xaxis = hist8->GetXaxis();
  yaxis = hist8->GetYaxis();
  xaxis->SetTitle("cos(#theta*)");
  yaxis->SetTitle("Entries");
  yaxis->SetTitleOffset(1.3);
  yaxis->SetRangeUser(0.00001, 1);
  c08->SetLogy();
  c08->cd();
  hist8->SetLineWidth(2);
  hist8->Draw("HIST");
  legend->Clear();
  //legend->AddEntry(hist8,"2017 signal MC ","f");
  //legend->Draw();
  c08->Print("s_cos.png");

  TCanvas *c09 = new TCanvas("c9","ptm AK8Jet",1200,900);
  xaxis = hist9->GetXaxis();
  yaxis = hist9->GetYaxis();
  xaxis->SetTitle("pt/M");
  yaxis->SetTitle("Entries");
  yaxis->SetTitleOffset(1.3);
  yaxis->SetRangeUser(0.00001, 1);
  c09->SetLogy();
  c09->cd();
  hist9->SetLineWidth(2);
  hist9->Draw("HIST");
  legend->Clear();
  //legend->AddEntry(hist9,"2017 signal MC ","f");
  //legend->Draw();
  c09->Print("s_ptm.png");

  TCanvas *c10 = new TCanvas("c10","invmass",1200,900);
  xaxis = hist10->GetXaxis();
  yaxis = hist10->GetYaxis();
  xaxis->SetTitle("M_{j#gamma} (GeV)");
  yaxis->SetTitle("Entries");
  yaxis->SetTitleOffset(1.3);
  yaxis->SetRangeUser(0.00001, 1);
  c10->SetLogy();
  c10->cd();
  hist10->SetLineWidth(2);
  hist10->Draw("HIST");
  legend->Clear();
  //legend->AddEntry(hist10,"2017 signal MC ","f");
  //legend->Draw();
  c10->Print("s_invmass.png");

  TCanvas *c11 = new TCanvas("c11","seperation",1200,900);
  xaxis = hist11->GetXaxis();
  yaxis = hist11->GetYaxis();
  xaxis->SetTitle("#Delta R");
  yaxis->SetTitle("Entries");
  yaxis->SetTitleOffset(1.3);
  yaxis->SetRangeUser(0.00001, 1);
  c11->SetLogy();
  c11->cd();
  hist11->SetLineWidth(2);
  hist11->Draw("HIST");
  legend->Clear();
  //legend->AddEntry(hist11,"2017 signal MC ","f");
  //legend->Draw();
  c11->Print("s_seperation.png");

  TCanvas *c12 = new TCanvas("c12","eta_eiff",1200,900);
  xaxis = hist12->GetXaxis();
  yaxis = hist12->GetYaxis();
  xaxis->SetTitle("#eta diff");
  yaxis->SetTitle("Entries");
  yaxis->SetTitleOffset(1.3);
  yaxis->SetRangeUser(0.00001, 1);
  c12->SetLogy();
  c12->cd();
  hist12->SetLineWidth(2);
  hist12->Draw("HIST");
  legend->Clear();
  //legend->AddEntry(hist12,"2017 signal MC ","f");
  //legend->Draw();
  c12->Print("s_etadiff.png");

  TCanvas *c13 = new TCanvas("c13","phi_eiff",1200,900);
  xaxis = hist13->GetXaxis();
  yaxis = hist13->GetYaxis();
  xaxis->SetTitle("#phi diff");
  yaxis->SetTitle("Entries");
  yaxis->SetTitleOffset(1.3);
  yaxis->SetRangeUser(0.00001, 1);
  c13->SetLogy();
  c13->cd();
  hist13->SetLineWidth(2);
  hist13->Draw("HIST");
  legend->Clear();
  //legend->AddEntry(hist13,"2017 signal MC ","f");
  //legend->Draw();
  c13->Print("s_phidiff.png");

  TCanvas *c14 = new TCanvas("c14","mvaID val",1200,900);
  xaxis = hist14->GetXaxis();
  yaxis = hist14->GetYaxis();
  xaxis->SetTitle("Photon mvaID val");
  yaxis->SetTitle("Entries");
  yaxis->SetTitleOffset(1.3);
  yaxis->SetRangeUser(0.00001, 1);
  c14->SetLogy();
  c14->cd();
  hist14->SetLineWidth(2);
  hist14->Draw("HIST");
  legend->Clear();
  //legend->AddEntry(hist14,"2017 signal MC ","f");
  //legend->Draw();
  c14->Print("p_mvaID.png");

}
