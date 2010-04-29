#pragma once
#include <stdio.h>
#include "Scene.h"

class Scene;

class RayTracer
{
public:
	AggregatePrimitive *myPrimitive;
	double *myThit;
	Intersection *myIn;
	vector <Light*> myLights;
	int myMaxDepth;

	RayTracer(Scene* theScene, double *thit, Intersection *in, int maxDepth);
	RayTracer(void);
	~RayTracer(void);
	void trace(Ray& ray, int depth, Color* color);
	Color shade(LocalGeo local, BRDF brdf, Ray lray, Color lcolor, Ray &ray);
	Ray createReflectedRay(LocalGeo local, Ray &ray);
};
