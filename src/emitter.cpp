#include "emitter.h"
#include <cstdlib>
#include <cstdio>

Emitter::Emitter(Uint a, Uint l, Uint s, Uint x, Uint y) :
	mAmp(a), mWLen(l*1000), mSpeed(s), mPos(x, y), mTimeAcc(0), mWCache(NULL)
{
	renewCache();
}

Emitter::~Emitter()
{
	free(mWCache);
}

void Emitter::renewCache()
{
	mWCacheLen = getWLen()/2;
	if(mWCache==NULL) free(mWCache);
	mWCache = (int8_t *) malloc(mWCacheLen*sizeof(int8_t));
	Float step = PI/((Float)mWCacheLen);
	Float res;
	for(Uint i=0; i<mWCacheLen; i++)
	{
		res = ((Float)mAmp) * sin(((Float)i)*step);
#ifdef DEBUG
		if(res < -128. || res > 127.)
		{
			printf("WARNING: Sine wave result exceeds int8_t boundaries\n");
			res = clamp(res, -128., 127.);
		}
#endif //DEBUG
		mWCache[i] = (int8_t)(res);
	}
	mWCacheLen_2 = mWCacheLen<<1;
}

int8_t Emitter::calcWave(Uint d)
{
	//we add mwCacheLen_2 to avoid d from possibly warping to negative
	d = (d+mWCacheLen_2-mTimeAcc/1000)%mWCacheLen_2;
	if(d >= mWCacheLen) //negative
		return -(mWCache[d-mWCacheLen]);
	return mWCache[d];

}

