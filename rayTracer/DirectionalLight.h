#pragma once
#include "Light.h"

#include "nv_mathdecl.h"
#include "nv_math.h"
#include "nv_algebra.h"


class DirectionalLight :
	public Light
{
public:
	vec3 myDirection;
	Color myColor;
	double attenuation[3];

	DirectionalLight(vec3 direction, Color *color);
	DirectionalLight(void);
	~DirectionalLight(void);

	void generateLightRay(LocalGeo &local, Ray *lray, Color *lcolor);
	void getAttenuation(double* att);
};
