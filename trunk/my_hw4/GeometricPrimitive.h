#pragma once 
#include "Primitive.h"
#include "Shape.h"
#include "Material.h"
#include "Transformation.h"

class GeometricPrimitive :
	public Primitive
{
public:
	Transformation objToWorld, worldToObj;
	Shape* myShape;
	Material* myMat;
	
	GeometricPrimitive(Shape* shape, Material* mat, Transformation objToWorld, Transformation worldToObj);
	//GeometricPrimitive(Shape* shape, Material* mat);
	~GeometricPrimitive(void);
	bool intersect(Ray& ray, double* thit, Intersection* in);
	bool intersectP(Ray &ray);
	void getBRDF(LocalGeo &local, BRDF* brdf);
};
