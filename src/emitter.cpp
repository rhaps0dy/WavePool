#include "emitter.h"
#include <cstdlib>
#include <cstdio>

void Emitter::init(Uint a, Uint l, Uint s, Uint x, Uint y, Uint w, Uint h)
{
	mAmp = a;
	mWLen = l*1000;
	mSpeed = s;
	mPos.x = x;
	mPos.y = y;
	mTimeAcc = 0;
	mWCache = NULL;
	mDistCache = NULL;

	renewCache();
	resize(w, h);
}

void Emitter::destroy()
{
	if(mWCache) free(mWCache);
}

void Emitter::renewCache()
{
	mWCacheLen = getWLen()/2;
	if(mWCache) free(mWCache);
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

int8_t Emitter::calcWave(Uint x, Uint y)
{
	Uint d;
	//we add mwCacheLen_2 to avoid d from possibly warping to negative
	d = (mDistCache[y*mWidth+x]+mWCacheLen_2-mTimeAcc/1000)%mWCacheLen_2;
	if(d >= mWCacheLen) //negative
		return -(mWCache[d-mWCacheLen]);
	return mWCache[d];
}

void Emitter::resize(Uint w, Uint h)
{
	if(mDistCache) free(mDistCache);
	mDistCache = (Uint *) malloc(w*h*sizeof(Uint));
	mWidth = w;
	mHeight = h;
	renewDistCache();
}

void Emitter::renewDistCache()
{
	Uint x, y, _x, _y;
	#pragma omp parallel for default(shared) private(x, y, _x, _y)
	for(y=0; y<mHeight; y++)
		for(x=0; x<mWidth; x++)
		{
			_x = (x>getX() ? x-getX() : getX()-x);
			_y = (y>getX() ? y-getY() : getY()-y);
			mDistCache[y*mWidth+x] = isqrt(_x*_x+_y*_y);
		}
}
