// Filename: TStNanoDiffJetPi0TreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif


#include "TStNanoDiffJetPi0TreeMaker.h"
#include "StEvent/StEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StSpinPool/StJetEvent/StJetEvent.h"
#include "StSpinPool/StJetSkimEvent/StJetSkimEvent.h"
#include "StSpinPool/StJetEvent/StJetCandidate.h"
#include "StSpinPool/StJetEvent/StJetTower.h"
#include "StSpinPool/StJetEvent/StJetParticle.h"
#include "StSpinPool/StJetEvent/StJetVertex.h"
#include "StSpinPool/StJetEvent/StJetTrack.h"
#include "StSpinPool/StSpinDbMaker/StSpinDbMaker.h"
#include "StJetFinder/StProtoJet.h"
#include "StJetMaker/StJetMaker2015.h"
#include "StJetMaker/StJetSkimEventMaker.h"
#include "StEvent/StFmsPointPair.h"
#include "StEvent/StFmsCollection.h"
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"
#include "StEvent/StTriggerId.h"
#include "StEvent/StEmcPoint.h"
#include "StEvent/StEmcCollection.h"

#include "TStJetEvent.h"
#include "TStJetSkimEvent.h"
#include "TStJetCandidate.h"
#include "BrContainers/TStFmsPointPairData.h"
#include "BrContainers/TStFmsPointData.h"


ClassImp(TStNanoDiffJetPi0TreeMaker)

//_____________________________________________________________________________ 
TStNanoDiffJetPi0TreeMaker::TStNanoDiffJetPi0TreeMaker(const char *name):StMaker(name)
{
    mOutJetEvent = new TStJetEvent();
    mOutSkimEvent = mOutJetEvent->GetEvent();
    mTrigFlag = kFALSE;
    
    mJetMaker = 0;
    mSkimEventMaker = 0;
    mInJetEvent = 0;
    mInSkimEvent = 0;

    mEtaMax = 6.0;
    mEtaMin = -2.0;
    mZdist = 735.0;
    mRunPeriod = 15;
    mMode = "pi0";

    mEventCount = 1;

    //RP Buffer
    TStRpsTrackData::Class()->IgnoreTObjectStreamer();
    mRpsArray = new TClonesArray("TStRpsTrackData");    

    //outtextSmall.open("BBCwestADCSumSmall.txt");
    //outtextLarge.open("BBCwestADCSumLarge.txt");
}

//_____________________________________________________________________________ 
TStNanoDiffJetPi0TreeMaker::~TStNanoDiffJetPi0TreeMaker()
{
    //
    delete mOutJetEvent;

    //RP Buffer    
    delete mRpsArray;
}


//_____________________________________________________________________________ 
Int_t TStNanoDiffJetPi0TreeMaker::Init()
{
    mOutFile = new TFile(mOutName, "recreate");
    mTree = new TTree("T", "EM Jet Analysis Tree");
    TStJetEvent::Class()->IgnoreTObjectStreamer();
    mTree->Branch("jetEvents", &mOutJetEvent, 256000, 99);

    //branch to hold FMS photons (used in FastJet) info from FMS maker
    
    //RP branch
    mTree->Branch("rpTrack", &mRpsArray, 256000, 99);
    

    cout << "Initialized NanoJetMaker!" <<endl;
    return kStOK;
}


