#include "colorselect.h"

const Color ColorSelect::NONE = Color::BLACK; 
const Color ColorSelect::colors[] = {
	Color::RED,
	Color::GREEN,
	Color::BLUE,
	Color::WHITE,
	Color::CYAN,
	Color::YELLOW,
	Color::PURPLE };

ColorSelect::ColorSelect(Uint _x1, Uint _y1, Uint _x2, Uint _y2)
{
	setCoords(_x1, _y1, _x2, _y2);
}

void ColorSelect::paintOn(Image *img)
{
	Uint w = (x2-x1)/numColors;
	for(Uint y=y1; y<y2; y++)
	{
		for(Uint i=0; i<numColors-1; i++)
			for(Uint x = x1+w*i; x < x1+w*(i+1); x++)
				img->setPixel(x, y, colors[i]);
		for(Uint x = x1+w*(numColors-1); x<x2; x++)
			img->setPixel(x, y, colors[numColors-1]);
	}
}

void ColorSelect::setCoords(Uint _x1, Uint _y1, Uint _x2, Uint _y2)
{
	if(_x2 > _x1)
	{
		x1 = _x1;
		x2 = _x2;
	}
	else
	{
		x1 = _x2;
		x2 = _x1;
	}
	if(_y2 > _y1)
	{
		y1 = _y1;
		y2 = _y2;
	}
	else
	{
		y1 = _y2;
		y2 = _y1;
	}
}

Color ColorSelect::getColor(Vector2 *p)
{
	Uint y = (Uint)p->y;
	if(y < y1 || y >= y2) return NONE;

	Uint x = (Uint)p->x;
	Uint w = (x2-x1)/numColors;
	x = (x-x1)/w;
	if(x<numColors) return colors[x];
	return NONE;
}
