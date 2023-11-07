
#include "DiffJetXFCalc.h"
#include "RootInclude.h"
#include "StRootInclude.h"
#include "BrJetMaker/TStJetCandidate.h"
#include "BrJetMaker/TStJetEvent.h"
#include "BrJetMaker/TStJetSkimEvent.h"
#include "BrContainers/TStRpsTrackData.h"
#include <iostream>

using namespace std;

//Use wildcat in the file name to combine many files.
void DiffJetXFCalc(TString inFileName, TString outName, TString mode, TString det)
{
    TChain *ch = new TChain("T");
    ch->Add(inFileName);
    
    TStJetEvent *jetEvent = 0;
    TStJetSkimEvent *skimEvent;
    TStJetCandidate *jet;
    TStJetTower *tower;
    TStJetParticle *particle;
    TStRpsTrackData *rpsTrack =0;    

    TClonesArray *rpsArr = new TClonesArray("TStRpsTrackData");

    ch->SetBranchAddress("jetEvents", &jetEvent);
    ch->SetBranchAddress("rpTrack",&rpsArr);

	if (mode == "AN") cout << "start analysis physics! Not systematic." << endl;
	if (mode == "Ecorr") cout << "start energy correction systematic" << endl;
	if (mode == "Esum") cout << "start sum energy cut systematic" << endl;
	if (mode == "ROF") cout << "start ROF systematic" << endl;
	if (mode != "ROF") cout << "not doing ROF systematic" << endl;
	if (mode == "smallBBC") cout << "start small BBC systematic" << endl;
	if (mode == "largeBBC") cout << "start large BBC systematic" << endl;

    
    TFile *outFile = new TFile(outName, "recreate");
    TH1I *h1_fill_index = new TH1I("h1_fill_index","fill index",180,0,180);
    TH1I *h1_event_type = new TH1I("h1_event_type","event type",4,0,4);
    TH1D *h1nJets_all = new TH1D("h1nJets_all", "Number of Jets from All [TPC/BEMC + EEMC + FMS]", 10, 0, 10);
    TH1D *h1nJets  = new TH1D("h1nJets ", "Number of  Jets [FMS or EEMC]", 10, 0, 10);
    TH1D *h1spinB = new TH1D("h1spinB", "Blue beam spin; Blue beam spin", 5, -2, 2);
    TH1D *h1spinY = new TH1D("h1spinY", "Yellow beam spin; Yellow beam spin", 5, -2, 2);
    TH1D *h1TrigType = new TH1D("h1TrigType", "Trigger Type; Trigger Type", 10, 0, 10);
    TH1D *polBlue_Allfill = new TH1D("polBlue_Allfill","blue beam polarization distribution (all fill)",90,0.4,0.7);
    TH1D *polYellow_Allfill = new TH1D("polYellow_Allfill","yellow beam polarization distribution (all fill)",90,0.4,0.7);
   
    TH1D *h1_pol_uncertainty = new TH1D("h1_pol_uncertainty","pol fill uncertainty by fill index",180,0,180);

    TH1D *h1TowerEta = new TH1D("h1TowerEta", "Tower Eta", 100, 1.0, 4.5);
    TH1D *h1TowerPhi = new TH1D("h1TowerPhi", "Tower Phi", 100, -3.2, 3.2);
    TH1D *h1TowerE = new TH1D("h1TowerE", "Tower E", 250, 0, 250);

    TH1D *h1Eta = new TH1D ("h1Eta", "EM Jet Eta; Jet #eta", 100, 0.5, 5.0);
    TH1D *h1Phi = new TH1D ("h1Phi", "EM Jet Phi; Jet #phi [rad]", 100, -3.3, 3.3);
    TH1D *h1E = new TH1D ("h1E", "EM Jet E; Jet E [GeV]", 200, 0.0, 150.0);
    TH1D *h1E_corr = new TH1D ("h1E_corr", "EM Jet E with corrected from MC; EM Jet E [GeV]", 200, 0.0, 150.0);
    TH1D *h1dUE_corr = new TH1D ("h1dUE_corr", "EM Jet dUE; EM Jet dUE [GeV]", 100, 0.0, 50.0);
    TH1D *h1UE_corr = new TH1D ("h1UE_corr", "EM Jet E with corrected from UE; EM Jet E [GeV]", 200, 0.0, 150.0);
    TH1D *h1JetE_s = new TH1D ("h1JetE_s", "EM Jet E [small cells]; Jet E [GeV]", 200, 0.0, 150.0);
    TH1D *h1JetE_l = new TH1D ("h1JetE_l", "EM Jet E [large cells]; EM Jet E [GeV]", 200, 0.0, 150.0);    
    TH1D *h1Pt = new TH1D ("h1Pt", "EM Jet Pt; EM Jet Pt [GeV/c]", 100, 0.0, 20.0);
    TH1D *h1Pt_nocut = new TH1D ("h1Pt_nocut", "Jet Pt (no cut); Jet Pt [GeV/c]", 100, 0.0, 20.0);
    TH1I *h1nPhotons = new TH1I("h1nPhotons", "number of photons in EM jets; Number of Photons", 20, 0, 20);
    TH1D *h1vtxZ = new TH1D("h1vtxZ", "Jet vetrex z; Jet vertex z [cm]", 100, -200, 200);
    TH1D *h1_smallBBCsumEast = new TH1D("h1_smallBBCsumEast","small BBC ADC sum for east side BBC",400,0,20000);
    TH1D *h1_largeBBCsumEast = new TH1D("h1_largeBBCsumEast","large BBC ADC sum for east side BBC",400,0,20000);
    TH1D *h1_smallBBCsumWest = new TH1D("h1_smallBBCsumWest","small BBC ADC sum for west side BBC",400,0,20000);
    TH1D *h1_largeBBCsumWest = new TH1D("h1_largeBBCsumWest","large BBC ADC sum for west side BBC",400,0,20000);
    TH2D *h2_sumEwest_vs_smallBBCsumWest = new TH2D("h2_sumEwest_vs_smallBBCsumWest","sum west side energy vs small BBC ADC sum for west side BBC;small BBC sum;energy[GeV]",200,0,10000,150,210.0,360.0);
    TH2D *h2_sumEwest_vs_smallBBCsumEast = new TH2D("h2_sumEwest_vs_smallBBCsumEast","sum west side energy vs small BBC ADC sum for east side BBC;small BBC sum;energy[GeV]",200,0,10000,150,210.0,360.0);
    TH2D *h2_RPtrkEwest_vs_smallBBCsumWest = new TH2D("h2_RptrkEwest_vs_smallBBCsumWest","west side RP track energy vs small BBC ADC sum for west side BBC;small BBC sum;energy[GeV]",200,0,10000,100,180.0,280.0);
    TH2D *h2_RPtrkEeast_vs_smallBBCsumEast = new TH2D("h2_RptrkEeast_vs_smallBBCsumEast","east side RP track energy vs small BBC ADC sum for east side BBC;small BBC sum;energy[GeV]",200,0,10000,100,180.0,280.0);


    TH1I *h1_nRpTracks = new TH1I("h1_nRpTracks", "Number of RP tracks", 20, 0, 20);     
    TH1D *h1_totalRpP = new TH1D("h1_totalRpP", "total RP trk P;P [GeV/c]", 250, 300.0, 600.0);
    TH1D *h1_EastRpP = new TH1D("h1_EastRpP", "East RP trk P;P [GeV/c]", 100, 180.0, 280.0);
    TH1D *h1_WestRpP = new TH1D("h1_WestRpP", "West RP trk P;P [GeV/c]", 100, 180.0, 280.0);
    TH1D *h1_EastRpP_doublediff = new TH1D("h1_EastRpP_doublediff", "East RP trk P for double diffractive process;P [GeV/c]", 100, 180.0, 280.0);
    TH1D *h1_WestRpP_doublediff = new TH1D("h1_WestRpP_doublediff", "West RP trk P for double diffractive process;P [GeV/c]", 100, 180.0, 280.0);
    TH1D *h1_WestRpP_singlediff = new TH1D("h1_WestRpP_singlediff", "West RP trk P for single diffractive process;P [GeV/c]", 100, 180.0, 280.0);
    TH1I *h1_RpNPlanes = new TH1I("h1_RpNPlanes","Number of RP planes;N Planes",9,0,9);
    TH1I *h1_RpNPlanes_East = new TH1I("h1_RpNPlanes_East","Number of RP planes in East side;N Planes East",9,0,9);
    TH1I *h1_RpNPlanes_West = new TH1I("h1_RpNPlanes_West","Number of RP planes in West side;N Planes West",9,0,9);
    TH1D *h1_sumEwest = new TH1D("h1_sumEwest","sum of west side RP track energy and EM Jet energy;sum E [GeV]",100, 220.0, 320.0);
    TH1D *h1_sumEwest_XF0510 = new TH1D("h1_sumEwest_XF0510","sum of west side RP track energy and EM Jet energy (0.05< x_{F} < 0.1 ) );sum E [GeV]",180, 220.0, 400.0);
    TH1D *h1_sumEwest_XF1015 = new TH1D("h1_sumEwest_XF1015","sum of west side RP track energy and EM Jet energy (0.1 < x_{F} < 0.15) );sum E [GeV]",180, 220.0, 400.0);
    TH1D *h1_sumEwest_XF1520 = new TH1D("h1_sumEwest_XF1520","sum of west side RP track energy and EM Jet energy (0.15 < x_{F} < 0.2 ) );sum E [GeV]",180, 220.0, 400.0);
    TH1D *h1_sumEwest_XF2025 = new TH1D("h1_sumEwest_XF2025","sum of west side RP track energy and EM Jet energy (0.2 < x_{F} < 0.25) );sum E [GeV]",180, 220.0, 400.0);
    TH1D *h1_sumEwest_XF2530 = new TH1D("h1_sumEwest_XF2530","sum of west side RP track energy and EM Jet energy (0.25 < x_{F} < 0.3 ) );sum E [GeV]",180, 220.0, 400.0);
    TH1D *h1_sumEwest_XF30 = new TH1D("h1_sumEwest_XF30","sum of west side RP track energy and EM Jet energy (x_{F} > 0.3 ) );sum E [GeV]",100, 220.0, 320.0);
    TH1D *h1_pT_XF1015 = new TH1D("h1_pT_XF1015","p_{T} for EM jets 0.1 < x_{F} < 0.15 ;p_{T} [GeV/c]",50,1.0,6.0);
    TH1D *h1_pT_XF1520 = new TH1D("h1_pT_XF1520","p_{T} for EM jets 0.15< x_{F} < 0.2  ;p_{T} [GeV/c]",50,1.0,6.0);
    TH1D *h1_pT_XF2025 = new TH1D("h1_pT_XF2025","p_{T} for EM jets 0.2 < x_{F} < 0.25 ;p_{T} [GeV/c]",50,1.0,6.0);
    TH1D *h1_pT_XF2530 = new TH1D("h1_pT_XF2530","p_{T} for EM jets 0.25< x_{F} < 0.3  ;p_{T} [GeV/c]",50,1.0,6.0);
    TH1D *h1_pT_XF30 = new TH1D("h1_pT_XF30","p_{T} for EM jets 0.3 < x_{F} < 0.45 ;p_{T} [GeV/c]",50,1.0,6.0);
    TH1I *h1_nPhoton_XF1015 = new TH1I("h1_nPhoton_XF1015","Photon multiplicity for EM jets 0.1 < x_{F} < 0.15 ;Photon multiplicity",5,1.0,6.0);
    TH1I *h1_nPhoton_XF1520 = new TH1I("h1_nPhoton_XF1520","Photon multiplicity for EM jets 0.15< x_{F} < 0.2  ;Photon multiplicity",5,1.0,6.0);
    TH1I *h1_nPhoton_XF2025 = new TH1I("h1_nPhoton_XF2025","Photon multiplicity for EM jets 0.2 < x_{F} < 0.25 ;Photon multiplicity",5,1.0,6.0);
    TH1I *h1_nPhoton_XF2530 = new TH1I("h1_nPhoton_XF2530","Photon multiplicity for EM jets 0.25< x_{F} < 0.3  ;Photon multiplicity",5,1.0,6.0);
    TH1I *h1_nPhoton_XF30 = new TH1I("h1_nPhoton_XF30","Photon multiplicity for EM jets 0.3 < x_{F} < 0.45 ;Photon multiplicity",5,1.0,6.0);
    TH1D *h1_sumEwest_beforeBBCcut = new TH1D("h1_sumEwest_beforeBBCcut","sum of west side RP track energy and EM Jet energy before BBC cut;sum E [GeV]",100, 220.0, 320.0);

    TH1D *h1_nPhoton_count_only1westRP = new TH1D("h1_nPhoton_count_only1westRP","number of photon by only 1 west RP track",5,1.0,6.0);
    TH1D *h1_nPhoton_count_1east1westRP = new TH1D("h1_nPhoton_count_1east1westRP","number of photon by 1 east and 1 west RP track",5,1.0,6.0);
    TH1D *h1_xF_count_only1westRP = new TH1D("h1_xF_count_only1westRP","xF distribution by only 1 west RP track",5,0.1,0.35);
    TH1D *h1_xF_count_1east1westRP = new TH1D("h1_xF_count_1east1westRP","xF distribution by 1 east and 1 west RP track",5,0.1,0.35);
    TH1D *h1_pT_count_only1westRP = new TH1D("h1_pT_count_only1westRP","pT distribution by only 1 west RP track",5,1.0,3.5);
    TH1D *h1_pT_count_1east1westRP = new TH1D("h1_pT_count_1east1westRP","pT distribution by 1 east and 1 west RP track",5,1.0,3.5);
	

    TH1D *h1_xF_0510 = new TH1D("h1_xF_0510","x_{F} distribution in [0.05,0.1];x_{F}",50,0.05,0.1);
    TH1D *h1_xF_1015 = new TH1D("h1_xF_1015","x_{F} distribution in [0.1,0.15];x_{F}",50,0.1,0.15);
    TH1D *h1_xF_1520 = new TH1D("h1_xF_1520","x_{F} distribution in [0.15,0.2];x_{F}",50,0.15,0.2);
    TH1D *h1_xF_2025 = new TH1D("h1_xF_2025","x_{F} distribution in [0.2,0.25];x_{F}",50,0.2,0.25);
    TH1D *h1_xF_2530 = new TH1D("h1_xF_2530","x_{F} distribution in [0.25,0.3];x_{F}",50,0.25,0.3);
    TH1D *h1_xF_3045 = new TH1D("h1_xF_3045","x_{F} distribution in [0.3,0.45];x_{F}",50,0.3,0.45);

    TH1D *h1_xF_1015_noCorr = new TH1D("h1_xF_1015_noCorr","x_{F} distribution in [0.1,0.15] (no correction);x_{F}",50,0.1,0.15);
    TH1D *h1_xF_1520_noCorr = new TH1D("h1_xF_1520_noCorr","x_{F} distribution in [0.15,0.2] (no correction);x_{F}",50,0.15,0.2);
    TH1D *h1_xF_2025_noCorr = new TH1D("h1_xF_2025_noCorr","x_{F} distribution in [0.2,0.25] (no correction);x_{F}",50,0.2,0.25);
    TH1D *h1_xF_2530_noCorr = new TH1D("h1_xF_2530_noCorr","x_{F} distribution in [0.25,0.3] (no correction);x_{F}",50,0.25,0.3);
    TH1D *h1_xF_3045_noCorr = new TH1D("h1_xF_3045_noCorr","x_{F} distribution in [0.3,0.45] (no correction);x_{F}",150,0.3,0.45);

    TH1D *h1_xF_1015_SysDiff = new TH1D("h1_xF_1015_SysDiff","Systematic uncertainty for E correction x_{F} distribution in [0.1,0.15] ;Difference ",50,0,0.20);
    TH1D *h1_xF_1520_SysDiff = new TH1D("h1_xF_1520_SysDiff","Systematic uncertainty for E correction x_{F} distribution in [0.15,0.2] ;Difference ",50,0,0.20);
    TH1D *h1_xF_2025_SysDiff = new TH1D("h1_xF_2025_SysDiff","Systematic uncertainty for E correction x_{F} distribution in [0.2,0.25] ;Difference ",50,0,0.20);
    TH1D *h1_xF_2530_SysDiff = new TH1D("h1_xF_2530_SysDiff","Systematic uncertainty for E correction x_{F} distribution in [0.25,0.3] ;Difference ",50,0,0.20);
    TH1D *h1_xF_3045_SysDiff = new TH1D("h1_xF_3045_SysDiff","Systematic uncertainty for E correction x_{F} distribution in [0.3,0.45] ;Difference ",50,0,0.20);

    TH2D *h2EvsPt = new TH2D("h2EvsPt", "Eng vs Pt; Pt [GeV/C]; E [GeV]", 100, 0, 10.0, 120, 0, 120.0);
    TH2D *h2PtvsE = new TH2D("h2PtvsE", "Pt vs E; E [GeV]; Pt [GeV/c]", 120,  0, 120.0, 100, 0, 10.0);
    TH2D *h2nPhVsEng = new TH2D("h2nPhVsEng", "Number of photons vs Eng; E [Gev]; No. of Photons", 120, 0, 120.0, 20, 0, 20);
    TH2D *h2_EMJetE_vs_ProtonE = new TH2D("h2_EmJetE_vs_ProtonE", "EM Jet sum Energy vs West Proton Energy; West Proton Energy [GeV]; EM Jet sum Energy [GeV]", 100, 180.0, 280.0, 100, 5.0, 105.0);

    TH2D *h2PtvsXF = new TH2D("h2PtvsXF","EM-jet P_{T} vs x_{F} ; x_{F} ; P_{T} bin ",5,0.1,0.35,2,1,3);

    Int_t Phibin=0;
    if (det == "fms") {Phibin=16;}
    if (det == "eemc"){Phibin=16;}
    Int_t XFbin = 20;
    TH2D *h2_BU_xF_vs_phi = new TH2D("h2_BU_xF_vs_phi","Blue beam spin +1 x_{F} vs #phi ; #phi [rad] ; x_{F}",Phibin,-TMath::Pi(),TMath::Pi(),XFbin,0,1);
    TH2D *h2_BD_xF_vs_phi = new TH2D("h2_BD_xF_vs_phi","Blue beam spin -1 x_{F} vs #phi ; #phi [rad] ; x_{F}",Phibin,-TMath::Pi(),TMath::Pi(),XFbin,0,1);
    TH2D *h2_YU_xF_vs_phi = new TH2D("h2_YU_xF_vs_phi","Yellow beam spin +1 x_{F} vs #phi ; #phi [rad] ; x_{F}",Phibin,-TMath::Pi(),TMath::Pi(),XFbin,0,1);
    TH2D *h2_YD_xF_vs_phi = new TH2D("h2_YD_xF_vs_phi","Yellow beam spin -1 x_{F} vs #phi ; #phi [rad] ; x_{F}",Phibin,-TMath::Pi(),TMath::Pi(),XFbin,0,1);
    TH2D *h2_BU_xF_vs_phi_nPhoton12 = new TH2D("h2_BU_xF_vs_phi_nPhoton12","Blue beam spin +1 x_{F} vs #phi ; #phi [rad] ; x_{F}",Phibin,-TMath::Pi(),TMath::Pi(),XFbin,0,1);
    TH2D *h2_BD_xF_vs_phi_nPhoton12 = new TH2D("h2_BD_xF_vs_phi_nPhoton12","Blue beam spin -1 x_{F} vs #phi ; #phi [rad] ; x_{F}",Phibin,-TMath::Pi(),TMath::Pi(),XFbin,0,1);
    TH2D *h2_YU_xF_vs_phi_nPhoton12 = new TH2D("h2_YU_xF_vs_phi_nPhoton12","Yellow beam spin +1 x_{F} vs #phi ; #phi [rad] ; x_{F}",Phibin,-TMath::Pi(),TMath::Pi(),XFbin,0,1);
    TH2D *h2_YD_xF_vs_phi_nPhoton12 = new TH2D("h2_YD_xF_vs_phi_nPhoton12","Yellow beam spin -1 x_{F} vs #phi ; #phi [rad] ; x_{F}",Phibin,-TMath::Pi(),TMath::Pi(),XFbin,0,1);


    TF1 *f1 = new TF1("f1","[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x+[6]*x*x*x*x*x*x");
    f1->SetParameter(0,8.934910e0);
    f1->SetParameter(1,-6.63987e-1);
    f1->SetParameter(2,1.51376e-1);
    f1->SetParameter(3,-6.65678e-3);
    f1->SetParameter(4,1.56144e-4);
    f1->SetParameter(5,-1.84953e-6);
    f1->SetParameter(6,8.65440e-9);

    TF1 *f2 = new TF1("f2","[0]+[1]*x");
    f2->SetParameter(0,1.12965);
    f2->SetParameter(1,1.07394);
    	TF1 *f3 = new TF1("f3","[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x");
    	TF1 *f4 = new TF1("f4","[0]+[1]*x");

    if (mode == "ECorr")
    {
    	f3->SetParameter(0,1.11349e+01);
    	f3->SetParameter(1,-1.58394e-01);
    	f3->SetParameter(2,1.58873e-02);
    	f3->SetParameter(3,-6.66110e-03);
    	f3->SetParameter(4,6.85146e-05);
    	f3->SetParameter(5,7.97991e-05);
    	f4->SetParameter(0,1.75278);
    	f4->SetParameter(1,1.01717);
    	f4->SetParameter(2,1.14455e-03);

    }

    Double_t etaMin;
    Double_t etaMax;
    Double_t detZ; //For FMS
    Double_t pTcut = 1.0;
	Double_t FMSTrigThresP1[9]={1.84,2.76,3.68,1.26,1.84,2.53,1.26,1.84,2.76};
	Double_t FMSTrigThresP2[9]={1.84,2.76,3.68,1.15,1.84,2.18,1.15,1.84,2.76};
    if(det == "fms")
    {
	etaMin = 2.8;
	etaMax = 3.8;
	detZ = 735.; //For FMS

	h1TrigType->GetXaxis()->SetBinLabel(1,"FMS JP0");
	h1TrigType->GetXaxis()->SetBinLabel(2,"FMS JP1");
	h1TrigType->GetXaxis()->SetBinLabel(3,"FMS JP2");
	h1TrigType->GetXaxis()->SetBinLabel(4,"Small BS1");
	h1TrigType->GetXaxis()->SetBinLabel(5,"Small BS2");
	h1TrigType->GetXaxis()->SetBinLabel(6,"Small BS3");
	h1TrigType->GetXaxis()->SetBinLabel(7,"Large BS1");
	h1TrigType->GetXaxis()->SetBinLabel(8,"Large BS2");
	h1TrigType->GetXaxis()->SetBinLabel(9,"Large BS3");	
    }
    else if(det == "eemc")
    {
	etaMin = 1.0;
	etaMax = 2.0;
	detZ = kEEmcZSMD; //For EEMC

	h1TrigType->GetXaxis()->SetBinLabel(1,"EHT0");
	h1TrigType->GetXaxis()->SetBinLabel(2,"JP1");
	h1TrigType->GetXaxis()->SetBinLabel(3,"JP2");
	h1TrigType->GetXaxis()->SetBinLabel(4,"EHT0*EJP1*L2Egamma");
	h1TrigType->GetXaxis()->SetBinLabel(5,"JP2*L2JetHigh");
	h1TrigType->GetXaxis()->SetBinLabel(6,"BHT1*VPDMB-30");
	h1TrigType->GetXaxis()->SetBinLabel(7,"BHT0*BBCMB");
	h1TrigType->GetXaxis()->SetBinLabel(8,"BHT1*BBCMB");
	h1TrigType->GetXaxis()->SetBinLabel(9,"BHT2*BBCMB");		
    }
    else
    {
	cout << "Invalid detector" <<endl;
	return;
    }
    
    
    Double_t jetX, jetY, eta, phi, theta, vtxZ, eng, eng_corr, eng_corr_sys, eng_corr_diff, pt,nEMjets_cut,xF;
    Int_t nJets = 0;
    
    cout << "Total Entries to be processed: "<< ch->GetEntries() <<endl;

    ifstream polfile,runfile;
    Double_t UnixTime,StartTime,P0B,dP0B,dPBdT,ddPBdT,P0Y,dP0Y,dPYdT,ddPYdT , dUnixTime;
    int thisFillNum, thisRunNum;
    int fillit;

    double runStartTime[1000],runEndTime[1000],nRunEvent[1000];
    for (int k=0;k<1000;k=k+1)
    {

	runStartTime[k]=9999999;
	runEndTime[k]=0;
	nRunEvent[k]=0;

    }

    Double_t thisFillEvent=0;
    Double_t ThisFillUncertainty=0;

    struct polPara
    {

	int fillNum;
	long startrun,endrun;
	double StartTime,P0B,dP0B,dPBdT,ddPBdT,P0Y,dP0Y,dPYdT,ddPYdT;

    } polFms[180], polEemc[60];

    vector<int> FillNumber;
    std::vector<int>::iterator it;

    if(det == "fms")
    {
    	polfile.open("polarization_fms_run17.txt");
	runfile.open("run17RPFill_run.list");
	for (int i=0; i<180; ++i)
	{

	polfile >> polFms[i].fillNum >> polFms[i].StartTime >> polFms[i].P0B >> polFms[i].dP0B >> polFms[i].dPBdT >> polFms[i].ddPBdT >> polFms[i].P0Y >> polFms[i].dP0Y >> polFms[i].dPYdT >> polFms[i].ddPYdT ;	
	runfile >> polFms[i].fillNum >> polFms[i].startrun >> polFms[i].endrun;

	FillNumber.push_back(polFms[i].fillNum);

	//cout<<"fill number: "<<FillNumber[i]<<endl;


	}
  
    }
    else if(det == "eemc")
    {
    	polfile.open("polarization_eemc.txt");
	runfile.open("run15PhysFill_run.list");
	for (int i=0; i<59; ++i)
	{

	polfile >> polEemc[i].fillNum >> polEemc[i].StartTime >> polEemc[i].P0B >> polEemc[i].dP0B >> polEemc[i].dPBdT >> polEemc[i].ddPBdT >> polEemc[i].P0Y >> polEemc[i].dP0Y >> polEemc[i].dPYdT >> polEemc[i].ddPYdT ;	
	runfile >> polEemc[i].fillNum >> polEemc[i].startrun >> polEemc[i].endrun;

	FillNumber.push_back(polEemc[i].fillNum);

	//cout<<"fill number: "<<FillNumber[i]<<endl;


	}
    }
    else
    {
	cout << "Invalid detector" <<endl;
	return;
    }


    for(Int_t evt = 0; evt < ch->GetEntries(); ++evt)
    {
	ch->GetEntry(evt);

	if(evt % 1000 == 0)
	    cout << "Events Processed: "<< evt <<endl;
	
	skimEvent = jetEvent->GetEvent();
	
	h1spinB->Fill(skimEvent->GetSpinB());
	h1spinY->Fill(skimEvent->GetSpinY());
	vtxZ = skimEvent->GetVertexZ();
	h1_smallBBCsumEast->Fill(skimEvent->GetBbcSumSmall(0));
	h1_largeBBCsumEast->Fill(skimEvent->GetBbcSumLarge(0));
	h1_smallBBCsumWest->Fill(skimEvent->GetBbcSumSmall(1));
	h1_largeBBCsumWest->Fill(skimEvent->GetBbcSumLarge(1));
	if (fabs(vtxZ) > 80) continue;
		thisFillNum = skimEvent->GetFillNumber();
		thisRunNum = skimEvent->GetRunNumber();

	//cut on BBC ADC sum
	//if (skimEvent->GetBbcSumSmall(1) > 100) continue;
	//if (skimEvent->GetBbcSumLarge(1) >  60) continue;
	//if (skimEvent->GetBbcSumSmall(0) >  60) continue;
	//if (skimEvent->GetBbcSumLarge(0) >  50) continue;
	
	
	// for(Int_t t = 0; t < 9; ++t) //Moved to the end of envet loop
	// {
	//     if(skimEvent->GetTrigFlag(t))
	// 	h1TrigType->Fill(t);
	// }

	//Exclude FMS small-bs3 trigger that gives ring of fire issue. But this removes most of high energetic jets.
	//if((det == "fms") && (mode != "ROF"))
	//{
	//    if(skimEvent->GetTrigFlag(5))
	//	continue;
	//}
	//Alternative way to reduce ring of fire, require: BBCMult > 2 and TofMult > 2
	
	nJets = 0;

	//roman pot cut	
	Int_t nRpsTracks = rpsArr->GetEntriesFast();
	h1_nRpTracks->Fill(nRpsTracks);
	//if (nRpsTracks != 1) continue;
	if ((nRpsTracks > 2)||(nRpsTracks == 0)) continue;
	int nRpsTracksEast=0;	
	int nRpsTracksWest=0;	
	Double_t totalRpP=0;
	Double_t EastRpP=0;
	Double_t WestRpP=0;
	Double_t RpPx, RpPy;
	bool goodevent=true;
    	Double_t RpWestPpJetE=0;
	Int_t goodjets=0;


	for (int iRpTrk=0;iRpTrk < nRpsTracks; ++iRpTrk)
	{	
		rpsTrack = (TStRpsTrackData*)rpsArr->At(iRpTrk);

		h1_RpNPlanes->Fill(rpsTrack->GetNplanes());	
		if(rpsTrack->GetBranch() == 0 || rpsTrack->GetBranch() == 1) //East RP :: O East Up RP, 1: East Down RP
                {
			nRpsTracksEast=nRpsTracksEast+1;
			EastRpP = rpsTrack->GetP();
			if (rpsTrack->GetP() > 260) {goodevent=false;continue;}
			//continue;
                }
		else 
		{
			if(rpsTrack->GetBranch() == 2 || rpsTrack->GetBranch() == 3) // 2:  West Up RP, 3: West Down RP
                	{
				nRpsTracksWest = nRpsTracksWest+1;
				WestRpP = rpsTrack->GetP();
				RpWestPpJetE=rpsTrack->GetP();
				//continue;
                	}
			else continue;
		}	
		if (rpsTrack->GetNplanes()<8) {goodevent=false;continue;}
		RpPx = rpsTrack->GetPx();
		RpPy = rpsTrack->GetPy();
		//if (rpsTrack->GetThetaX() < -2 || rpsTrack->GetThetaX() > 2) {goodevent=false;continue;}
		//if (fabs(rpsTrack->GetThetaY()) <  1.5 || fabs(rpsTrack->GetThetaY()) > 4.5) {goodevent=false;continue;}
		if (!(((fabs(RpPx) < 0.5)&&(RpPy < 0.4)&&(RpPy > 0.25)) || ((fabs(RpPx) < 0.5)&&(RpPy < -0.25)&&(RpPy > -0.4)))) {goodevent=false;continue;}


	}
	if ((nRpsTracksEast==2)||(nRpsTracksWest==2)||(!goodevent)) {continue;}
	h1_totalRpP->Fill(totalRpP);
//	if ((nRpsTracksEast==1)&&((skimEvent->GetBbcSumSmall(0) > 100)||(skimEvent->GetBbcSumLarge(0) > 60))) continue;

	if (jetEvent->GetNumberOfJets() == 0) continue;
	if ((nRpsTracksEast==1)&&(nRpsTracksWest==0)) continue;
	//if ((nRpsTracksWest==1)) continue;
	
	h1nJets_all->Fill(jetEvent->GetNumberOfJets());

	if(nJets > 0)
	{
	    for(Int_t t = 0; t < 9; ++t)
	    {
		if(skimEvent->GetTrigFlag(t))
		    h1TrigType->Fill(t);
	    }
	}
    

	for(Int_t j = 0; j <  jetEvent->GetNumberOfJets(); ++j)
	{
	    jet = jetEvent->GetJet(j);
	   	    
	    eta = jet->GetEta();
	    phi = jet->GetPhi();
	    theta =  2 * atan( exp(-eta) );

	    if(eta < etaMin || eta > etaMax) //Conside only EEMC and FMS coverage
		continue;

	    ++nJets;
	    eng = jet->GetE();
	//    if (jet->GetUedPt() < 0) eng = eng + jet->GetUedPt();
	//    if ((eng > 5)&&(eng < 10)) {eng_corr = f1->Eval(eng);} 
	//	else {if ((eng > 10)&&(eng < 60)) {eng_corr = f2->Eval(eng);} else {eng_corr = eng;}}
	    //cout<<"original EM jet E:"<<eng<<"	corrected EM jet E:"<<eng_corr<<endl;
	    eng_corr = eng;
	    pt = jet->GetPt();
	    RpWestPpJetE=RpWestPpJetE+eng_corr;
	    
	    if(j == 0)
		h1vtxZ->Fill(vtxZ);
	   

	    //if (eng < 2) continue;
	    //if (pt < 2) continue; 
	 
   
	    h2EvsPt->Fill(pt, eng_corr);
	    h2PtvsE->Fill(eng_corr, pt);
	    h2nPhVsEng->Fill(eng, jet->GetNumberOfTowers());

//	    bool good_emjet_pt = false;
//	    for(Int_t t = 0; t < 9; ++t) 
//	    {
//	        if(skimEvent->GetTrigFlag(t))
//		{
//
//			if (thisRunNum < 16069039) {if ((pt > pTcut)&&(FMSTrigThresP1[t] < pt)) {good_emjet_pt = true;break;}}
//			else {if ((pt > pTcut)&&(FMSTrigThresP2[t] < pt)) {good_emjet_pt = true;break;}}
//		}
//	    }
//	    if (!good_emjet_pt) continue;
	    
	    for(Int_t k = 0; k < jet->GetNumberOfTowers(); ++k)
	    {
		tower = jet->GetTower(k);
	
		h1TowerEta->Fill(tower->GetEta());
		h1TowerPhi->Fill(tower->GetPhi());
		h1TowerE->Fill(tower->GetEnergy());
	    }
		++goodjets;
	}

	if (nRpsTracksWest==1) {h1_sumEwest_beforeBBCcut->Fill(RpWestPpJetE);}
	h2_sumEwest_vs_smallBBCsumWest->Fill(skimEvent->GetBbcSumSmall(1),RpWestPpJetE);
	h2_sumEwest_vs_smallBBCsumEast->Fill(skimEvent->GetBbcSumSmall(0),RpWestPpJetE);
	if (det == "fms")
	{
		if (nRpsTracksWest==1) {h2_RPtrkEwest_vs_smallBBCsumWest->Fill(skimEvent->GetBbcSumSmall(1),WestRpP);}
		if (nRpsTracksEast==1) {h2_RPtrkEeast_vs_smallBBCsumEast->Fill(skimEvent->GetBbcSumSmall(0),EastRpP);}
	}
//	if ((det=="fms")&&(mode!="largeBBC")&&(mode!="smallBBC")) {if ((nRpsTracksWest==1)&&((skimEvent->GetBbcSumSmall(1) > 100)||(skimEvent->GetBbcSumLarge(1) > 60))) {continue;}}
//	if ((det=="fms")&&(mode=="largeBBC")) {if ((nRpsTracksWest==1)&&((skimEvent->GetBbcSumSmall(1) > 100)||(skimEvent->GetBbcSumLarge(1) > 63))) {continue;}}
//	if ((det=="fms")&&(mode=="smallBBC")) {if ((nRpsTracksWest==1)&&((skimEvent->GetBbcSumSmall(1) > 105)||(skimEvent->GetBbcSumLarge(1) > 60))) {continue;}}
	if (nRpsTracksWest==1) {h1_sumEwest->Fill(RpWestPpJetE);h2_EMJetE_vs_ProtonE->Fill(WestRpP, (RpWestPpJetE - WestRpP));}
	if (goodjets == 0) {continue;}

	//if ((RpWestPpJetE < 102)||(nRpsTracksWest==0))
	//if ((RpWestPpJetE < 108)&&(nRpsTracksWest==1))
	//if ((RpWestPpJetE > 140)&&(RpWestPpJetE < 150)&&(nRpsTracksWest==1))
	if ((nRpsTracksWest==1))
	{

		if ((nRpsTracksEast == 0) && (nRpsTracksWest == 1)) h1_event_type->Fill(2);
		if ((nRpsTracksEast == 1) && (nRpsTracksWest == 1)) h1_event_type->Fill(3);

		//cout<<"fill number"<<skimEvent->GetFillNumber()<<endl;
		it = std::find (FillNumber.begin(), FillNumber.end(), thisFillNum);
		fillit = it - FillNumber.begin();
	  UnixTime=skimEvent->GetUnixTime();
	  h1_fill_index->Fill(fillit);
	  if (det == "fms")
	  {
	  	Double_t polBlue = polFms[fillit].P0B + (UnixTime - polFms[fillit].StartTime) /3600. * polFms[fillit].dPBdT; 
	  	Double_t polYellow = polFms[fillit].P0Y + (UnixTime - polFms[fillit].StartTime) /3600. * polFms[fillit].dPYdT;
	  	dUnixTime = (UnixTime - polFms[fillit].StartTime);
		thisRunNum = thisRunNum - polFms[fillit].startrun;
		//cout << "this run num diff:"<<thisRunNum<<"	d Unix Time:"<<dUnixTime<<endl;
		if (dUnixTime > runEndTime[thisRunNum]) {runEndTime[thisRunNum] = dUnixTime;}
		if (dUnixTime < runStartTime[thisRunNum]) {runStartTime[thisRunNum] = dUnixTime;}
		nRunEvent[thisRunNum]=nRunEvent[thisRunNum] +1;		

	  	//cout<<"Blue beam pol:"<<polBlue<<endl;
	  	polBlue_Allfill->Fill(polBlue/100.);
	  	polYellow_Allfill->Fill(polYellow/100.);

	  }

	  thisFillEvent=thisFillEvent+1;

	  if (det == "eemc")
	  {
	  	Double_t polBlue = polEemc[fillit].P0B + (UnixTime - polEemc[fillit].StartTime) /3600. * polEemc[fillit].dPBdT; 
	  	Double_t polYellow = polEemc[fillit].P0Y + (UnixTime - polEemc[fillit].StartTime) /3600. * polEemc[fillit].dPYdT;
	  	polBlue_Allfill->Fill(polBlue/100.);
	  	polYellow_Allfill->Fill(polYellow/100.);
		//ThisFillUncertainty = ThisFillUncertainty + pow((polEemc[fillit].ddPBdT*(UnixTime - polEemc[fillit].StartTime)/3600.),2);
	  }
	  

	  for(Int_t j = 0; j <  jetEvent->GetNumberOfJets(); ++j)
	  {


	    jet = jetEvent->GetJet(j);
	    eta = jet->GetEta();
	    phi = jet->GetPhi();
	    if(eta < etaMin || eta > etaMax) //Conside only EEMC and FMS coverage
		continue;
	    eng = jet->GetE();
	    xF = 0;
 	    if (det == "fms") {xF = eng / 255.;}
	    if (det == "eemc") {xF = eng * tanh(eta) / 100.;}


	    if ((xF > 0.1)&& (xF < 0.15)) {h1_xF_1015_noCorr->Fill(xF);}
	    if ((xF > 0.15)&& (xF < 0.2)) {h1_xF_1520_noCorr->Fill(xF);}
	    if ((xF > 0.2)&& (xF < 0.25)) {h1_xF_2025_noCorr->Fill(xF);}
	    if ((xF > 0.25)&& (xF < 0.3)) {h1_xF_2530_noCorr->Fill(xF);}
	    if ((xF > 0.3)&&(xF < 0.45)) {h1_xF_3045_noCorr->Fill(xF);}
	    if (jet->GetUedPt() < 0) eng = eng + jet->GetUedPt();
	    h1E->Fill(eng);
	//    if ((eng > 5)&&(eng < 10)) {eng_corr = f1->Eval(eng);} 
	//	else {if ((eng > 10)&&(eng < 60)) {eng_corr = f2->Eval(eng);} else {eng_corr = eng;}}
	//	//eng_corr = f1->Eval(eng);
	    eng_corr = eng;
	    h1E_corr->Fill(eng_corr);
	    pt = jet->GetPt();
	    h1Eta->Fill(eta);
	    h1Phi->Fill(phi);
	    h1Pt->Fill(pt);
	    h1nPhotons->Fill(jet->GetNumberOfTowers());
	    //if (eng > 50) continue;
 	    //if (det == "fms") {xF = eng / 100.;}
	    //if (det == "eemc") {xF = eng * tanh(eta) / 100.;}
	    xF = 0;
 	    if (det == "fms") {xF = eng_corr / 100.;}
	    if (det == "eemc") {xF = eng_corr * tanh(eta) / 100.;}

	    //bool good_emjet_pt = false;
	    //for(Int_t t = 0; t < 9; ++t) 
	    //{
	    //    if(skimEvent->GetTrigFlag(t))
	    //    {

	    //    	if (thisRunNum < 16069039) {if ((pt > pTcut)&&(FMSTrigThresP1[t] < pt)) {good_emjet_pt = true;break;}}
	    //    	else {if ((pt > pTcut)&&(FMSTrigThresP2[t] < pt)) {good_emjet_pt = true;break;}}
	    //    }
	    //}
	    //if (!good_emjet_pt) continue;


	    if ((xF > 0.05)&& (xF < 0.1)) {h1_sumEwest_XF0510->Fill(RpWestPpJetE);h1_xF_0510->Fill(xF);}
	    if ((xF > 0.1)&& (xF < 0.15)) {h1_sumEwest_XF1015->Fill(RpWestPpJetE);h1_xF_1015->Fill(xF);}
	    if ((xF > 0.15)&& (xF < 0.2)) {h1_sumEwest_XF1520->Fill(RpWestPpJetE);h1_xF_1520->Fill(xF);}
	    if ((xF > 0.2)&& (xF < 0.25)) {h1_sumEwest_XF2025->Fill(RpWestPpJetE);h1_xF_2025->Fill(xF);}
	    if ((xF > 0.25)&& (xF < 0.3)) {h1_sumEwest_XF2530->Fill(RpWestPpJetE);h1_xF_2530->Fill(xF);}
	    if ((xF > 0.3)&&(xF < 0.45)) {h1_sumEwest_XF30->Fill(RpWestPpJetE);h1_xF_3045->Fill(xF); xF = 0.32;}
	    if (xF > 0.45) {continue;}

	    if (mode != "Esum")
	    {
	    
		        if ((xF<0.2)&&(RpWestPpJetE > 108)) continue;
	    		if ((xF<0.3)&&(RpWestPpJetE > 110)) continue;
	    		if ((xF<0.45)&&(RpWestPpJetE > 115)) continue;

	    }
	    else
	    {
	    
		        if ((xF<0.2)&&(RpWestPpJetE > 112)) continue;
	    		if ((xF<0.3)&&(RpWestPpJetE > 114)) continue;
	    		if ((xF<0.45)&&(RpWestPpJetE > 120)) continue;

	    }


	    if ((xF > 0.1)&& (xF < 0.15)) {h1_pT_XF1015->Fill(pt);h1_nPhoton_XF1015->Fill(jet->GetNumberOfTowers());}
	    if ((xF > 0.15)&& (xF < 0.2)) {h1_pT_XF1520->Fill(pt);h1_nPhoton_XF1520->Fill(jet->GetNumberOfTowers());}
	    if ((xF > 0.2)&& (xF < 0.25)) {h1_pT_XF2025->Fill(pt);h1_nPhoton_XF2025->Fill(jet->GetNumberOfTowers());}
	    if ((xF > 0.25)&& (xF < 0.3)) {h1_pT_XF2530->Fill(pt);h1_nPhoton_XF2530->Fill(jet->GetNumberOfTowers());}
	    if ((xF > 0.3)&&(xF < 0.45)) {h1_pT_XF30->Fill(pt);h1_nPhoton_XF30->Fill(jet->GetNumberOfTowers());}

	    if (mode == "Ecorr")
	    {

	    	if ((eng > 5)&&(eng < 10)) {eng_corr = f1->Eval(eng); eng_corr_sys = f3->Eval(eng); eng_corr_diff = 1.0*fabs(eng_corr - eng_corr_sys)/eng_corr;} 
			else {if ((eng > 10)&&(eng < 60)) {eng_corr = f2->Eval(eng); eng_corr_sys = f4->Eval(eng);eng_corr_diff = 1.0*fabs(eng_corr - eng_corr_sys)/eng_corr;} else {eng_corr = eng; eng_corr_diff = -1;}}
	    	//if ((xF > 0.1)&& (xF < 0.15) && (eng_corr_diff > 0)) {h1_xF_1015_SysDiff->Fill(eng_corr_diff);}
	    	//if ((xF > 0.15)&& (xF < 0.2) && (eng_corr_diff > 0)) {h1_xF_1520_SysDiff->Fill(eng_corr_diff);}
	    	if ((xF > 0.1)&& (xF < 0.2) && (eng_corr_diff > 0)) {h1_xF_1520_SysDiff->Fill(eng_corr_diff);}
	    	if ((xF > 0.2)&& (xF < 0.25) && (eng_corr_diff > 0)) {h1_xF_2025_SysDiff->Fill(eng_corr_diff);}
	    	if ((xF > 0.25)&& (xF < 0.3) && (eng_corr_diff > 0)) {h1_xF_2530_SysDiff->Fill(eng_corr_diff);}
	    	if ((xF > 0.3)&& (xF < 0.45) && (eng_corr_diff > 0)) {h1_xF_3045_SysDiff->Fill(eng_corr_diff);}

	    }

	    Double_t Yphi=0;
	    if (phi > 0) {Yphi = TMath::Pi() - phi;} else {Yphi = -1.0 * TMath::Pi() - phi;}

	    if ((xF<0.2)&&(xF>0.1)) {xF = 0.175;}

	    if (skimEvent->GetSpinB() == 1) h2_BU_xF_vs_phi->Fill(phi,xF);
	    if (skimEvent->GetSpinB() == -1) h2_BD_xF_vs_phi->Fill(phi,xF);
	    if (skimEvent->GetSpinY() == 1) h2_YU_xF_vs_phi->Fill(Yphi,xF);
	    if (skimEvent->GetSpinY() == -1) h2_YD_xF_vs_phi->Fill(Yphi,xF);

	    if (jet->GetNumberOfTowers() <= 2)
	    {

	    	if (skimEvent->GetSpinB() == 1) h2_BU_xF_vs_phi_nPhoton12->Fill(phi,xF);
	    	if (skimEvent->GetSpinB() == -1) h2_BD_xF_vs_phi_nPhoton12->Fill(phi,xF);
	    	if (skimEvent->GetSpinY() == 1) h2_YU_xF_vs_phi_nPhoton12->Fill(Yphi,xF);
	    	if (skimEvent->GetSpinY() == -1) h2_YD_xF_vs_phi_nPhoton12->Fill(Yphi,xF);

	    }
	    if ((nRpsTracksEast==1)&&(nRpsTracksWest==1)) 
	    {

		h1_nPhoton_count_1east1westRP->Fill(jet->GetNumberOfTowers());
		h1_pT_count_1east1westRP->Fill(pt);
		h1_xF_count_1east1westRP->Fill(xF);

	    }
	    if ((nRpsTracksEast==0)&&(nRpsTracksWest==1)) 
	    {

		h1_nPhoton_count_only1westRP->Fill(jet->GetNumberOfTowers());
		h1_pT_count_only1westRP->Fill(pt);
		h1_xF_count_only1westRP->Fill(xF);

	    }	
	    if (pt > 2) {h2PtvsXF->Fill(xF , 2.5);} else {h2PtvsXF->Fill(xF , 1.5);}

	    if (nRpsTracksEast==1) {h1_EastRpP->Fill(EastRpP);}
	    if (nRpsTracksWest==1) {h1_WestRpP->Fill(WestRpP);}
	    if ((nRpsTracksEast==1) && (nRpsTracksWest==1)) {h1_EastRpP_doublediff->Fill(EastRpP);h1_WestRpP_doublediff->Fill(WestRpP);}
	    if ((nRpsTracksWest==1) && (nRpsTracksEast==0)) {h1_WestRpP_singlediff->Fill(WestRpP);}

	  }	
	}

	    //Particle Branch is for simulated data only
	    // for(Int_t k = 0; k < jet->GetNumberOfParticles(); ++k)
	    // {
	    // 	particle = jet->GetParticle(k);
	
	    // }	    
	h1nJets->Fill(nJets);


    }
	for (int k=0;k<1000;k=k+1)
	{
		if (nRunEvent[k]>0) 
		{
			cout<<"k:"<<k<<"	n EVents this run:"<<nRunEvent[k]<<"	start time:"<<runStartTime[k]<<"	end time:"<<runEndTime[k]<<endl;
			ThisFillUncertainty = ThisFillUncertainty + (runEndTime[k] + runStartTime[k]) / 7200.0 * nRunEvent[k];
			
		}

	}
    if (thisFillEvent>0) {h1_pol_uncertainty->Fill(fillit,(sqrt(pow(polFms[fillit].dP0B,2) + pow(1.0*ThisFillUncertainty * polFms[fillit].ddPBdT/thisFillEvent , 2))));}

    outFile->Write();
    outFile->Close();
    delete ch;
}
