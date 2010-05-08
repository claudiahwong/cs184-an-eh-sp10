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
	double alpha, beta;
	alpha = tan(fovxTerm/2)*((sample.x-((double) screenWidth/2.0))/((double) screenWidth/2.0));
	beta = tan((fovy/2.0)*nv_to_rad)*((((double) screenHeight/2.0)-sample.y)/((double) screenHeight/2.0));
	vec3 direction = alpha*u + beta*v - w;
	direction.normalize();

	Point eyePoint = Point(eye.x, eye.y, eye.z);
	ray->pos = eyePoint;
	ray->dir = direction;
	ray->t_min = 0.001;
	ray->t_max = 8000;
}