#include "wavepool.h"

WavePool::WavePool(Uint w, Uint h) :
	img(w, h), mWidth(w), mHeight(h), mSelectedIndex(0)
{
}

WavePool::~WavePool()
{
	for(Uint i=0; i<mEmitters.size(); i++)
		mEmitters[i].destroy();
}

Image *WavePool::getNewImage()
{
	Uint iy, ix, i, x, y;
	Color c;

	#pragma omp parallel for default(shared) private(iy, ix, i, c)
	for(iy=0; iy<mHeight; iy++)
		for(ix=0; ix<mWidth; ix++)
		{
			c.r = c.g = c.b = 255/2;
			for(i=0; i<mEmitters.size(); i++)
				c.v[mEmitters[i].color] += mEmitters[i].calcWave(ix, iy);
			img.setPixel(ix, iy, c);
		}
	c.r = c.g = c.b = (unsigned char)255;
	#pragma omp parallel for default(shared) private(i, x, y, ix, iy)
	for(i=0; i<mEmitters.size(); i++)
	{
		x = mEmitters[i].getX();
		y = mEmitters[i].getY();
		for(ix=x-4; ix<=x+4; ix++)
			img.setPixel(ix, y, c);
		for(iy=y-4; iy<=y+4; iy++)
			img.setPixel(x, iy, c);
		if(i==mSelectedIndex)
		{
			for(ix=x-4; ix<=x+4; ix++)
				for(iy=y-4; iy<=y+4; iy++)
					img.setPixelSafe(ix, iy, c);
		}
	}
	return &img;
}

void WavePool::update(Uint dt)
{
	#pragma omp parallel for
	for(Uint i=0; i<mEmitters.size(); i++)
		mEmitters[i].addTime(dt);
}

void WavePool::resize(Uint w, Uint h)
{
	img.resizeNoCopy(w, h);
	#pragma omp parallel for
	for(Uint i=0; i<mEmitters.size(); i++)
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

void WavePool::select(Vector2 *p)
{
	Float distRecord = (Float)(mHeight > mWidth ? mHeight : mWidth);
	for(Uint i=0; i<mEmitters.size(); i++)
	{
		Float d = p->distance(mEmitters[i].getPos());
		if(d<distRecord)
		{
			distRecord = d;
			mSelectedIndex = i;
		}
	}
	if(distRecord > 10.)
		mSelectedIndex = mEmitters.size();
}
void WavePool::move(Vector2 *p)
{
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].setPos((Uint)p->x, ((Uint)p->y));
}

void WavePool::add(Vector2 *p)
{
	mEmitters.emplace_back();
	mSelectedIndex = mEmitters.size()-1;
	mEmitters[mSelectedIndex].init(100, 100, 100, p->x, p->y, mWidth, mHeight);
}

void WavePool::remove()
{
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].destroy();
	mEmitters.erase(mEmitters.begin()+mSelectedIndex);
}

void WavePool::varWL(int amt)
{
}

void WavePool::varAmp(int amt)
{
}

void WavePool::varSpd(int amt)
{
}

void WavePool::setColor(unsigned char c)
{
}