//_____________________________________________________________________________
Int_t TStNanoDiffJetPi0TreeMaker::Make()
{

    cout << "start NanoDst event"<< endl;
    mMuDst = (StMuDst*)GetInputDS("MuDst");
    mEvent = (StEvent*)GetInputDS("StEvent");

    // Check if mMuDst or mEvent is valid
    if(!mMuDst)
    {
	LOG_ERROR << "TSt<Template>Maker::Make - No MuDst found" <<endm;
	return kStFatal;
    }
    mMuEvent = mMuDst->event();
    mRpsArray->Clear();    
    mOutJetEvent->Reset();
    mOutSkimEvent->SetBbcSumSmall(0,mMuEvent->triggerData()->bbcADCSum(east));
    mOutSkimEvent->SetBbcSumSmall(1,mMuEvent->triggerData()->bbcADCSum(west));
    mOutSkimEvent->SetBbcSumLarge(0,mMuEvent->triggerData()->bbcADCSumLargeTile(east));
    mOutSkimEvent->SetBbcSumLarge(1,mMuEvent->triggerData()->bbcADCSumLargeTile(west));
    //mOutSkimEvent->SetBbcSumLarge(0,mMuEvent->bbcTriggerDetector().adcSumEastLarge());
    //mOutSkimEvent->SetBbcSumLarge(1,mMuEvent->bbcTriggerDetector().adcSumWestLarge());
    //cout << "zdc sum above threshold:" << mMuEvent->triggerData()->zdcADC(west,0) << endl;
    for(Int_t ew = 0; ew < 2; ew++)
    {
	// ZDC
	mAdcSum[ew] = 0;
	for(Int_t pmt = 1; pmt <= 3; pmt++)
	{
	   mAdcSum[ew] += mMuEvent->triggerData()->zdcADC((StBeamDirection)ew,pmt); 
	}
	mOutSkimEvent->SetZdcSum(ew, mAdcSum[ew]);
    cout << "zdc ADC sum: " <<  mAdcSum[ew] << endl;
	// VPD
	mAdcSum[ew] = 0;
	for(Int_t pmt = 1; pmt <= 16; pmt++)
	{
	   mAdcSum[ew] += mMuEvent->triggerData()->vpdADC((StBeamDirection)ew,pmt); // replace by vpdADCSum if same
	}
	mOutSkimEvent->SetVpdSum(ew, mAdcSum[ew]);
    cout << "vpd ADC sum: " <<  mAdcSum[ew] << endl;
    } 
    mOutSkimEvent->SetTofMultiplicity(mMuEvent->triggerData()->tofMultiplicity());
    cout << "tof mult: " <<  mMuEvent->triggerData()->tofMultiplicity() << endl;

    //cout << "zdc west ADC sum: " <<  mAdcSum[1] << endl;

    //cout << mMuEvent->runNumber() << mMuEvent->eventInfo().id() << " small BBC	"<< mMuEvent->triggerData()->bbcADCSum(west) <<endl;
    //cout << mMuEvent->runNumber() << mMuEvent->eventInfo().id() << " large BBC	"<< mMuEvent->triggerData()->bbcADCSumLargeTile(west) <<endl;

    //cout<< "start NanoDst Tree!"<<endl;

    for(Int_t k = 0; k < 20; ++k)
    {
	int mTrigId = mMuEvent->triggerIdCollection().nominal().triggerId(k);
	

	switch(mTrigId)
	{
	    case 480810 :
		mOutSkimEvent->SetTrigFlag(0, 1);
		break;
	    case 480830 :
		mOutSkimEvent->SetTrigFlag(0, 1);
		break;
	    case 480809 :
		mOutSkimEvent->SetTrigFlag(1, 1);
		break;
	    case 480829 :
		mOutSkimEvent->SetTrigFlag(1, 1);
		break;
	    case 480808 :
		mOutSkimEvent->SetTrigFlag(2, 1);
		break;
	    case 480828 :
		mOutSkimEvent->SetTrigFlag(2, 1);
		break;
	    case 480801 :
		mOutSkimEvent->SetTrigFlag(3, 1);
		break;
	    case 480821 :
		mOutSkimEvent->SetTrigFlag(3, 1);
		break;
	    case 480841 :
		mOutSkimEvent->SetTrigFlag(3, 1);
		break;
	    case 480802 :
		mOutSkimEvent->SetTrigFlag(4, 1);
		break;
	    case 480822 :
		mOutSkimEvent->SetTrigFlag(4, 1);
		break;
	    case 480803 :
		mOutSkimEvent->SetTrigFlag(5, 1);
		break;
	    case 480823 :
		mOutSkimEvent->SetTrigFlag(5, 1);
		break;
	    case 480843 :
		mOutSkimEvent->SetTrigFlag(5, 1);
		break;
	    case 480804 :
		mOutSkimEvent->SetTrigFlag(6, 1);
		break;
	    case 480824 :
		mOutSkimEvent->SetTrigFlag(6, 1);
		break;
	    case 480844 :
		mOutSkimEvent->SetTrigFlag(6, 1);
		break;
	    case 480805 :
		mOutSkimEvent->SetTrigFlag(7, 1);
		break;
	    case 480825 :
		mOutSkimEvent->SetTrigFlag(7, 1);
		break;
	    case 480806 :
		mOutSkimEvent->SetTrigFlag(8, 1);
		break;
	    case 480826 :
		mOutSkimEvent->SetTrigFlag(8, 1);
		break;
	
	    case 480301 :
		mOutSkimEvent->SetTrigFlag(0, 1);
		break;
	    case 480404 :
		mOutSkimEvent->SetTrigFlag(1, 1);
		break;
	    case 480414 :
		mOutSkimEvent->SetTrigFlag(1, 1);
		break;
	    case 480401 :
		mOutSkimEvent->SetTrigFlag(2, 1);
		break;
	    case 480411 :
		mOutSkimEvent->SetTrigFlag(2, 1);
		break;
	    case 480501 :
		mOutSkimEvent->SetTrigFlag(3, 1);
		break;
	    case 480405 :
		mOutSkimEvent->SetTrigFlag(4, 1);
		break;
	    case 480415 :
		mOutSkimEvent->SetTrigFlag(4, 1);
		break;
	    case 480202 :
		mOutSkimEvent->SetTrigFlag(5, 1);
		break;
	    case 480203 :
		mOutSkimEvent->SetTrigFlag(6, 1);
		break;
	    case 480204 :
		mOutSkimEvent->SetTrigFlag(7, 1);
		break;
	    case 480205 :
		mOutSkimEvent->SetTrigFlag(8, 1);
		break;

	
	    case 570404 :
		mOutSkimEvent->SetTrigFlag(0, 1);
		break;
	    case 570403 :
		mOutSkimEvent->SetTrigFlag(1, 1);
		break;
	    case 570401 :
		mOutSkimEvent->SetTrigFlag(2, 1);
		break;
	    case 570402 :
		mOutSkimEvent->SetTrigFlag(3, 1);
		break;
	    case 570214 :
		mOutSkimEvent->SetTrigFlag(4, 1);
		break;
	    case 570215 :
		mOutSkimEvent->SetTrigFlag(5, 1);
		break;
	    case 31 :
		mOutSkimEvent->SetTrigFlag(6, 1);
		break;
	    case 570201 :
		mOutSkimEvent->SetTrigFlag(7, 1);
		break;
	    case 35 :
		mOutSkimEvent->SetTrigFlag(8, 1);
		break;
	    case 34 :
		mOutSkimEvent->SetTrigFlag(9, 1);
		break;
	    case 570203 :
		mOutSkimEvent->SetTrigFlag(10, 1);
		break;
	
	    case 570810 :
		mOutSkimEvent->SetTrigFlag(0, 1);
		break;
	    case 570830 :
		mOutSkimEvent->SetTrigFlag(0, 1);
		break;
	    case 570850 :
		mOutSkimEvent->SetTrigFlag(0, 1);
		break;
	    case 51 :
		mOutSkimEvent->SetTrigFlag(0, 1);
		break;
	    case 570809 :
		mOutSkimEvent->SetTrigFlag(1, 1);
		break;
	    case 570829 :
		mOutSkimEvent->SetTrigFlag(1, 1);
		break;
	    case 570849 :
		mOutSkimEvent->SetTrigFlag(1, 1);
		break;
	    case 50 :
		mOutSkimEvent->SetTrigFlag(1, 1);
		break;
	    case 570808 :
		mOutSkimEvent->SetTrigFlag(2, 1);
		break;
	    case 570828 :
		mOutSkimEvent->SetTrigFlag(2, 1);
		break;
	    case 570848 :
		mOutSkimEvent->SetTrigFlag(2, 1);
		break;
	    case 49 :
		mOutSkimEvent->SetTrigFlag(2, 1);
		break;
	    case 570801 :
		mOutSkimEvent->SetTrigFlag(3, 1);
		break;
	    case 570821 :
		mOutSkimEvent->SetTrigFlag(3, 1);
		break;
	    case 570841 :
		mOutSkimEvent->SetTrigFlag(3, 1);
		break;
	    case 570861 :
		mOutSkimEvent->SetTrigFlag(3, 1);
		break;
	    case 42 :
		mOutSkimEvent->SetTrigFlag(3, 1);
		break;
	    case 570802 :
		mOutSkimEvent->SetTrigFlag(4, 1);
		break;
	    case 570822 :
		mOutSkimEvent->SetTrigFlag(4, 1);
		break;
	    case 570842 :
		mOutSkimEvent->SetTrigFlag(4, 1);
		break;
	    case 43 :
		mOutSkimEvent->SetTrigFlag(4, 1);
		break;
	    case 570803 :
		mOutSkimEvent->SetTrigFlag(5, 1);
		break;
	    case 570823 :
		mOutSkimEvent->SetTrigFlag(5, 1);
		break;
	    case 570843 :
		mOutSkimEvent->SetTrigFlag(5, 1);
		break;
	    case 44 :
		mOutSkimEvent->SetTrigFlag(5, 1);
		break;
	    case 570804 :
		mOutSkimEvent->SetTrigFlag(6, 1);
		break;
	    case 570824 :
		mOutSkimEvent->SetTrigFlag(6, 1);
		break;
	    case 570844 :
		mOutSkimEvent->SetTrigFlag(6, 1);
		break;
	    case 570864 :
		mOutSkimEvent->SetTrigFlag(6, 1);
		break;
	    case 45 :
		mOutSkimEvent->SetTrigFlag(6, 1);
		break;
	    case 570805 :
		mOutSkimEvent->SetTrigFlag(7, 1);
		break;
	    case 570825 :
		mOutSkimEvent->SetTrigFlag(7, 1);
		break;
	    case 570845 :
		mOutSkimEvent->SetTrigFlag(7, 1);
		break;
	    case 46 :
		mOutSkimEvent->SetTrigFlag(7, 1);
		break;
	    case 570806 :
		mOutSkimEvent->SetTrigFlag(8, 1);
		break;
	    case 570826 :
		mOutSkimEvent->SetTrigFlag(8, 1);
		break;
	    case 570846 :
		mOutSkimEvent->SetTrigFlag(8, 1);
		break;
	    case 47 :
		mOutSkimEvent->SetTrigFlag(8, 1);
		break;
	}


    }	

    //tof and bbc multiplicity. Needed to minimize FMS ring of fire.
    mOutSkimEvent->SetTofMult(mMuEvent->triggerData()->tofMultiplicity()); 
    mOutSkimEvent->SetTofTrayMult(mMuEvent->btofTrayMultiplicity()); 
    //bbc multiplicity
    Int_t tac, adc;
    Int_t bbcMult = 0;
    Int_t east = 0;
    for(Int_t i = 1; i <= 16; i++)
    {
	tac = mMuEvent->triggerData()->bbcTDC((StBeamDirection)east,i);
	if(tac > 100 && tac < 2400)
	{
	    adc = mMuEvent->triggerData()->bbcADC((StBeamDirection)east, i);
	    if(adc > 50)
		bbcMult++;
	    //bbcEsum += trgd->bbcADC(east,i);
	}
    } 
    mOutSkimEvent->SetBbcMult(bbcMult);


    Int_t status;

    if (mRunPeriod == 15) {status = MakeRps();if (status != kStOK) return status;}

    char cMode = mMode[0];
    //cout<<"cMode:"<<cMode<<endl;

    switch(cMode)
    {

	case '0':
	{status = MakeEMjets(); break; }
	case '1':
	{status = MakeFMSPi0(); break; }
	case '2':
	{status = MakeEEMCPi0(); break; }
	case '3':
	{status = MakeRps(); break; }
	case '4':
	{status = MakeRps(); status = MakeEMjets();  break; }
	case '5':
	{status = MakeRps(); status = MakeEMjets(); status = MakeFMSPi0();  break; }
	default:
	{status = kStSkip; }

    }	    

    //if (status != kStOK) return status;


    mTree->Fill();
        
    return kStOK;
    
}



