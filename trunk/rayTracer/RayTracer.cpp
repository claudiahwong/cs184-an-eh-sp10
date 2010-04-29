#include "RayTracer.h"

RayTracer::RayTracer(Scene* theScene, double *thit, Intersection *in, int maxDepth) {
	myPrimitive = new AggregatePrimitive(theScene->myPrimitivesVector);
	myThit = thit;
	myIn = in;
	myLights = theScene->myLightsVector;
	myMaxDepth = maxDepth;
}

RayTracer::RayTracer(void)
{
}

RayTracer::~RayTracer(void)
{
}

void RayTracer::trace(Ray& ray, int depth, Color* color) {
	
	BRDF *brdf = new BRDF();
	Ray *lray = new Ray();
	Color *lcolor = new Color();

	if (depth > myMaxDepth) {
		color->r = 0.0;
		color->g = 0.0;
		color->b = 0.0;
		return;
	}
	if (!myPrimitive->intersect(ray, myThit, myIn)) {
		// No intersection
		//Make the color black and return
		color->r = 0.0;
		color->g = 0.0;
		color->b = 0.0;
		return;
	}

	// Obtain the brdf at intersection point
	myIn->primitive->getBRDF(myIn->localGeo, brdf);

	*color += (brdf->myKa + brdf->myKe);
	// There is an intersection, loop through all light source
	for (vector<Light*>::iterator l = myLights.begin(); l != myLights.end(); l++) {
		(*l)->generateLightRay(myIn->localGeo, lray, lcolor);
		// Check if the light is blocked or not
		if (!myPrimitive->intersectP(*lray)) {
			// If not, do shading calculation for this
			*color += shade(myIn->localGeo, *brdf, *lray, *lcolor, ray);
		}
	}

	/*if (color->r > 1) color->r = 1;
	if (color->g > 1) color->g = 1;
	if (color->b > 1) color->b = 1;*/

	Ray *reflectedRay = new Ray();
	Color tempColor;
	// Handle mirror reflection
	if (brdf->myKr > 0) {
		*reflectedRay = createReflectedRay(myIn->localGeo, ray);
		//Make a recursive call to trace the reflected ray
		trace(*reflectedRay, depth+1, &tempColor);
		*color += (brdf->myKr * tempColor);
	}

	/*if (color->r > 1) color->r = 1;
	if (color->g > 1) color->g = 1;
	if (color->b > 1) color->b = 1;*/
}

Color RayTracer::shade(LocalGeo local, BRDF brdf, Ray lray, Color lcolor, Ray &ray)
{
	Color result = Color(0.0, 0.0, 0.0);
	
	vec3 lrayDir = lray.dir;
	lrayDir.normalize();
	vec3 rayDir = ray.dir;
	rayDir.normalize();

	vec3 half_angle = vec3(lrayDir - rayDir);
	half_angle.normalize();

	/*lray.dir.normalize();
	ray.dir.normalize();

	vec3 half_angle = vec3(lray.dir - ray.dir);
	half_angle.normalize();*/
	
	double ldirDotN = dot(lray.dir, local.normal); 
	if (ldirDotN < 0) ldirDotN = 0;
	double nDotHAngle = dot(local.normal, half_angle);
	if (nDotHAngle < 0) nDotHAngle = 0;
	double nDotHAngle_toShininess = pow(nDotHAngle, (double)brdf.myShininess);
	
	Color kdTerm;
	kdTerm.setEqual(brdf.myKd * ldirDotN);
	Color ksTerm;
	ksTerm.setEqual(brdf.myKs * nDotHAngle_toShininess);
	Color kdTermPlusKsTerm;
	kdTermPlusKsTerm.setEqual(kdTerm + ksTerm);
	Color theRest;
	theRest.setEqual(lcolor * kdTermPlusKsTerm);
	result += theRest;
	return result;
}

Ray RayTracer::createReflectedRay(LocalGeo local, Ray &ray) {
	vec3 reflectedDir;
	
	vec3 rayDir = ray.dir;
	rayDir.normalize();

	double rayDirDotNormal = dot(rayDir, local.normal);
	vec3 Ndirection = scale(local.normal, 2*rayDirDotNormal);
	//vec3 mult2 = scale(Ndirection, 2);
	reflectedDir = rayDir - Ndirection;

	vec3 pos = vec3(local.pos.myX, local.pos.myY, local.pos.myZ);
	vec3 resultingPos = pos + .001 * reflectedDir;
	Point Pos = Point(resultingPos.x, resultingPos.y, resultingPos.z);
	return Ray(Pos, reflectedDir, 0, 8000);
}