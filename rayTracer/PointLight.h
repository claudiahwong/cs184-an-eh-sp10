#pragma once
#include "Light.h"

class PointLight :
	public Light
{
public:
	Point myPos;
	Color myColor;
	double attenuation[3];

	PointLight(double x, double y, double z, Color c, double *att);
	PointLight(void);
	~PointLight(void);

	void generateLightRay(LocalGeo &local, Ray *lray, Color *lcolor);
	void getAttenuation(double* att);
};
