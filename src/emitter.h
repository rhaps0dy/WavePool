#ifndef _emitter_h_
#define _emitter_h_

#include "framework.h"

/*
 * A sine wave emitter
 */
class Emitter
{
private:
	//all waves move at this speed (px/s)
	static constexpr Float WAVE_SPEED=1000.;
	//origin of the wave
	Vector2 mPos;
	//amplitude of the wave
	Float mAmp;
	//angular frequency of the wave (rad/s)
	Float mFreq;
	//accumulated time (s)
	Float mTimeAcc;
	//period of the sine wave (s)
	Float mPeriod;

public:
	inline void setAmp(Float a) { mAmp = a; }
	inline void setFreq(Float f) { mFreq = 2.*PI*f; }
	inline void addAmp(Float a) { mAmp += a; }
	inline void addFreq(Float f) { mFreq += 2.*PI*f; }
	inline Float getAmp() { return mAmp; }
	inline Float getFreq() { return mFreq/(2.*PI); }
	inline void setPos(Float x, Float y) { mPos.set(x, y); }
	inline Vector2 getPos() { return mPos; }
	inline Float getX() { return mPos.x; }
	inline Float getY() { return mPos.y; }

	Emitter(double a, double f, double x, double y);

	inline void addTime(Float dt)
	{
		mTimeAcc += dt;
		if(mTimeAcc > mPeriod)
			mTimeAcc -= mPeriod;
	}

	inline Float calcWave(Float x, Float y)
	{
		return mAmp*sin(mFreq*mTimeAcc - WAVE_SPEED*distance(x, y, mPos.x, mPos.y));
	}
};

#endif //_emitter_h_
