#pragma once
#include "Ray.h"
#include "Point.h"
#include "LocalGeo.h"
#include "nv_math.h"
#include "nv_mathdecl.h"
#include "nv_algebra.h"
#include "Shape.h"
#include <stdio.h>

class Triangle : public Shape
{
public:
	
	Point v1, v2, v3;
	vec3 vector1, vector2, vector3;
	vec3 normal;
	vec3 n1, n2, n3;
	bool spec_normal;

	Triangle(Point vertex1, Point vertex2, Point vertex3);
	Triangle::Triangle(Point vertex1, Point vertex2, Point vertex3, vec3 nor1, vec3 nor2, vec3 nor3);
	~Triangle(void);

	bool intersect(Ray& ray, double* t_hit, LocalGeo* local);
	bool intersectP(Ray& ray);
};