//____________________________________________________________________________
Int_t TStNanoDiffJetPi0TreeMaker::MakeEMjets()
{
    
   
    //cout << "start EM jets" << endl;
    mJetMaker = (StJetMaker2015*)GetMaker("StJetMaker2015"); 
    mSkimEventMaker = (StJetSkimEventMaker*)GetMaker("StJetSkimEventMaker"); 
    if(!mJetMaker || !mSkimEventMaker)
    {
	LOG_ERROR << "TStNanoDiffJetPi0TreeMaker::Make - No JetMaker or SkimEventMaker found" <<endm;
	return kStFatal;	
    }

    //cout<<"start generating NanoDst!"<<endl;

    mInJetEvent = mJetMaker->event("AntiKtR070NHits12"); //Set the branch name from a Set function
    mInSkimEvent = mSkimEventMaker->event();

    assert(mInJetEvent && mInSkimEvent);
    assert(mInJetEvent->runId() == mInSkimEvent->runId() && mInJetEvent->eventId() == mInSkimEvent->eventId());

    //intermediate variables
    Int_t spin4Bit;
    Int_t spinB;
    Int_t spinY;

    if ((mInSkimEvent->bx7() > 30 && mInSkimEvent->bx7() < 40) || (mInSkimEvent->bx7() > 110 && mInSkimEvent->bx7() < 120))
	    return kStSkip;
    
//    if(mInJetEvent->numberOfJets() == 0)
//	return kStSkip;
	
    mInVertex = mInJetEvent->vertex(); //same as mInJetEvent->vertex(0), i.e. highest ranked vertex only
    if (!mInVertex)
	return kStSkip;


    mTrigFlag = kFALSE;
//    for(Int_t k = 0; k < mMaxTriggers; ++k)
//    {
//	mTrigFlag = mMuEvent->triggerIdCollection().nominal().isTrigger(mTrigIds[k]);
//	if(mTrigFlag)
//	{
//	    mOutSkimEvent->SetTrigFlag(k, 1);
//	    mOutSkimEvent->SetTrigBit(k);
//	}
//    }	



    vtxZ = mInVertex->position().z();

    
    mOutSkimEvent->SetRunNumber(mInJetEvent->runId());
    mOutSkimEvent->SetEventId(mInJetEvent->eventId());
    mOutSkimEvent->SetVertexZ(vtxZ);
    //mOutSkimEvent->SetUnixTime(mInSkimEvent->unixTime()); //skimEvent does not have ET to GEM correction for SL20a
    mOutSkimEvent->SetUnixTime(mInJetEvent->unixTime());
    //cout << "unix time:" << mInJetEvent->unixTime() <<endl;
    mOutSkimEvent->SetBbcWestRate(mInSkimEvent->bbcWestRate());
    mOutSkimEvent->SetBbcEastRate(mInSkimEvent->bbcEastRate());
    mOutSkimEvent->SetFillNumber((Int_t)mInSkimEvent->fill());
    
    spin4Bit = mInSkimEvent->spin4usingBx48(); 
    /*
      Spin info to be interpreted based on: https://drupal.star.bnl.gov/STAR/blog/oleg/spin-patterns-and-polarization-direction
      spin = 10:  STAR pol B+ Y+
      =  9:  STAR pol B+ Y-
      =  6:  STAR pol B- Y+
      =  5:  STAR pol B- Y-
      (Note: Bunches can also be unpolarized, i.e. other possible values for the 4-spin bits are 1, 2, 4, and 8.)
    */

    switch(spin4Bit) 
    {
    case 5:
    {
	spinB = -1;
	spinY = -1;
	break;
    }
    case 6:
    {
	spinB = -1;
	spinY = +1;
	break;
    }
    case 9:
    {
	spinB = +1;
	spinY = -1;
	break;
    }
    case 10:
    {
	spinB = +1;
	spinY = +1;
	break;
    }
    default: //either one is unpolarized or undefined
	spinB = 0;
	spinY = 0;
    }

    mOutSkimEvent->SetSpinB(spinB);
    mOutSkimEvent->SetSpinY(spinY);


    Int_t mNFmsPoints;
    mFmsColl = (StFmsCollection*)mEvent->fmsCollection();
    if (!mFmsColl) {mNFmsPoints = 0;} else {mNFmsPoints = mFmsColl->numberOfPoints();}
    mOutSkimEvent->SetFmsPointMult(mNFmsPoints);

    Double_t eta;
    Double_t phi;
    Double_t eng;
    Double_t pt;
    Double_t jetX;
    Double_t jetY;
    Double_t theta;
    Double_t rt;
    Int_t nPhotons;
    for(Int_t i = 0; i < mInVertex->numberOfJets(); ++i) 
    {	    
	eta = mInVertex->jet(i)->eta();

	if(eta < mEtaMin  || eta > mEtaMax) 
	    continue;
	    
	phi = mInVertex->jet(i)->phi();
	eng = mInVertex->jet(i)->E();
	pt = mInVertex->jet(i)->pt();
	nPhotons = mInVertex->jet(i)->numberOfTowers();
	rt = mInVertex->jet(i)->rt();
	    
	theta =  2 * atan( exp(-eta) );
	jetX = (mZdist - vtxZ) * tan(theta) * cos(phi);
	jetY = (mZdist - vtxZ) * tan(theta) * sin(phi);

	mOutJet = mOutJetEvent->NewJet();	    
	mOutJet->SetNphotons(nPhotons);
	mOutJet->SetEta(eta);
	mOutJet->SetPhi(phi);
	mOutJet->SetE(eng);
	mOutJet->SetPt(pt);
	mOutJet->SetX(jetX);
	mOutJet->SetY(jetY);
	mOutJet->SetRt(rt);
	mOutJet->SetUedPt(mInVertex->jet(i)->ueDensity()["OffAxisConesR070"]*mInVertex->jet(i)->area());
	//cout << "Density: "<< mInVertex->jet(i)->ueDensity()["OffAxisConesR070"] <<" Area: "<< mInVertex->jet(i)->area() <<endl;

	//Add Tower info
	for(Int_t j = 0; j < mInVertex->jet(i)->numberOfTowers(); ++j)
	{
	    mInTower = mInVertex->jet(i)->tower(j);
	    mOutTower = mOutJetEvent->NewTower();
	    mOutJetEvent->CopyTower(mInTower, mOutTower);		    
	    mOutJet->AddTower(mOutTower);
	}

	//Add particle info
	for(Int_t j = 0; j < mInVertex->jet(i)->numberOfParticles(); ++j)
	{
	    mInParticle = mInVertex->jet(i)->particle(j);
	    mOutParticle = mOutJetEvent->NewParticle();
	    mOutJetEvent->CopyParticle(mInParticle, mOutParticle);
	    mOutJet->AddParticle(mOutParticle);		
	}

	//Add track info (if needed) in a similar fashion as tower and particles	    
    }
    return kStOk;    
    
}

