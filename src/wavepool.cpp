#include "wavepool.h"

WavePool::WavePool(Uint w, Uint h) :
	img(w, h), mWidth(w), mHeight(h),
	mNumEmitters(0), mEmitters(NULL)
{
	mEmitters = new Emitter(255./2., 1., mWidth/2., mHeight/2.);
}

WavePool::~WavePool()
{
	delete mEmitters;
}

Image *WavePool::getNewImage()
{
	//we use dual variables for speed. We use 8 bytes of memory more but
	//we avoid a lot runtime casts.
	Uint ix, iy;
	Float fx, fy;
	Color c;
	for(iy=0, fy=0.0; iy<mWidth; iy++, fy+=1.)
		for(ix=0, fx=0.0; ix<mWidth; ix++, fx+=1.)
		{
			c.r = (Uint)(mEmitters->calcWave(fx, fy) + 255./2.);
			img.setPixel(ix, iy, c);
		}
	return &img;
}

void WavePool::update(Float dt)
{
	mEmitters->addTime(dt);
}
