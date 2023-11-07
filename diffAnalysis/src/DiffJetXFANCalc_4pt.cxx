// Filename: DiffJetAnaTreeQa.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Jun 11 13:08:16 2020 (-0400)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "StRootInclude.h"
#include <vector>
#include <iostream>
#include "TGraph.h"

using namespace std;

//Use wildcat in the file name to combine many files.
void DiffJetXFANCalc_4pt(TString inFileName, TString outName, TString outNameText, TString det)
{

    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    gStyle->SetOptDate(0);
    gStyle->SetEndErrorSize(10);

    TChain *ch = new TChain("T");
    
    TFile *file = new TFile(inFileName);   
    TFile *outFile = new TFile(outName, "recreate");

    ofstream outputtext;
    outputtext.open(outNameText);
    //ifstream sysinputtext;
    //sysinputtext.open("run15_FMS_SystematicError_in.txt");


    TH2D *BlueBeamUp;
    TH2D *BlueBeamDown;
    TH2D *YellowBeamUp;
    TH2D *YellowBeamDown;
    TH1D *polBlue_Allfill;
    TH1D *polYellow_Allfill;
    TH1D *h1_XF1015;
    TH1D *h1_XF1520;
    TH1D *h1_XF2025;
    TH1D *h1_XF2545;


    BlueBeamUp = (TH2D*) file->Get("h2_BU_xF_vs_phi");
    BlueBeamDown = (TH2D*) file->Get("h2_BD_xF_vs_phi");
    YellowBeamUp = (TH2D*) file->Get("h2_YU_xF_vs_phi");
    YellowBeamDown = (TH2D*) file->Get("h2_YD_xF_vs_phi");
    polBlue_Allfill = (TH1D*) file->Get("polBlue_Allfill");
    polYellow_Allfill = (TH1D*) file->Get("polYellow_Allfill");
    h1_XF1015 = (TH1D*) file->Get("h1_xF_1015");
    h1_XF1520 = (TH1D*) file->Get("h1_xF_1520");
    h1_XF2025 = (TH1D*) file->Get("h1_xF_2025");
    h1_XF2545 = (TH1D*) file->Get("h1_xF_2545");

    Double_t pi=3.14;
    Double_t polB,polY;
    int nDrawPoints = 4;
    ifstream Frac_file;
    Frac_file.open("frac.txt");
	Double_t Frac_sig[4], Frac_un[4];
    for (int i=0; i<4; i++) {Frac_file >> Frac_sig[i] >> Frac_un[i];}
    //int nDrawPoints = 7;

    if (det == "fms")
    {

	int XFbins, nXFusebins, Phibins;
	XFbins = 20;
	nXFusebins = 6;
	Phibins=16;
	Double_t BlueUpNum[Phibins+1][XFbins+1];
	Double_t BlueDownNum[Phibins+1][XFbins+1];
	Double_t YellowUpNum[Phibins+1][XFbins+1];
	Double_t YellowDownNum[Phibins+1][XFbins+1];
	Double_t PhiPoint[] = {pi/16.,3*pi/16.,5*pi/16.,7*pi/16.,9*pi/16.,11*pi/16.,13*pi/16.,15*pi/16.};

	

	Double_t PhiPointErr[] = {0,0,0,0,0,0,0,0};
	//Double_t XFpoint[] = {0.05,0.15,0.25,0.35,0.45,0.55};
	//Double_t XFpoint[] = {0.025,0.075,0.125,0.175,0.225,0.275,0.325,0.375};
	//Double_t XFpoint[] = {0.125,0.175,0.225,0.275,0.325,0.375,0.425,0.475};
	Double_t XFpoint[4];
	XFpoint[0] = h1_XF1015->GetMean();
	XFpoint[1] = h1_XF1520->GetMean();
	XFpoint[2] = h1_XF2025->GetMean();
	XFpoint[3] = h1_XF2545->GetMean();
	//Double_t XFpointYellow[] = {0.120,0.170,0.220,0.270,0.320,0.370,0.420,0.470};
	Double_t XFpointYellow[4];
	XFpointYellow[0] = XFpoint[0] - 0.005;
	XFpointYellow[1] = XFpoint[1] - 0.005;
	XFpointYellow[2] = XFpoint[2] - 0.005;
	XFpointYellow[3] = XFpoint[3] - 0.005;
	Double_t XFpointErr[] = {0,0,0,0,0,0,0,0};
	Double_t XFSysBluepointErr[] = {0.002,0.002,0.002,0.002,0.002,0.002,0.002,0.002};
	Double_t XFSysYellowpointErr[] = {0.001,0.001,0.001,0.001,0.001,0.001,0.001,0.001};

	Double_t SysErr[20];
	Double_t YellowSysErr[20];
	Double_t sysB , sysY;

	polB=polBlue_Allfill->GetMean();
	polY=polYellow_Allfill->GetMean();

	cout << "pol Blue:" << polB << "pol Yellow:" << polY << endl;

	//get number of event within each bin by XF and phi
	for (int i=0; i<Phibins ; i=i+1)
	{

		for (int j=2; j<XFbins; j=j+1)
		{

			BlueUpNum[i][j] = BlueBeamUp->GetBinContent(i+1,j+1);
			cout<<"Blue beam Up x:"<<i<<" y:"<<j<<" n entries:"<<BlueUpNum[i][j]<<endl; 
			BlueDownNum[i][j] = BlueBeamDown->GetBinContent(i+1,j+1);
			cout<<"Blue beam Down x:"<<i<<" y:"<<j<<" n entries:"<<BlueDownNum[i][j]<<endl; 
			YellowUpNum[i][j] = YellowBeamUp->GetBinContent(i+1,j+1);
			cout<<"Yellow beam Up x:"<<i<<" y:"<<j<<" n entries:"<<BlueUpNum[i][j]<<endl; 
			YellowDownNum[i][j] = YellowBeamDown->GetBinContent(i+1,j+1);
			cout<<"Yellow beam Down x:"<<i<<" y:"<<j<<" n entries:"<<BlueDownNum[i][j]<<endl; 

		}

	}

	Double_t rawAN[Phibins/2];
	Double_t rawANErr[Phibins/2];
	Double_t YellowrawAN[Phibins/2];
	Double_t YellowrawANErr[Phibins/2];
	Double_t ANresult[nXFusebins-2];
	Double_t ANresultErr[nXFusebins-2];
	Double_t YellowANresult[nXFusebins-2];
	Double_t YellowANresultErr[nXFusebins-2];
	Double_t ANresult_sig[nXFusebins-2];
	Double_t ANresultErr_sig[nXFusebins-2];
	Double_t YellowANresult_sig[nXFusebins-2];
	Double_t YellowANresultErr_sig[nXFusebins-2];
	Double_t a,b,c,d ,  rawANnomin , rawANdenomin, rawANErrnomin , rawANErrdenomin;
	Double_t xF;

	TGraphErrors *BlueBeamRawAN[nXFusebins];
	TGraphErrors *BlueBeamAN;
	TGraphErrors *BlueBeamAN_sig;
	TGraphErrors *BlueBeamSys;
	TGraphErrors *YellowBeamRawAN[nXFusebins];
	TGraphErrors *YellowBeamAN;
	TGraphErrors *YellowBeamAN_sig;
	TGraphErrors *YellowBeamSys;

	
	TF1 *BlueFit;
	TF1 *YellowFit;

	for (int j=2; j<nXFusebins; j=j+1)
	{


		for(int i=0; i<(Phibins / 2); i=i+1)
		{

			a = BlueUpNum[i+(Phibins/2)][j]; //a: N^{up} (phi)
			b = BlueDownNum[i][j]; //b: N^{down} (phi + pi)
			c = BlueDownNum[i+(Phibins/2)][j]; //c: N^{down} (phi)
			d = BlueUpNum[i][j]; //d: N^{up} (phi + pi)

			rawANnomin = sqrt(a*b) - sqrt(c*d);
			rawANdenomin = sqrt(a*b) + sqrt(c*d);
			rawAN[i] = rawANnomin / rawANdenomin;

			rawANErrnomin = b*c*d + a*c*d + a*b*d + a*b*c;
			rawANErrdenomin = pow(rawANdenomin,4);
			rawANErr[i] = sqrt(1.0*rawANErrnomin / rawANErrdenomin);
			cout<<"XF "<<(0.05*(j))<<" to "<<(0.05*(j+1))<<" phi bin: "<<(0.125*(i))<<"pi to "<<(0.125*(i+1))<<"pi  raw AN="<<rawAN[i]<<"  raw AN uncertainty="<<rawANErr[i]<<endl;

			a = YellowUpNum[i+(Phibins/2)][j]; //a: N^{up} (phi)
			b = YellowDownNum[i][j]; //b: N^{down} (phi + pi)
			c = YellowDownNum[i+(Phibins/2)][j]; //c: N^{down} (phi)
			d = YellowUpNum[i][j]; //d: N^{up} (phi + pi)

			rawANnomin = sqrt(a*b) - sqrt(c*d);
			rawANdenomin = sqrt(a*b) + sqrt(c*d);
			YellowrawAN[i] = rawANnomin / rawANdenomin;

			rawANErrnomin = b*c*d + a*c*d + a*b*d + a*b*c;
			rawANErrdenomin = pow(rawANdenomin,4);
			YellowrawANErr[i] = sqrt(rawANErrnomin / rawANErrdenomin);


		}
		
		BlueBeamRawAN[j] = new TGraphErrors(Phibins / 2,PhiPoint , rawAN, PhiPointErr,rawANErr);
		BlueBeamRawAN[j]->SetName(Form("BlueBeamRawAN_%i",j));
		BlueBeamRawAN[j]->SetTitle(Form("Blue Beam Raw A_{N} by x_{F} %.2f to %.2f",0.05*j, 0.05*(j+1)));
		BlueBeamRawAN[j]->GetXaxis()->SetTitle("#phi");
		BlueBeamRawAN[j]->GetYaxis()->SetTitle("raw A_{N}");
		//BlueBeamRawAN[j]->Write();

		BlueFit = new TF1("BlueFit","[0]*cos(x) + [1]");
		BlueFit->SetParLimits(1,-0.2,0.2);
		BlueBeamRawAN[j]->Fit("BlueFit");
		ANresult[j-2] = 1.0* BlueFit->GetParameter(0) / polB;
		ANresultErr[j-2] = 1.0* BlueFit->GetParError(0) / polB;
		ANresult_sig[j-2] = 1.0* ANresult[j-2] / Frac_sig[j-2];
		ANresultErr_sig[j-2] = 1.0* ANresultErr[j-2] / Frac_sig[j-2];
		BlueBeamRawAN[j]->Write();

		//sysinputtext>>xF>>sysB >> sysY;
		//SysErr[j-2] = fabs(ANresult[j-2] * sysB);
		cout<<"AN result "<<j-2<<": "<<ANresult[j-2]<<"	AN statics un: "<<ANresultErr[j-2]<<endl;

		outputtext<<"B"<<"    "<<XFpoint[j-2]<<"    "<<ANresult[j-2]<<"    "<<ANresultErr[j-2]<<endl;

		YellowBeamRawAN[j] = new TGraphErrors(Phibins / 2,PhiPoint , YellowrawAN, PhiPointErr,YellowrawANErr);
		YellowBeamRawAN[j]->SetName(Form("YellowBeamRawAN_%i",j));
		YellowBeamRawAN[j]->SetTitle(Form("Yellow Beam Raw A_{N} by x_{F} %.2f to %.2f",0.05*j, 0.05*(j+1)));
		YellowBeamRawAN[j]->GetXaxis()->SetTitle("#phi");
		YellowBeamRawAN[j]->GetYaxis()->SetTitle("raw A_{N}");
		//BlueBeamRawAN[j]->Write();

		YellowFit = new TF1("YellowFit","[0]*cos(x) + [1]");
		YellowFit->SetParLimits(1,-0.2,0.2);
		YellowBeamRawAN[j]->Fit("YellowFit");
		YellowANresult[j-2] = YellowFit->GetParameter(0) / polY;
		YellowANresultErr[j-2] = YellowFit->GetParError(0) / polY;
		YellowANresult_sig[j-2] = 1.0* YellowANresult[j-2] / Frac_sig[j-2];
		YellowANresultErr_sig[j-2] = 1.0* YellowANresultErr[j-2] / Frac_sig[j-2];
		YellowBeamRawAN[j]->Write();
		//YellowSysErr[j-2] = fabs(YellowANresult[j-2] * sysY);
		outputtext<<"Y"<<"    "<<XFpoint[j-2]<<"    "<<YellowANresult[j-2]<<"    "<<YellowANresultErr[j-2]<<endl;
	}

	//BlueBeamAN = new TGraphErrors((nXFusebins-2) , XFpoint , ANresult , XFpointErr , ANresultErr);
	BlueBeamAN = new TGraphErrors(nDrawPoints , XFpoint , ANresult , XFpointErr , ANresultErr);
	TF1 *ANconstant = new TF1("ANconstant","[0]");
	//BlueBeamAN->Fit("ANconstant");
	BlueBeamAN->SetName("BlueBeamAN");
	BlueBeamAN->SetTitle("A_{N} (x_{F} > 0)");
	BlueBeamAN->GetXaxis()->SetTitle("X_{F}");
	BlueBeamAN->GetYaxis()->SetTitle("A_{N}");
	BlueBeamAN->Write();
	//ANconstant->SetLineWidth(3);
	//ANconstant->Write();
	//cout<<"Blue beam A_{N} with constant fit:"<<ANconstant->GetParameter(0)<<"+/-"<<ANconstant->GetParError(0)<<" value/error ratio:"<<1.0*fabs(ANconstant->GetParameter(0))/(ANconstant->GetParError(0))<<endl;

	//BlueBeamSys = new TGraphErrors(nDrawPoints , XFpoint , ANresult , XFSysBluepointErr , SysErr);
	//BlueBeamSys->SetTitle("A_{N} Sys (x_{F} > 0)");
	//BlueBeamSys->GetXaxis()->SetTitle("X_{F}");
	//BlueBeamSys->GetYaxis()->SetTitle("A_{N}");
	//YellowBeamAN = new TGraphErrors((nXFusebins-2) , XFpoint , YellowANresult , XFpointErr , YellowANresultErr);
	YellowBeamAN = new TGraphErrors(nDrawPoints , XFpointYellow , YellowANresult , XFpointErr , YellowANresultErr);
	TF1 *ANconstantY = new TF1("ANconstantY","[0]");
	//YellowBeamAN->Fit("ANconstantY");
	//YellowBeamAN->GetFunction("ANconstantY")->SetLineColor(6);
	YellowBeamAN->SetName("YellowBeamAN");
	YellowBeamAN->SetTitle("A_{N} (x_{F} < 0)");
	YellowBeamAN->GetXaxis()->SetTitle("X_{F}");
	YellowBeamAN->GetYaxis()->SetTitle("A_{N}");
	YellowBeamAN->Write();
	//ANconstantY->SetLineColor(6);
	//ANconstantY->SetLineWidth(3);
	//ANconstantY->Write();
	//cout<<"Yellow beam A_{N} with constant fit:"<<ANconstantY->GetParameter(0)<<"+/-"<<ANconstantY->GetParError(0)<<" value/error ratio:"<<1.0*fabs(ANconstantY->GetParameter(0))/(ANconstantY->GetParError(0))<<endl;
	//YellowBeamSys = new TGraphErrors(nDrawPoints , XFpointYellow , YellowANresult , XFSysYellowpointErr , YellowSysErr);
	//YellowBeamSys->SetTitle("A_{N} Sys (x_{F} > 0)");
	//YellowBeamSys->GetXaxis()->SetTitle("X_{F}");
	//YellowBeamSys->GetYaxis()->SetTitle("A_{N}");

	TCanvas *ANplot = new TCanvas("ANPlot","",200,10,1024,768);
	ANplot->DrawFrame(2., -0.06, 250., 0.2);
	//ANplot->SetLeftMargin(0.05);
	//ANplot->SetRightMargin(0.05);
	TH2D * temp = new TH2D("temp","A_{N}",8,0.1,0.35,20,-0.06,0.06);
	//TH2D * temp = new TH2D("temp","A_{N}",8,0.1,0.35,20,-0.05,0.05);
	temp->GetYaxis()->SetTitle("A_{N}");
  	temp->GetYaxis()->SetTitleOffset(0.9);
  	temp->GetXaxis()->SetTitle("|x_{F}|");
  	temp->Draw();
	YellowBeamAN->SetMarkerColor(kRed+1);
	YellowBeamAN->SetMarkerSize(2);
	YellowBeamAN->SetMarkerStyle(22);
	YellowBeamAN->SetLineWidth(2);
	YellowBeamAN->SetLineColor(kRed+1);
	YellowBeamAN->Draw("P");
	//YellowBeamSys->SetMarkerColor(kRed+1);
	//YellowBeamSys->SetMarkerSize(2);
	//YellowBeamSys->SetLineColor(kRed+1);
	//YellowBeamSys->SetLineWidth(2);
	//YellowBeamSys->SetFillStyle(0);
	//YellowBeamSys->SetFillColorAlpha(1,0.5);
	//YellowBeamSys->Draw("e5");
	BlueBeamAN->SetMarkerColor(kBlue+1);
	BlueBeamAN->SetMarkerSize(2);
	BlueBeamAN->SetMarkerStyle(20);
	BlueBeamAN->SetLineColor(kBlue+1);
	BlueBeamAN->SetLineWidth(2);
	BlueBeamAN->Draw("P");
	//BlueBeamSys->SetMarkerColor(kBlue+1);
	//BlueBeamSys->SetMarkerSize(2);
	//BlueBeamSys->SetLineColor(kBlue+1);
	//BlueBeamSys->SetLineWidth(2);
	//BlueBeamSys->SetFillStyle(0);
	//BlueBeamSys->SetFillColorAlpha(1,0.5);
	//BlueBeamSys->Draw("e5");

	TLegend *leg = new TLegend(0.7,0.2,0.88,0.35);
	leg->SetTextSize(0.06);
	leg->AddEntry(BlueBeamAN,"x_{F} > 0","p");
	leg->AddEntry(YellowBeamAN,"x_{F} < 0","p");
	leg->SetBorderSize(0);
	leg->Draw("same");
	TLine *line1 = new TLine(0.1, 0, 0.35, 0);
	//TLine *line1 = new TLine(0.1, 0, 0.35, 0);
  	line1->SetLineStyle(7);
  	line1->Draw("same");

	//TLatex T1;
	//T1.SetTextAlign(11);
   	//T1.SetTextSize(0.035);
	//T1.SetTextColor(kRed);
	//T1.DrawLatex(0.11,0.09,"STAR Preliminary");

	TLatex T2;
        T2.SetTextAlign(11);
        T2.SetTextSize(0.035);
        T2.SetTextColor(kBlack);
   	T2.DrawLatex(0.11,0.05,"p^{#uparrow} + p #rightarrow EM jet + p + X");
   	T2.DrawLatex(0.11,0.04,"#sqrt{s} = 510 GeV");
   	//T2.DrawLatex(0.11,-0.09,"3.0\% polarization scale uncertainty not shown");
	
	
	ANplot->SaveAs("FMS_ANplot_4pt.pdf");
	ANplot->SaveAs("FMS_ANplot_4pt.png");
	ANplot->SaveAs("FMS_ANplot_4pt.root");


	//draw signal AN
	TCanvas *ANplot2 = new TCanvas("ANPlot2","",200,10,1024,768);
	ANplot2->DrawFrame(2., -0.06, 250., 0.2);
	//ANplot->SetLeftMargin(0.05);
	//ANplot->SetRightMargin(0.05);
	TH2D * temp2 = new TH2D("temp2","A_{N}",8,0.1,0.35,20,-0.1,0.15);
	//TH2D * temp = new TH2D("temp","A_{N}",8,0.1,0.35,20,-0.05,0.05);
	temp2->GetYaxis()->SetTitle("A_{N}");
  	temp2->GetYaxis()->SetTitleOffset(0.9);
  	temp2->GetXaxis()->SetTitle("|x_{F}|");
  	temp2->Draw();

	//TLegend *leg = new TLegend(0.7,0.2,0.88,0.35);
	//leg->SetTextSize(0.06);
	//leg->AddEntry(BlueBeamAN,"x_{F} > 0","p");
	//leg->AddEntry(YellowBeamAN,"x_{F} < 0","p");
	//leg->SetBorderSize(0);
	leg->Draw("same");
	//TLine *line1 = new TLine(0.1, 0, 0.30, 0);
	////TLine *line1 = new TLine(0.1, 0, 0.35, 0);
  	//line1->SetLineStyle(7);
  	line1->Draw("same");

	//TLatex T1;
	//T1.SetTextAlign(11);
   	//T1.SetTextSize(0.035);
	//T1.SetTextColor(kRed);
	//T1.DrawLatex(0.11,0.09,"STAR Preliminary");

	TLatex T3;
        T3.SetTextAlign(11);
        T3.SetTextSize(0.035);
        T3.SetTextColor(kBlack);
   	T3.DrawLatex(0.11,0.14,"p^{#uparrow} + p #rightarrow EM jet + p + X");
   	T3.DrawLatex(0.11,0.12,"#sqrt{s} = 510 GeV");
   	//T3.DrawLatex(0.11,0.13,"1.1\% polarization scale uncertainty not shown");

	YellowBeamAN_sig = new TGraphErrors(nDrawPoints , XFpointYellow , YellowANresult_sig , XFpointErr , YellowANresultErr_sig);
	//YellowBeamAN_sig->GetFunction("ANconstantY")->SetLineColor(6);
	YellowBeamAN_sig->SetName("YellowBeamAN");
	YellowBeamAN_sig->SetTitle("A_{N} (x_{F} < 0)");
	YellowBeamAN_sig->GetXaxis()->SetTitle("X_{F}");
	YellowBeamAN_sig->GetYaxis()->SetTitle("A_{N}");
	YellowBeamAN_sig->SetMarkerColor(kRed+1);
	YellowBeamAN_sig->SetMarkerSize(2);
	YellowBeamAN_sig->SetMarkerStyle(22);
	YellowBeamAN_sig->SetLineWidth(2);
	YellowBeamAN_sig->SetLineColor(kRed+1);
	YellowBeamAN_sig->Draw("P");

	BlueBeamAN_sig = new TGraphErrors(nDrawPoints , XFpoint , ANresult_sig , XFpointErr , ANresultErr_sig);
	BlueBeamAN_sig->SetName("BlueBeamAN");
	BlueBeamAN_sig->SetTitle("A_{N} (x_{F} > 0)");
	BlueBeamAN_sig->GetXaxis()->SetTitle("X_{F}");
	BlueBeamAN_sig->GetYaxis()->SetTitle("A_{N}");
	BlueBeamAN_sig->SetMarkerColor(kBlue+1);
	BlueBeamAN_sig->SetMarkerSize(2);
	BlueBeamAN_sig->SetMarkerStyle(20);
	BlueBeamAN_sig->SetLineColor(kBlue+1);
	BlueBeamAN_sig->SetLineWidth(2);
	BlueBeamAN_sig->Draw("P");

	ANplot2->SaveAs("FMS_ANplot_4pt_sig.pdf");
	ANplot2->SaveAs("FMS_ANplot_4pt_sig.png");
	ANplot2->SaveAs("FMS_ANplot_4pt_sig.root");


	BlueBeamAN_sig->Fit("ANconstant");
	cout<<"Blue beam signal A_{N} with constant fit:"<<ANconstant->GetParameter(0)<<"+/-"<<ANconstant->GetParError(0)<<" value/error ratio:"<<1.0*fabs(ANconstant->GetParameter(0))/(ANconstant->GetParError(0))<<endl;
	YellowBeamAN_sig->Fit("ANconstantY");
	cout<<"Yellow beam signal A_{N} with constant fit:"<<ANconstantY->GetParameter(0)<<"+/-"<<ANconstantY->GetParError(0)<<" value/error ratio:"<<1.0*fabs(ANconstantY->GetParameter(0))/(ANconstantY->GetParError(0))<<endl;


    }

















    if (det == "eemc")
    {

	int XFbins, nXFusebins, Phibins;
	XFbins = 20;
	nXFusebins = 4;
	Phibins=16;
	Double_t BlueUpNum[Phibins+1][XFbins+1];
	Double_t BlueDownNum[Phibins+1][XFbins+1];
	Double_t YellowUpNum[Phibins+1][XFbins+1];
	Double_t YellowDownNum[Phibins+1][XFbins+1];
	Double_t PhiPoint[] = {pi/16.,3*pi/16.,5*pi/16.,7*pi/16.,9*pi/16.,11*pi/16.,13*pi/16.,15*pi/16.};

	

	Double_t PhiPointErr[] = {0,0,0,0,0,0,0,0};
	//Double_t XFpoint[] = {0.05,0.15,0.25,0.35,0.45,0.55};
	//Double_t XFpoint[] = {0.025,0.075,0.125,0.175,0.225,0.275,0.325,0.375};
	Double_t XFpoint[] = {0.075,0.125,0.175,0.225,0.275,0.325,0.375,0.425};
	Double_t XFpointYellow[] = {0.07,0.120,0.170,0.220,0.270,0.320,0.370,0.420};
	Double_t XFpointErr[] = {0,0,0,0,0,0,0,0};
	Double_t XFSysBluepointErr[] = {0.002,0.002,0.002,0.002,0.002,0.002,0.002,0.002};
	Double_t XFSysYellowpointErr[] = {0.001,0.001,0.001,0.001,0.001,0.001,0.001,0.001};

	Double_t XFsigFrac[] = {0.966, 0.985, 0.994, 1., 1., 1., 1., 1.};

	Double_t SysErr[20];
	Double_t YellowSysErr[20];
	Double_t sysB , sysY;

	polB=polBlue_Allfill->GetMean();
	polY=polYellow_Allfill->GetMean();

	//get number of event within each bin by XF and phi
	for (int i=0; i<Phibins ; i=i+1)
	{

		for (int j=1; j<XFbins; j=j+1)
		{

			BlueUpNum[i][j] = BlueBeamUp->GetBinContent(i+1,j+1);
			cout<<"Blue beam Up x:"<<i<<" y:"<<j<<" n entries:"<<BlueUpNum[i][j]<<endl; 
			BlueDownNum[i][j] = BlueBeamDown->GetBinContent(i+1,j+1);
			cout<<"Blue beam Down x:"<<i<<" y:"<<j<<" n entries:"<<BlueDownNum[i][j]<<endl; 
			YellowUpNum[i][j] = YellowBeamUp->GetBinContent(i+1,j+1);
			cout<<"Yellow beam Up x:"<<i<<" y:"<<j<<" n entries:"<<BlueUpNum[i][j]<<endl; 
			YellowDownNum[i][j] = YellowBeamDown->GetBinContent(i+1,j+1);
			cout<<"Yellow beam Down x:"<<i<<" y:"<<j<<" n entries:"<<BlueDownNum[i][j]<<endl; 

		}

	}

	Double_t rawAN[Phibins/2];
	Double_t rawANErr[Phibins/2];
	Double_t YellowrawAN[Phibins/2];
	Double_t YellowrawANErr[Phibins/2];
	Double_t ANresult[nXFusebins-2];
	Double_t ANresultErr[nXFusebins-2];
	Double_t YellowANresult[nXFusebins-2];
	Double_t YellowANresultErr[nXFusebins-2];
	Double_t ANresult_sig[nXFusebins-2];
	Double_t ANresultErr_sig[nXFusebins-2];
	Double_t YellowANresult_sig[nXFusebins-2];
	Double_t YellowANresultErr_sig[nXFusebins-2];
	Double_t a,b,c,d ,  rawANnomin , rawANdenomin, rawANErrnomin , rawANErrdenomin;
	Double_t xF;

	TGraphErrors *BlueBeamRawAN[nXFusebins];
	TGraphErrors *BlueBeamAN;
	TGraphErrors *BlueBeamAN_sig;
	TGraphErrors *BlueBeamSys;
	TGraphErrors *YellowBeamRawAN[nXFusebins];
	TGraphErrors *YellowBeamAN;
	TGraphErrors *YellowBeamAN_sig;
	TGraphErrors *YellowBeamSys;

	
	TF1 *BlueFit;
	TF1 *YellowFit;

	for (int j=1; j<nXFusebins; j=j+1)
	{


		for(int i=0; i<(Phibins / 2); i=i+1)
		{

			a = BlueUpNum[i+(Phibins/2)][j]; //a: N^{up} (phi)
			b = BlueDownNum[i][j]; //b: N^{down} (phi + pi)
			c = BlueDownNum[i+(Phibins/2)][j]; //c: N^{down} (phi)
			d = BlueUpNum[i][j]; //d: N^{up} (phi + pi)

			rawANnomin = sqrt(a*b) - sqrt(c*d);
			rawANdenomin = sqrt(a*b) + sqrt(c*d);
			rawAN[i] = rawANnomin / rawANdenomin;

			rawANErrnomin = b*c*d + a*c*d + a*b*d + a*b*c;
			rawANErrdenomin = pow(rawANdenomin,4);
			rawANErr[i] = sqrt(rawANErrnomin / rawANErrdenomin);
			cout<<"XF "<<(0.05*(j))<<" to "<<(0.05*(j+1))<<" phi bin: "<<(0.125*(i))<<"pi to "<<(0.125*(i+1))<<"pi  raw AN="<<rawAN[i]<<"  raw AN uncertainty="<<rawANErr[i]<<endl;

			a = YellowUpNum[i+(Phibins/2)][j]; //a: N^{up} (phi)
			b = YellowDownNum[i][j]; //b: N^{down} (phi + pi)
			c = YellowDownNum[i+(Phibins/2)][j]; //c: N^{down} (phi)
			d = YellowUpNum[i][j]; //d: N^{up} (phi + pi)

			rawANnomin = sqrt(a*b) - sqrt(c*d);
			rawANdenomin = sqrt(a*b) + sqrt(c*d);
			YellowrawAN[i] = rawANnomin / rawANdenomin;

			rawANErrnomin = b*c*d + a*c*d + a*b*d + a*b*c;
			rawANErrdenomin = pow(rawANdenomin,4);
			YellowrawANErr[i] = sqrt(rawANErrnomin / rawANErrdenomin);


		}
		
		BlueBeamRawAN[j] = new TGraphErrors(Phibins / 2,PhiPoint , rawAN, PhiPointErr,rawANErr);
		BlueBeamRawAN[j]->SetName(Form("BlueBeamRawAN_%i",j));
		BlueBeamRawAN[j]->SetTitle(Form("Blue Beam Raw A_{N} by x_{F} %.2f to %.2f",0.05*j, 0.05*(j+1)));
		BlueBeamRawAN[j]->GetXaxis()->SetTitle("#phi");
		BlueBeamRawAN[j]->GetYaxis()->SetTitle("raw A_{N}");
		//BlueBeamRawAN[j]->Write();

		BlueFit = new TF1("BlueFit","[0]*cos(x) + [1]");
		BlueFit->SetParLimits(1,-0.2,0.2);
		BlueBeamRawAN[j]->Fit("BlueFit");
		ANresult[j-1] = BlueFit->GetParameter(0) / polB / XFsigFrac[j-1];
		ANresultErr[j-1] = BlueFit->GetParError(0) / polB / XFsigFrac[j-1];
		ANresult_sig[j-1] = ANresult[j-1] / Frac_sig[j-1];
		ANresultErr_sig[j-1] = ANresultErr[j-1] / Frac_sig[j-1];
		BlueBeamRawAN[j]->Write();

		//sysinputtext>>xF>>sysB >> sysY;
		//SysErr[j-2] = fabs(ANresult[j-2] * sysB);
		//cout<<"AN result "<<j-2<<": "<<ANresult[j-2]<<"	AN statics un: "<<ANresultErr[j-2]<<"	AN syst un:"<<SysErr[j-2]<<endl;

		outputtext<<"B"<<"    "<<XFpoint[j-1]<<"    "<<ANresult[j-1]<<"    "<<ANresultErr[j-1]<<endl;
		cout<<"B"<<"    "<<XFpoint[j-1]<<"    "<<ANresult[j-1]<<"    "<<ANresultErr[j-1]<<endl;
		//outputtext<<"B"<<"    "<<XFpoint[j-2]<<"    "<<ANresult[j-2]<<"    "<<ANresultErr[j-2]<<"   "<<SysErr[j-2]<<endl;

		YellowBeamRawAN[j] = new TGraphErrors(Phibins / 2,PhiPoint , YellowrawAN, PhiPointErr,YellowrawANErr);
		YellowBeamRawAN[j]->SetName(Form("YellowBeamRawAN_%i",j));
		YellowBeamRawAN[j]->SetTitle(Form("Yellow Beam Raw A_{N} by x_{F} %.2f to %.2f",0.05*j, 0.05*(j+1)));
		YellowBeamRawAN[j]->GetXaxis()->SetTitle("#phi");
		YellowBeamRawAN[j]->GetYaxis()->SetTitle("raw A_{N}");
		//BlueBeamRawAN[j]->Write();

		YellowFit = new TF1("YellowFit","[0]*cos(x) + [1]");
		//YellowFit->SetParLimits(1,-0.5,0.5);
		YellowBeamRawAN[j]->Fit("YellowFit");
		YellowANresult[j-1] = YellowFit->GetParameter(0) / polY / XFsigFrac[j-1];
		YellowANresultErr[j-1] = YellowFit->GetParError(0) / polY / XFsigFrac[j-1];
		YellowBeamRawAN[j]->Write();
//		YellowSysErr[j-2] = fabs(YellowANresult[j-2] * sysY);
//		outputtext<<"Y"<<"    "<<XFpoint[j-2]<<"    "<<YellowANresult[j-2]<<"    "<<YellowANresultErr[j-2]<<"	"<<YellowSysErr[j-2]<<endl;
		cout<<"Y"<<"    "<<XFpoint[j-1]<<"    "<<YellowANresult[j-1]<<"    "<<YellowANresultErr[j-1]<<endl;
		outputtext<<"Y"<<"    "<<XFpoint[j-1]<<"    "<<YellowANresult[j-1]<<"    "<<YellowANresultErr[j-1]<<endl;
	}

	//BlueBeamAN = new TGraphErrors((nXFusebins-2) , XFpoint , ANresult , XFpointErr , ANresultErr);
	BlueBeamAN = new TGraphErrors(nDrawPoints , XFpoint , ANresult , XFpointErr , ANresultErr);
	TF1 *ANconstant = new TF1("ANconstant","[0]");
	BlueBeamAN->Fit("ANconstant");
	BlueBeamAN->SetName("BlueBeamAN");
	BlueBeamAN->SetTitle("A_{N} (x_{F} > 0)");
	BlueBeamAN->GetXaxis()->SetTitle("X_{F}");
	BlueBeamAN->GetYaxis()->SetTitle("A_{N}");
	BlueBeamAN->Write();
	ANconstant->SetLineWidth(3);
	//ANconstant->Write();
	cout<<"Blue beam A_{N} with constant fit:"<<ANconstant->GetParameter(0)<<"+/-"<<ANconstant->GetParError(0)<<" value/error ratio:"<<1.0*fabs(ANconstant->GetParameter(0))/(ANconstant->GetParError(0))<<endl;

	BlueBeamAN_sig = new TGraphErrors(nDrawPoints , XFpoint , ANresult_sig , XFpointErr , ANresultErr_sig);
	BlueBeamAN_sig->Fit("ANconstant");
	BlueBeamAN_sig->SetName("BlueBeamAN");
	BlueBeamAN_sig->SetTitle("A_{N} (x_{F} > 0)");
	BlueBeamAN_sig->GetXaxis()->SetTitle("X_{F}");
	BlueBeamAN_sig->GetYaxis()->SetTitle("A_{N}");
	BlueBeamAN_sig->Write();
	ANconstant->SetLineWidth(3);
	//ANconstant->Write();
	cout<<"Blue beam A_{N} with constant fit:"<<ANconstant->GetParameter(0)<<"+/-"<<ANconstant->GetParError(0)<<" value/error ratio:"<<1.0*fabs(ANconstant->GetParameter(0))/(ANconstant->GetParError(0))<<endl;


//	BlueBeamSys = new TGraphErrors(nDrawPoints , XFpoint , ANresult , XFSysBluepointErr , SysErr);
//	BlueBeamSys->SetTitle("A_{N} Sys (x_{F} > 0)");
//	BlueBeamSys->GetXaxis()->SetTitle("X_{F}");
//	BlueBeamSys->GetYaxis()->SetTitle("A_{N}");
	//YellowBeamAN = new TGraphErrors((nXFusebins-2) , XFpoint , YellowANresult , XFpointErr , YellowANresultErr);
	YellowBeamAN = new TGraphErrors(nDrawPoints , XFpointYellow , YellowANresult , XFpointErr , YellowANresultErr);
	TF1 *ANconstantY = new TF1("ANconstantY","[0]");
	YellowBeamAN->Fit("ANconstantY");
	YellowBeamAN->GetFunction("ANconstantY")->SetLineColor(6);
	YellowBeamAN->SetName("YellowBeamAN");
	YellowBeamAN->SetTitle("A_{N} (x_{F} < 0)");
	YellowBeamAN->GetXaxis()->SetTitle("X_{F}");
	YellowBeamAN->GetYaxis()->SetTitle("A_{N}");
	YellowBeamAN->Write();
	//ANconstantY->SetLineColor(6);
	ANconstantY->SetLineWidth(3);
	//ANconstantY->Write();
	cout<<"Yellow beam A_{N} with constant fit:"<<ANconstantY->GetParameter(0)<<"+/-"<<ANconstantY->GetParError(0)<<" value/error ratio:"<<1.0*fabs(ANconstantY->GetParameter(0))/(ANconstantY->GetParError(0))<<endl;
//	YellowBeamSys = new TGraphErrors(nDrawPoints , XFpointYellow , YellowANresult , XFSysYellowpointErr , YellowSysErr);
//	YellowBeamSys->SetTitle("A_{N} Sys (x_{F} > 0)");
//	YellowBeamSys->GetXaxis()->SetTitle("X_{F}");
//	YellowBeamSys->GetYaxis()->SetTitle("A_{N}");


	YellowBeamAN_sig = new TGraphErrors(nDrawPoints , XFpointYellow , YellowANresult_sig , XFpointErr , YellowANresultErr_sig);
	YellowBeamAN_sig->Fit("ANconstantY");
	YellowBeamAN_sig->GetFunction("ANconstantY")->SetLineColor(6);
	YellowBeamAN_sig->SetName("YellowBeamAN");
	YellowBeamAN_sig->SetTitle("A_{N} (x_{F} < 0)");
	YellowBeamAN_sig->GetXaxis()->SetTitle("X_{F}");
	YellowBeamAN_sig->GetYaxis()->SetTitle("A_{N}");
	YellowBeamAN_sig->Write();
	//ANconstantY->SetLineColor(6);
	ANconstantY->SetLineWidth(3);
	//ANconstantY->Write();
	cout<<"Yellow beam A_{N} with constant fit:"<<ANconstantY->GetParameter(0)<<"+/-"<<ANconstantY->GetParError(0)<<" value/error ratio:"<<1.0*fabs(ANconstantY->GetParameter(0))/(ANconstantY->GetParError(0))<<endl;

	TCanvas *ANplot = new TCanvas("ANPlot","",200,10,1024,768);
	ANplot->DrawFrame(2., -0.06, 250., 0.2);
	//ANplot->SetLeftMargin(0.05);
	//ANplot->SetRightMargin(0.05);
	TH2D * temp = new TH2D("temp","A_{N}",9,0.05,0.3,40,-0.10,0.10);
	//TH2D * temp = new TH2D("temp","A_{N}",8,0.1,0.5,30,-0.10,0.10);
	temp->GetYaxis()->SetTitle("A_{N}");
  	temp->GetYaxis()->SetTitleOffset(0.9);
  	temp->GetXaxis()->SetTitle("x_{F}");
  	temp->Draw();
	YellowBeamAN->SetMarkerColor(kRed+1);
	YellowBeamAN->SetMarkerSize(2);
	YellowBeamAN->SetMarkerStyle(22);
	YellowBeamAN->SetLineWidth(2);
	YellowBeamAN->SetLineColor(kRed+1);
	YellowBeamAN->Draw("P");
//	YellowBeamSys->SetMarkerColor(kRed+1);
//	YellowBeamSys->SetMarkerSize(2);
//	YellowBeamSys->SetLineColor(kRed+1);
//	YellowBeamSys->SetLineWidth(2);
//	YellowBeamSys->SetFillStyle(0);
//	YellowBeamSys->SetFillColorAlpha(1,0.5);
//	YellowBeamSys->Draw("e5");
	BlueBeamAN->SetMarkerColor(kBlue+1);
	BlueBeamAN->SetMarkerSize(2);
	BlueBeamAN->SetMarkerStyle(20);
	BlueBeamAN->SetLineColor(kBlue+1);
	BlueBeamAN->SetLineWidth(2);
	BlueBeamAN->Draw("P");
//	BlueBeamSys->SetMarkerColor(kBlue+1);
//	BlueBeamSys->SetMarkerSize(2);
//	BlueBeamSys->SetLineColor(kBlue+1);
//	BlueBeamSys->SetLineWidth(2);
//	BlueBeamSys->SetFillStyle(0);
//	BlueBeamSys->SetFillColorAlpha(1,0.5);
//	BlueBeamSys->Draw("e5");

	TLegend *leg = new TLegend(0.7,0.7,0.88,0.85);
	leg->SetTextSize(0.06);
	leg->AddEntry(BlueBeamAN,"x_{F} > 0","p");
	leg->AddEntry(YellowBeamAN,"x_{F} < 0","p");
	leg->SetBorderSize(0);
	leg->Draw("same");
	TLine *line1 = new TLine(0.1, 0, 0.3, 0);
  	line1->SetLineStyle(7);
  	line1->Draw("same");

//	TLatex T1;
//	T1.SetTextAlign(11);
//   	T1.SetTextSize(0.035);
//	T1.SetTextColor(kRed);
//	T1.DrawLatex(0.11,0.09,"STAR Preliminary");
//
//	TLatex T2;
//        T2.SetTextAlign(11);
//        T2.SetTextSize(0.035);
//        T2.SetTextColor(kBlack);
//   	T2.DrawLatex(0.11,0.07,"p^{#uparrow} + p #rightarrow EM jet + p + X");
//   	T2.DrawLatex(0.11,0.05,"#sqrt{s} = 200 GeV");
//   	T2.DrawLatex(0.11,-0.09,"3.0\% polarization scale uncertainty not shown");
	
	
	ANplot->SaveAs("EEMC_ANplot.pdf");
	ANplot->SaveAs("EEMC_ANplot.png");
	ANplot->SaveAs("EEMC_ANplot.root");

  }



    outFile->Write();
    outFile->Close();
    delete ch;
}
