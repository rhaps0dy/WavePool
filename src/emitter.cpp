#include "emitter.h"
#include <cstdlib>
#include <cstdio>

Emitter::Emitter(double a, double l, double s, double x, double y) :
	mAmp(a), mWLen(l), mSpeed(s), mPos(x, y), mTimeAcc(0.), mWCache(NULL)
{
	makeAbs(&mWLen);
	makeAbs(&mAmp);
	makeAbs(&mSpeed);
	renewCache();
}

Emitter::~Emitter()
{
	free(mWCache);
}

void Emitter::renewCache()
{
	mWCacheLen = (Uint)(mWLen/2.);
	if(mWCache) free(mWCache);
	mWCache = (Float *) malloc(mWCacheLen*sizeof(Float));
	Float step = (Float)mWCacheLen;
	step = PI/step;
	for(Uint i=0; i<mWCacheLen; i++)
	{
		mWCache[i] = mAmp*sin((Float)(i)*step);
	}
	mWCacheLen_2 = mWCacheLen<<1;
}

Float Emitter::calcWave(Float x, Float y)
{
	//return mAmp*sin(2*PI*(distance(x, y, mPos.x, mPos.y)-mTimeAcc)/mWLen);
	Uint d = (Uint)(distance(x, y, mPos.x, mPos.y)-mTimeAcc);
	d = d%mWCacheLen_2;
	if(d >= mWCacheLen) //negative
		return -(mWCache[d-mWCacheLen]);
	return mWCache[d];
}

void Emitter::addTime(Float dt)
{
	mTimeAcc += dt*mSpeed;
	while(mTimeAcc > mWLen)
		mTimeAcc -= mWLen;
}