//_____________________________________________________________________________
Int_t TStNanoDiffJetPi0TreeMaker::MakeRps()
{
    //------ Using afterburner ----
    // if(mUseRpsAfterburner)
    // {    
    //	mAfterburner->updateVertex(0.000415, 0.000455, 0.0); // specific to run 15 pp200 trans !!! To be set with set method
    //	mRpsMuColl = mAfterburner->process(); // executes afterburner
    // }
    //------
  

    mRpsMuColl = mMuDst->RpsCollection();  //No afterburner
    if(!mRpsMuColl)
    {
	cout<<"No RP data for this event"<<endl;
    mOutSkimEvent->SetRPMult(0);
	return kStSkip;
    }
   
    if (mRunPeriod == 17)
	{
    	mOutSkimEvent->SetRunNumber(mMuEvent->runNumber());
    	mOutSkimEvent->SetUnixTime(mMuEvent->eventInfo().time());
    	mOutSkimEvent->SetEventId(mMuEvent->eventInfo().id());
	//cout<<"Event time:"<<mMuEvent->eventInfo().time() << "	Event ID:"<<mMuEvent->eventInfo().id()<<endl;
	}

    mRpNtracks = mRpsMuColl->numberOfTracks();
    //cout << "nRP track:" << mRpNtracks << endl;
    mOutSkimEvent->SetRPMult(mRpNtracks);
    Int_t trkType;
    for(Int_t i = 0; i < mRpNtracks; ++i)
    {	
	mRpsTrackData =  new((*mRpsArray)[i])TStRpsTrackData();	
	mRpsTrk = mRpsMuColl->track(i);
	trkType = (mRpsTrk->type() == StMuRpsTrack::rpsGlobal) ? 1 : 0;
	
	mRpsTrackData->SetType(trkType);
	mRpsTrackData->SetNplanes(mRpsTrk->planesUsed());
	mRpsTrackData->SetBranch(mRpsTrk->branch());	
	mRpsTrackData->SetTheta(1000.0*mRpsTrk->theta());	
	mRpsTrackData->SetThetaX(1000.0*mRpsTrk->theta(0));	
	mRpsTrackData->SetThetaY(1000.0*mRpsTrk->theta(1));	
	mRpsTrackData->SetEta(mRpsTrk->eta());
	mRpsTrackData->SetPhi(mRpsTrk->phi());
	mRpsTrackData->SetPt(mRpsTrk->pt());		
	mRpsTrackData->SetP(mRpsTrk->p());		
	mRpsTrackData->SetPx(mRpsTrk->pVec().x());		
	mRpsTrackData->SetPy(mRpsTrk->pVec().y());		
	mRpsTrackData->SetPz(mRpsTrk->pVec().z());		
	mRpsTrackData->SetXi(mRpsTrk->xi(mBeamMom)); // Beam momentum is approximate		
	mRpsTrackData->SetMt(-1.0*mRpsTrk->t(mBeamMom));	
    }

    //afterburner
    //mAfterburner->clear(); //Critical!!!
    
    return kStOk;    
}



