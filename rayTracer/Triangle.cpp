#include "Triangle.h"

Triangle::Triangle(Point vertex1, Point vertex2, Point vertex3)
{
	spec_normal = 0;
	
	v1 = vertex1;
	v2 = vertex2;
	v3 = vertex3;
	
	vector1 = vec3(v1.myX, v1.myY, v1.myZ);
	vector2 = vec3(v2.myX, v2.myY, v2.myZ);
	vector3 = vec3(v3.myX, v3.myY, v3.myZ);

	// Compute the normal
	cross(normal, vector2 - vector1, vector3 - vector1);
	normal.normalize();
}

Triangle::Triangle(Point vertex1, Point vertex2, Point vertex3, vec3 nor1, vec3 nor2, vec3 nor3)
{
	spec_normal = 1;
	
	v1 = vertex1;
	v2 = vertex2;
	v3 = vertex3;
	
	n1 = nor1;
	n2 = nor2;
	n3 = nor3;

	vector1 = vec3(v1.myX, v1.myY, v1.myZ);
	vector2 = vec3(v2.myX, v2.myY, v2.myZ);
	vector3 = vec3(v3.myX, v3.myY, v3.myZ);

	// Compute the normal
	cross(normal, vector2 - vector1, vector3 - vector1);
	normal.normalize();
}

Triangle::~Triangle(void)
{
}

bool Triangle::intersect(Ray& ray, double* t_hit, LocalGeo* local)
{
	//vec3 p0 = vec3(ray.pos.myX, ray.pos.myY, ray.pos.myZ);
	//vec3 p1 = ray.dir;
	//vec3 A = vec3(vector1);

	double xe, ye, ze,
		xa, ya, za,
		xb, yb, zb,
		xc, yc, zc,
		xd, yd, zd;

	double beta, gamma;

	double a, b, c,
		d, e, f,
		g, h, i,
		j, k, l;

	double eimhf,
		gfmdi,
		dhmeg,
		akmjb,
		jcmal,
		blmkc;

	double M;

	xe = ray.pos.myX; ye = ray.pos.myY; ze = ray.pos.myZ;
	xa = vector1.x; ya = vector1.y; za = vector1.z;
	xb = vector2.x; yb = vector2.y; zb = vector2.z;
	xc = vector3.x; yc = vector3.y; zc = vector3.z;
	xd = ray.dir.x; yd = ray.dir.y; zd = ray.dir.z;

	a = xa - xb; b = ya-yb; c = za-zb;
	d = xa - xc; e = ya-yc; f = za-zc;
	g = xd; h = yd; i = zd;
	j = xa - xe; k = ya - ye; l = za - ze;

	eimhf = e*i - h*f; 
	gfmdi = g*f - d*i;
	dhmeg = d*h - e*g;
	akmjb = a*k - j*b;
	jcmal = j*c - a*l;
	blmkc = b*l - k*c;
	
	M = a*(eimhf) + b*(gfmdi) + c*(dhmeg);

	*t_hit = (-((f*akmjb) + (e*jcmal) + (d*blmkc)) / M);
	if (*t_hit < ray.t_min || *t_hit > ray.t_max) {
		return false;
	}
	gamma = ((i*akmjb) + (h*jcmal) + (g*blmkc)) / M;
	if (gamma < 0 || gamma > 1) {
		return false;
	}
	
	beta = ((j*eimhf) + (k*gfmdi) + (l*dhmeg)) / M;
	if (beta < 0 || beta > (1 - gamma)) return false;

	//printf("thit = %f \n", *t_hit);
	
	vec3 result = vec3(xe, ye, ze) + (*t_hit)*(ray.dir);
	local->pos = Point(result.x, result.y, result.z);
	
	if (!spec_normal) {
		local->normal = normal;
	} else {
		double alpha = 1 - beta - gamma;
		local->normal = alpha * n1 + beta * n2 + gamma * n3;
		//vec3 na, nb;

		//na.x = (n1.x * (result.x - v2.myX) + n2.x * (v1.myX - result.x))/(v1.myX - v2.myX);
		//na.y = (n1.y * (result.y - v2.myY) + n2.y * (v1.myY - result.y))/(v1.myY - v2.myY);
		//na.x = (n1.z * (result.z - v2.myZ) + n2.z * (v1.myZ - result.z))/(v1.myZ - v2.myZ);
	
		//nb.x = (n1.x * (result.x - v3.myX) + n3.x * (v1.myX - result.x))/(v1.x - v3.x);
		//nb.y = (n1.y * (result.y - v3.myY) + n3.y * (v1.myY - result.y))/(v1.y - v3.y);
		//nb.z = (n1.z * (result.z - v3.myZ) + n3.z * (v1.myZ - result.z))/(v1.z - v3.z);
		//
		//local->normal.x = na.x * ()
	
	}

	return true;
	
	/*
	if (dot(normal, p1) != 0.0) {
		t_hit = new float (dot(normal, A - p0) / dot(normal, p1));
		if (ray.t_min <= *t_hit && *t_hit <= ray.t_max) {
			local->normal = normal;
			vec3 result = p0 + (*t_hit)*(p1);
			local->pos = Point(result.x, result.y, result.z);
			return true;
		} else { return false; }
	} else { return false; }
	*/
}

bool Triangle::intersectP(Ray& ray) {
	//vec3 p0 = vec3(ray.pos.myX, ray.pos.myY, ray.pos.myZ);
	//vec3 p1 = ray.dir;
	//vec3 A = vec3(vector1);
//	printf("I'm checking if intersect tri\n");
	double xe, ye, ze,
		xa, ya, za,
		xb, yb, zb,
		xc, yc, zc,
		xd, yd, zd;

	double beta, gamma, t_hit;

	double a, b, c,
		d, e, f,
		g, h, i,
		j, k, l;

	double eimhf,
		gfmdi,
		dhmeg,
		akmjb,
		jcmal,
		blmkc;

	double M;

	xe = ray.pos.myX; ye = ray.pos.myY; ze = ray.pos.myZ;
	xa = vector1.x; ya = vector1.y; za = vector1.z;
	xb = vector2.x; yb = vector2.y; zb = vector2.z;
	xc = vector3.x; yc = vector3.y; zc = vector3.z;
	xd = ray.dir.x; yd = ray.dir.y; zd = ray.dir.z;

	a = xa - xb; b = ya-yb; c = za-zb;
	d = xa - xc; e = ya-yc; f = za-zc;
	g = xd; h = yd; i = zd;
	j = xa - xe; k = ya - ye; l = za - ze;

	eimhf = e*i - h*f; 
	gfmdi = g*f - d*i;
	dhmeg = d*h - e*g;
	akmjb = a*k - j*b;
	jcmal = j*c - a*l;
	blmkc = b*l - k*c;
	
	M = a*(eimhf) + b*(gfmdi) + c*(dhmeg);

	t_hit = -((f*akmjb) + (e*jcmal) + (d*blmkc)) / M;
	if (t_hit < ray.t_min || t_hit > ray.t_max) return false;

	gamma = ((i*akmjb) + (h*jcmal) + (g*blmkc)) / M;
	if (gamma < 0 || gamma > 1) return false;
	
	beta = ((j*eimhf) + (k*gfmdi) + (l*dhmeg)) / M;
	if (beta < 0 || beta > (1 - gamma)) return false;
	
	return true;
	


	/*
	if (dot(normal, p1) != 0.0) {
		float t_hit = dot(normal, A - p0) / dot(normal, p1);
		if (ray.t_min <= t_hit && t_hit <= ray.t_max) { 
			return true;
		} else { return false; }
	} else { return false; }
	*/
}
