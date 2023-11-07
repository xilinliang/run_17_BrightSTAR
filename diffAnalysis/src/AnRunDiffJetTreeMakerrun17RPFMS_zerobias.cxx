// Filename: AnRunDiffJetTreeMakerrun17RPFMS_zerobias.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Tue Nov 24 00:32:49 2020 (-0500)
// URL: jlab.org/~latif

#include <iostream>
#include <fstream>
#include "RootInclude.h"
#include "StJetMaker/towers/StjTowerEnergyCutFMS.h"
#include "StJetMaker/StAnaPars.h"
#include "StRootInclude.h"

#include "StJetMaker/StJetMaker2015.h"
#include "StJetMaker/StEmVertexMaker.h"
#include "BrightStInclude.h"
#include "BrJetMaker/TStNanoDiffJetPi0TreeMaker.h"
#include "BrNanoDstMaker/TStRpFilterMaker_zerobias.h"

using namespace std;

void AnRunDiffJetTreeMakerrun17RPFMS_zerobias(TString inFile, TString outFile, TString det, TString stream, Bool_t isMC)
{
    TStopwatch sw;
    sw.Start();

    //TStar::gConfig->Print();
    //Bool_t isMC;
    //isMC = kFALSE;
    //isMC = kTRUE;

    //det = "eemc"; //<------------------ Only for cron job. Should be commented in all other cases
        
    if(!(det == "fms" || det == "eemc" || det == "rp"))
    {
	cout << "Invalid detector name" <<endl;
	return;
    }
    
    TString Jetfile = (TString)"jets_" + outFile;
    TString Uefile = (TString)"ueoc_" + outFile;
    TString Skimfile = (TString)"skim_" + outFile;

    //-------------------------------------------
    //if(!TStar::gBrDebug)
    //{
        gMessMgr->SetLimit("I", 0);
        gMessMgr->SetLimit("Q", 0);
        gMessMgr->SetLimit("W", 0);
    //}
    //-------------------------------------------

    StChain* chain = new StChain;
    StMuDstMaker* muDstMaker = new StMuDstMaker(0, 0, "", inFile, "", 1000);
    StMuDbReader* muDstDb = StMuDbReader::instance();
    
    Int_t runNumber;    
    if(!isMC)
	runNumber = TStRunList::GetRunFromFileName((string)inFile, stream);
    else
	runNumber = 16066000;
    cout<<"runnumber:"<<runNumber<<endl;
    if(runNumber < 1)
    {
    	cout << "Unable to get run number" <<endl;
    	return;
    }
    Int_t runPeriod = runNumber / 1000000 -1;
    cout<< "run Period:"<< runPeriod<<endl;
    if ((runPeriod!=15) && (runPeriod!=17)) return;

    double maxE;
    if (runPeriod == 15) {maxE = 200;}
    if (runPeriod == 17) {maxE = 510;}


//    const Int_t kMaxTriggers = 9;
//    Int_t trigIds[kMaxTriggers];
//    TString triggerNames_fms[kMaxTriggers] = {"FMS-JP0", "FMS-JP1", "FMS-JP2", "FMS-sm-bs1", "FMS-sm-bs2", "FMS-sm-bs3", "FMS-lg-bs1", "FMS-lg-bs2", "FMS-lg-bs3"};
//    TString triggerNames_eemc[kMaxTriggers] = {"EHT0", "JP1", "JP2", "EHT0*EJP1*L2Egamma", "JP2*L2JetHigh", "BHT1*VPDMB-30", "BHT0*BBCMB", "BHT1*BBCMB", "BHT2*BBCMB"};

    //Consider FMS or EEMC EM Jets only. Note, bulk of the jets come from mid-rapidity.
    //Unless this cut is applied, file size will be large and no need to make separate nano Dst.
    Double_t etaMax = 4.5;
    Double_t etaMin = 0.8;
    
    if((runPeriod == 15)||(det == "fms"))
    {
	TH1D* hEvtCount = new TH1D("hEvtCount", "Event counts", 20, 0, 20);
	TStRpFilterMaker_zerobias* filterMaker = new TStRpFilterMaker_zerobias("TStRpFilterMaker_zerobias"); // Filter/Skip Events if no RP or FMS BS/JP Trigger
	filterMaker->printTriggerId();
	filterMaker->SetHist1d(hEvtCount);
	filterMaker->SetRunPeriod(runPeriod);
	if(det == "fms")
	{
//	    for(Int_t i = 0; i < kMaxTriggers; ++i)
//	    {
//		trigIds[i] = TStTrigDef::GetTrigId(runNumber, triggerNames_fms[i]);
//		filterMaker->addTrigger(trigIds[i]);
//	    }
//	    filterMaker->addVetoTrigger(TStTrigDef::GetTrigId(runNumber,"FMS-LED"));
		//run15
		filterMaker->addTrigger(480801);//FMS-sm-bs1
		filterMaker->addTrigger(480802);//FMS-sm-bs2
		filterMaker->addTrigger(480803);//FMS-sm-bs3
		filterMaker->addTrigger(480804);//FMS-lg-bs1
		filterMaker->addTrigger(480805);//FMS-lg-bs2
		filterMaker->addTrigger(480806);//FMS-lg-bs3
		filterMaker->addTrigger(480808);//FMS-JP2
		filterMaker->addTrigger(480809);//FMS-JP1
		filterMaker->addTrigger(480810);//FMS-JP0
		filterMaker->addTrigger(480821);//FMS-sm-bs1
		filterMaker->addTrigger(480822);//FMS-sm-bs2
		filterMaker->addTrigger(480823);//FMS-sm-bs3
		filterMaker->addTrigger(480824);//FMS-lg-bs1
		filterMaker->addTrigger(480825);//FMS-lg-bs2
		filterMaker->addTrigger(480826);//FMS-lg-bs3
		filterMaker->addTrigger(480828);//FMS-JP2
		filterMaker->addTrigger(480829);//FMS-JP1
		filterMaker->addTrigger(480830);//FMS-JP0
		filterMaker->addTrigger(480841);//FMS-sm-bs1
		filterMaker->addTrigger(480843);//FMS-sm-bs3
		filterMaker->addTrigger(480844);//FMS-lg-bs1

		filterMaker->addVetoTrigger(480813);//FMS-LED
		filterMaker->addVetoTrigger(480833);//FMS-LED

		//run17
		filterMaker->addTrigger(570801);//FMS-sm-bs1
		filterMaker->addTrigger(570821);//FMS-sm-bs1
		filterMaker->addTrigger(570841);//FMS-sm-bs1
		filterMaker->addTrigger(570861);//FMS-sm-bs1
		filterMaker->addTrigger(42);//FMS-sm-bs1
		filterMaker->addTrigger(570802);//FMS-sm-bs2
		filterMaker->addTrigger(570822);//FMS-sm-bs2
		filterMaker->addTrigger(570842);//FMS-sm-bs2
		filterMaker->addTrigger(43);//FMS-sm-bs2
		filterMaker->addTrigger(570803);//FMS-sm-bs3
		filterMaker->addTrigger(570823);//FMS-sm-bs3
		filterMaker->addTrigger(570843);//FMS-sm-bs3
		filterMaker->addTrigger(44);//FMS-sm-bs3
		filterMaker->addTrigger(570804);//FMS-lg-bs1
		filterMaker->addTrigger(570824);//FMS-lg-bs1
		filterMaker->addTrigger(570844);//FMS-lg-bs1
		filterMaker->addTrigger(570864);//FMS-lg-bs1
		filterMaker->addTrigger(45);//FMS-lg-bs1
		filterMaker->addTrigger(570805);//FMS-lg-bs2
		filterMaker->addTrigger(570825);//FMS-lg-bs2
		filterMaker->addTrigger(570845);//FMS-lg-bs2
		filterMaker->addTrigger(46);//FMS-lg-bs2
		filterMaker->addTrigger(570806);//FMS-lg-bs3
		filterMaker->addTrigger(570826);//FMS-lg-bs3
		filterMaker->addTrigger(570846);//FMS-lg-bs3
		filterMaker->addTrigger(47);//FMS-lg-bs3
		filterMaker->addTrigger(570808);//FMS-JP2
		filterMaker->addTrigger(570828);//FMS-JP2
		filterMaker->addTrigger(570848);//FMS-JP2
		filterMaker->addTrigger(49);//FMS-JP2
		filterMaker->addTrigger(570809);//FMS-JP1
		filterMaker->addTrigger(570829);//FMS-JP1
		filterMaker->addTrigger(570849);//FMS-JP1
		filterMaker->addTrigger(50);//FMS-JP1
		filterMaker->addTrigger(570810);//FMS-JP0
		filterMaker->addTrigger(570830);//FMS-JP0
		filterMaker->addTrigger(570850);//FMS-JP0
		filterMaker->addTrigger(570870);//FMS-JP0
		filterMaker->addTrigger(51);//FMS-JP0

		filterMaker->addVetoTrigger(570813);//FMS-LED
		filterMaker->addVetoTrigger(570833);//FMS-LED
		filterMaker->addVetoTrigger(570853);//FMS-LED
		filterMaker->addVetoTrigger(53);//FMS-LED
		filterMaker->addVetoTrigger(54);//FMS-LED
		filterMaker->addVetoTrigger(7);//FMS-LED
		filterMaker->addVetoTrigger(8);//FMS-LED
		filterMaker->addVetoTrigger(2);//FMS-LED
		filterMaker->addVetoTrigger(10);//FMS-LED

	}
	else if(det == "eemc")
	{
//	    for(Int_t i = 0; i < kMaxTriggers; ++i)
//	    {
//		trigIds[i] = TStTrigDef::GetTrigId(runNumber, triggerNames_eemc[i]);
//		filterMaker->addTrigger(trigIds[i]);
//	    }
		filterMaker->addTrigger(480301);//EHT0
		filterMaker->addTrigger(480404);//JP1
		filterMaker->addTrigger(480414);//JP1
		filterMaker->addTrigger(480401);//JP2
		filterMaker->addTrigger(480411);//JP2
		filterMaker->addTrigger(480501);//EHT-*EJP1*L2Egamma
		filterMaker->addTrigger(480405);//JP2*L2JetHigh
		filterMaker->addTrigger(480415);//JP2*L2JetHigh
		filterMaker->addTrigger(480202);//BHT1*VPDMB-30
		filterMaker->addTrigger(480203);//BHT0*BBCMB
		filterMaker->addTrigger(480204);//BHT1*BBCMB
		filterMaker->addTrigger(480205);//BHT2*BBCMB

		//run 17
		filterMaker->addTrigger(570404);//JP0*VPDMB30
		filterMaker->addTrigger(570403);//JP1*VPDMB30
		filterMaker->addTrigger(570411);//JP2
		filterMaker->addTrigger(570402);//JP2*L2JetHigh
		filterMaker->addTrigger(570214);//BHT1*VPD30
		filterMaker->addTrigger(570215);//BHT2*BBCMB
		filterMaker->addTrigger(31);//BHT2*BBCMB*L2Bgamma
		filterMaker->addTrigger(570201);//BHT3
		filterMaker->addTrigger(35);//EHT0*BBCMB*L2Egamma
		filterMaker->addTrigger(34);//EHT0*BBCMB
		filterMaker->addTrigger(570201);//EHT1
	}
    }        
    cout<<"Here is OK!"<<endl;
    St_db_Maker* starDb = new St_db_Maker("StarDb","MySQL:StarDb");
    starDb->SetAttr("blacklist", "ist");
    starDb->SetAttr("blacklist", "mtd");
    starDb->SetAttr("blacklist", "pmd");
    //starDb->SetAttr("blacklist", "pp2pp");
    starDb->SetAttr("blacklist", "pxl");
    starDb->SetAttr("blacklist", "ssd");
    starDb->SetAttr("blacklist", "svt");
    starDb->SetDebug();

    StEEmcDbMaker* eemcDb = new StEEmcDbMaker;
    StSpinDbMaker* spinDb = new StSpinDbMaker;
    StFmsDbMaker*  fmsDb  = new StFmsDbMaker("fmsDb"); 
    if (runPeriod == 17)
    {
        	
	fmsDb->readGainCorrFromText(false);

    }
    if (runPeriod == 15)
    {	 
    	fmsDb->SetAttr("fmsGainCorr","fmsGainCorr-BNL-C");
    	Bool_t isHotCh[4][571] = {0};
    	Int_t nHotCh = 0;
    	TStFmsHotChDB *fmsHotChDb = new TStFmsHotChDB();
    	fmsHotChDb->GetHotChList(runNumber, isHotCh);
    	cout << "The following FMS cells are masked:" <<endl;
    	for(int i = 0; i < 4; ++i)
    	{
    	    for(int j = 0; j < 571; ++j)
    	    {
    	        if(isHotCh[i][j])
    	        {
    	    	cout << "det "<< (i + 1)<< " ch "<< (j+1) << " hot/bad status:"<< isHotCh[i][j] <<endl;
    	    	++nHotCh;
    	        }
    	    }
    	}
    	cout << "Total manually masked bad / hot channels: "<< nHotCh <<endl;

    	fmsDb->maskChannels(isHotCh);
    }
    
    StEmcADCtoEMaker* adc = new StEmcADCtoEMaker;
    adc->saveAllStEvent(true);
    StFmsHitMaker*   fmshitMk = new StFmsHitMaker();
    StFmsPointMaker* fmsptMk  = new StFmsPointMaker();
	//fmshitMk->SetReadMuDst(0);
	//fmsptMk->SetReadMuDst(0);


//    if(isMC)
//	fmshitMk->SetReadMuDst(1);                //for simu set to 1
    //-------------------------------------------
    StEmVertexMaker *emVertexMkr = new StEmVertexMaker("StEmVertexMaker");
    TString bbcSlewingData = (TString) getenv("STARHOME") + "/database/bbc_slewing_run15_pp200.dat"; 
    emVertexMkr->ReadBbcSlewing(bbcSlewingData.Data()); //CKim

    StJetSkimEventMaker* skimEventMaker = new StJetSkimEventMaker("StJetSkimEventMaker", muDstMaker, Skimfile);

    StJetMaker2015* jetmaker = new StJetMaker2015("StJetMaker2015");
    jetmaker->setJetFile(Jetfile);
    jetmaker->setJetFileUe(Uefile);

    StAnaPars* anapars12 = new StAnaPars;
    anapars12->useTpc  = true;
    anapars12->useBemc = false;
    //if (det=="fms" || det == "rp")
    if (det=="fms")
    {
    	anapars12->useEemc = false;
    	anapars12->useFms  = true;
    }
    if (det=="eemc" || det == "rp")
    //if (det=="eemc")
    {
    	anapars12->useEemc = true;
    	anapars12->useFms  = false;
    }
    //anapars12->useFmsHit = true; //CKim
    anapars12->addTpcCut(new StjTrackCutFlag(0));
    anapars12->addTpcCut(new StjTrackCutNHits(12));
    anapars12->addTpcCut(new StjTrackCutPossibleHitRatio(0.51));
    anapars12->addTpcCut(new StjTrackCutDca(3));
    anapars12->addTpcCut(new StjTrackCutPt(0.2,200));
    anapars12->addTpcCut(new StjTrackCutEta(-2.5,2.5));
    anapars12->addTpcCut(new StjTrackCutLastPoint(125));

    //BEMC cuts
    anapars12->addBemcCut(new StjTowerEnergyCutBemcStatus(1));
    anapars12->addBemcCut(new StjTowerEnergyCutAdc(4,3)); //ADC-ped>4 AND ADC-ped>3*RMS
    anapars12->addBemcCut(new StjTowerEnergyCutEt(0.2));

    //EEMC cuts
    anapars12->addEemcCut(new StjTowerEnergyCutBemcStatus(1));
    anapars12->addEemcCut(new StjTowerEnergyCutAdc(4,3)); //ADC-ped>4 AND ADC-ped>3*RMS
    anapars12->addEemcCut(new StjTowerEnergyCutEt(0.2));

    //FMS cuts, CKim
    //anapars12->addFmsCut(new StjTowerEnergyCutFMS(0.2, 200)); //min, max //Latif: changed to 0.2, it was set to 3 by Chong
    anapars12->addFmsCut(new StjTowerEnergyCutFMS(2.0, 200)); //!!!!!!!!!! Updated based on Simulation !!!!!!
    //* 3 GeV cut was determined by RUN15 calibration condition: Zgg < 0.7 + pairE > 20 GeV

    //Jet cuts
    anapars12->addJetCut(new StProtoJetCutPt(0.01,200));
    anapars12->addJetCut(new StProtoJetCutEta(-1,5)); //CKim, extend to FMS acceptance

    //Set anti-kt R=0.7 parameters
    StFastJetPars* AntiKtR070Pars = new StFastJetPars;

    //UE from Zilong, code from EjRunEmJetTreeMaker
     StFastJetAreaPars *JetAreaPars = new StFastJetAreaPars;
    JetAreaPars->setGhostMaxRap(5.0);	//Needed to extend to forward rapidity
    JetAreaPars->setGhostArea(0.04);    //0.04 was set for mid-rapidity. Find an optimal value for FMS / EEMC cell size
	

    AntiKtR070Pars->setJetAlgorithm(StFastJetPars::antikt_algorithm);
    AntiKtR070Pars->setRparam(0.7);
    AntiKtR070Pars->setRecombinationScheme(StFastJetPars::E_scheme);
    AntiKtR070Pars->setStrategy(StFastJetPars::Best);
    AntiKtR070Pars->setPtMin(1);

    jetmaker->addBranch("AntiKtR070NHits12",anapars12,AntiKtR070Pars);
    StOffAxisConesPars *off070 = new StOffAxisConesPars(0.7);
    jetmaker->addUeBranch("OffAxisConesR070", off070);

    

    TStNanoDiffJetPi0TreeMaker *nanoMaker = new TStNanoDiffJetPi0TreeMaker("NanoDiffJetTreeMaker");
    nanoMaker->SetOutFileName((TString)"NanoDiffJetTree_" + outFile);
    nanoMaker->SetEtaMax(etaMax);
    nanoMaker->SetEtaMin(etaMin);
    nanoMaker->SetRunPeriod(runPeriod);
    //nanoMaker->SetThisMode("0EMjets"); 
    nanoMaker->SetThisMode("4EMjetsRP"); 
    //nanoMaker->SetThisMode("5EMjetsPi0RP"); 

    chain->Init();
    chain->EventLoop();
    chain->Finish();
    delete chain;

    sw.Stop();
    sw.Print();

    cout << "-----------> Deleting Original jet finder files !!! <--------------------" <<endl;
    gROOT->ProcessLine(".! rm jets_*.root ueoc_*root skim_*.root");
    
    std::cout <<"Done!" <<endl;
    
    return;
}//Main