//_____________________________________________________________________________
Int_t TStNanoDiffJetPi0TreeMaker::MakeFMSPi0()
{

    Int_t mBunchid7bit,spin4Bit;
    Int_t spinB;
    Int_t spinY;

    mSpinDbMaker = static_cast<StSpinDbMaker*>(GetMaker("spinDb"));

    mOutSkimEvent->SetRunNumber(mMuEvent->runNumber());
    mOutSkimEvent->SetUnixTime(mMuEvent->eventInfo().time());
    mOutSkimEvent->SetEventId(mMuEvent->eventInfo().id());
    mBunchid7bit = mMuEvent->triggerData()->bunchId7Bit();	    
    if ((mBunchid7bit > 30 && mBunchid7bit < 40) || (mBunchid7bit > 110 && mBunchid7bit < 120))
	    return kStSkip;
    cout<<"runnumber:"<<mMuEvent->runNumber()<<"	time:"<<mMuEvent->eventInfo().time()<<"		bunch id:"<<mBunchid7bit<< endl;

    //spin4Bit = mSpinDbMaker->spin4usingBX7(mBunchid7bit);
    ///*
    //  Spin info to be interpreted based on: https://drupal.star.bnl.gov/STAR/blog/oleg/spin-patterns-and-polarization-direction
    //  spin = 10:  STAR pol B+ Y+
    //  =  9:  STAR pol B+ Y-
    //  =  6:  STAR pol B- Y+
    //  =  5:  STAR pol B- Y-
    //  (Note: Bunches can also be unpolarized, i.e. other possible values for the 4-spin bits are 1, 2, 4, and 8.)
    //*/

    //switch(spin4Bit) 
    //{
    //case 5:
    //{
    //    spinB = -1;
    //    spinY = -1;
    //    break;
    //}
    //case 6:
    //{
    //    spinB = -1;
    //    spinY = +1;
    //    break;
    //}
    //case 9:
    //{
    //    spinB = +1;
    //    spinY = -1;
    //    break;
    //}
    //case 10:
    //{
    //    spinB = +1;
    //    spinY = +1;
    //    break;
    //}
    //default: //either one is unpolarized or undefined
    //    spinB = 0;
    //    spinY = 0;
    //}

    //mOutSkimEvent->SetSpinB(spinB);
    //mOutSkimEvent->SetSpinY(spinY);

    mFmsColl = (StFmsCollection*)mEvent->fmsCollection();
    if (!mFmsColl)
    {

	    cout<<"No FMS point data for this mEvent"<<endl;
    		mOutSkimEvent->SetFmsPointMult(0);
	    return kStSkip;

    }

    Int_t mFmsNpairs;
    Int_t mNFmsPoints;
    Double_t mFmsPairE;
    Double_t mFmsPairM;
    Double_t mFmsPairPt;
    Double_t mFmsPairEta;
    Double_t mFmsPairPhi;
    Double_t mFmsPairZgg;
    Double_t mFmsPairDgg;
    Double_t mFmsPairX;
    Double_t mFmsPairY;

    //cout << "start getting point pairs!" << endl;
    mPointPairs = mFmsColl->pointPairsEnergySorted();
    mFmsNpairs = mFmsColl->numberOfPointPairs();
    mNFmsPoints = mFmsColl->numberOfPoints();
    mOutSkimEvent->SetFmsPointMult(mNFmsPoints);
    cout<<"n FMS pairs:"<<mFmsNpairs <<endl;
    for (Int_t i = 0; i < mFmsNpairs; ++i)
    {
	mPair = mPointPairs[i];

	mFmsPairE = mPair->energy();
	mFmsPairM = mPair->mass();	      
	mFmsPairPt = mPair->pT();
	mFmsPairEta = mPair->eta();
	mFmsPairPhi = mPair->phi();
	mFmsPairZgg = mPair->zgg();
	mFmsPairDgg = mPair->dgg();
	mFmsPairX = mPair->x();
	mFmsPairY = mPair->y();

	mOutPointPair = mOutJetEvent->NewPointPair();
	mOutPointPair->SetE(mFmsPairE);
	mOutPointPair->SetM(mFmsPairM);
	mOutPointPair->SetPt(mFmsPairPt);
	mOutPointPair->SetEta(mFmsPairEta);
	mOutPointPair->SetPhi(mFmsPairPhi);
	mOutPointPair->SetZgg(mFmsPairZgg);
	mOutPointPair->SetDgg(mFmsPairDgg);
	mOutPointPair->SetX(mFmsPairX);
	mOutPointPair->SetY(mFmsPairY);
    }

    return kStOk;    

}


