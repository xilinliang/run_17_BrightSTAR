#include "StHardDiffractionFilter.h"

#include "StarGenerator/EVENT/StarGenEvent.h"
#include "StarGenerator/EVENT/StarGenStats.h"


StHardDiffractionFilter::StHardDiffractionFilter(const char* name) : StarFilterMaker(name){

}

int StHardDiffractionFilter::Filter( StarGenEvent *_event)
{

	StarGenEvent& event = (_event)? *_event : *mEvent;
	//Pythia8::Info  &info  = mPythia->info;

	//if (info.isDiffractiveA()||info.isDiffractiveB()) {return StarGenEvent::kAccept;} else {return StarGenEvent::kReject;}	
	if (event.GetHardDiffraction()) {return StarGenEvent::kAccept;} else {return StarGenEvent::kReject;}	


}


