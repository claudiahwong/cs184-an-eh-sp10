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
	
	/*BRDF *brdf = new BRDF();
	Ray *lray = new Ray();
	Color *lcolor = new Color();*/
	BRDF brdf = BRDF();
	Ray lray = Ray();
	Color lcolor = Color();

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
	myIn->primitive->getBRDF(myIn->localGeo, &brdf);

	*color += (brdf.myKa + brdf.myKe);
	// There is an intersection, loop through all light source
	for (vector<Light*>::iterator l = myLights.begin(); l != myLights.end(); l++) {
		(*l)->generateLightRay(myIn->localGeo, &lray, &lcolor);
		// Check if the light is blocked or not
		if (!myPrimitive->intersectP(lray)) {
			// If not, do shading calculation for this
			double att[3];
			(*l)->getAttenuation(att);
			double d = lray.pos.dist(myIn->localGeo.pos);
			double totalAtt = att[0] + att[1] * d + att[2] * pow(d, 2);
			*color += shade(myIn->localGeo, brdf, lray, lcolor, ray, totalAtt);
		}
	}

	/*Ray *reflectedRay = new Ray();
	Color *tempColor = new Color();*/
	Ray reflectedRay = Ray();
	Color tempColor = Color();
	// Handle mirror reflection
	if (brdf.myKr > 0) {
		reflectedRay = createReflectedRay(myIn->localGeo, ray);
		//Make a recursive call to trace the reflected ray
		trace(reflectedRay, depth+1, &tempColor);
		*color += (brdf.myKr * (tempColor));
	}

	/*Ray *refractedRay = new Ray();
	Color *tempColor2 = new Color();
	bool *refract = new bool;*/
	Ray refractedRay = Ray();
	Color tempColor2 = Color();
	bool *refract = new bool;
	// Handle refraction next
	if (brdf.myKrefract > 0) {
		refractedRay = createRefractedRay(myIn->localGeo, ray, brdf.myRIndex, refract);
		if (*refract == true){
			trace(refractedRay, depth+1, &tempColor);
			*color += (brdf.myKrefract * (tempColor));
		}
	}
	/*delete brdf;
	delete lray;
	delete lcolor;
	delete reflectedRay;
	delete refractedRay;
	delete tempColor;
	delete tempColor2;*/
	delete refract;
}

Color RayTracer::shade(LocalGeo local, BRDF brdf, Ray lray, Color lcolor, Ray &ray, double totalAttenuation)
{
	Color result = Color(0.0, 0.0, 0.0);

	vec3 half_angle = vec3(lray.dir - ray.dir);
	half_angle.normalize();
	
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
	Color LightColor;
	LightColor.setEqual(lcolor);
	LightColor/=(totalAttenuation);
	theRest.setEqual(LightColor * kdTermPlusKsTerm);
	result += theRest;
	return result;
}

Ray RayTracer::createReflectedRay(LocalGeo local, Ray &ray) {
	vec3 reflectedDir;

	double rayDirDotNormal = dot(ray.dir, local.normal);
	vec3 normal = local.normal;
	vec3 Ndirection = (2*rayDirDotNormal) * normal;
	reflectedDir = ray.dir - Ndirection;
	reflectedDir.normalize();

	vec3 pos = vec3(local.pos.myX, local.pos.myY, local.pos.myZ);
	vec3 resultingPos = pos + .01 * reflectedDir;
	Point Pos = Point(resultingPos.x, resultingPos.y, resultingPos.z);
	return Ray(Pos, reflectedDir, 0, 8000);
}

Ray RayTracer::createRefractedRay(LocalGeo local, Ray &ray, float rindex, bool* refract) {
	
	float n = 1.0 / rindex;
	vec3 N = vec3(local.normal);
	if (dot(local.normal, ray.dir) > 0) {
		N *= -1;
		n = rindex;
	}
	double cosI = dot(N, ray.dir);
	double sinT2 = n * n * (1.0 - cosI * cosI);
	if (sinT2 > 1.0)
	{
		*refract = false;
		return Ray();
	}
	*refract = true;
	vec3 result = vec3(n * ray.dir - (n + sqrt(1 - sinT2)) * N);
	result.normalize();
	vec3 pos = vec3(local.pos.myX, local.pos.myY, local.pos.myZ);
	pos += (.01 * result);

	return Ray(Point(pos.x, pos.y, pos.z), result, 0, 8000);

}
