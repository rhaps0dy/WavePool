#ifndef _wavepool_h_
#define _wavepool_h_

#include "emitter.h"
#include "framework.h"
#include "image.h"

#include <vector>

class WavePool
{
private:
	Image img;
	Uint mWidth, mHeight;

	std::vector<Emitter> mEmitters;

	Uint mSelectedIndex;

public:

	WavePool(Uint w, Uint h);
	~WavePool();

	Image *getNewImage();
	void update(Uint dt);

	void resize(Uint w, Uint h);

	void select(Vector2 *p);
	void move(Vector2 *p);
	void add(Vector2 *p);
	void remove();
	void varWL(int amt);
	void varAmp(int amt);
	void varSpd(int amt);
	void setColor(unsigned char c);
};

#endif //_wavepool_h_
