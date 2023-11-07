// Filename: TStFmsSimTreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif


#include "TStFmsSimTreeMaker.h"
#include "StEvent/StEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuFmsCollection.h"
#include "StMuDSTMaker/COMMON/StMuFmsPoint.h"
#include "StSpinPool/StFmsTriggerMaker/StFmsTriggerMaker.h"
#include "StJetMaker/St_pythia_Maker.h"

ClassImp(TStFmsSimTreeMaker)

//_____________________________________________________________________________ 
TStFmsSimTreeMaker::TStFmsSimTreeMaker(const char *name):StMaker(name)
{
    //FMS Buffer
    mFmsPointArray = new TClonesArray("StMuFmsPoint");
    mNevents = 0;
    mPyEvt = 0;
    mPyChain = new TChain("PythiaTree");
    //Pythia event buffer
    mInPythiaEvent = new StPythiaEvent();
}

//_____________________________________________________________________________ 
TStFmsSimTreeMaker::~TStFmsSimTreeMaker()
{
    //
}


//_____________________________________________________________________________ 
Int_t TStFmsSimTreeMaker::Init()
{
    mFile = new TFile(mOutName, "RECREATE");
    mTree = new TTree("PythiaTree", "Sim Tree");
    mPyChain->Add(mPythiaFile);

    mPyChain->SetBranchAddress("PythiaBranch", &mInPythiaEvent);
    
    mTree->Branch("runId", &mRunId, "runId/I");
    mTree->Branch("eventId", &mEventId, "eventId/I");
    mTree->Branch("trigBit", &mTriggerBit, "trigBit/I");
    mTree->Branch("pythiaEvent", &mInPythiaEvent, 256000, 99);
    mTree->Branch("fmsPoint", &mFmsPointArray, 256000, 99);

    mMuDstMaker = (StMuDstMaker*)GetMaker("MuDst");
    mChain = mMuDstMaker->chain();
    mChain->SetBranchAddress("FmsPoint", &mFmsPointArray); //If you do setaddress, fmsPoint access in all other Maker will be lost (check) because of Reset?.
        
    return kStOK;
}
//_____________________________________________________________________________
void TStFmsSimTreeMaker::Reset()
{
    mInPythiaEvent->Clear();
    mFmsPointArray->Clear();    
}
    
//_____________________________________________________________________________
Int_t TStFmsSimTreeMaker::Make()
{
    mMuDst = (StMuDst*)GetInputDS("MuDst");
    mEvent = (StEvent*)GetInputDS("StEvent");
    //mPythiaMaker = (St_pythia_Maker*)GetMaker("St_pythia_Maker"); //This does not synchronize pythia tree if it has different number of events (happens when filter is used)
    
    // Check if mMuDst or mEvent is valid
    if(!mMuDst)
    {
	LOG_ERROR << "TStFmsSimMaker::Make - No MuDst found" <<endm;
	Reset();
	return kStFatal;
    }

    if(!mPythiaMaker)
    {
	LOG_ERROR << "TStFmsSimMaker::Make - No PythiaMaker found" <<endm;
	Reset();
	return kStFatal;
    }
    if(mNevents % 1000 == 0)
	cout << "Events Processed: " << mNevents <<endl;

    mPyChain->GetEntry(mPyEvt);
    
    mMuEvent = mMuDst->event();
    mRunId = mMuEvent->runNumber();
    mEventId = mMuEvent->eventId();

    //--------- Synchronize Pythia event with reconstructed events ---------
    while(mInPythiaEvent->eventId() != mEventId)
    {
    	cout << "Event id did not match, going to next Pythia event..." <<endl;
    	mPyChain->GetEntry(++mPyEvt);
    }
    cout << "Match Found !!!" <<endl;

    
    //---------------------- FMS Trigger Simulator ---------------------------------------
    StFmsTriggerMaker* fmstrig = (StFmsTriggerMaker*)GetMakerInheritsFrom("StFmsTriggerMaker");
    assert(fmstrig);

    Int_t trg1 = 0;                                                                                                           
    Int_t trg2 = 0; 
    Int_t trg3 = 0; 
    Int_t trg4 = 0; 
    Int_t trg5 = 0; 
    Int_t trg6 = 0;
    Int_t trg7 = 0; 
    Int_t trg8 = 0; 
    Int_t trg9 = 0;

    if( fmstrig->FmsSmallClusterTh0())  trg1 = 1;
    if( fmstrig->FmsSmallClusterTh1())  trg2 = 1;
    if( fmstrig->FmsSmallClusterTh2())  trg3 = 1;
    if( fmstrig->FmsLargeClusterTh0())  trg4 = 1;
    if( fmstrig->FmsLargeClusterTh1())  trg5 = 1;
    if( fmstrig->FmsLargeClusterTh2())  trg6 = 1;
    if( fmstrig->FmsJetPatchTh0())  trg7 = 1;
    if( fmstrig->FmsJetPatchTh1())  trg8 = 1;
    if( fmstrig->FmsJetPatchTh2())  trg9 = 1;
    fmstrig->Clear();

    mTriggerBit = 1000000000 + trg1*100000000 + trg2*10000000 + trg3*1000000 + trg4*100000 + trg5*10000 + trg6*1000 + trg7*100 + trg8*10 + trg9;

    cout<<" TRIGGER : SmBS 1 2 LgBS 1 2 JP 1 2   : "<<trg1<<"  "<<trg2<<"  "<<trg3<<"  "<<trg4<<"  "<<trg5<<"  "<<trg6<<"  "<<trg7<<"  "<<trg8<<"  "<<trg9<<"  "<<mTriggerBit<<endl;

    //---------- Keep only triggered events ----------
    if(mTriggerBit == 1000000000)
    {
	Reset();
	return kStOK;
    }
    //----------- Fill Pythia Event Information ----------------------------------
    // Will be stored from the branch directly
    
    //---------- Fill FMS Information -----------------
    //Will be stored from MuDst root file directly
    
    mTree->Fill();
    
    ++mNevents;

    Reset();
    return kStOK;
}

//_____________________________________________________________________________
Int_t TStFmsSimTreeMaker::Finish()
{
    //Write histograms to root file etc.

    mFile->Write();
    
    return kStOK;
}
