#pragma once
#include <stdio.h>
#include "Scene.h"
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>

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
	void directIllumination(Ray& ray, Color* color, BRDF brdf);
	void pathTrace2(Ray& ray, int depth, Color* color, bool indirect);
	void pathTrace(Ray& ray, int depth, Color* color);
	void trace(Ray& ray, int depth, Color* color);
	Color shade(LocalGeo local, BRDF brdf, Ray lray, Color lcolor, Ray &ray, double totalAttenuation);
	Ray createReflectedRay(LocalGeo local, Ray &ray);
	Ray createRefractedRay(LocalGeo local, Ray &ray, float rindex, bool* refract);
};
