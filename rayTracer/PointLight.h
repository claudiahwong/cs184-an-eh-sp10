#pragma once
#include "Light.h"

class PointLight :
	public Light
{
public:
	Point myPos;
	Color myColor;

	PointLight(double x, double y, double z, Color c);
	PointLight(void);
	~PointLight(void);

	void generateLightRay(LocalGeo &local, Ray *lray, Color *lcolor);
};
