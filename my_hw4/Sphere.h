#pragma once
#include "Shape.h"
#include "Camera.h"
#include <math.h>
#include <stdio.h>

class Sphere:
	public Shape
{
public:
	double x, y, z, r;
	Camera myCam;
	Sphere(double posx, double posy, double posz, double radius, Camera cam);
	//Sphere(float posx, float posy, float posz, float radius);
	~Sphere(void);
	
	bool intersect(Ray& ray, double* t_hit, LocalGeo* local);
	bool intersectP(Ray &ray);
};
