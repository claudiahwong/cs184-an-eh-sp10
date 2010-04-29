#pragma once
#include "Ray.h"
#include "Intersection.h"

class Intersection;

class Primitive
{
public:
	Primitive(void);
	~Primitive(void);

	virtual bool intersect(Ray& ray, double* thit, Intersection* in)=0;
	virtual bool intersectP(Ray &ray)=0;
	virtual void getBRDF(LocalGeo &local, BRDF* brdf)=0;
};
