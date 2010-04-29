#pragma once
#include "Ray.h"
#include "LocalGeo.h"
#include "Color.h"

class Light
{
public:
	Light(void);
	~Light(void);

	virtual void generateLightRay(LocalGeo &local, Ray *lray, Color *lcolor)=0;
};