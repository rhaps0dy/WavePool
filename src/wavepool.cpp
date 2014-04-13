#include "wavepool.h"
#include <cstdlib>

WavePool::WavePool(Uint w, Uint h) :
	img(w, h), mWidth(w), mHeight(h),
	mNumEmitters(2), mEmitters(NULL)
{
	mEmitters = (Emitter *)malloc(mNumEmitters*sizeof(Emitter));
	mEmitters[0] = Emitter(255./4., 100., 50., 100., 100.);
	mEmitters[1] = Emitter(255./4., 200., 25., 500., 500.);
}

WavePool::~WavePool()
{
	free(mEmitters);
}

Image *WavePool::getNewImage()
{
	Uint iy, ix, i;
	Color c;
	Float sum;

//	#pragma omp parallel for default(shared) private(iy, ix, i, c, sum)
	for(iy=0; iy<mHeight; iy++)
	{
		for(ix=0; ix<mWidth; ix++)
		{
			sum=0.0;
			for(i=0; i<mNumEmitters; i++)
				sum += mEmitters[i].calcWave((Float)ix, (Float)iy);
			c.r = (unsigned char)(round(sum+255./2.));
			img.setPixel(ix, iy, c);
			if(ix==0)printf("%d=%d=%f ", c.r, img.getPixel(ix, iy).r, sum);
		}
	}
	return &img;
}

void WavePool::update(Float dt)
{
	#pragma omp parallel for
	for(Uint i=0; i<mNumEmitters; i++)
		mEmitters[i].addTime(dt);
}

void WavePool::resize(Uint w, Uint h)
{
	img.resizeNoCopy(w, h);
	#pragma omp parallel for
	for(Uint i=0; i<mNumEmitters; i++)
	{
		Float x = mEmitters[i].getX();
		Float y = mEmitters[i].getY();
		x /= (Float)mWidth;
		y /= (Float)mHeight;
		x *= (Float)w;
		y *= (Float)h;
		mEmitters[i].setPos(x, y);
	}
	mWidth = w;
	mHeight = h;
}
