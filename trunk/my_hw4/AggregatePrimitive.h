#pragma once
#include "Primitive.h"
#include <vector>
#include <stdio.h>

#define vector std::vector

class AggregatePrimitive :
	public Primitive
{
public:
	vector<Primitive*> myPrimitives;

	AggregatePrimitive(vector<Primitive*> list);
	AggregatePrimitive(void);
	~AggregatePrimitive(void);

	bool intersect(Ray& ray, double* thit, Intersection* in);
	bool intersectP(Ray &ray);
	void getBRDF(LocalGeo &local, BRDF* brdf);
};
