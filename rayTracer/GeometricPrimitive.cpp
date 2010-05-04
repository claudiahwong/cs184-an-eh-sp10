#include "GeometricPrimitive.h"

//GeometricPrimitive::GeometricPrimitive(Shape *shape, Material *mat)
//{
//	myShape = shape;
//	myMat = mat;
//}


GeometricPrimitive::GeometricPrimitive(Shape *shape, Material *mat, Transformation toWorld, Transformation toObj)
{
	myShape = shape;
	myMat = mat;
	objToWorld = toWorld;
	worldToObj = toObj;
}


GeometricPrimitive::~GeometricPrimitive(void)
{
}

//bool GeometricPrimitive::intersect(Ray& ray, double* thit, Intersection* in) {
//	LocalGeo local;
//	if(!myShape->intersect(ray, thit, &local)) return false;
//	in->primitive = this;
//	in->localGeo = local;
//	return true;
//}
//
//bool GeometricPrimitive::intersectP(Ray &ray) {
//	return myShape->intersectP(ray);
//}

bool GeometricPrimitive::intersect(Ray& ray, double* thit, Intersection *in) {
	Ray oray = worldToObj*ray;
	LocalGeo olocal;
	if(!myShape->intersect(oray, thit, &olocal)) return false;
	in->primitive = this;
	in->localGeo.normal = (objToWorld*olocal).normal;
	in->localGeo.pos = (objToWorld*olocal).pos;
	return true;
}

bool GeometricPrimitive::intersectP(Ray& ray) {
	Ray oray = worldToObj*ray;
	return myShape->intersectP(oray);

}

void GeometricPrimitive::getBRDF(LocalGeo &local, BRDF* brdf) {
	myMat->materialBRDF(local, brdf);
}