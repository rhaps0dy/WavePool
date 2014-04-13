#ifndef _wavepool_h_
#define _wavepool_h_

#include "emitter.h"
#include "framework.h"
#include "image.h"

class WavePool
{
private:
	Image img;
	Uint mWidth, mHeight;

	Uint mNumEmitters;
	Emitter *mEmitters;

public:
	WavePool(Uint w, Uint h);
	~WavePool();

	Image *getNewImage();
	void update(Float dt);

	void resize(Uint w, Uint h);
};

#endif //_wavepool_h_
