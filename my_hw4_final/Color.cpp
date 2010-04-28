#include "Color.h"

Color::Color(void)
{
	r = 0;
	g = 0;
	b = 0;
}

Color::Color(float red, float green, float blue)
{
	r = red;
	g = green;
	b = blue;
}

Color::~Color(void)
{
}

void Color::setEqual(Color dest) {
	this->r = dest.r;
	this->g = dest.g;
	this->b = dest.b;
}
