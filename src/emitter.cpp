#include "emitter.h"

Emitter::Emitter(double a, double f, double x, double y) :
	mPos(x, y), mAmp(a), mFreq(2.*PI*f), mTimeAcc(0)
{
	mPeriod = 1./getFreq();
}
