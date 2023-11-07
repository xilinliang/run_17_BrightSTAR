
#include "StRootInclude.h"
#include "RootInclude.h"
#include "cppInclude.h"
#include <string>
#include "StEvent/StEvent.h"
#include "StEvent/StEventInfo.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "MCOutput/MCOutput.h"
#include "StarGenerator/EVENT/StarGenEvent.h"
#include "StarGenerator/EVENT/StarGenParticle.h"
#include "StSpinPool/StJetSkimEvent/StPythiaEvent.h"

// #include "TStTrigDef.h"

//Use file name with wildcard to chain many files
void FmsSimJetQa(
            Int_t nentries,
	    Int_t fileID
    )
{

    TFile *outFile = new TFile(Form("/star/data01/pwg/liangxl/myBrightSTAR/root_result/FmsSimRpParticle/RpFmsSimParticle_Run17_%i_evt1000.root",fileID), "recreate");
    
    ifstream inputHardDiff;
    inputHardDiff.open(Form("/star/data01/pwg/liangxl/SimTest/starsimRootrun17HardDiff/check_hard_diff_%i.txt",fileID));

    // Open jet & skim files
    TChain* jetChain = new TChain("jet");

    Int_t nFiles_jet;
//    nFiles_jet = jetChain->Add(Form("/star/data01/pwg/liangxl/BrightSTAR_EMjet/root_result/FmsSimJetResult/singlediff/FmsJet_Run15_%i_evt1000.root",fileID));
    //nFiles_jet = jetChain->Add(Form("/star/data01/pwg/liangxl/BrightSTAR_EMjet/root_result/FmsSimJetResult/FmsJet_Run15_%i_evt1000.root",fileID));
    nFiles_jet = jetChain->Add(Form("/star/data01/pwg/liangxl/myBrightSTAR/root_result/FmsSimJetResult2/FmsJet_Run17_%i_evt500.root",fileID));



    StarGenEvent* genevent=0;
    //StPythiaEvent* genevent=0;
    StarGenParticle* genparticle=0;

    TChain* PythiaTree = new TChain("genevents");
    PythiaTree->Add(Form("/star/data01/pwg/liangxl/SimTest/starsimRootrun17HardDiff/pythia8_%i.starsim.root",fileID));
    PythiaTree->SetBranchAddress("primaryEvent",&genevent);


    Int_t nEvents = PythiaTree->GetEntries();
    if(nentries == -1 || nentries > nEvents)
	nentries = nEvents;

    //cout << "Number of files added: "<<nFiles_jet <<endl;
    //cout << "Total Entries to be processed: "<< nentries <<endl;
    //Check if added files are sorted properly which is crucial for skimChain vs jetChain synchronization
    //jetChain->ls();

    // Set jet buffer
    StJetEvent* jetEvent = 0;
    jetChain->SetBranchAddress("AntiKtR070NHits12",&jetEvent);

    // set MuDst
//    StMuEvent* mMuEvent = 0;
    //MuDstChain->SetBranchAddress("MuEvent",&mMuEvent);

//    StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"",Form("/star/data01/pwg/liangxl/BrightSTAR_EMjet/root_result/FmsSimMuDstResult/singlediff/FmsSim_Run15_%i_evt1000.MuDst.root",fileID),"");
    //StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"",Form("/star/data01/pwg/liangxl/BrightSTAR_EMjet/root_result/FmsSimMuDstResult/FmsSim_Run15_%i_evt1000.MuDst.root",fileID),"");

    Int_t checkharddiff;
    Bool_t check_hard_diff_event[1000];
	for (int id1=0; id1 < 1000; ++id1)
	{

		inputHardDiff >> checkharddiff;
		if (checkharddiff==1) {cout<<id1<<" is hard diffraction event"<<endl;check_hard_diff_event[id1] = true;} else {check_hard_diff_event[id1] = false;}

	}

    TString title;
	title = " ";
    //-- All Triggers -------
//    TH1D *h1TrigTypes = new TH1D ("h1TrigTypes", "Trigger Types" + title + "; Trigger Types", 30, 0, 30);
//
//    h1TrigTypes->GetXaxis()->SetBinLabel(3,"Small BS1");
//    h1TrigTypes->GetXaxis()->SetBinLabel(5,"Small BS2");
//    h1TrigTypes->GetXaxis()->SetBinLabel(7,"Small BS3");
//    h1TrigTypes->GetXaxis()->SetBinLabel(9,"Large BS1");
//    h1TrigTypes->GetXaxis()->SetBinLabel(11,"Large BS2");
//    h1TrigTypes->GetXaxis()->SetBinLabel(13,"Large BS3");
//    h1TrigTypes->GetXaxis()->SetBinLabel(15,"FMS-JP0");
//    h1TrigTypes->GetXaxis()->SetBinLabel(17,"FMS-JP1");
//    h1TrigTypes->GetXaxis()->SetBinLabel(19,"FMS-JP2");
//    h1TrigTypes->GetXaxis()->SetBinLabel(21,"None");
//    
    TH1D *h1nJets = new TH1D ("h1nJets", "Number of Jets" + title + "; No. of Jets", 10, -1, 9);
//
//    TH1D *h1nVtx = new TH1D("h1nVtx", "Number of Vertices" + title + "; No of vertices", 5, 0, 5);
//    TH1D* h1Vtxz = new TH1D("h1Vtxz", "Vtx z Distribution [All Sources]" + title + "; Vertex z [cm]",100, -200, 200);
//    
//    TH1D *h1JetEta = new TH1D ("h1JetEta", "EM Jet Eta" + title + ";EM Jet #eta", 100, -1.5, 5.0);
//    TH1D *h1JetPhi = new TH1D ("h1JetPhi", "EM Jet Phi" + title + ";EM Jet #phi [rad]", 100, -3.1, 3.1);
//    TH1D *h1JetPhi_East = new TH1D ("h1JetPhi_East", "EM Jet Phi sorted by east RP track" + title + ";EM Jet #phi [rad]", 100, -3.14, 3.14);
//    TH1D *h1JetPhi_West = new TH1D ("h1JetPhi_West", "EM Jet Phi sorted by west RP track" + title + ";EM Jet #phi [rad]", 100, -3.14, 3.14);
//    TH1D *h1JetE = new TH1D ("h1JetE", "EM Jet E" + title + ";EM Jet E [GeV]", 100, 0.0, 70.0);
//    TH1D *h1JetE_East = new TH1D ("h1JetE_East", "EM Jet E sorted by only east RP track" + title + ";EM Jet E [GeV]", 100, 0.0, 70.0);
//    TH1D *h1JetE_West = new TH1D ("h1JetE_West", "EM Jet E sorted by only west RP track" + title + ";EM Jet E [GeV]", 100, 0.0, 70.0);
//    TH1D *h1JetE_s = new TH1D ("h1JetE_s", "EM Jet E [small cells]" + title + "; Jet E [GeV]", 100, 0.0, 85.0);
//    TH1D *h1JetE_l = new TH1D ("h1JetE_l", "EM Jet E [large cells]" + title + "; Jet E [GeV]", 100, 0.0, 70.0);
//    TH1D *h1JetPt = new TH1D ("h1JetPt", "Jet Pt" + title + "; EM Jet Pt [GeV/c]", 40, 0.0, 5.0);
//    TH1D *h1JetPt_East = new TH1D ("h1JetPt_East", "EM Jet Pt sorted by only east RP track" + title + ";EM Jet Pt [GeV/c]", 40, 0.0, 5.0);
//    TH1D *h1JetPt_West = new TH1D ("h1JetPt_West", "EM Jet Pt sorted by only west RP track" + title + ";EM Jet Pt [GeV/c]", 40, 0.0, 5.0);
//    TH1D *h1JetVtxZ = new TH1D ("h1JetVtxZ", "EM Jet Vtx z" + title + ";EM Jet vtx z [cm]", 100, -200.0, 200.0);
//    TH1D *h1nPhotons = new TH1D("h1nPhotons", "number of photons in EM jets" + title + "; Number of photons", 10, 0, 10);
//    TH1D *h1_nRpTracks = new TH1D("h1_nRpTracks", "Number of RP tracks", 10, 0, 10);     
    TH1D *h1RpEta = new TH1D ("h1RpEta", "RP track Eta" + title + "; #eta", 250, -10.0, 10.0);
    TH1D *h1RpPhi = new TH1D ("h1RpPhi", "RP track Phi" + title + "; #phi [rad]", 50, -3.14, 3.14);
    TH1D *h1RpEta_East = new TH1D ("h1RpEta_East", "Only 1 east RP track Eta with FMS response" + title + "; #eta", 250, -10.0, 10.0);
    TH1D *h1RpPhi_East = new TH1D ("h1RpPhi_East", "Only 1 east RP track Phi with FMS response; #phi [rad]", 50, -3.14, 3.14);
    TH1D *h1RpEta_West = new TH1D ("h1RpEta_West", "Only 1 west RP track Eta with FMS response" + title + "; #eta", 250, -10.0, 10.0);
    TH1D *h1RpPhi_West = new TH1D ("h1RpPhi_West", "Only 1 west RP track Phi with FMS response" + title + "; #phi [rad]", 50, -3.14, 3.14);
    TH1D *h1RpPt_East = new TH1D ("h1RpPt_East", "Rp Pt sorted by east side with FMS response" + title + "; Pt [GeV/c]", 30, 0.0, 3.0);
    TH1D *h1RpPt_West = new TH1D ("h1RpPt_West", "Rp Pt sorted by west side with FMS response" + title + "; Pt [GeV/c]", 30, 0.0, 3.0);
    TH1D *h1RpP_East = new TH1D ("h1RpP_East", "Rp P sorted by east side with FMS response" + title + "; P [GeV/c]", 130, 0.0, 310.0);
    TH1D *h1RpP_West = new TH1D ("h1RpP_West", "Rp P sorted by west side with FMS response" + title + "; P [GeV/c]", 130, 0.0, 310.0);

    TH1D *h1_PartPrEta = new TH1D ("h1_PartPrEta","particle level proton Eta;#eta",100,-10.0,10.0);    
    TH1D *h1_PartPrPhi = new TH1D ("h1_PartPrPhi", "particle level proton Phi" + title + "; #phi [rad]", 50, -3.14, 3.14);
//    TH1D *h1_PartNoRpPrEta = new TH1D ("h1_PartNoRpPrEta","particle level proton Eta (not detected by RP);#eta",100,-10.0,10.0);    
//    TH1D *h1_PartNoRpPrPhi = new TH1D ("h1_PartNoRpPrPhi", "particle level proton Phi (not detected by RP)" + title + "; #phi [rad]", 50, -3.14, 3.14);
    TH1D *h1PartPrPt_East = new TH1D ("h1PartPrPt_East", "Particle level proton Pt sorted by east side" + title + "; Pt [GeV/c]", 30, 0.0, 3.0);
    TH1D *h1PartPrPt_West = new TH1D ("h1PartPrPt_West", "Particle level proton Pt sorted by west side" + title + "; Pt [GeV/c]", 30, 0.0, 3.0);
    TH1D *h1PartPrP_East = new TH1D ("h1PartPrP_East", "Particle level proton P sorted by east side" + title + "; P [GeV/c]", 130, 50.0, 310.0);
    TH1D *h1PartPrP_West = new TH1D ("h1PartPrP_West", "Particle level proton P sorted by west side" + title + "; P [GeV/c]", 130, 50.0, 310.0);
    TH1D *h1_sumE = new TH1D ("h1_sumE", "sum RP P and photons E" + title + "; E [GeV]", 130, 0.0, 310.0);
    TH1D *h1_BBC_E_diff = new TH1D ("h1_BBC_E_diff", "final state E in small BBC range (sig)" + title + "; E [GeV]", 200, 0.0, 200.0);
    TH1D *h1_BBC_E_nondiff = new TH1D ("h1_BBC_E_nondiff", "final state E in small BBC range (bkg)" + title + "; E [GeV]", 200, 0.0, 200.0);
    TH1D *h1_sumBBC_E_diff = new TH1D ("h1_sumBBC_E_diff", "summing all final state E in west small BBC range (sig)" + title + "; E [GeV]", 200, 0.0, 200.0);
    TH1D *h1_sumBBC_E_nondiff = new TH1D ("h1_sumBBC_E_nondiff", "summing all final state E in west small BBC range (bkg)" + title + "; E [GeV]", 200, 0.0, 200.0);
    TH1D *h1_nPart_E_diff_west = new TH1D ("h1_nPart_E_diff_west", "n charged particle in west small BBC range (sig)" + title +"", 50, 0.0, 50.0);
    TH1D *h1_nPart_E_nondiff_west = new TH1D ("h1_nPart_E_nondiff_west", "n charged particles in west small BBC range (bkg)" + title + "", 50, 0.0, 50.0);
    TH1D *h1_nPart_E_diff_east = new TH1D ("h1_nPart_E_diff_east", "n charged particle in east small BBC range (sig)" + title +"", 50, 0.0, 50.0);
    TH1D *h1_nPart_E_nondiff_east = new TH1D ("h1_nPart_E_nondiff_east", "n charged particles in east small BBC range (bkg)" + title + "", 50, 0.0, 50.0);
//
//    TH1D* h1PartPrVtxz = new TH1D("h1PartPrVtxz", "Particle level proton Vertex z Distribution [only from proton beam]" + title + "; Vertex z [cm]",100, -20, 20);
//    TH1D *h1RPandPartPrP_Diff = new TH1D ("h1RPandPartPrP_Diff", "Momentum difference between best match Roman Pot track and Particle level proton P" + title + "; #Delta P [GeV/c]", 100, 0.0, 50.0);
//
//    TH1D *h1_PartPrEta_other = new TH1D ("h1_PartPrEta_other","particle level proton Eta (from other decay);#eta",100,-10.0,10.0);    
//    TH1D *h1_PartPrPhi_other = new TH1D ("h1_PartPrPhi_other", "particle level proton Phi (from other decay)" + title + "; #phi [rad]", 50, -3.14, 3.14);
//    TH1D *h1PartPrPt_other = new TH1D ("h1PartPrPt_other", "Particle level proton Pt from other decay" + title + "; Pt [GeV/c]", 100, 0.0, 10.0);
//    TH1D *h1PartPrP_other = new TH1D ("h1PartPrP_other", "Particle level proton P from other decay" + title + "; P [GeV/c]", 80, 0.0, 160.0);
//
//    TH1D *h1PartPrMotherPID = new TH1D ("h1PartPrMotherPID", "Particle level proton mother PID from other decay or beam" + title + "; Trigger Types", 16, 0, 16);
//    h1PartPrMotherPID->GetXaxis()->SetBinLabel(3,"-3");
//    h1PartPrMotherPID->GetXaxis()->SetBinLabel(5,"-2");
//    h1PartPrMotherPID->GetXaxis()->SetBinLabel(7,"0");
//    h1PartPrMotherPID->GetXaxis()->SetBinLabel(9,"2101");
//    h1PartPrMotherPID->GetXaxis()->SetBinLabel(11,"2103");
//    h1PartPrMotherPID->GetXaxis()->SetBinLabel(13,"2203");
//    h1PartPrMotherPID->GetXaxis()->SetBinLabel(15,"other");
//
//    TH1D *h1PartPrBeam = new TH1D ("h1PartPrBeam", "Particle level proton from  beam", 7, 0, 7);
//    h1PartPrBeam->GetXaxis()->SetBinLabel(3,"no additional proton from beam");
//    h1PartPrBeam->GetXaxis()->SetBinLabel(5,"additional proton from beam");
//    TH1D *h1PartPrMatch = new TH1D ("h1PartPrMatch", "Roman Pot track match with Particle level proton", 7, 0, 7);
//    h1PartPrBeam->GetXaxis()->SetBinLabel(3,"no match");
//    h1PartPrBeam->GetXaxis()->SetBinLabel(5,"match");
//
//    TH2D *h2_PartNoPrPhi_vs_Eta = new TH2D("h2_PartNoPrPhi_vs_Eta", "Particle level proton #phi vs #eta (not detected by RP);#eta ;#phi [rad];",100, -10.0, 10.0,100, -3.14, 3.14);   
//
//    TH2D *h2Jetxy = new TH2D ("h2Jetxy", "EM Jet Position [FMS]" + title + "; Jet X [cm]; Jet Y [cm]", 100, -100, 100, 100, -100, 100);
//    TH2D *h2JetEP = new TH2D ("h2Jetep_fms", "Jet Eta Phi [FMS]" + title + "; Jet #eta; Jet #phi", 100, 2.5, 4.5, 100, -3.2, 3.2);
//    
//    TH2D *h2Jetxy_eemc = new TH2D ("h2Jetxy_eemc", "EM Jet position [EEMC]" + title + "; Jet X [cm]; Jet Y [cm]", 100, -200.0, 200.0, 100, -200, 200);
//    TH2D *h2EvsPt = new TH2D("h2EvsPt", "Eng vs Pt" + title + "; Pt [GeV/C]; E [GeV]", 100, 0, 20, 100, 0, 100);
//    TH2D *h2PtvsE = new TH2D("h2PtvsE", "Pt vs E" + title + "; E [GeV]; Pt [GeV/c]", 100,  0, 100, 100, 0, 20);
//    TH2D *h2_RpPhi_vs_JetPhi_East = new TH2D("h2_RpPhi_vs_JetPhi_East", "East RP track #phi vs Jet #phi;Jet #phi [rad]; RP track #phi [rad]",100, -3.14, 3.14,100, -3.14, 3.14);   
//    TH2D *h2_RpPhi_vs_JetPhi_West = new TH2D("h2_RpPhi_vs_JetPhi_West", "West RP track #phi vs Jet #phi;Jet #phi [rad]; RP track #phi [rad]",100, -3.14, 3.14,100, -3.14, 3.14);   
//    TH2D *h2_RPPrP_vs_PartPrP_west = new TH2D("h2_RPPrP_vs_PartPrP_west","Particle level Proton P vs Roman Pot Track P at west side;P[GeV/c];P[GeV/c]",75,30.,180.,75,30.,180.);
//    TH2D *h2_RPPrP_vs_PartPrP_east = new TH2D("h2_RPPrP_vs_PartPrP_east","Particle level Proton P vs Roman Pot Track P at east side;P[GeV/c];P[GeV/c]",75,30.,180.,75,30.,180.);
//    TH2D *h2_RPPrEta_vs_PartPrEta_other = new TH2D("h2_RPPrEta_vs_PartPrEta_other","Particle level Proton Eta vs Roman Pot Track Eta for best match;Roman Pot #eta;Particle level #eta",100,-10.,10.,100,-10.,10.);

 
    TLorentzVector lv;
    TVector3 v3;
    Double_t eta;
    Double_t phi;
    Double_t E;
    Double_t pt;
    Double_t z_vtx;
    Double_t jetX;
    Double_t jetY;
    Double_t theta;
    Int_t trigBit;
    string trgBitStr;
    StJetParticle *part;
    Int_t vtxType = -1;
    Int_t RptrackPos = 0;
    Double_t RpTrackPhi = -999;
    Double_t vtxZ = -999;
    Double_t RpWestTrkP=0;
    Double_t RpEastTrkP=0;
    //----- Event loop ----------
    //Loads entries from Jet Tree and Skim Tree simultaneously
    for (int iEntry = 0; iEntry < nentries; ++iEntry)
    {
	if (jetChain->GetEvent(iEntry) <= 0) break;

	//muDstMaker->Make();
	//mMuEvent = muDstMaker->muDst()->event();
	////MuDstChain->GetEntry(iEntry);
	////Int_t mId = mMuEvent->eventInfo().id();
	//Int_t mId = mMuEvent->eventInfo().id();
//	//cout<<"event ID:"<<mId<<endl;

	// Should not be null
	assert(jetEvent);

	if (iEntry % 1000 == 0) cout << iEntry << endl;
	
	// if(jetEvent->numberOfJets() == 0)
	//     continue;

	StJetVertex* vertex = jetEvent->vertex(0);
	//if (!vertex) continue;
	if (!vertex) {vtxZ = 0;}
	else
	{	
		vtxType = -1;
		vtxZ = vertex->position().z();
		//h1nVtx->Fill(jetEvent->numberOfVertices());
	}	

	// if(vertex->ranking() < 0) continue; //Not applicable for EM jet	
	// if(fabs(vertex->position().z()) > 80.0) continue;


	//Trigger Types
//	trigBit = jetEvent->runId();
//
//	trgBitStr = to_string(trigBit);
//	for(Int_t ii = 1; ii < 10; ++ii)
//	{
//	    if(trgBitStr[ii] == '1')
//		h1TrigTypes->Fill(2*ii);
//	}	
//	
//	if(trigBit == 1000000000)
//	{
//	    h1TrigTypes->Fill(2*10);
//	    continue;
//	}
	
	
	
	
	h1nJets->Fill(jetEvent->numberOfJets());
  }

//    Int_t nEvents = PythiaTree->GetEntries();
//    if(nentries == -1 || nentries > nEvents)
//	nentries = nEvents;
  for (Int_t evt=0; evt < nEvents; evt=evt+1)
  {

//	//It seems most people loop over vertices. Find the justification.
//	//I am looping over number of jets
//	for(Int_t i = 0; i < jetEvent->numberOfJets(); ++i) 
//	{
//	    eta = jetEvent->jet(i)->eta();
//	    phi = jetEvent->jet(i)->phi();
//	    E = jetEvent->jet(i)->E();
//	    pt = jetEvent->jet(i)->pt();
//	    z_vtx = jetEvent->jet(i)->vertex()->position().z();
//	    theta =  2 * atan( exp(-eta) );
//	    jetX = (735. - z_vtx) * tan(theta) * cos(phi);
//	    jetY = (735. - z_vtx) * tan(theta) * sin(phi);
//	    
//	    h1JetEta->Fill(eta);
//	    h1JetPhi->Fill(phi);
//	    h1JetPt->Fill(pt);
//	    h1JetVtxZ->Fill(z_vtx); // no vtx found case i.e. vtx = 0 will dominate if all types filled together
//
//	    if (RptrackPos == -1) //east side RP
//	    {
//		h1JetPhi_East->Fill(phi);
//		h1JetPt_East->Fill(pt);
//		h1JetE_East->Fill(E);
//		h2_RpPhi_vs_JetPhi_East->Fill(phi,RpTrackPhi);
//	    }
//
//	    if (RptrackPos == 1) //west side RP
//	    {
//		h1JetPhi_West->Fill(phi);
//		h1JetPt_West->Fill(pt);
//		h1JetE_West->Fill(E);
//		h2_RpPhi_vs_JetPhi_West->Fill(phi,RpTrackPhi);
//	    }
//
//	    h1JetE->Fill(E);
//	    if(eta > 2.0 && eta < 3.15) //large cells
//		h1JetE_l->Fill(E); 
//	    if(eta > 3.15 && eta < 4.0)  //small cells
//		h1JetE_s->Fill(E);
//	    
//	    if(eta > 2.5 && eta < 4.5)  //actual range 2.65 - 3.9
//	    {
//		h2Jetxy->Fill(jetX, jetY);
//		h2JetEP->Fill(eta, phi);
//	    }
//	    if(eta > 1.0 && eta < 2.0)   // actual range 1.086 - 2.0
//	    {
//		jetX = (kEEmcZSMD - z_vtx) * tan(theta) * cos(phi);
//		jetY = (kEEmcZSMD - z_vtx) * tan(theta) * sin(phi);
//		h2Jetxy_eemc->Fill(jetX, jetY);
//	    }
//
//	    //if(eta > 2.65 && eta < 4.5)
//		h1nPhotons->Fill(jetEvent->jet(i)->numberOfTowers());
//		//h1nPhotons->Fill(jetEvent->jet(i)->numberOfParticles());
//
//	    if(eta < 2.0  || eta > 4.5) //Consider FMS EM Jets only (for trigger specific plots)
//		continue;
//
//	    h2EvsPt->Fill(pt, E);
//	    h2PtvsE->Fill(E, pt);
//	}//end Jet loop



	PythiaTree->GetEntry(evt);
	bool GetPr,AddPrBeam, EMjet; 
	GetPr = false;
	EMjet = false;
	AddPrBeam = false;
	Double_t Pdiff=999.;
	Double_t sumE=0;
	Double_t eta=0;
	Double_t totalchargePartWest = 0;
	Double_t totalchargePartEast = 0;
	Double_t Part_Eta_best=999.;
	//cout << evt<< " Process ID:"<<genevent->processId() << endl;
	//cout << "Generater ID:"<<genevent->GetGeneratorId() << endl;
	//cout<<"Pythia Tree n particle:"<<genevent->GetNumberOfParticles()<<endl;
	  //if (check_hard_diff_event[evt] == 1) 
	  //{
	for (Int_t i=0; i < genevent->GetNumberOfParticles();++i)
	{

		StarGenParticle *genpart = genevent->operator[](i);
		//cout<<"No."<<i<<" particle Status:"<<genpart->GetStatus()<<" particle PID:"<<genpart->GetId()<<" daughter part:"<<genpart->GetFirstDaughter()<<endl;
		if ((!GetPr)&&(genpart->GetStatus() == 1) && (genpart->GetId() == 2212) && (genpart->GetFirstDaughter() == 0)) 
		{

			h1_PartPrEta->Fill(genpart->momentum().Eta());
			h1_PartPrPhi->Fill(genpart->momentum().Phi());
			//cout << "Particle level Pr P:"<<genpart->momentum().P() <<endl;
			//h1PartPrVtxz->Fill(genpart->GetVz());
//			if (nRpsTracks == 0) 
//			{
//				h1_PartNoRpPrEta->Fill(genpart->momentum().Eta());
//				h1_PartNoRpPrPhi->Fill(genpart->momentum().Phi());
//				h2_PartNoPrPhi_vs_Eta->Fill(genpart->momentum().Eta(),genpart->momentum().Phi());
//			}
	
			if (genpart->momentum().Eta() < 0)
			{
				h1PartPrPt_East->Fill(genpart->momentum().Pt());
				h1PartPrP_East->Fill(genpart->momentum().P());
//				if (RpEastTrkP >0) h2_RPPrP_vs_PartPrP_east->Fill(RpEastTrkP,genpart->momentum().P());
				//if ((RpEastTrkP >0)&&(fabs(genpart->momentum().Eta()-RP_Eta_best)<0.5)) {Pdiff= fabs(RpEastTrkP-genpart->momentum().P()); Part_Eta_best=genpart->momentum().Eta();}
			}
			else
			{
				h1PartPrPt_West->Fill(genpart->momentum().Pt());
				h1PartPrP_West->Fill(genpart->momentum().P());
				sumE = genpart->momentum().P();
				GetPr = true;
				continue;
//				if (RpWestTrkP >0) h2_RPPrP_vs_PartPrP_west->Fill(RpWestTrkP,genpart->momentum().P());
				//if ((RpWestTrkP >0)&&(fabs(genpart->momentum().Eta()-RP_Eta_best)<0.5)) {Pdiff= fabs(RpWestTrkP-genpart->momentum().P()); Part_Eta_best=genpart->momentum().Eta(); }

			}


		}
			//cout << i<<"	"<<genpart->GetStatus() <<"	"<< genpart->GetFirstDaughter()<<"	"<<genpart->momentum().P()<<endl;
//		if ((GetPr)&&(genpart->GetStatus() == 1) && (genpart->GetId() == 2212) && (genpart->GetFirstDaughter() == 0)) 
//		{
//
//			h1PartPrPt_other->Fill(genpart->momentum().Pt());
//			h1PartPrP_other->Fill(genpart->momentum().P());
//			h1_PartPrEta_other->Fill(genpart->momentum().Eta());
//			h1_PartPrPhi_other->Fill(genpart->momentum().Phi());
//			Int_t lastMother = genpart->GetLastMother();
//			StarGenParticle *genpartMother = genevent->operator[](lastMother);
//			cout<<"other decay proton last mother:"<<genpart->GetLastMother()<<" last mother part PID:"<<genpartMother->GetId()<<endl;	
//			switch (genpartMother->GetId())
//			{
//
//				case -3: h1PartPrMotherPID->Fill(2); break;
//				case -2: h1PartPrMotherPID->Fill(4); break;
//				case 0: {h1PartPrMotherPID->Fill(6); AddPrBeam = true;} break;
//				case 2101: h1PartPrMotherPID->Fill(8); break;
//				case 2103: h1PartPrMotherPID->Fill(10); break;
//				case 2203: h1PartPrMotherPID->Fill(12); break;
//				default: h1PartPrMotherPID->Fill(14); break;
//
//			}
//			if ((RpEastTrkP >0)&&(genpart->momentum().Eta() < 0) && (fabs(RpEastTrkP-genpart->momentum().P())<Pdiff)&&(fabs(genpart->momentum().Eta()-RP_Eta_best)<0.5)) {Pdiff= fabs(RpEastTrkP-genpart->momentum().P());Part_Eta_best=genpart->momentum().Eta();}
//			if ((RpWestTrkP >0)&&(genpart->momentum().Eta() > 0) && (fabs(RpWestTrkP-genpart->momentum().P())<Pdiff)&&(fabs(genpart->momentum().Eta()-RP_Eta_best)<0.5)) {Pdiff= fabs(RpWestTrkP-genpart->momentum().P());Part_Eta_best=genpart->momentum().Eta();}
//
//		}



	//}

	//RpWestTrkP=0;
	//RpEastTrkP=0;
	//if (AddPrBeam) {h1PartPrBeam->Fill(4);} else {h1PartPrBeam->Fill(2);}
	//if (Pdiff < 999) {h1RPandPartPrP_Diff->Fill(Pdiff);h2_RPPrEta_vs_PartPrEta_other->Fill(RP_Eta_best,Part_Eta_best);}	
	//if (Pdiff < 999) {h1PartPrMatch->Fill(4);} else {h1PartPrMatch->Fill(2);}

    } // End particle

    //}

    Double_t sumE_BBC=0;
    if ((check_hard_diff_event[evt] == 1)&&(GetPr))
    {

	totalchargePartWest = 0;
	totalchargePartEast = 0;
	for (Int_t i=0; i < genevent->GetNumberOfParticles();++i)
	{

		StarGenParticle *genpart = genevent->operator[](i);
		if ((genpart->GetStatus() > 0)&& ((genpart->GetFirstDaughter()) == 0))
		{

			eta = genpart->momentum().Eta();
			cout << eta <<endl;
			if ((eta > 3.5)&&(eta<4.5))
			{
				h1_BBC_E_diff->Fill(genpart->momentum().P());
				sumE_BBC = sumE_BBC + genpart->momentum().P();
				if (genpart->GetId() != 22) {++totalchargePartWest;}

			}
			if ((eta > -4.5)&&(eta<-3.5))
			{
				if (genpart->GetId() != 22) {++totalchargePartEast;}
			}

		//	if ((eta > 2.5)&&(eta<4.4)) 
		//	{
		//		
		//		sumE = sumE + genpart->momentum().P();
		//		EMjet = true;
		//	}

		}

	}  
	h1_sumBBC_E_diff->Fill(sumE_BBC);
	h1_nPart_E_diff_west->Fill(totalchargePartWest);
	h1_nPart_E_diff_east->Fill(totalchargePartEast);
  } //end selecting diffractive
    else
    if ((check_hard_diff_event[evt] == 0)&&(GetPr))
    {

	totalchargePartWest = 0;
	totalchargePartEast = 0;
	for (Int_t i=0; i < genevent->GetNumberOfParticles();++i)
	{

		StarGenParticle *genpart = genevent->operator[](i);

		if ((genpart->GetStatus() > 0)&& ((genpart->GetFirstDaughter()) == 0))
		{

			eta = genpart->momentum().Eta();
			cout << eta <<"		"<< genpart->GetId()<<endl;
			if ((eta > 3.5)&&(eta<4.5))
			{
				h1_BBC_E_nondiff->Fill(genpart->momentum().P());
				sumE_BBC = sumE_BBC + genpart->momentum().P();
				if (genpart->GetId() != 22) {++totalchargePartWest;}

			}
			if ((eta > -4.5)&&(eta<-3.5))
			{
				if (genpart->GetId() != 22) {++totalchargePartEast;}
			}
		//	if ((eta > 2.5)&&(eta<4.4)) 
		//	{
		//		
		//		sumE = sumE + genpart->momentum().P();
		//		EMjet = true;
		//	}

		}
	}	
	h1_sumBBC_E_nondiff->Fill(sumE_BBC);
	h1_nPart_E_nondiff_west->Fill(totalchargePartWest);
	h1_nPart_E_nondiff_east->Fill(totalchargePartEast);

    }



}
    outFile->Write();
    outFile->Close();
    //muDstMaker = 0;
    //mMuEvent = 0;
    //delete jetChain;
}
