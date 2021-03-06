#include "wavepool.h"

WavePool::WavePool(Uint w, Uint h) :
	mWidth(w), mHeight(h), mSelectedIndex(0), color(Color::WHITE)
{
	complementary = color.getComplementary();
}

WavePool::~WavePool()
{
	for(Uint i=0; i<mEmitters.size(); i++)
		mEmitters[i].destroy();
}

#define MIN(X, Y) ((X)<(Y) ? (X) : (Y))
#define MAX(X, Y) ((X)>(Y) ? (X) : (Y))
#define LIMITZERO(X) ((X) - MIN((X), 4))
#define LIMITTO(X, W) (MIN((X)+4, ((W)-1)))

void WavePool::generateNewImage(Image *img)
{
	Uint iy, ix, i, x, y;
	Color c;
	Float val, r, g, b;

	#pragma omp parallel for default(shared) private(iy, ix, i, c, val, r, g, b)
	for(iy=0; iy<mHeight; iy++)
		for(ix=0; ix<mWidth; ix++)
		{
			r=g=b=0.;
			for(i=0; i<mEmitters.size(); i++)
			{
				val = (Float)mEmitters[i].calcWave(ix, iy);
				c = mEmitters[i].c;
				r += val*((Float)c.r)/255.;
				g += val*((Float)c.g)/255.;
				b += val*((Float)c.b)/255.;
			}
			c.r = (unsigned char)(clamp<Float>(r+(((Float)c.r)*.5), 0., 255.));
			c.g = (unsigned char)(clamp<Float>(g+(((Float)c.g)*.5), 0., 255.));
			c.b = (unsigned char)(clamp<Float>(b+(((Float)c.b)*.5), 0., 255.));
			img->setPixel(ix, iy, c);
		}
	for(i=0; i<mEmitters.size(); i++)
	{
		x = mEmitters[i].getX();
		y = mEmitters[i].getY();
		if(i==mSelectedIndex)
		{
			for(ix=LIMITZERO(x); ix<=LIMITTO(x, mWidth); ix++)
				for(iy=LIMITZERO(y); iy<=LIMITTO(y, mHeight); iy++)
					img->setPixel(ix, iy, complementary);
			continue;
		}
		for(ix=LIMITZERO(x); ix<=LIMITTO(x, mWidth); ix++)
			img->setPixel(ix, y, complementary);
		for(iy=LIMITZERO(y); iy<=LIMITTO(y, mHeight); iy++)
			img->setPixel(x, iy, complementary);
	}
}

#undef MIN
#undef MAX
#undef LIMITZERO
#undef LIMITTO

void WavePool::update(Uint dt)
{
	for(Uint i=0; i<mEmitters.size(); i++)
		if(mEmitters[i].running)
			mEmitters[i].addTime(dt);
}

void WavePool::resize(Uint w, Uint h)
{
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

void WavePool::select(Pos2 *p)
{
	Float distRecord = (Float)(mHeight > mWidth ? mHeight : mWidth);
	for(Uint i=0; i<mEmitters.size(); i++)
	{
		Float d = distance(p->x, p->y, mEmitters[i].getX(), mEmitters[i].getY());
		if(d<distRecord)
		{
			distRecord = d;
			mSelectedIndex = i;
		}
	}
	if(distRecord > 10.)
		mSelectedIndex = mEmitters.size();
}
void WavePool::move(Pos2 *p)
{
	if(mSelectedIndex==mEmitters.size()) return;
	Uint x = (Uint)p->x;
	Uint y = (Uint)p->y;
	if(x >= mWidth || y >= mHeight) return;
	mEmitters[mSelectedIndex].setPos(x, y);
#ifdef PRINT_CHANGES
	printf("Position %u, %u\n", x, y);
#endif
}

void WavePool::add(Pos2 *p)
{
	Uint x = (Uint)p->x;
	Uint y = (Uint)p->y;
	if(x >= mWidth || y >= mHeight) return;
	mEmitters.push_back(Emitter());
	mSelectedIndex = mEmitters.size()-1;
	mEmitters[mSelectedIndex].init(100, 100, 100, (Uint)p->x, (Uint)p->y, mWidth, mHeight);
	mEmitters[mSelectedIndex].c = color;
#ifdef PRINT_CHANGES
	printf("Added emitter in position %u, %u\n", (Uint)p->x, (Uint)p->y);
#endif
}

void WavePool::remove()
{
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].destroy();
	mEmitters.erase(mEmitters.begin()+mSelectedIndex);
#ifdef PRINT_CHANGES
	printf("Removed emitter %u\n", mSelectedIndex);
#endif
	mSelectedIndex = mEmitters.size();
}

void WavePool::setColor(Color *c)
{
	color = *c;
	complementary = color.getComplementary();
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].c = color;
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
#ifdef PRINT_CHANGES
	printf("Wavelength: %upx\n", mEmitters[mSelectedIndex].getWLen());
#endif
}

void WavePool::rmWL(Uint amt)
{
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].setWLen(sCOf(mEmitters[mSelectedIndex].getWLen(), amt));
#ifdef PRINT_CHANGES
	printf("Wavelength: %upx\n", mEmitters[mSelectedIndex].getWLen());
#endif
}

void WavePool::addAmp(Uint amt)
{
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].setAmp(aCOf(mEmitters[mSelectedIndex].getAmp(), amt));
#ifdef PRINT_CHANGES
	printf("Amplitude: %u\n", mEmitters[mSelectedIndex].getAmp());
#endif
}

void WavePool::rmAmp(Uint amt)
{
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].setAmp(sCOf(mEmitters[mSelectedIndex].getAmp(), amt));
#ifdef PRINT_CHANGES
	printf("Amplitude: %u\n", mEmitters[mSelectedIndex].getAmp());
#endif
}

void WavePool::addSpd(Uint amt)
{
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].setSpeed(aCOf(mEmitters[mSelectedIndex].getSpeed(), amt));
#ifdef PRINT_CHANGES
	printf("Speed: %upx/s\n", mEmitters[mSelectedIndex].getSpeed());
#endif
}

void WavePool::rmSpd(Uint amt)
{
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].setSpeed(sCOf(mEmitters[mSelectedIndex].getSpeed(), amt));
#ifdef PRINT_CHANGES
	printf("Speed: %upx/s\n", mEmitters[mSelectedIndex].getSpeed());
#endif
}

void WavePool::togglePause()
{
	if(mSelectedIndex==mEmitters.size()) return;
	mEmitters[mSelectedIndex].togglePause();
}
