#pragma once
#include "nv_math.h"
#include "nv_algebra.h"
#include "Point.h"

class LocalGeo
{
public:
	Point pos;
	vec3 normal;
	LocalGeo(Point p, vec3 norm);
	LocalGeo(void);
	~LocalGeo(void);
};