//_____________________________________________________________________________
Int_t TStNanoDiffJetPi0TreeMaker::MakeEEMCPi0()
{


	cout<<"start Make EEMC pi0"<<endl;
	mEEmcTreePart1 = (StEEmcTreeMaker_t*)GetMaker("EEmcTreeMaker");
    if(!mEEmcTreePart1)
    {
	cout << "TStNanoDstMaker::Make !StEEmcTreeMaker_t" <<endl;
	return kStErr;
    }
	cout<<"EEMC Tree Part 1 number:"<< mEEmcTreePart1->getNumPart1EventsWritten() <<"	Event count:"<<mEventCount << endl;
	if(mEEmcTreePart1->getNumPart1EventsWritten() != mEventCount)
	    return kFALSE;
    ++mEventCount;
    Int_t mBunchid7bit,spin4Bit;
    Int_t spinB;
    Int_t spinY;

    mSpinDbMaker = static_cast<StSpinDbMaker*>(GetMaker("spinDb"));

    mOutSkimEvent->SetRunNumber(mMuEvent->runNumber());
    mOutSkimEvent->SetUnixTime(mMuEvent->eventInfo().time());
    mOutSkimEvent->SetEventId(mMuEvent->eventInfo().id());
    mBunchid7bit = mMuEvent->triggerData()->bunchId7Bit();	    
    if ((mBunchid7bit > 30 && mBunchid7bit < 40) || (mBunchid7bit > 110 && mBunchid7bit < 120))
	    return kStSkip;
    cout<<"runnumber:"<<mMuEvent->runNumber()<<"	time:"<<mMuEvent->eventInfo().time()<<"		bunch id:"<<mBunchid7bit<< endl;

    spin4Bit = mSpinDbMaker->spin4usingBX7(mBunchid7bit);
    /*
      Spin info to be interpreted based on: https://drupal.star.bnl.gov/STAR/blog/oleg/spin-patterns-and-polarization-direction
      spin = 10:  STAR pol B+ Y+
      =  9:  STAR pol B+ Y-
      =  6:  STAR pol B- Y+
      =  5:  STAR pol B- Y-
      (Note: Bunches can also be unpolarized, i.e. other possible values for the 4-spin bits are 1, 2, 4, and 8.)
    */

    switch(spin4Bit) 
    {
    case 5:
    {
	spinB = -1;
	spinY = -1;
	break;
    }
    case 6:
    {
	spinB = -1;
	spinY = +1;
	break;
    }
    case 9:
    {
	spinB = +1;
	spinY = -1;
	break;
    }
    case 10:
    {
	spinB = +1;
	spinY = +1;
	break;
    }
    default: //either one is unpolarized or undefined
	spinB = 0;
	spinY = 0;
    }

    mOutSkimEvent->SetSpinB(spinB);
    mOutSkimEvent->SetSpinY(spinY);

    mEEmcTreePart3 = (StEEmcTreeMaker_t*)GetMaker("EEmcTreeWriter");
    if (!mEEmcTreePart3)
    {
	cout <<"TSTNanoDstMaker::MakeEEmc - !StEEmcTreeMaker. Returning empty photon/pion list" <<endl;
	return kStErr;
    }

    //--- Fill EEMC Photons ----
    TIter arrItr = mEEmcTreePart3->getEEmcParticleCandidateIter1(); // Iterator for TClonesArray of EEmcParticleCandidate_t    
    Int_t nPoints = 0;
    while ((mInPoint = (EEmcParticleCandidate_t*)arrItr.Next()))
    {
	mOutPoint = mOutJetEvent->NewPoint();
	mOutPoint->SetE(mInPoint->E);
	mOutPoint->SetPt(mInPoint->PT);
	mOutPoint->SetX(mInPoint->position.X());
	mOutPoint->SetY(mInPoint->position.Y());
	mOutPoint->SetPx(mInPoint->momentum.X());
	mOutPoint->SetPy(mInPoint->momentum.Y());
	mOutPoint->SetPz(mInPoint->momentum.Z());
	mOutPoint->SetEta(mInPoint->momentum.PseudoRapidity());
	mOutPoint->SetPhi(mInPoint->momentum.Phi());
	
	++nPoints;	
    }

    //--- Fill EEMC Pions ----
    arrItr = mEEmcTreePart3->getEEmcParticleCandidateIter2(); // Iterator for TClonesArray of EEmc2ParticleCandidate_t    
    Int_t nPions = 0;
    while ((mInPion = (EEmc2ParticleCandidate_t*)arrItr.Next()))
    {

	mOutPointPair = mOutJetEvent->NewPointPair();
	mOutPointPair->SetE(mInPion->E);
	mOutPointPair->SetM(mInPion->M);
	mOutPointPair->SetPt(mInPion->PT);
	mOutPointPair->SetZgg(mInPion->Z);
	mOutPointPair->SetDgg(mInPion->D);
	mOutPointPair->SetX(mInPion->position.X());
	mOutPointPair->SetY(mInPion->position.Y());
	//mOutPointPair->SetPx(mInPoint->momentum.X());
	//mOutPointPair->SetPy(mInPoint->momentum.Y());
	//mOutPointPair->SetPz(mInPoint->momentum.Z());
	mOutPointPair->SetEta(mInPion->momentum.PseudoRapidity());
	mOutPointPair->SetPhi(mInPion->momentum.Phi());
	
	++nPions;	
    }

    cout<<"nPoints/nHits: "<<nPoints<<"	nPions: "<<nPions<<endl;
    return kStOK;


}




//_____________________________________________________________________________
Int_t TStNanoDiffJetPi0TreeMaker::Finish()
{
    //Write histograms to root file etc.
    mOutFile->Write();
    return kStOK;
}

//_____________________________________________________________________________
//void TStNanoDiffJetPi0TreeMaker::SetTrigIds(Int_t *trigIds)
//{
//    for(Int_t i = 0; i < mMaxTriggers; ++i)
//	mTrigIds[i] = trigIds[i];
//}
