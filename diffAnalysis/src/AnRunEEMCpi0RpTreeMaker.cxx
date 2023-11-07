// Filename: AnRunEEMCpi0RpTreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Nov 13 17:25:43 2019 (-0500)
// URL: jlab.org/~latif

#include <fstream>
#include "StRootInclude.h"
#include "RootInclude.h"
#include "BrightStInclude.h"
#include "BrNanoDstMaker/TStRpFilterMaker.h"
#include "BrJetMaker/TStNanoDiffJetPi0TreeMaker.h"

#include <vector>
#include <iostream>
using namespace std;

void AnRunEEMCpi0RpTreeMaker(TString fileList, TString outFile, Bool_t showMsg,  Int_t maxEvents)
{
    TH1D* hEvtCount = new TH1D("hEvtCount", "Event counts", 20, 0, 20);
    TStopwatch sw;
    sw.Start();

    if(!showMsg)
    {
	gMessMgr->SetLimit("I", 0);   //Disable StInfo messages including Skipped event message
	gMessMgr->SetLimit("Q", 0);   //Disable StQAInfo messages (includes event processing status)
    }
    Int_t runNumber;    
	runNumber = TStRunList::GetRunFromFileName((string)fileList, "eemc");
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

    
    StChain *chain		= new StChain;
    StMuDstMaker *muDstMaker	= new StMuDstMaker(0, 0, "", fileList, "", 1000);
    St_db_Maker *starDb		= new St_db_Maker("db","$HOME/StarDb","MySQL:StarDb","$STAR/StarDb");

    //-------- Keep these commented out for Cold QCD Nano Dst files -----------
    // starDb->SetAttr("blacklist", "emc");
    // starDb->SetAttr("blacklist", "eemc");
    //starDb->SetAttr("blacklist", "pp2pp");
    // starDb->SetAttr("blacklist", "tof");
    // starDb->SetAttr("blacklist", "tpc");
    
    starDb->SetAttr("blacklist", "ftpc");
    starDb->SetAttr("blacklist", "ist");
    starDb->SetAttr("blacklist", "mtd");
    starDb->SetAttr("blacklist", "pmd");
    starDb->SetAttr("blacklist", "pxl");
    starDb->SetAttr("blacklist", "ssd");
    starDb->SetAttr("blacklist", "svt");
    starDb->SetAttr("blacklist", "fgt");
    
    StSpinDbMaker* spindb	= new StSpinDbMaker("spinDb");
    
    //======================================================== Trigger Filter ==============================================
	TStRpFilterMaker* filterMaker = new TStRpFilterMaker("TStRpFilterMaker"); // Filter/Skip Events if no RP or FMS BS/JP Trigger
	filterMaker->printTriggerId();
	filterMaker->SetHist1d(hEvtCount);
	filterMaker->SetRunPeriod(runPeriod);

    //run15
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

    Int_t nEvents = muDstMaker->chain()->GetEntries();
    //====================================================== EEMC =============================================
	cout << "\033[1;31m=============== It only keeps events for which there is a valid EEMC event/response ====================================\033[0m"<<endl;
	Bool_t isMC = kFALSE;
	StEEmcDbMaker       *eemcDbMaker;
	StEEmcA2EMaker      *a2EMakerPtr;
	StEEmcEnergyMaker_t *energyMakerPtr;
	StEEmcTreeMaker_t   *treeMakerPtr;
	StSpinInfoMaker_t   *spinInfoMakerPtr;
	// just to make sure StEvent is found for the trig simu
	StMuDst2StEventMaker *muDst2StEvent = new StMuDst2StEventMaker();

	// Initialize EEMC database
	eemcDbMaker = new StEEmcDbMaker("eemcDb");
	
	//~~ PART 1 ~~~
	// Energy to ADC maker
	a2EMakerPtr = new StEEmcA2EMaker("EEmcA2EMaker");
	a2EMakerPtr->database("eemcDb");          // sets db connection
	a2EMakerPtr->source("MuDst",1);           // sets mudst as input
	a2EMakerPtr->threshold(3.0,0);            // tower threshold
	a2EMakerPtr->threshold(3.0,1);            // pre1 threshold 
	a2EMakerPtr->threshold(3.0,2);            // pre2 threshold
	a2EMakerPtr->threshold(3.0,3);            // post threshold
	a2EMakerPtr->threshold(3.0,4);            // smdu threshold
	a2EMakerPtr->threshold(3.0,5);            // smdv threshold

	//
	// Now start things particular to the StEEmcTreeMaker
	//

	//
	// create spin info maker
	spinInfoMakerPtr = new StSpinInfoMaker_t( "SpinInfoMaker" );

	// Energy Structure Maker
	energyMakerPtr = new StEEmcEnergyMaker_t( "energyMkr", "EEmcA2EMaker" );
	energyMakerPtr->setStripThres( isMC ? 0.0 : 0.001 );
	energyMakerPtr->setTowerThres( isMC ? 0.0 : 1.0 );

	// I'm told one must use slow simulator to get pedestals correct for L2,
	// but I'll put it after the A2Emaker and Energy maker, so it doesn't mess them up

	//
	// Tree Maker
	//
	TString outputFileName = (TString)"eemcTreeP1_" + outFile;
	treeMakerPtr = new StEEmcTreeMaker_t( "EEmcTreeMaker" );
	treeMakerPtr->setTreeStatus( StEEmcTreeMaker_t::PART_1, StEEmcTreeMaker_t::WRITE,  outputFileName );
	treeMakerPtr->setTreeStatus( StEEmcTreeMaker_t::PART_2, StEEmcTreeMaker_t::IGNORE, "" );
	treeMakerPtr->setTreeStatus( StEEmcTreeMaker_t::PART_3, StEEmcTreeMaker_t::IGNORE, "" );
	treeMakerPtr->setMaxNumEvents( nEvents );
	treeMakerPtr->setEEmcEnergyMkr( energyMakerPtr );
	treeMakerPtr->doSpinInfoIO( !isMC );
	treeMakerPtr->doEvtHddrIO( 1 );
	treeMakerPtr->setHTTPthres( 0, 0 );
 
	if( !isMC )
	    treeMakerPtr->setSpinInfoMkr( spinInfoMakerPtr );

	//~~~ PART 2 and 3 ~~
	TString str1 = (TString)"eemcTreeP2_" + outFile;
	TString str2 = (TString)"eemcTreeP3_" + outFile;
	const Char_t *eemcTreePart2FileName = str1.Data();
	const Char_t *eemcTreePart3FileName = str2.Data();

	std::cout << "***** Instanciating all the classes *****" << endl;

	//
	// some variables that tend to be made global
	//
	StEEmcTowerClusterFinder_t              *towerClusterFinderPtr = 0;
	StEEmcStripClusterFinderTSIU_t          *stripClusterFinderPtr = 0;
	StEEmcPointFinderIU_t                   *pointFinderPtr        = 0;
	StEEmcEnergyApportionerIU_t             *energyApportionerPtr  = 0;
	StEEmcHitMakerSimple_t                  *hitMakerPtr           = 0;
	StEEmcTreeMaker_t                       *treeReaderPtr         = 0;
	StEEmcTreeMaker_t                       *treeWriterPtr         = 0;
   

	//
	// TREE MAKER FOR READING
	//
	treeReaderPtr = new StEEmcTreeMaker_t( "EEmcTreeReader" );
	//treeReaderPtr->setTreeStatus( StEEmcTreeMaker_t::PART_1, StEEmcTreeMaker_t::READ,   eemcTreePart1FileName );
	treeReaderPtr->setTreeStatus( StEEmcTreeMaker_t::PART_1, StEEmcTreeMaker_t::IGNORE, "" );
	treeReaderPtr->setTreeStatus( StEEmcTreeMaker_t::PART_2, StEEmcTreeMaker_t::IGNORE, "" );
	treeReaderPtr->setTreeStatus( StEEmcTreeMaker_t::PART_3, StEEmcTreeMaker_t::IGNORE, "" );
	treeReaderPtr->doSpinInfoIO( !isMC );
	treeReaderPtr->doEvtHddrIO( 1 );
	treeReaderPtr->setMaxNumEvents( muDstMaker->chain()->GetEntries() );
	treeReaderPtr->setEEmcTreeReader( treeMakerPtr );

	//
	// CREATE ALL THE FINDERS AND THE HIT MAKER
	//

	// tower cluster finder
	//    towerClusterFinderPtr = new StEEmcTowerClusterFinderMinesweeper_t();
	//    towerClusterFinderPtr->setSeedEnergyThreshold( 2.0 );

	// strip cluster finder
	stripClusterFinderPtr = new StEEmcStripClusterFinderTSIU_t();

	// parameter set d

	stripClusterFinderPtr->setNumSmoothIters( 10 );
	stripClusterFinderPtr->setNumStripsPerSide( 3 );
	stripClusterFinderPtr->setMinStripsPerCluster( 5 );
	stripClusterFinderPtr->setSeedAbsThres( 0.002 );
	stripClusterFinderPtr->setSeedRelThres( 0.0 );
	stripClusterFinderPtr->setMinEnergyPerCluster( 0.003 );

	// point finder
	pointFinderPtr = new StEEmcPointFinderIU_t();

	// energy apportioner
	energyApportionerPtr = new StEEmcEnergyApportionerIU_t();
	energyApportionerPtr->setCheckTowerBits(0);

	// Hit maker
	hitMakerPtr = new StEEmcHitMakerSimple_t ( "hitMaker",
						   "EEmcTreeReader",
						   towerClusterFinderPtr,
						   stripClusterFinderPtr,
						   pointFinderPtr,
						   energyApportionerPtr
	    );
	hitMakerPtr->doClusterTowers( 0 );
	hitMakerPtr->doClusterPreShower1( 0 );
	hitMakerPtr->doClusterPreShower2( 0 );
	hitMakerPtr->doClusterPostShower( 0 );
	hitMakerPtr->doClusterSMDStrips( 1 );

	//
	// Extra things if MC
	//

	// Associate hits with tracks, if it is MC data
	//    if( isMC )
	//       mcHitMakerPtr = new StMcEEmcHitMakerStrips_t( "mcHitMaker", "responseTreeReader", "hitMaker" );

	//
	// TREE MAKER FOR WRITING
	//
	treeWriterPtr = new StEEmcTreeMaker_t( "EEmcTreeWriter" );
	treeWriterPtr->setTreeStatus( StEEmcTreeMaker_t::PART_1, StEEmcTreeMaker_t::IGNORE, "" );
	treeWriterPtr->setTreeStatus( StEEmcTreeMaker_t::PART_2, StEEmcTreeMaker_t::WRITE,  eemcTreePart2FileName );
	treeWriterPtr->setTreeStatus( StEEmcTreeMaker_t::PART_3, StEEmcTreeMaker_t::WRITE,  eemcTreePart3FileName );
	treeWriterPtr->doSpinInfoIO( 0 );
	treeWriterPtr->doEvtHddrIO( 0 );
	treeWriterPtr->doMakePairs( 1 );
	treeWriterPtr->setEEmcTreeReader( treeReaderPtr );
	treeWriterPtr->setEEmcHitMkr( hitMakerPtr );	
    //======================================================== Nano DST Maker ==============================================
    TStNanoDiffJetPi0TreeMaker *nanoMaker = new TStNanoDiffJetPi0TreeMaker("NanoDiffJetTreeMaker");
    Double_t etaMax = 4.5;
    Double_t etaMin = 0.8;
    nanoMaker->SetOutFileName((TString)"NanoDiffpi0Tree_" + outFile);
    nanoMaker->SetEtaMax(etaMax);
    nanoMaker->SetEtaMin(etaMin);
    nanoMaker->SetRunPeriod(15);
    nanoMaker->SetThisMode("2EEMCpi0"); 
	
    cout << "----------->Total Events to be processed: "<< nEvents <<" <----------------"<<endl;
    
    chain->Init();
    if(maxEvents != -1)
	chain->EventLoop(maxEvents);  // Run specified number of events
    else
	chain->EventLoop(nEvents);  // Run all events
    chain->Finish();
    delete chain;

    cout << "Status:SUCCESS!!" <<endl;

    sw.Stop();
    sw.Print();
    gROOT->ProcessLine(".! rm eemcTreeP1*.root eemcTreeP2*.root eemcTreeP3*.root");

    return;
}
