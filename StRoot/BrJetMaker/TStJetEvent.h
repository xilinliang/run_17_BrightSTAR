// Filename: TStJetEvent.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Jun 14 01:17:35 2020 (-0400)
// URL: jlab.org/~latif

#ifndef TSTJETEVENT_H
#define TSTJETEVENT_H

#include <string>
#include <iostream>
#include "TObject.h"
#include "TClonesArray.h"
#include "TStJetCandidate.h"
#include "BrContainers/TStFmsPointPairData.h"
#include "BrContainers/TStFmsPointData.h"
#include "TStJetSkimEvent.h"

class StJetElement;
class StJetParticle;
class StJetTower;


class TStJetEvent : public TObject
{
private:    
    TStJetSkimEvent *mSkimEvent;
    TClonesArray *mPointPairs;
    TClonesArray *mPoints;
    TClonesArray *mJets;
    TClonesArray *mTowers;
    TClonesArray *mParticles;
    //TClonesArray *mTracks;

public:    
   
    TStJetEvent()
    {
	mSkimEvent = new TStJetSkimEvent();
	mPointPairs = new TClonesArray("TStFmsPointPairData");
	mPoints = new TClonesArray("TStFmsPointData");
	mJets = new TClonesArray("TStJetCandidate");
	mTowers = new TClonesArray("TStJetTower");
	mParticles = new TClonesArray("TStJetParticle");
	//mTracks = new TClonesArray("TStJetTrack");
    }
    ~TStJetEvent()
    {
	delete mSkimEvent;
	delete mPointPairs;
	delete mPoints;
	delete mJets;
	delete mTowers;
	delete mParticles;
	//delete mTracks;
    }

    TStJetSkimEvent* GetEvent(){return mSkimEvent;}
    TStFmsPointPairData* GetPointPair(Int_t i){return (TStFmsPointPairData*)mPointPairs->At(i);}
    TStFmsPointData* GetPoint(Int_t i){return (TStFmsPointData*)mPoints->At(i);}
    TStJetCandidate* GetJet(Int_t i){return (TStJetCandidate*)mJets->At(i);}
    TStJetTower* GetTower(Int_t i){return (TStJetTower*)mTowers->At(i);}
    TStJetParticle* GetParticle(Int_t i){return (TStJetParticle*)mParticles->At(i);}
    //TStJetTrack* GetTrack(Int_t i){return (TStJetTrack*)mTracks->At(i);}

    Int_t GetNumberOfPointPairs(){return mPointPairs->GetEntriesFast();}
    Int_t GetNumberOfPoints(){return mPoints->GetEntriesFast();}
    Int_t GetNumberOfJets(){return mJets->GetEntriesFast();}
    Int_t GetNumberOfTowers(){return mTowers->GetEntriesFast();}
    Int_t GetNumberOfParticles(){return mParticles->GetEntriesFast();}
    //Int_t GetNumberOfTracks(){return mTracks->GetEntriesFast();}

    TStFmsPointPairData* NewPointPair() { return new ((*mPointPairs)[mPointPairs->GetEntriesFast()]) TStFmsPointPairData; }
    TStFmsPointData* NewPoint() { return new ((*mPoints)[mPoints->GetEntriesFast()]) TStFmsPointData; }
    TStJetCandidate* NewJet() { return new ((*mJets)[mJets->GetEntriesFast()]) TStJetCandidate; }
    //TStJetTrack* NewTrack() { return new ((*mTracks)[mTracks->GetEntriesFast()]) TStJetTrack; }
    TStJetTower* NewTower() { return new ((*mTowers)[mTowers->GetEntriesFast()]) TStJetTower; }
    TStJetParticle* NewParticle() { return new ((*mParticles)[mParticles->GetEntriesFast()]) TStJetParticle; }

    //void CopyTrack(StJetTrack* t, TStJetTrack* track);  //Copy from t to track
    void CopyTower(StJetTower*  t, TStJetTower* tower);  //Copy from t to tower
    void CopyParticle(StJetParticle* t, TStJetParticle* particle);
    
    void Reset()
    {
	mSkimEvent->Reset();
	mPointPairs->Clear();
	mPoints->Clear();
	mJets->Clear();
	//mTracks->Clear();
	mTowers->Clear();
	mParticles->Clear();	
    }

	       	
    ClassDef(TStJetEvent, 1 )
};

#endif
