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
	Float mAmp;
	//wavelength (px)
	Float mWLen;
	//transfer speed (px/s)
	Float mSpeed;
	//origin of the wave
	Vector2 mPos;
	//Accumulated time
	Float mTimeAcc;
	//cached wave function. We cache T/2, with 1px resolution approx
	Float *mWCache;
	Uint mWCacheLen;
	//2*mWCacheLen
	Uint mWCacheLen_2;

	void renewCache();

public:
	inline Float getAmp() { return mAmp; }
	inline Float getWLen() { return mWLen; }
	inline Float getSpeed() { return mSpeed; }

	inline void setAmp(Float a) { mAmp = a; renewCache();}
	inline void setWLen(Float wl) { mWLen = wl; renewCache();}
	inline void setSpeed(Float s) { mSpeed = s; }

	inline void shiftPhase(Float p) { addTime(p/mSpeed); }

	inline void setPos(Float x, Float y) { mPos.set(x, y); }
	inline Vector2 getPos() { return mPos; }
	inline Float getX() { return mPos.x; }
	inline Float getY() { return mPos.y; }

	Emitter(double a, double l, double s, double x, double y);
	~Emitter();

	void addTime(Float dt);
	Float calcWave(Float x, Float y);
};

#endif //_emitter_h_
