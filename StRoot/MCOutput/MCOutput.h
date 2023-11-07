#ifndef __MCOutput_hh__
#define __MCOutput_hh__

#include "TObject.h"
#include <vector>
//#include "TrueLevel.h"
#include "StMuDSTMaker/COMMON/StMuRpsCollection.h"

using namespace std;

//class PP2PPEndOfEventAction;

class MCOutput : public TObject {

//	friend class PP2PPEndOfEventAction;
public:

	//MCOutput() ;
	//~MCOutput() {;}

	inline StMuRpsCollection getRpsCollection(void) const;
	//inline TrueLevel getTrueLevel(void) const;
	inline int getRunNumber(void) const;

        inline void setRpsCollection(const StMuRpsCollection a);
//        inline void setTrueLevel(const TrueLevel a);
	inline void setRunNumber(const int runNum);

private:
//	TrueLevel mTrueLevel;
	StMuRpsCollection mRpsCollection;
	int mRunNumber;

ClassDef(MCOutput,1)
};

inline StMuRpsCollection MCOutput::getRpsCollection(void) const   { return mRpsCollection; }
//inline TrueLevel MCOutput::getTrueLevel(void) const   { return mTrueLevel; }
inline int MCOutput::getRunNumber(void) const   { return mRunNumber; }

inline void MCOutput::setRpsCollection(const StMuRpsCollection a)     { mRpsCollection = a; }
//inline void MCOutput::setTrueLevel(const TrueLevel a)     { mTrueLevel = a; }
inline void MCOutput::setRunNumber(const int runNum)     { mRunNumber = runNum; }
#endif
