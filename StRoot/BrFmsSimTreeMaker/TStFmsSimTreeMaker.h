// Filename: TStFmsSimTreeMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:38:04 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTFMSSIMTREEMAKER_H
#define TSTFMSSIMTREEMAKER_H

#include "StMaker.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TChain.h"
#include "BrContainers/TStFmsPointData.h"
#include "StSpinPool/StJetSkimEvent/StPythiaEvent.h"

class StEvent;
class StMuDstMaker;
class StMuDst;
class StMuEvent;
class StMuFmsCollection;  
class StMuFmsPoint;
class St_pythia_Maker;

class TStFmsSimTreeMaker : public StMaker
{
private:
    StMuDstMaker *mMuDstMaker;  
    StMuDst *mMuDst;  
    StEvent* mEvent;
    StMuEvent *mMuEvent;
    St_pythia_Maker *mPythiaMaker;
    
    TString mOutName = "FmsSimTree.root";
    TFile *mFile;
    TTree *mTree = 0;
    Int_t mNevents;
    TChain *mPyChain;
    TString mPythiaFile;
    Int_t mPyEvt;
    TChain *mChain;
    
    Int_t mRunId;
    Int_t mEventId;
    Int_t mTriggerBit;
    TClonesArray *mFmsPointArray;
    StPythiaEvent *mInPythiaEvent;

protected:
    void Reset();
    
public: 
    TStFmsSimTreeMaker(const char *name  = "FmsSimTreeMaker");
    virtual ~TStFmsSimTreeMaker();
    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();
    // virtual Int_t InitRun  (int runumber){return 0;}; // Overload empty StMaker::InitRun 
    // virtual Int_t FinishRun(int runumber){return 0;}; // Overload empty StMaker::FinishRun 
    void SetPythiaFile(TString pythiaFile){mPythiaFile = pythiaFile;}
    void SetOutFileName(TString outName){mOutName = outName;}
    
    ClassDef(TStFmsSimTreeMaker,1) 
};

#endif

