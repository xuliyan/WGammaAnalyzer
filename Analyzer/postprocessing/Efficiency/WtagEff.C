#include "/afs/cern.ch/work/x/xuyan/work5/PROD17/AN/AN-19-280/utils/general/CMS_lumi.C"
void WtagEff(){
	
	gROOT->SetBatch(1);
	lumi_13TeV = "";
	writeExtraText = 1;
	lumiTextOffset = 0.15;
	bool plot_CMS = true;
	extraText = "Simulation";
	lumiTextSize = 0.45;
	cmsTextSize = 0.55;
	int iPeriod = 5;
	int iPos = 0;
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);
	gStyle->SetTitleSize(0.05,"XYZ");
	gStyle->SetLabelSize(0.05,"XYZ");
	gStyle->SetFrameLineWidth(2);
	gStyle->SetLegendTextSize(0.03);
	gStyle->SetHistLineWidth(3);
	gStyle->SetEndErrorSize(5);
  
	TF1 f1("fun1","pol4",700,8000);
	TF1 f2("fun2","pol4",700,8000);
	TF1 f3("fun3","pol4",700,8000);
	TF1 f4("fun4","pol4",700,8000);
	TF1 f5("fun5","pol4",700,8000);
	TF1 f6("fun6","pol4",700,8000);
	TF1 f7("fun7","pol4",700,8000);
	TF1 f8("fun8","pol4",700,8000);
	
	f1.SetParameters(0.1205,2.27175e-05,-1.61675e-08,3.01212e-12,-1.75654e-16);
	f2.SetParameters(0.125982,6.60661e-06,-9.84768e-09,1.93535e-12,-1.19678e-16);
	f3.SetParameters(0.086658,9.71057e-05,-4.07936e-08,6.37841e-12,-3.44559e-16);
	f4.SetParameters(0.0409758,0.000139242,-5.68289e-08,8.87649e-12,-4.79103e-16);
	f5.SetParameters(0.133411,0.000128356,-5.31035e-08,8.31797e-12,-4.41572e-16);
	f6.SetParameters(0.141324,0.000104595,-4.50253e-08,6.94785e-12,-3.76472e-16);
	f7.SetParameters(0.0897404,0.000243561,-9.09059e-08,1.34751e-11,-6.96436e-16);
	f8.SetParameters(0.0199947,0.000307806,-1.1486e-07,1.70622e-11,-8.85001e-16);
	
	TGraph *gr9 = new TGraph();
	TGraph *gr10 = new TGraph();
	TGraph *gr13 = new TGraph();
	TGraph *gr14 = new TGraph();
	TGraphErrors *gr1 = new TGraphErrors();

	int ipoints = 0;
	for(int i=700; i<8001; i+=10){
	  TH1F temp("","",1000,0,1);
	  gr9->SetPoint(ipoints,i,f1.Eval(i)/f5.Eval(i));
	  gr10->SetPoint(ipoints,i,f2.Eval(i)/f6.Eval(i));
	  gr13->SetPoint(ipoints,i,f3.Eval(i)/f7.Eval(i));
	  gr14->SetPoint(ipoints,i,f4.Eval(i)/f8.Eval(i));
	  temp.Fill(f1.Eval(i)/f5.Eval(i));
	  temp.Fill(f2.Eval(i)/f6.Eval(i));
	  temp.Fill(f3.Eval(i)/f7.Eval(i));
	  temp.Fill(f4.Eval(i)/f8.Eval(i));
	  gr1->SetPoint(ipoints,i,(f1.Eval(i)/f5.Eval(i) + f2.Eval(i)/f6.Eval(i) + f3.Eval(i)/f7.Eval(i) + f4.Eval(i)/f8.Eval(i))/4);
	  gr1->SetPointError(ipoints,0,temp.GetStdDev());
	  ipoints++;
	}
	
	gr9->SetLineColor(kPink+9);
	gr10->SetLineColor(kBlue);
	gr13->SetLineColor(kPink+9);
	gr14->SetLineColor(kBlue);
	gr1->SetLineColor(kSpring-1);
	gr1->SetFillColor(kSpring-1);
	gr1->SetFillStyle(3244);
	gr9->SetLineWidth(3);
	gr10->SetLineWidth(3);
	gr13->SetLineWidth(3);
	gr14->SetLineWidth(3);
	gr1->SetLineWidth(3);
	gr13->SetLineStyle(2);
	gr14->SetLineStyle(2);
	
	TLegend *leg = new TLegend(0.3,0.6,0.85,0.75);
	leg->SetBorderSize(0);
	leg->SetNColumns(2);
	leg->SetFillStyle(0);

	TCanvas *c3 = new TCanvas("c3","",2400,1800);
	c3->cd();
	c3->SetBottomMargin(0.15);
	c3->SetLeftMargin(0.13);
	TAxis *xaxis = gr1->GetXaxis();
	TAxis *yaxis = gr1->GetYaxis();
	xaxis->SetTitle("m_{X} [GeV]");
	xaxis->SetTitleOffset(1);
	xaxis->SetLimits(500,8200); 
	yaxis->SetRangeUser(0.3,0.7);
	yaxis->SetTitle("#varepsilon_{W tag}");
	yaxis->SetTitleOffset(1.2);
	gr1->Draw("A4 L");
	gr9->Draw("LSAME");
	gr10->Draw("LSAME");
	gr13->Draw("LSAME");
	gr14->Draw("LSAME");
	
	lumi_13TeV = "";
	CMS_lumi(c3,5,iPos);
	leg->Clear();
	leg->AddEntry(gr9,"Spin-0, #Gamma_{X} / m_{X} = 0.01%","l");
	leg->AddEntry(gr10,"Spin-0, #Gamma_{X} / m_{X} = 5%","l");
	leg->AddEntry(gr13,"Spin-1, #Gamma_{X} / m_{X} = 0.01%","l");
	leg->AddEntry(gr14,"Spin-1, #Gamma_{X} / m_{X} = 5%","l");
	leg->AddEntry(gr1,"2017 Average","fl");
	leg->Draw();
	//c3->SetGrid();
	c3->Print("WtagEff17.png");
	c3->Print("WtagEff17.pdf");
	c3->Print("WtagEff17.svg");
}