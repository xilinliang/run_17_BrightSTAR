// Filename: DiffJetGetRunList.cxx
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

using namespace std;

//Use wildcat in the file name to combine many files.
void DiffJetGetRunList(TString inFileName, TString outName, TString det)
{
    TChain *ch = new TChain("T");
    ch->Add(inFileName);
    
    TStJetEvent *jetEvent = 0;
    TStJetSkimEvent *skimEvent;
    TStJetCandidate *jet;
    TStJetTower *tower;
    TStJetParticle *particle;
    TStRpsTrackData *rpsTrack =0;    

    TClonesArray *rpsArr = new TClonesArray("TStRpsTrackData");

    ch->SetBranchAddress("jetEvents", &jetEvent);
    ch->SetBranchAddress("rpTrack",&rpsArr);

    ofstream outtext;
    outtext.open(outName);
    
    
    Double_t jetX, jetY, eta, phi, theta, vtxZ, eng, pt,nEMjets_cut;
    Int_t nJets = 0;
    Int_t runnum = 0;

	vector<int>run;


    cout << "Total Entries to be processed: "<< ch->GetEntries() <<endl;

    for(Int_t evt = 0; evt < ch->GetEntries(); ++evt)
    {
	ch->GetEntry(evt);

	if(evt % 1000 == 0)
	    cout << "Events Processed: "<< evt <<endl;
	
	skimEvent = jetEvent->GetEvent();
	runnum = skimEvent->GetRunNumber();
	
	nJets = 0;

	//roman pot cut	
	Int_t nRpsTracks = rpsArr->GetEntriesFast();
	if (nRpsTracks > 0)
	{

		if (std::find(run.begin(),run.end(),runnum) == run.end()) {run.push_back(runnum);}


	}
    }

	sort(run.begin(),run.end());
	for (int i=0; i<run.size();i++) {outtext<<run[i]<<endl;}

   // outFile->Write();
   // outFile->Close();
    delete ch;
}
