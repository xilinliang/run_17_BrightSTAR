#ifndef __StHardDiffractionFilter_h__
#define __StHardDiffractionFilter_h__


// StHardDiffractionFilter : filter hard diffraction event
// Author: Xilin Liang

#include "StarFilterMaker.h"
#include "StarGenerator/EVENT/StarGenEvent.h"
#include "StarGenerator/Pythia8_1_62/include/Pythia.h"

class StHardDiffractionFilter : public StarFilterMaker
{

public:
	StHardDiffractionFilter (const char* name="hardDiff" );
	virtual ~StHardDiffractionFilter() {}

	int Filter ( StarGenEvent *event = 0);


private:

protected:

	Pythia8::Pythia *mPythia;
	const Pythia8::Pythia& pythia(){ return *mPythia; }

#if 1 // we dont really need, but this triggers cons to create dictionary
  ClassDef(StHardDiffractionFilter,0);
#endif

};

#endif
