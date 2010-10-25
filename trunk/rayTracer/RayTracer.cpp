#include "RayTracer.h"

RayTracer::RayTracer(Scene* theScene, double *thit, Intersection *in, int maxDepth) {
	myPrimitive = new AggregatePrimitive(theScene->myPrimitivesVector);
	myThit = thit;
	myIn = in;
	myLights = theScene->myLightsVector;
	myMaxDepth = maxDepth;
	srand ( time(NULL) );
}

RayTracer::RayTracer(void)
{
}

RayTracer::~RayTracer(void)
{
}

void RayTracer::pathTrace(Ray& ray, int depth, Color* color) {

	BRDF brdf = BRDF();
	Ray lray = Ray();
	Color lcolor = Color();

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
	Color mcolor = Color();

	if (brdf.myType == 1)	// DIFF
	{
		mcolor.setEqual(brdf.myKd);
	}

	if (brdf.myType == 2)	// SPEC
	{
		mcolor.setEqual(brdf.myKs);
	}

	double p;
	if (mcolor.r > mcolor.g && mcolor.r > mcolor.b) {
		p = mcolor.r;
	} else if (mcolor.g > mcolor.b) {
		p = mcolor.g;
	} else {
		p = mcolor.b;
	}

	if (depth > myMaxDepth) {
		//Russian Roulette
		double r0 = (float)rand()/RAND_MAX;
		if (r0 < p) {
			mcolor /= p;
		} else {
			color->setEqual(brdf.myKe);
			return;
		}
	}

	// DIFFuse
	if (brdf.myType == 1) {
		double r1 = 2 * M_PI * (float)rand()/RAND_MAX;	// phi
		double r2 = (float)rand()/RAND_MAX;	// z
		double r3 = sqrt(r2);

		vec3 w;
		
		if (dot(ray.dir, myIn->localGeo.normal) < 0) {	// NEED TO RE-CHECK
			w = myIn->localGeo.normal;
		} else {
			w = -1*myIn->localGeo.normal;
		}

		vec3 u;
		if (fabs(w.x) > 0.1) {
			u = cross(u, vec3(0, 1, 0), w);
		} else {
			u = cross(u, vec3(1, 0, 0), w);
		}
		u.normalize();

		vec3 v;
		v = cross(v, w, u);

		vec3 newRaydir = scale(u, cos(r1)*r3) + scale(v, sin(r1)*r3) + scale(w, sqrt(1-r2));
		newRaydir.normalize();

		Color tempColor = Color();
		Ray tempRay = Ray();

		vec3 pos = vec3(myIn->localGeo.pos.myX, myIn->localGeo.pos.myY, myIn->localGeo.pos.myZ);
		vec3 resultingPos = pos + .01 * newRaydir;
		Point Pos = Point(resultingPos.x, resultingPos.y, resultingPos.z);
		pathTrace(Ray(Pos, newRaydir, 0, 8000), depth+1, &tempColor);
		//*color += (brdf.myKr * (tempColor));
		//*color = mcolor;
		*color = brdf.myKe + (mcolor*tempColor);
		return;
	} else if (brdf.myType == 2) {
		
	}
	//else if (obj.refl == SPEC)            // Ideal SPECULAR reflection 
    // return obj.e + f.mult(radiance(Ray(x,r.d-n*2*n.dot(r.d)),depth,Xi));
}

void RayTracer::trace(Ray& ray, int depth, Color* color) {

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

	Ray reflectedRay = Ray();
	Color tempColor = Color();
	// Handle mirror reflection
	if (brdf.myKr > 0) {
		reflectedRay = createReflectedRay(myIn->localGeo, ray);
		int glossy = 0;
		if (glossy) {
			// generate r'
			float totalR, totalG, totalB;
			totalR = 0.0;
			totalG = 0.0;
			totalB = 0.0;
			int num = 1;

			// Make a basis
			vec3 w = reflectedRay.dir;
			vec3 u;
			vec3 ranVec = vec3(.00424, 1, .00764); // Hardcoded a random vector
			cross(u, ranVec, w);
			u.normalize();
			vec3 v;
			cross(v, w, u);

			Ray r_prime;
			r_prime.pos.myX = reflectedRay.pos.myX;
			r_prime.pos.myY = reflectedRay.pos.myY;
			r_prime.pos.myZ = reflectedRay.pos.myZ;
			r_prime.t_min = 0;
			r_prime.t_max = 8000;
			float a = .0005;	// hard-coded a, which is the blurring factor for glossy reflection
			int k, total;
			total = 0;

			for (k = 0; k < num; k++){
				float epsilon = (float)rand()/RAND_MAX;
				float epsilon2 = (float)rand()/RAND_MAX;

				float i = -a/2.0 + epsilon * a;
				float j = -a/2.0 + epsilon2 * a;

				r_prime.dir = reflectedRay.dir + scale(u, i) + scale(v, j);
				r_prime.dir.normalize();

				// if r' is below the surface, make it above the surface
				if (dot(r_prime.dir, myIn->localGeo.normal) < 0.0) {
					r_prime.dir = reflectedRay.dir - scale(u, i) - scale(v, j);
					r_prime.dir.normalize();
				}

				trace(r_prime, depth+1, &tempColor);
				totalR += tempColor.r;
				totalG += tempColor.g;
				totalB += tempColor.b;
				tempColor.r = 0;
				tempColor.g = 0;
				tempColor.b = 0;

			}
			totalR /= (float) num;
			totalG /= (float) num;
			totalB /= (float) num;
			totalR *= (float) brdf.myKr.r;
			totalG *= (float) brdf.myKr.g;
			totalB *= (float) brdf.myKr.b;
			Color resultColor = Color(totalR, totalG, totalB);
			*color += ((resultColor));
		} else {
			trace(reflectedRay, depth+1, &tempColor);
			*color += (brdf.myKr * (tempColor));
		}
	}

	Ray refractedRay = Ray();
	Color tempColor2 = Color();
	bool *refract = new bool;
	// Handle refraction next
	if (brdf.myKrefract > 0) {
		refractedRay = createRefractedRay(myIn->localGeo, ray, brdf.myRIndex, refract);
		if (*refract == true){
			trace(refractedRay, depth+1, &tempColor2);
			*color += (brdf.myKrefract * (tempColor2));
		}
	}
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
