#ifndef _wavepool_h_
#define _wavepool_h_

#include "emitter.h"
#include "framework.h"
#include "image.h"
#include "includes.h"

#include <vector>

class WavePool
{
private:
	Uint mWidth, mHeight;

	std::vector<Emitter> mEmitters;

	Uint mSelectedIndex;
	Color color;
	Color complementary;
public:

	WavePool(Uint w, Uint h);
	~WavePool();

	void generateNewImage(Image *img);
	void update(Uint dt);

	void resize(Uint w, Uint h);

	void select(Pos2 *p);
	void move(Pos2 *p);
	void add(Pos2 *p);
	void remove();

	void addWL(Uint amt);
	void rmWL(Uint amt);

	void addAmp(Uint amt);
	void rmAmp(Uint amt);

	void addSpd(Uint amt);
	void rmSpd(Uint amt);

	void togglePause();

	void setColor(Color *c);
};

#endif //_wavepool_h_
