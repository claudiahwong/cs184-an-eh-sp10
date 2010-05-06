/*
#include "Camera.h"

Camera::Camera(int sizex, int sizey, vec3 eye, vec3 center, vec3 up, float fov)
{
	mySizex = sizex;
	mySizey = sizey;
	myEye = eye;
	myCenter = center;
	myUp = up;
	myFov = fov;
}

Camera::~Camera(void)
{
}

void Camera::generateRay(Sample &sample, Ray *ray) {
	// t^2 (p1.p1) + 2tP1.(P0-C)+ (P0-C).(P0-C)-r^2 = 0
	float alpha = tan(myFov/2 * nv_pi / 180) * ((sample.x - (float)mySizex/2)/(float)(mySizex/2));
	float beta = tan(myFov/2 * nv_pi / 180) * ((float)mySizey/2 - sample.y)/(float)(mySizey/2));

	vec3 w, u, v;
	w = myEye;
	w.normalize();
	u = cross(u, myUp, w);
	u.normalize();
	v = cross(v, w, u);

	ray->dir = alpha*u + beta*v - w;
	(ray->dir).normalize();

}*/

#include "Camera.h"

Camera::Camera(void) {

}

Camera::Camera(vec3 lookFrom, vec3 lookAt, vec3 upv, float fovY, float znear, int width, int height)
{
	eye = lookFrom;
	center = lookAt;
	up = upv;
	//fovx = nv_to_deg * 2 * atan(width/(2.0 * znear));
	fovx = fovY;
	fovy = fovY;
	screenWidth = width;
	screenHeight = height;
}

Camera::~Camera(void)
{
}

void Camera::generateRay(Sample &sample, Ray *ray)
{
	vec3 w = eye - center;
	w.normalize();
	
	vec3 u;
	cross(u, up, w);
	u.normalize();

	vec3 v;
	cross(v, w, u);
	
	double z = screenHeight/2/tan(fovy/2*nv_to_rad);
	double fovxTerm =  2 * atan(screenWidth/2/z);
	//(fovy/2.0)*nv_to_rad* (screenWidth/((double) screenHeight))
	//printf("u: (%f, %f, %f), v: (%f, %f, %f), w: (%f, %f, %f)\n", u.x, u.y, u.z, v.x, v.y, v.z, w.x, w.y, w.z);
	//printf("sample: (%d, %d)\n", sample.x, sample.y);
	double alpha, beta;
	alpha = tan(fovxTerm/2)*((sample.x-((double) screenWidth/2.0))/((double) screenWidth/2.0));
	//alpha = tan((fovy/2.0)*nv_to_rad)*((sample.x-((float) screenWidth/2.0))/((float) screenWidth/2.0));
	beta = tan((fovy/2.0)*nv_to_rad)*((((double) screenHeight/2.0)-sample.y)/((double) screenHeight/2.0));
	//printf("alpha: %f, beta: %f\n", alpha, beta);
	vec3 direction = alpha*u + beta*v - w;
	//printf("dir b/4 normalize: (%f, %f, %f)\n", direction.x, direction.y, direction.z);
	direction.normalize();
	//printf("dir after normalize: (%f, %f, %f)\n", direction.x, direction.y, direction.z);

	Point eyePoint = Point(eye.x, eye.y, eye.z);
	ray->pos = eyePoint;
	ray->dir = direction;
	ray->t_min = 0.001;
	ray->t_max = 8000;
	//ray = new Ray (eyePoint, vec3(direction), 0, 800.0);
}