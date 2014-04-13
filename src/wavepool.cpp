#include "wavepool.h"

WavePool::WavePool(Uint w, Uint h) :
	img(w, h), mWidth(w), mHeight(h), mNumEmitters(2)
{
	mEmitters = new Emitter[mNumEmitters];
	mEmitters[0].init(255/4, 100, 100, 100, 100, w, h);
	mEmitters[1].init(255/4, 200, 25, 500, 500, w, h);
}

WavePool::~WavePool()
{
	for(Uint i=0; i<mNumEmitters; i++)
		mEmitters[i].destroy();
	delete[] mEmitters;
}

Image *WavePool::getNewImage()
{
	Uint iy, ix, i;
	Color c;
	int8_t sum;

	#pragma omp parallel for default(shared) private(iy, ix, i, c, sum)
	for(iy=0; iy<mHeight; iy++)
		for(ix=0; ix<mWidth; ix++)
		{
			sum = 255/2;
			for(i=0; i<mNumEmitters; i++)
				sum += mEmitters[i].calcWave(ix, iy);
			c.r = (unsigned char)sum;
			img.setPixel(ix, iy, c);
		}
	return &img;
}

void WavePool::update(Uint dt)
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
		mEmitters[i].resize(w, h);
	}
	mWidth = w;
	mHeight = h;
}
