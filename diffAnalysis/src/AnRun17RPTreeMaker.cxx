// Filename: AnRun17RPTreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Nov 13 17:25:43 2019 (-0500)
// URL: jlab.org/~latif

#include <iostream>
#include <fstream>
#include "StRootInclude.h"
#include "RootInclude.h"
#include "BrightStInclude.h"
#include "BrNanoDstMaker/TStRpFilterMaker.h"
#include "BrJetMaker/TStNanoDiffJetPi0TreeMaker.h"

using namespace std;

void AnRun17RPTreeMaker(TString fileList, TString outFile)
{
    //TStRunList::MakeFileList(16072057, 1);
    //fileList = TStar::Config->GetFileList();
    //fileList = "/star/u/kabir/pwg/data/st_fms_16066050_raw_5000002.MuDst.root";

    //TStopwatch sw;
    //sw.Start();

    
    //------- For FMS stream ----------------
    string fileName;
    Double_t etaMax = 4.5;
    Double_t etaMin = 0.8;
//    if(fileList.Contains(".list"))
//    {
//	ifstream inFile(fileList);
//	if(!inFile)
//	{
//	    cout << "Unable to read run number from file list" <<endl;
//	    return;
//	}
//	getline(inFile, fileName);
//	inFile.close();
//	cout << "\n------->Warning: Setting trigger ID based on first run number only: "<< fileName <<"<-----\n"<<endl;
//    }
//    else
	fileName = fileList;
    
    Int_t runNumber;    
	runNumber = TStRunList::GetRunFromFileName((string)fileList, "rp");
    cout<<"runnumber:"<<runNumber<<endl;
    if(runNumber < 1)
    {
    	cout << "Unable to get run number" <<endl;
    	return;
    }
    Int_t runPeriod = runNumber / 1000000 -1;
    cout<< "run Period:"<< runPeriod<<endl;
    if (runPeriod!=17) return;
    double maxE;
    if (runPeriod == 17) {maxE = 510;}

     gMessMgr->SetLimit("I", 0);   //Disable StInfo messages including Skipped event message
     gMessMgr->SetLimit("Q", 0);   //Disable StQAInfo messages (includes event processing status)
    
    StChain *chain = new StChain;
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", fileList, "", 200);
    TChain *ch = muDstMaker->chain();
    //St_db_Maker* starDb = new St_db_Maker("db", "MySQL:StarDb", "$STAR/StarDb");
    ////StSpinDbMaker* spindb = new StSpinDbMaker("spinDb");
    ////StFmsDbMaker* fmsDb = new StFmsDbMaker("fmsDb");
    //starDb->SetAttr("blacklist", "ist");
    //starDb->SetAttr("blacklist", "mtd");
    //starDb->SetAttr("blacklist", "pmd");
    ////starDb->SetAttr("blacklist", "pp2pp");
    //starDb->SetAttr("blacklist", "pxl");
    //starDb->SetAttr("blacklist", "ssd");
    //starDb->SetAttr("blacklist", "svt");
    //starDb->SetDebug();

    //Enable new FMS calibration
    //fmsDb->SetAttr("fmsGainCorr","fmsGainCorr-BNL-C");
    
    //muDstMaker->SetStatus("*", 0);
    ////muDstMaker->SetStatus("Fms*", 1);
    //muDstMaker->SetStatus("pp2pp*", 1);
    //muDstMaker->SetStatus("MuEvent*", 1);

    //Int_t nEvents = muDstMaker->chain()->GetEntries();
    //cout<<"nEvents:"<<nEvents << endl;

//    //-------------- Filter/Skip Events if no RP or FMS BS/JP Trigger----------
//    TH1D* hEvtCount = new TH1D("hEvtCount", "Event counts", 20, 0, 20);
//    TStRpFilterMaker* filterMaker = new TStRpFilterMaker("TStRpFilterMaker");
//    filterMaker->SetHist1d(hEvtCount);
//        filterMaker->SetRunPeriod(runPeriod);
//    //----------------
//   
//    filterMaker->addTrigger(570711);//RP_CPT2
//    filterMaker->addTrigger(570701);//RP_CPT2
//    filterMaker->addTrigger(480803);//FMS-sm-bs3
//    filterMaker->addTrigger(480804);//FMS-lg-bs1
//    filterMaker->addTrigger(480805);//FMS-lg-bs2
//    filterMaker->addTrigger(480806);//FMS-lg-bs3
//    filterMaker->addTrigger(480808);//FMS-JP2
//    filterMaker->addTrigger(480809);//FMS-JP1
//    filterMaker->addTrigger(480810);//FMS-JP0
//    filterMaker->addTrigger(480821);//FMS-sm-bs1
//    filterMaker->addTrigger(480822);//FMS-sm-bs2
//    filterMaker->addTrigger(480823);//FMS-sm-bs3
//    filterMaker->addTrigger(480824);//FMS-lg-bs1
//    filterMaker->addTrigger(480825);//FMS-lg-bs2
//    filterMaker->addTrigger(480826);//FMS-lg-bs3
//    filterMaker->addTrigger(480828);//FMS-JP2
//    filterMaker->addTrigger(480829);//FMS-JP1
//    filterMaker->addTrigger(480830);//FMS-JP0
//    filterMaker->addTrigger(480841);//FMS-sm-bs1
//    filterMaker->addTrigger(480843);//FMS-sm-bs3
//    filterMaker->addTrigger(480844);//FMS-lg-bs1
//
//    filterMaker->addVetoTrigger(480813);//FMS-LED
//    filterMaker->addVetoTrigger(480833);//FMS-LED


    cout<<"filter"<<endl;
    //StEventMaker* eventMk = new StEventMaker();
//    StFmsHitMaker*   fmsHitMk   = new StFmsHitMaker();
//    StFmsPointMaker* fmsPointMk = new StFmsPointMaker();
//    // FPS
//    StFmsFpsMaker * fpsMk = new StFmsFpsMaker();
//    
//    // mudst reading
//    // if 0, get info from StTriggerData from StTriggerDataMaker/StEvent/MuDst
//    // and apply new DB; cluster finding/fitting is redone
//    // if 1, cluster finding/fitting is NOT redone (i.e., mudst result is read
//    // out); however, some recalculations (like point positions) from updated DB values
//    // are done
//    fmsPointMk->SetReadMuDst(0); // (0)
//    fmsHitMk->SetReadMuDst(0);   // (0)
//    fpsMk->setReadMuDST(1);      // (1) // (reverse 0<->1 convention as FmsHitMaker and FmsPointMaker)
//
//    // shower shape
//    // 0='use lednev params', 1='use zhanwen params', 2='use yuxi params'
//    // fmsPointMk->setShowerShapeWithAngle(1); // (1)
//  
//    // cluster merging over large/small boundary
//    // if 1, merges large and small cell clusters (experimental!; default 1 in StFmsPointMaker)
//    fmsPointMk->setMergeSmallToLarge(1); // (1)
//
//    // vertex correction
//    // if 0, no vertex correction; if 1, use MuDst BBC vertex based on run11 calibration (needs update?)
//    fmsPointMk->setVertexZ(1); // (1)

    //Int_t nEvents = 1000;
    //cout << "----------->Total Events to be processed: "<< nEvents <<" <----------------"<<endl;
   
    TStNanoDiffJetPi0TreeMaker *nanoMaker = new TStNanoDiffJetPi0TreeMaker("NanoDiffJetPi0TreeMaker");
    nanoMaker->SetOutFileName((TString)"NanoDiffRPTree_" + outFile);
    nanoMaker->SetEtaMax(etaMax);
    nanoMaker->SetEtaMin(etaMin);
    nanoMaker->SetRunPeriod(runPeriod);
    nanoMaker->SetThisMode("3RP");


    chain->Init();    
    chain->EventLoop();
    //chain->EventLoop(nEvents);  // Run specified number of events
    chain->Finish();

    //sw.Stop();
    //sw.Print();
    cout << "Status:SUCCESS!!" <<endl;

    delete chain;
}
