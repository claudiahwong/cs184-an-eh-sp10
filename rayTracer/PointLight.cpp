#include "PointLight.h"

PointLight::PointLight(double x, double y, double z, Color c, double *att) {
	myPos = Point(x, y, z);
	myColor.setEqual(c);
	attenuation[0] = *att;
	attenuation[1] = (*att)+1;
	attenuation[2] = (*att)+2;
}

PointLight::PointLight(void)
{
}

PointLight::~PointLight(void)
{
}

void PointLight::generateLightRay(LocalGeo &local, Ray *lray, Color *lcolor) {
	lcolor->setEqual(myColor);
	
	lray->dir = (vec3(myPos.myX, myPos.myY, myPos.myZ) - vec3(local.pos.myX, local.pos.myY, local.pos.myZ));
	lray->dir.normalize();
	
	
	vec3 temp;
	temp = vec3(local.pos.myX, local.pos.myY, local.pos.myZ) + .001 * (lray->dir);
	lray->pos.myX = temp.x;
	lray->pos.myY = temp.y;
	lray->pos.myZ = temp.z;

	lray->t_min = 0;
	lray->t_max = 100000000; // hundred million
}

void PointLight::getAttenuation(double* att) {
	*att = attenuation[0];
	*(att+1) = attenuation[1];
	*(att+2) = attenuation[2];
}