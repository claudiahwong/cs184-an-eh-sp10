#include "AggregatePrimitive.h"

AggregatePrimitive::AggregatePrimitive(vector<Primitive*> list) {
	for (vector<Primitive*>::iterator p = list.begin(); p != list.end(); p++) {
			myPrimitives.push_back(*p);
	}
}

AggregatePrimitive::AggregatePrimitive(void)
{
}

AggregatePrimitive::~AggregatePrimitive(void)
{
}

bool AggregatePrimitive::intersect(Ray &ray, double *thit, Intersection *in) 
{
	Primitive* closestPrim;
	double thit_min = 100000000;
	bool hasHit = false;
	
	for (vector<Primitive*>::iterator p = myPrimitives.begin(); p != myPrimitives.end(); p++) {
		if((*p)->intersect(ray, thit, in)) {
			if (*thit < thit_min) {
				hasHit = true;
				thit_min = *thit;
				closestPrim = *p;
			}
		}
	}

	if (hasHit) {
		closestPrim->intersect(ray, thit, in);
		return true;
	}
	return false;
}

bool AggregatePrimitive::intersectP(Ray &ray) 
{

	for (vector<Primitive*>::iterator p = myPrimitives.begin(); p != myPrimitives.end(); p++) {
		if((*p)->intersectP(ray)) {
			return true;
		}
	}
	return false;
}

void AggregatePrimitive::getBRDF(LocalGeo &local, BRDF* brdf) 
{
	exit(1);
}
