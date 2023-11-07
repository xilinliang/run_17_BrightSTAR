// Filename: TStNanoDiffJetPi0TreeMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:38:04 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTNANODIFFJETPI0TREEMAKER_H
#define TSTNANODIFFJETPI0TREEMAKER_H

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "StMaker.h"

#include "StFmsDbMaker/StFmsDbMaker.h"
#include "StMuDSTMaker/COMMON/StMuRpsCollection.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrack.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrackPoint.h"
#include "StMuDSTMaker/COMMON/StMuTriggerIdCollection.h"
#include "BrContainers/TStRpsTrackData.h"
#include "StEEmcUtil/EEmcGeom/EEmcGeomDefs.h"
#include "StEEmcPool/StEEmcTreeMaker/StEEmcTreeMaker.h"
#include "StEEmcPool/EEmcTreeContainers/EEmcParticleCandidate.h"
#include "StEEmcPool/EEmcTreeContainers/EEmc2ParticleCandidate.h"
#include "StJetMaker/mudst/StMuEmcPosition.h"
#include "StEmcUtil/geometry/StEmcGeom.h"

class StEvent;
class StMuDst;
class StMuEvent;
class StFmsPointPair;
class StFmsCollection;
class StSpinDbMaker;
class StJetMaker2015;
class StJetEvent;
class StJetTower;
class StJetParticle;
class StJetVertex;
class StJetSkimEvent;
class StJetSkimTrig;
class StJetSkimEventMaker;

class TStJetEvent;
class TStFmsPointPairData;
class TStFmsPointData;
class TStJetSkimEvent;
class TStJetCandidate;
class TStJetTower;
class TStJetParticle;
using namespace std;

class TStNanoDiffJetPi0TreeMaker : public StMaker
{
private:
    StMuDst *mMuDst;  
    StEvent* mEvent;
    StMuEvent *mMuEvent;
    Int_t mEventCount;
    
    StJetMaker2015 *mJetMaker;
    StJetSkimEventMaker *mSkimEventMaker;
    StJetEvent* mInJetEvent;
    StJetSkimEvent* mInSkimEvent;
    StJetParticle *mInParticle;
    StJetTower *mInTower;
    StJetVertex *mInVertex;
    
    TFile *mOutFile;
    TTree *mTree;
    TString mOutName = "NanoJetTree.root";
    TStJetEvent *mOutJetEvent;
    TStFmsPointPairData *mOutPointPair;
    TStFmsPointData *mOutPoint;
    TStJetSkimEvent *mOutSkimEvent;    
    TStJetCandidate *mOutJet;
    TStJetParticle *mOutParticle;
    TStJetTower *mOutTower;

    StFmsDbMaker *mFmsDbMk;
    StSpinDbMaker *mSpinDbMaker;
    StFmsCollection *mFmsColl;
    StFmsPointPair *mPair;
    vector<StFmsPointPair*> mPointPairs;

    StEEmcTreeMaker_t *mEEmcTreePart1;
    StEEmcTreeMaker_t *mEEmcTreePart3;
    EEmcParticleCandidate_t *mInPoint;
    EEmc2ParticleCandidate_t *mInPion;

    //ofstream outtextSmall;
    //ofstream outtextLarge;

    static const Int_t mMaxTriggers = 9;
    StJetSkimTrig* mJetTrig[mMaxTriggers];
    Int_t mTrigIds[mMaxTriggers];
    Bool_t mTrigFlag;
    Double_t mEtaMax;
    Double_t mEtaMin;
    Double_t mZdist;
    Double_t vtxZ;

    Int_t mAdcSum[2];

    Int_t mRunPeriod;
    string mMode;

    //--- RP ---
    StMuRpsCollection *mRpsMuColl;
    //StMuRpsCollection2 *mRpsMuColl; //Use after burner
    //StMuRpsUtil* mAfterburner;
    StMuRpsTrack *mRpsTrk;
    Double_t mBeamMom;
    
    Int_t mRpNtracks;
    TClonesArray  *mRpsArray;
    TStRpsTrackData *mRpsTrackData;

    
public: 
    TStNanoDiffJetPi0TreeMaker(const char *name  = "NanoJetTreeMaker");
    virtual ~TStNanoDiffJetPi0TreeMaker();
    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();
    void SetOutFileName(TString fileName){mOutName = fileName;}
    // virtual Int_t InitRun  (int runumber){return 0;}; // Overload empty StMaker::InitRun 
    // virtual Int_t FinishRun(int runumber){return 0;}; // Overload empty StMaker::FinishRun 
    void SetEtaMax(Double_t etaMax){mEtaMax = etaMax;}
    void SetEtaMin(Double_t etaMin){mEtaMin = etaMin;}
    void SetDetZdist(Double_t z){mZdist = z;}
    void SetRunPeriod(Int_t runPeriod){mRunPeriod = runPeriod;}
    void SetThisMode(string mode){mMode = mode;}

    //void SetTrigIds(Int_t *trigIds);
    Int_t MakeRps();
    Int_t MakeEMjets();
    Int_t MakeFMSPi0();
    Int_t MakeEEMCPi0();
    
    ClassDef(TStNanoDiffJetPi0TreeMaker,1) 
};

#endif

