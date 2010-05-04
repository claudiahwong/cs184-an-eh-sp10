#include "Sphere.h"

Sphere::Sphere(double posx, double posy, double posz, double radius, Camera cam)
//Sphere::Sphere(float posx, float posy, float posz, float radius)
{
	x = posx;
	y = posy;
	z = posz;
	r = radius;
	myCam.eye = cam.eye;
	myCam.center = cam.center;
	//myCam = Camera(cam.eye, cam.center, cam.up, cam.fovy, 1.0, cam.screenWidth, cam.screenHeight);
}

Sphere::~Sphere(void)
{
}

bool Sphere::intersect(Ray& ray, double* t_hit, LocalGeo* local) {
	vec3 eyeMinusCenter = vec3(ray.pos.myX, ray.pos.myY, ray.pos.myZ) - vec3(x, y, z);

	nv_scalar dirDotEyeMinusCenter = dot(ray.dir, eyeMinusCenter);
	nv_scalar dirDotdir = dot(ray.dir, ray.dir);
	nv_scalar discriminant = dirDotEyeMinusCenter* dirDotEyeMinusCenter - dot(ray.dir, ray.dir) * (dot(eyeMinusCenter, eyeMinusCenter) - r * r);
	double sqrt_discriminant = sqrt(discriminant);

	if (discriminant >= 0) {
		if (-dirDotEyeMinusCenter >= sqrt_discriminant) {
			*t_hit = (-dirDotEyeMinusCenter - sqrt_discriminant)/dirDotdir;
		} else {
			*t_hit = (-dirDotEyeMinusCenter + sqrt_discriminant)/dirDotdir;
		}
		if (*t_hit > 0) {
			// setting pos
			vec3 result = vec3(ray.pos.myX, ray.pos.myY, ray.pos.myZ) + (*t_hit)*(ray.dir);
			local->pos = Point(result.x, result.y, result.z);
			/*(local->pos).myX = ray.pos.myX + *t_hit * (ray.dir).x;
			(local->pos).myY = ray.pos.myY + *t_hit * (ray.dir).y;
			(local->pos).myZ = ray.pos.myZ + *t_hit * (ray.dir).z;*/

			// setting normal
			local->normal = vec3((local->pos).myX - x, (local->pos).myY - y, (local->pos).myZ - z);
			(local->normal).normalize();

			return true;
		} else return false;
	} else {
		return false;
	}
}

bool Sphere::intersectP(Ray &ray) {
	//vec3 eyeMinusCenter = myCam.eye - myCam.center;
	vec3 eyeMinusCenter = vec3(ray.pos.myX, ray.pos.myY, ray.pos.myZ) - vec3(x, y, z);
	
	double t_hit;
	double dirDotEyeMinusCenter = dot(ray.dir, eyeMinusCenter);
	double dirDotdir = dot(ray.dir, ray.dir);
	double discriminant = dirDotEyeMinusCenter* dirDotEyeMinusCenter - dot(ray.dir, ray.dir) * (dot(eyeMinusCenter, eyeMinusCenter) - r * r);
	double sqrt_discriminant = sqrt(discriminant);

	if (discriminant >= 0) {
		if (-dirDotEyeMinusCenter >= sqrt_discriminant) {
			t_hit = (-dirDotEyeMinusCenter - sqrt_discriminant)/dirDotdir;
		} else {
			t_hit = (-dirDotEyeMinusCenter + sqrt_discriminant)/dirDotdir;
		}
		if (t_hit > 0) return true;
	}
	return false;
	/*if (discriminant >= 0) {
		return true;
	} else {
		return false;
	}*/
}