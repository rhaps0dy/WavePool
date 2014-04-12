#include "emitter.h"

Emitter::Emitter(double a, double f, double x, double y) :
	mAmp(a), mFreq(2.*PI*f), mPos(x, y)
{
	mPeriod = 1./getFreq();
}
