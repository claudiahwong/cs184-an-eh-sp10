#pragma once
#include "LocalGeo.h"
#include "BRDF.h"
#include "Primitive.h"

class Primitive;

class Intersection
{
public:
	LocalGeo localGeo;
	Primitive *primitive;
	
	Intersection(void);
	~Intersection(void);
};
