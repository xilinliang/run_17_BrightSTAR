// Filename: DiffJetRpCheck.cxx
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
#include <iostream>
#include <vector>

using namespace std;

//Use wildcat in the file name to combine many files.
void DiffJetRpCheck(Int_t runNo, TString outName, TString det)
{
    TChain *ch = new TChain("T");
    ch->Add(Form("NanoDst/run17RP/%i/NanoDiffRPTree_Run%i.root",runNo,runNo));
    
    TStJetEvent *jetEvent = 0;
    TStJetSkimEvent *skimEvent;
    TStJetCandidate *jet;
    TStJetTower *tower;
    TStJetParticle *particle;
    TStRpsTrackData *rpsTrack =0;    

    TClonesArray *rpsArr = new TClonesArray("TStRpsTrackData");

    ch->SetBranchAddress("jetEvents", &jetEvent);
    ch->SetBranchAddress("rpTrack",&rpsArr);

    vector<int> EventID;
    
    TFile *outFile = new TFile(outName, "recreate");
    TH1D *h1nJets_all = new TH1D("h1nJets_all", "Number of Jets from All [TPC/BEMC + EEMC + FMS]", 10, 0, 10);
    TH1D *h1nJets  = new TH1D("h1nJets ", "Number of  Jets [FMS or EEMC]", 10, 0, 10);
    TH1D *h1spinB = new TH1D("h1spinB", "Blue beam spin; Blue beam spin", 5, -2, 2);
    TH1D *h1spinY = new TH1D("h1spinY", "Yellow beam spin; Yellow beam spin", 5, -2, 2);
    TH1D *h1UnixTime = new TH1D("h1UnixTime", "Event Unix Time; Event Unix Time", 1000000, 1426e6, 1427e6);
    TH1D *h1TrigType = new TH1D("h1TrigType", "Trigger Type; Trigger Type", 10, 0, 10);
    
    TH1D *h1TowerEta = new TH1D("h1TowerEta", "Tower Eta", 100, 1.0, 4.5);
    TH1D *h1TowerPhi = new TH1D("h1TowerPhi", "Tower Phi", 100, -3.2, 3.2);
    TH1D *h1TowerE = new TH1D("h1TowerE", "Tower E", 100, 0, 100);

    TH1D *h1Eta = new TH1D ("h1Eta", "EM Jet Eta; Jet #eta", 100, 1.0, 5.0);
    TH1D *h1Phi = new TH1D ("h1Phi", "EM Jet Phi; Jet #phi [rad]", 100, -3.3, 3.3);
    TH1D *h1E = new TH1D ("h1E", "EM Jet E; Jet E [GeV]", 100, 0.0, 70.0);
    TH1D *h1JetE_s = new TH1D ("h1JetE_s", "EM Jet E [small cells]; Jet E [GeV]", 100, 0.0, 85.0);
    TH1D *h1JetE_l = new TH1D ("h1JetE_l", "EM Jet E [large cells]; Jet E [GeV]", 100, 0.0, 70.0);    
    TH1D *h1Pt = new TH1D ("h1Pt", "Jet Pt (Pt cut); Jet Pt [GeV/c]", 100, 0.0, 50.0);
    TH1D *h1Pt_nocut = new TH1D ("h1Pt_nocut", "Jet Pt (no cut); Jet Pt [GeV/c]", 100, 0.0, 50.0);
    TH1D *h1nPhotons = new TH1D("h1nPhotons", "number of photons in EM jets; Number of Photons", 20, 0, 20);
    TH1D *h1vtxZ = new TH1D("h1vtxZ", "Jet vetrex z; Jet vertex z [cm]", 100, -200, 200);
    TH1D *h1vtxZ_nocut = new TH1D("h1vtxZ_nocut", "Jet vetrex z no cut; Jet vertex z [cm]", 200, -200, 200);

    TH1D *h1_nRpTracks = new TH1D("h1_nRpTracks", "Number of RP tracks", 20, 0, 20);     
     TH1D *h1_nRpTracks_East = new TH1D("h1_nRpTracks_East", "Number of RP tracks on East", 10, 0, 10);
     TH1D *h1_nRpTracks_West = new TH1D("h1_nRpTracks_West", "Number of RP tracks on West", 10, 0, 10);
    TH1D *h1_wRpPt = new TH1D("h1_trkPtWest", "West RP trk Pt; RP track P_{T} [GeV/c]", 100, 0, 20);
    TH1D *h1_eRpPt = new TH1D("h1_trkPtEast", "East RP trk Pt; RP track P_{T} [GeV/c]", 100, 0, 20);    

    TH1D *h1_wRpP = new TH1D("h1_trkPWest", "West RP trk P; RP track P [GeV/c]", 200, 0, 200);
    TH1D *h1_eRpP = new TH1D("h1_trkPEast", "East RP trk P; RP track P [GeV/c]", 200, 0, 200);

    TH1D *h1_eRpP_only1 = new TH1D("h1_trkPEast_only", "(only 1) East RP trk P; RP track P [GeV/c]", 200, 0, 200);

    TH1D *h1_RpNPlanes = new TH1D("h1_RpNPlanes","Number of RP planes;N Planes",9,0,9);
    TH1I *h1_RpNPlanes_East = new TH1I("h1_RpNPlanes_East","Number of RP planes in East side;N Planes East",9,0,9);
    TH1I *h1_RpNPlanes_West = new TH1I("h1_RpNPlanes_West","Number of RP planes in West side;N Planes West",9,0,9);
    TH1D *h1_RpThetaX = new TH1D("h1_RpThetaX","Roman Pot #theta_{X};#theta_{X}",50,-5,5);
    TH1D *h1_RpThetaY = new TH1D("h1_RpThetaY","Roman Pot #theta_{Y};#theta_{Y}",50,-5,5);
    TH1D *h1_RpEta = new TH1D("h1_RpEta","Roman Pot #eta ; #eta",100,-10,10);
    TH1D *h1_RpPhi = new TH1D ("h1RpPhi", "Roman Pot Phi;  #phi [rad]", 100, -3.3, 3.3);
    TH2D *h2_RpPxPy = new TH2D ("h2_RpPxPy","Roman Pot trk P_{y} vs P_{x};P_{x} [GeV];P_{y} [GeV]",100,-0.5,0.5,100,-0.5,0.5);
    TH1I *h1_RpNPlanes_GlobalWest = new TH1I("h1_RpNPlanes_GlobalWest","Number of RP planes for global trk in West side;N Planes West",9,0,9);
    TH2D *h2_eRpP_wRpP = new TH2D ("h2_eRpP_wRpP", "East RP trk P vs West RP trk P;West RP trk P [GeV] ; East RP trk P [GeV]",150,10,160,150,10,160);

    TH2D *h2EvsPt = new TH2D("h2EvsPt", "Eng vs Pt; Pt [GeV/C]; E [GeV]", 100, 0, 20, 100, 0, 100);
    TH2D *h2PtvsE = new TH2D("h2PtvsE", "Pt vs E; E [GeV]; Pt [GeV/c]", 100,  0, 100, 100, 0, 20);
    TH2D *h2nPhVsEng = new TH2D("h2nPhVsEng", "Number of photons vs Eng; E [Gev]; No. of Photons", 100, 0, 100, 20, 0, 20);
    TH2D *h2xy_fms = new TH2D ("h2xy_fms", "Jet Position [FMS]; Jet X [cm]; Jet Y [cm]", 100, -100, 100, 100, -100, 100);
    TH2D *h2xy_eemc = new TH2D ("h2xy_eemc", "Jet Position [EEMC]; Jet X [cm]; Jet Y [cm]", 100, -250, 250, 100, -250, 250);
    TH2D *h2EtaPhi_fms = new TH2D ("h2EtaPhi_fms", "Jet Eta Phi [FMS]; Jet #eta; Jet #phi [rad]", 100, 2.5, 4.5, 100, -3.5, 3.5);
    TH2D *h2EtaPhi_eemc = new TH2D ("h2EtaPhi_eemc", "Jet Eta Phi [EEMC]; Jet #eta; Jet #phi [rad]", 100, 0.8, 2.5, 100, -3.5, 3.5);

    Double_t etaMin;
    Double_t etaMax;
    Double_t detZ; //For FMS
    if(det == "fms")
    {
	etaMin = 2.6;
	etaMax = 4.1;
	detZ = 735.; //For FMS
    }
    else if(det == "eemc")
    {
	etaMin = 1.0;
	etaMax = 2.0;
	detZ = kEEmcZSMD; //For EEMC
    }
    else
    {
	cout << "Invalid detector" <<endl;
	return;
    }
    
    
    Double_t jetX, jetY, eta, phi, theta, vtxZ, eng, pt,nEMjets_cut;
    Int_t nJets = 0;
    //Int_t UnixTime[1000000];
    
    cout << "Total Entries to be processed: "<< ch->GetEntries() <<endl;

    for(Int_t evt = 0; evt < ch->GetEntries(); ++evt)
    {
	ch->GetEntry(evt);

	if(evt % 1000 == 0)
	    cout << "Events Processed: "<< evt <<endl;
	
	skimEvent = jetEvent->GetEvent();
	
	h1UnixTime->Fill(skimEvent->GetUnixTime());
	//cout << "run:" << skimEvent->GetRunNumber() << "	event ID:" << skimEvent->GetEventId() << "	Unix Time:" << skimEvent->GetUnixTime() << endl;

	EventID.push_back(skimEvent->GetEventId());
	//UnixTime[evt] = skimEvent->GetUnixTime() % 100000;

//	h1spinB->Fill(skimEvent->GetSpinB());
//	h1spinY->Fill(skimEvent->GetSpinY());
//	h1nJets_all->Fill(jetEvent->GetNumberOfJets());
//	vtxZ = skimEvent->GetVertexZ();
//	h1vtxZ_nocut->Fill(vtxZ);	
//	if (fabs(vtxZ) > 80) continue;
//
//	// for(Int_t t = 0; t < 9; ++t) //Moved to the end of envet loop
//	// {
//	//     if(skimEvent->GetTrigFlag(t))
//	// 	h1TrigType->Fill(t);
//	// }
//
//	//Exclude FMS small-bs3 trigger that gives ring of fire issue. But this removes most of high energetic jets.
//	if(det == "fms")
//	{
//	    if(skimEvent->GetTrigFlag(5))
//		continue;
//	}
//	//Alternative way to reduce ring of fire, require: BBCMult > 2 and TofMult > 2
//	
//	nJets = 0;

	//roman pot cut	
	Int_t nRpsTracks = rpsArr->GetEntriesFast();
	h1_nRpTracks->Fill(nRpsTracks);
	//if (nRpsTracks != 1) continue;
	Int_t nRpsTrack_East =0;
	Int_t nRpsTrack_West =0;
	Double_t eRpTrkP = 0;
	Double_t wRpTrkP = 0;
	for (Int_t rpTrkid=0;rpTrkid < nRpsTracks; ++rpTrkid)
	{
		rpsTrack = (TStRpsTrackData*)rpsArr->At(rpTrkid);

		h1_RpNPlanes->Fill(rpsTrack->GetNplanes());	

		if (rpsTrack->GetNplanes()<5) continue;
		
		if(rpsTrack->GetBranch() == 0 || rpsTrack->GetBranch() == 1) //East RP :: O East Up RP, 1: East Down RP
                {++nRpsTrack_East;h1_RpNPlanes_East->Fill(rpsTrack->GetNplanes());} 
		else 
		{
			++nRpsTrack_West;
			h1_RpNPlanes_West->Fill(rpsTrack->GetNplanes());
			if (rpsTrack->GetType()==1) {h1_RpNPlanes_GlobalWest->Fill(rpsTrack->GetNplanes());}
		}

		//if (rpsTrack->GetNplanes()<5) continue;

		if(rpsTrack->GetBranch() == 0 || rpsTrack->GetBranch() == 1) //East RP :: O East Up RP, 1: East Down RP
                {
                	h1_eRpP->Fill(rpsTrack->GetP());
                	h1_eRpPt->Fill(rpsTrack->GetPt());
			eRpTrkP = rpsTrack->GetP();
                }
		else 
		{
			h1_wRpP->Fill(rpsTrack->GetP());
                        h1_wRpPt->Fill(rpsTrack->GetPt());
			wRpTrkP = rpsTrack->GetP();
		}

		h1_RpThetaX->Fill(rpsTrack->GetThetaX());
		h1_RpThetaY->Fill(rpsTrack->GetThetaY());	
		h1_RpEta->Fill(rpsTrack->GetEta());
		h1_RpPhi->Fill(rpsTrack->GetPhi());
		h2_RpPxPy->Fill(rpsTrack->GetPx(),rpsTrack->GetPy());
	}

	h1_nRpTracks_East->Fill(nRpsTrack_East);
        h1_nRpTracks_West->Fill(nRpsTrack_West);	

	if ((nRpsTrack_East == 1)&&(nRpsTrack_West == 0)) {h1_eRpP_only1->Fill(eRpTrkP);}
	if ((nRpsTrack_East == 1)&&(nRpsTrack_West == 1)) {h2_eRpP_wRpP->Fill(wRpTrkP,eRpTrkP);}
    }



    //For synchrimize purpose
    TChain *ch2 = new TChain("T");
    if (det == "eemc") {ch2->Add(Form("NanoDst/run17EEMCjets/%i/NanoDiffJetTree_Run%i.root",runNo,runNo));}
    if (det == "fms") {ch2->Add(Form("NanoDst/run17FMSjets/%i/NanoDiffJetTree_Run%i.root",runNo,runNo));}
    //ch2->Add(Form("NanoDst/run17RP/%i/NanoDiffRPTree_Run%i.root",runNo,runNo));
    ch2->SetBranchAddress("jetEvents", &jetEvent);
 
    bool match;
    std::vector<int>::iterator matchID;
    int id;

    for(Int_t evt = 0; evt < ch2->GetEntries(); ++evt)
    {
	ch2->GetEntry(evt);

	//if(evt % 1000 == 0)
	//    cout << "Events Processed: "<< evt <<endl;
	
	skimEvent = jetEvent->GetEvent();

	match = false;
	matchID = std :: find(EventID.begin(), EventID.end(),skimEvent->GetEventId());
		id = matchID - EventID.begin();

	//cout << "match ID?"<<id << endl;
	if (matchID != EventID.end())
	{
		match = true;	
		cout << "match ! event ID:"<<skimEvent->GetEventId() <<endl;
		//cout << "match!  run:" << skimEvent->GetRunNumber() << "	event ID:" << skimEvent->GetEventId() << "	Unix Time:" << skimEvent->GetUnixTime() % 100000 << "	RP event Unix Time:" << UnixTime[id] << endl;


	}
	//else {cout << "can't match!"<<endl;}

    }

//	for(Int_t j = 0; j <  jetEvent->GetNumberOfJets(); ++j)
//	{
//	    jet = jetEvent->GetJet(j);
//	   	    
//	    eta = jet->GetEta();
//	    phi = jet->GetPhi();
//	    theta =  2 * atan( exp(-eta) );
//
//	    if(eta < etaMin || eta > etaMax) //Conside only EEMC and FMS coverage
//		continue;
//
//	    ++nJets;
//	    eng = jet->GetE();
//	    pt = jet->GetPt();
//
//	    if(j == 0)
//		h1vtxZ->Fill(vtxZ);
//	   
//	    h1Pt_nocut->Fill(pt);
//
//	    if (eng < 2) continue;
//	    if (pt < 2) continue; 
//	    h1nPhotons->Fill(jet->GetNumberOfTowers());
//	    h1Eta->Fill(eta);
//	    h1Phi->Fill(phi);
//	    h1Pt->Fill(pt);
//	    h1E->Fill(eng);
//	 
//   
//	    h2EvsPt->Fill(pt, eng);
//	    h2PtvsE->Fill(eng, pt);
//	    h2nPhVsEng->Fill(eng, jet->GetNumberOfTowers());
//
//	    jetX = (detZ - vtxZ) * tan(theta) * cos(phi);
//	    jetY = (detZ - vtxZ) * tan(theta) * sin(phi);
//	    
//	    if(det == "fms")  //actual range 2.65 - 3.9
//	    {	
//		h2xy_fms->Fill(jetX, jetY);
//		h2EtaPhi_fms->Fill(eta, phi);
//
//		if(eta > 2.0 && eta < 3.15) //FMS large cells
//		    h1JetE_l->Fill(eng); 
//		if(eta > 3.15 && eta < 4.0)  //FMS small cells
//		    h1JetE_s->Fill(eng);		
//	    }
//	    
//	    if(det == "eemc")   // actual range 1.086 - 2.0
//	    {
//		h2xy_eemc->Fill(jetX, jetY);
//		h2EtaPhi_eemc->Fill(eta, phi);
//	    }
//	    
//	    for(Int_t k = 0; k < jet->GetNumberOfTowers(); ++k)
//	    {
//		tower = jet->GetTower(k);
//	
//		h1TowerEta->Fill(tower->GetEta());
//		h1TowerPhi->Fill(tower->GetPhi());
//		h1TowerE->Fill(tower->GetEnergy());
//	    }
//	}
//
//		
//
//	    //Particle Branch is for simulated data only
//	    // for(Int_t k = 0; k < jet->GetNumberOfParticles(); ++k)
//	    // {
//	    // 	particle = jet->GetParticle(k);
//	
//	    // }	    
//	h1nJets->Fill(nJets);
//
//	if(nJets > 0)
//	{
//	    for(Int_t t = 0; t < 9; ++t)
//	    {
//		if(skimEvent->GetTrigFlag(t))
//		    h1TrigType->Fill(t);
//	    }
//	}



    outFile->Write();
    outFile->Close();
    delete ch;
}
