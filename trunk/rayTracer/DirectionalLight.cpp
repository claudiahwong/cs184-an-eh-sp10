#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(vec3 direction, Color *color) 
{
	myDirection = vec3(direction);
	myDirection.normalize();
	myColor = Color(color->r, color->g, color->b);
}

DirectionalLight::DirectionalLight(void)
{
}

DirectionalLight::~DirectionalLight(void)
{
}

/*	ToDo:	Change vec3 to vec4 for all vectors... or at least
			for this part of the code.  */
void DirectionalLight::generateLightRay(LocalGeo &local, Ray *lray, Color *lcolor)
{
	lcolor->setEqual(myColor);
	
	vec3 temp;
	temp = vec3(local.pos.myX, local.pos.myY, local.pos.myZ) + .001 * (myDirection);
	lray->pos.myX = temp.x;
	lray->pos.myY = temp.y;
	lray->pos.myZ = temp.z;

	/*lray->pos = Point(local.pos.myX, local.pos.myY, local.pos.myZ);*/

	lray->dir = myDirection;
	lray->t_min = 0;
	lray->t_max = 100000000; // hundred million
}

void DirectionalLight::getAttenuation(double* att) {
	*att = 1;
	*(att+1) = 0;
	*(att+2) = 0;
}