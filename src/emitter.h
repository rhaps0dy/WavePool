#ifndef _emitter_h_
#define _emitter_h_

#include "framework.h"

/*
 * A sine wave emitter
 */
class Emitter
{
private:
	//amplitude of the wave
	Uint mAmp;
	//wavelength (px)
	Uint mWLen;
	//transfer speed (px/s)
	Uint mSpeed;
	//origin of the wave
	Vector2 mPos;
	//Accumulated time
	Uint mTimeAcc;
	//cached wave function. We cache T/2, with 1px resolution approx
	int8_t *mWCache;
	Uint mWCacheLen;
	//2*mWCacheLen
	Uint mWCacheLen_2;
	//pixel distance cache

	void renewCache();
public:
	inline Uint getAmp() { return mAmp; }
	inline Uint getWLen() { return mWLen/1000; }
	inline Uint getSpeed() { return mSpeed; }

	inline void setAmp(Uint a) { mAmp = a; renewCache();}
	inline void setWLen(Uint wl) { mWLen = wl*1000; renewCache();}
	inline void setSpeed(Uint s) { mSpeed = s; }

	inline void setPos(Uint x, Uint y) { mPos.set(x, y); }
	inline Vector2 getPos() { return mPos; }
	inline Uint getX() { return mPos.x; }
	inline Uint getY() { return mPos.y; }

	//for arrays
	void init(Uint a, Uint l, Uint s, Uint x, Uint y, Uint w, Uint h);
	void destroy();

	inline void addTime(Uint dt) { mTimeAcc = (dt*mSpeed+mTimeAcc)%mWLen; }
	int8_t calcWave(Uint x, Uint y);
	void resize(Uint w, Uint h);
};

#endif //_emitter_h_
