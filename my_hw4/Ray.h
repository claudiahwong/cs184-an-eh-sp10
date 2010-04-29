#pragma once
#include "Point.h"
#include "nv_math.h"
#include "nv_algebra.h"

class Ray
{
public:
	Point pos;
	vec3 dir;
	double t_min, t_max;
	Ray(void);
	Ray(Point position, vec3 direction, double min, double max);
	~Ray(void);
};
