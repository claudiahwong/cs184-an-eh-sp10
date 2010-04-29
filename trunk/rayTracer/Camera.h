/*
#pragma once
#include "Ray.h"
#include "Sample.h"
#include "nv_math.h"
#include "nv_algebra.h"
#include <math.h>

class Camera
{
public:
	vec3 myEye, myCenter, myUp;
	float myFov;
	int mySizex, mySizey;
	Camera(int sizex, int sizey, vec3 eye, vec3 center, vec3 up, float fov);
	~Camera(void);
	void generateRay(Sample &sample, Ray *ray);
};
*/

#include "Ray.h"
#include "Sample.h"
#include "Point.h"
#include "nv_math.h"
#include "nv_algebra.h"
#include <math.h>
#include <stdio.h>

class Camera 
{
public:
	vec3 eye, center, up;
	float fovx, fovy;
	int screenWidth, screenHeight;
	Camera(vec3 lookFrom, vec3 lookAt, vec3 upv, float fovY, float znear, int width, int height);
	Camera(void);
	~Camera(void);

	void generateRay(Sample &sample, Ray *ray);
};
