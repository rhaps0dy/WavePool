#ifndef _colorselect_h_
#define _colorselect_h_

#include "image.h"
#include "framework.h"

class ColorSelect
{
private:
	static const Uint numColors = 7;
	static const Color colors[numColors];

	Uint x1, y1, x2, y2;
public:
	static const Color NONE;
	ColorSelect(Uint _x1, Uint _y1, Uint _x2, Uint _y2);
	void paintOn(Image *img);
	void setCoords(Uint _x1, Uint _y1, Uint _x2, Uint _y2);
	Color getColor(Vector2 *p);
};

#endif
