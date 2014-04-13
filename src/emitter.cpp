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
	Uint squaredbit, remainder, root;
	x = (x>getX() ? x-getX() : getX()-x);
	y = (y>getX() ? y-getY() : getY()-y);
	root = x*x+y*y;
	// Integer square root
	if(root>1)
	{
		squaredbit = ((~((Uint)0)) >> 1) & ~((~((Uint)0)) >> 2);
		remainder = root;
		root = 0;
		while (squaredbit > 0) {
			if (remainder >= (squaredbit | root)) {
				remainder -= (squaredbit | root);
				root >>= 1;
				root |= squaredbit;
			} else {
				root >>= 1;
			}
			squaredbit >>= 2; 
		}
	}

	//we add mwCacheLen_2 to avoid d from possibly warping to negative
	root = (root+mWCacheLen_2-mTimeAcc/1000)%mWCacheLen_2;
	if(root >= mWCacheLen) //negative
		return -(mWCache[root-mWCacheLen]);
	return mWCache[root];
}

void Emitter::resize(Uint w, Uint h)
{
}
