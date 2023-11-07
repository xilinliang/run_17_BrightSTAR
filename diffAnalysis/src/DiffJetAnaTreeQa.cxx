// Filename: DiffJetAnaTreeQa.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Jun 11 13:08:16 2020 (-0400)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "StRootInclude.h"
#include "BrJetMaker/TStJetCandidate.h"
#include "BrJetMaker/TStJetEvent.h"
#include "BrJetMaker/TStJetSkimEvent.h"
#include "BrContainers/TStRpsTrackData.h"
#include "BrContainers/TStEventData.h"
#include <iostream>

using namespace std;

//Use wildcat in the file name to combine many files.
void DiffJetAnaTreeQa(TString inFileName, TString outName, TString det)
{
    TChain *ch = new TChain("T");
    ch->Add(inFileName);
    
    TStJetEvent *jetEvent = 0;
    TStJetSkimEvent *skimEvent;
    TStJetCandidate *jet;
    TStJetTower *tower;
    TStJetParticle *particle;
    TStRpsTrackData *rpsTrack =0;    
    TStEventData *event;

    TClonesArray *rpsArr = new TClonesArray("TStRpsTrackData");

    ch->SetBranchAddress("jetEvents", &jetEvent);
    ch->SetBranchAddress("rpTrack",&rpsArr);

    TString triggerNames_fms[9] = {"FMS-JP0", "FMS-JP1", "FMS-JP2", "FMS-sm-bs1", "FMS-sm-bs2", "FMS-sm-bs3", "FMS-lg-bs1", "FMS-lg-bs2", "FMS-lg-bs3"};
    TString triggerNames_eemc[9] = {"EHT0","JP1","JP2","EHT0*EJP1*L2Egamma","JP2*J2JetHigh","BHT1*VPDMB30","BHT0*BBCMB","BHT1*BBCMB","BHT2*BBCMB"};
    
    TFile *outFile = new TFile(outName, "recreate");
    TH1D *h1nJets_all = new TH1D("h1nJets_all", "Number of Jets from All [TPC/BEMC + EEMC + FMS]", 10, 0, 10);
    TH1D *h1nJets  = new TH1D("h1nJets ", "Number of Jets [FMS or EEMC]", 10, 0, 10);
    TH1I *h1trigId = new TH1I("h1trigId","Trigger ID [480XXX]",100,790,889);
    TH1D *h1spinB = new TH1D("h1spinB", "Blue beam spin; Blue beam spin", 5, -2, 2);
    TH1D *h1spinY = new TH1D("h1spinY", "Yellow beam spin; Yellow beam spin", 5, -2, 2);
    TH1D *h1UnixTime = new TH1D("h1UnixTime", "Event Unix Time; Event Unix Time", 1000000, 1426e6, 1427e6);

    TH1D *h1TrigType = new TH1D("h1TrigType", "Trigger Type; Trigger Type", 9, 0, 9);


    TH1D *h1TowerEta = new TH1D("h1TowerEta", "Tower Eta", 100, 1.0, 4.5);
    TH1D *h1TowerPhi = new TH1D("h1TowerPhi", "Tower Phi", 100, -3.2, 3.2);
    TH1D *h1TowerE = new TH1D("h1TowerE", "Tower E", 100, 0, 100);

    TH1D *h1Eta = new TH1D ("h1Eta", "EM Jet Eta; Jet #eta", 100, 0.5, 5.0);
    TH1D *h1Phi = new TH1D ("h1Phi", "EM Jet Phi; Jet #phi [rad]", 100, -3.3, 3.3);
    TH1D *h1E = new TH1D ("h1E", "EM Jet E; Jet E [GeV]", 100, 0.0, 100.0);
    TH1D *h1E_JP1 = new TH1D ("h1E_JP1", "EM Jet E (JP1); Jet E [GeV]", 100, 0.0, 100.0);
    TH1D *h1E_JP2 = new TH1D ("h1E_JP2", "EM Jet E (JP2); Jet E [GeV]", 100, 0.0, 100.0);
    TH1D *h1JetE_s = new TH1D ("h1JetE_s", "EM Jet E [small cells]; Jet E [GeV]", 100, 0.0, 85.0);
    TH1D *h1JetE_l = new TH1D ("h1JetE_l", "EM Jet E [large cells]; Jet E [GeV]", 100, 0.0, 70.0);    
    TH1D *h1Pt = new TH1D ("h1Pt", "EM Jet Pt; Jet Pt [GeV/c]", 100, 0.0, 10.0);
    TH1D *h1UEPt = new TH1D ("h1UEPt", "UE EM Jet Pt; EM Jet Pt [GeV/c]", 100, 0.0, 10.0);
    TH1D *h1dPt = new TH1D ("h1dPt", "UE EM Jet #Delta Pt; |#Delta(Pt)| [GeV/c]", 100, 0.0, 10.0);
    TH1D *h1Pt_JP1 = new TH1D ("h1Pt_JP1", "EM Jet Pt (JP1); Jet Pt [GeV/c]", 100, 0.0, 10.0);
    TH1D *h1Pt_JP2 = new TH1D ("h1Pt_JP2", "EM Jet Pt (JP2); Jet Pt [GeV/c]", 100, 0.0, 10.0);
    TH1D *h1nPhotons = new TH1D("h1nPhotons", "number of photons in EM jets; Number of Photons", 20, 0, 20);
    TH1D *h1vtxZ = new TH1D("h1vtxZ", "Jet vetrex z; Jet vertex z [cm]", 100, -200, 200);
    TH1D *h1_nRpTracks = new TH1D("h1_nRpTracks", "Number of RP tracks", 20, 0, 20);     
    TH1D *h1_wRpPt = new TH1D("h1_trkPtWest", "West RP trk Pt; RP track P_{T} [GeV/c]", 100, 0, 10);
    TH1D *h1_eRpPt = new TH1D("h1_trkPtEast", "East RP trk Pt; RP track P_{T} [GeV/c]", 100, 0, 10);    
    TH1D *h1_wRpP = new TH1D("h1_trkPWest", "West RP trk P; RP track P [GeV/c]", 200, 0, 200);
    TH1D *h1_eRpP = new TH1D("h1_trkPEast", "East RP trk P; RP track P [GeV/c]", 200, 0, 200);
    TH1D *h1_RpNPlanes = new TH1D("h1_RpNPlanes","Number of RP planes;N Planes",9,0,9);

    TH1I *h1_goodEvents = new TH1I("h1_goodEvents","Good Events;",5,0,5);

    TH2D *h2EvsPt = new TH2D("h2EvsPt", "Eng vs Pt; Pt [GeV/C]; E [GeV]", 100, 0, 100, 100, 0, 100);
    TH2D *h2PtvsE = new TH2D("h2PtvsE", "Pt vs E; E [GeV]; Pt [GeV/c]", 100,  0, 100, 100, 0, 100);
    TH2D *h2nPhVsEng = new TH2D("h2nPhVsEng", "Number of photons vs Eng; E [Gev]; No. of Photons", 100, 0, 100, 20, 0, 20);
    TH2D *h2xy_fms = new TH2D ("h2xy_fms", "EM Jet Position [FMS];EM Jet X [cm];EM Jet Y [cm]", 100, -100., 100., 100, -100., 100.);
    TH2D *h2xy_eemc = new TH2D ("h2xy_eemc", "EM Jet Position [EEMC];EM Jet X [cm];EM Jet Y [cm]", 100, -250, 250, 100, -250, 250);
    TH2D *h2EtaPhi_fms = new TH2D ("h2EtaPhi_fms", "EM Jet Eta Phi [FMS];EM Jet #eta; EM Jet #phi [rad]", 100, 2.5, 4.5, 100, -3.5, 3.5);
    TH2D *h2EtaPhi_eemc = new TH2D ("h2EtaPhi_eemc", "EM Jet Eta Phi [EEMC];EM Jet #eta;EM Jet #phi [rad]", 100, 0.8, 2.5, 100, -3.5, 3.5);

    Double_t etaMin;
    Double_t etaMax;
    Double_t detZ; //For FMS
    if(det == "fms")
    {
	etaMin = 2.0;
	etaMax = 4.5;
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
    
    
    Double_t jetX, jetY, eta, phi, theta, vtxZ, eng, pt, dpt, UEpt;
    Int_t nJets = 0;
    
    cout << "Total Entries to be processed: "<< ch->GetEntries() <<endl;

    for(Int_t evt = 0; evt < ch->GetEntries(); ++evt)
    {
	ch->GetEntry(evt);

	if(evt % 1000 == 0)
	    cout << "Events Processed: "<< evt <<endl;
	
	skimEvent = jetEvent->GetEvent();
	h1_goodEvents->Fill(0);

	//cout<<"Trigger ID:"<<skimEvent->GetTrigFlag(5)<<endl;	
	//h1trigId->Fill(skimEvent->GetTrigId() % 1000);
	h1UnixTime->Fill(skimEvent->GetUnixTime());
	//cout << "Unix time:" << skimEvent->GetUnixTime() << endl;
	h1spinB->Fill(skimEvent->GetSpinB());
	h1spinY->Fill(skimEvent->GetSpinY());
	h1nJets_all->Fill(jetEvent->GetNumberOfJets());
	vtxZ = skimEvent->GetVertexZ();
	
	 for(Int_t t = 0; t < 9; ++t) //Moved to the end of envet loop
	 {
	     if(skimEvent->GetTrigFlag(t))
	 	h1TrigType->Fill(t);
	 }

	//Exclude FMS small-bs3 trigger that gives ring of fire issue. But this removes most of high energetic jets.
//	if(det == "fms")
//	{
//	     if(skimEvent->GetTrigFlag(5))
//		continue;
//	}
	//Alternative way to reduce ring of fire, require: BBCMult > 2 and TofMult > 2
	
	nJets = 0;	
		if (skimEvent->GetRPMult() > 0) cout << "nRp track:"<<skimEvent->GetRPMult()<<endl;
		if (skimEvent->GetRPMult() > 0) h1_goodEvents->Fill(1);
	if (skimEvent->GetFmsPointMult()>0) h1_goodEvents->Fill(2);
		if (jetEvent->GetNumberOfJets() > 0) h1_goodEvents->Fill(3);
	for(Int_t j = 0; j <  jetEvent->GetNumberOfJets(); ++j)
	{
	    jet = jetEvent->GetJet(j);
	   	    
	    eta = jet->GetEta();
	    phi = jet->GetPhi();
	    theta =  2 * atan( exp(-eta) );

//	    if(eta < etaMin || eta > etaMax) //Conside only EEMC and FMS coverage
//		continue;

	    ++nJets;
	    eng = jet->GetE();
	    pt = jet->GetPt();
	    dpt = jet->GetUedPt();
	    UEpt = pt + dpt;
	    

	    if(j == 0)
		h1vtxZ->Fill(vtxZ);
	    
	    h1nPhotons->Fill(jet->GetNumberOfTowers());
	    h1Eta->Fill(eta);
	    h1Phi->Fill(phi);
	    h1Pt->Fill(pt);
	    h1E->Fill(eng);
	    h1dPt->Fill(fabs(dpt));
	    h1UEPt->Fill(UEpt);

	    h2EvsPt->Fill(pt, eng);
	    h2PtvsE->Fill(eng, pt);
	    h2nPhVsEng->Fill(eng, jet->GetNumberOfTowers());

	    jetX = (detZ - vtxZ) * tan(theta) * cos(phi);
	    jetY = (detZ - vtxZ) * tan(theta) * sin(phi);
	    
	    if(det == "fms")  //actual range 2.65 - 3.9
	    {	
		h2xy_fms->Fill(jetX, jetY);
		h2EtaPhi_fms->Fill(eta, phi);

		if(eta > 2.0 && eta < 3.15) //FMS large cells
		    h1JetE_l->Fill(eng); 
		if(eta > 3.15 && eta < 4.0)  //FMS small cells
		    h1JetE_s->Fill(eng);		
	    }
	    
	    if(det == "eemc")   // actual range 1.086 - 2.0
	    {
		h2xy_eemc->Fill(jetX, jetY);
		h2EtaPhi_eemc->Fill(eta, phi);
		if (skimEvent->GetTrigFlag(1)) {h1Pt_JP1->Fill(pt);h1E_JP1->Fill(eng);}
		if (skimEvent->GetTrigFlag(2)) {h1Pt_JP2->Fill(pt);h1E_JP2->Fill(eng);h1E_JP1->Fill(eng);}
	    }
	    
	    for(Int_t k = 0; k < jet->GetNumberOfTowers(); ++k)
	    {
		tower = jet->GetTower(k);
	
		h1TowerEta->Fill(tower->GetEta());
		h1TowerPhi->Fill(tower->GetPhi());
		h1TowerE->Fill(tower->GetEnergy());
	    }

		//rp tracks
		int nRpsTracksEast=0;	
		int nRpsTracksWest=0;	
	    	Int_t nRpsTracks = rpsArr->GetEntriesFast();
		h1_nRpTracks->Fill(nRpsTracks);
		if ((nRpsTracks > 2)||(nRpsTracks == 0)) continue;
		for (Int_t rpTrk=0; rpTrk<nRpsTracks;++rpTrk)
		{
		
			rpsTrack = (TStRpsTrackData*)rpsArr->At(rpTrk);

			h1_RpNPlanes->Fill(rpsTrack->GetNplanes());	
			if (rpsTrack->GetNplanes()<7) continue;
	
			if(rpsTrack->GetBranch() == 2 || rpsTrack->GetBranch() == 3) //West RP :: 2 West Up RP, 3: East Down RP
            		{
                	h1_wRpP->Fill(rpsTrack->GetP());
                	h1_wRpPt->Fill(rpsTrack->GetPt());
			++nRpsTracksWest;
			}

			if(rpsTrack->GetBranch() == 0 || rpsTrack->GetBranch() == 1) //East RP :: O East Up RP, 1: East Down RP
                        {
			h1_eRpP->Fill(rpsTrack->GetP());
			h1_eRpPt->Fill(rpsTrack->GetPt());
			++nRpsTracksEast;

			}


		}
		if ((jetEvent->GetNumberOfJets() > 0) && (nRpsTracksWest == 1)) h1_goodEvents->Fill(4);

	    //Particle Branch is for simulated data only
	    // for(Int_t k = 0; k < jet->GetNumberOfParticles(); ++k)
	    // {
	    // 	particle = jet->GetParticle(k);
	
	    // }	    
	}
	h1nJets->Fill(nJets);

	if(nJets > 0)
	{
	    for(Int_t t = 0; t < 9; ++t)
	    {
		if(skimEvent->GetTrigFlag(t))
		    h1TrigType->Fill(t);
	    }
	}
    }



    outFile->Write();
    outFile->Close();
    delete ch;
}
