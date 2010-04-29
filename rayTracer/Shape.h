#pragma once
#include "Ray.h"
#include "LocalGeo.h"
#include "nv_math.h"
#include "nv_mathdecl.h"
#include "nv_algebra.h"

class Shape
{
public:
	Shape(void);
	~Shape(void);

	/*	Test if the object intersects with the given ray
		If so, t_hit = t_intersect and local contains the
		normal at the point.
	*/
	virtual bool intersect (Ray &ray, double* t_hit, LocalGeo* local) = 0;
	virtual bool intersectP(Ray &ray) = 0;
};
