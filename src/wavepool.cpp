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
	int val;

	#pragma omp parallel for default(shared) private(iy, ix, i, c, val)
	for(iy=0; iy<mHeight; iy++)
		for(ix=0; ix<mWidth; ix++)
		{
			val = 0;
			for(i=0; i<mEmitters.size(); i++)
				val += mEmitters[i].calcWave(ix, iy);
			c.g = c.b = c.r = (uint8_t)clamp<int>(val+255/2, 0, 255);
			img.setPixel(ix, iy, c);
		}
	c.g = (unsigned char)255;
	c.r = c.b = (unsigned char)0;
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
	for(Uint i=0; i<mEmitters.size(); i++)
		if(mEmitters[i].running)
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
	printf("Position %u, %u\n", (Uint)p->x, (Uint)p->y);
}

void WavePool::add(Vector2 *p)
{
	mEmitters.push_back(Emitter());
	mSelectedIndex = mEmitters.size()-1;
	mEmitters[mSelectedIndex].init(100, 100, 100, (Uint)p->x, (Uint)p->y, mWidth, mHeight);
	printf("Added emitter in position %u, %u\n", (Uint)p->x, (Uint)p->y);
}

void WavePool::remove()
{
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].destroy();
	mEmitters.erase(mEmitters.begin()+mSelectedIndex);
	printf("Removed emitter %u\n", mSelectedIndex);
	mSelectedIndex = mEmitters.size();
}

void WavePool::setColor(unsigned char c)
{
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].color = c%3;
}

//add checking overflow
static inline Uint aCOf(Uint a, Uint b)
{
	b = a+b;
	if(b<a) b=((Uint)~0);
	return b;
}

//subtract checking overflow
static inline Uint sCOf(Uint a, Uint b)
{
	b = a-b;
	if(b>a) b=((Uint)0);
	return b;
}

void WavePool::addWL(Uint amt)
{
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].setWLen(aCOf(mEmitters[mSelectedIndex].getWLen(), amt));
	printf("Wavelength: %upx\n", mEmitters[mSelectedIndex].getWLen());
}
void WavePool::rmWL(Uint amt)
{
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].setWLen(sCOf(mEmitters[mSelectedIndex].getWLen(), amt));
	printf("Wavelength: %upx\n", mEmitters[mSelectedIndex].getWLen());
}

void WavePool::addAmp(Uint amt)
{
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].setAmp(aCOf(mEmitters[mSelectedIndex].getAmp(), amt));
	printf("Amplitude: %u\n", mEmitters[mSelectedIndex].getAmp());
}

void WavePool::rmAmp(Uint amt)
{
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].setAmp(sCOf(mEmitters[mSelectedIndex].getAmp(), amt));
	printf("Amplitude: %u\n", mEmitters[mSelectedIndex].getAmp());
}

void WavePool::addSpd(Uint amt)
{
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].setSpeed(aCOf(mEmitters[mSelectedIndex].getSpeed(), amt));
	printf("Speed: %upx/s\n", mEmitters[mSelectedIndex].getSpeed());
}

void WavePool::rmSpd(Uint amt)
{
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].setSpeed(sCOf(mEmitters[mSelectedIndex].getSpeed(), amt));
	printf("Speed: %upx/s\n", mEmitters[mSelectedIndex].getSpeed());
}

void WavePool::togglePause()
{
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].togglePause();
}
