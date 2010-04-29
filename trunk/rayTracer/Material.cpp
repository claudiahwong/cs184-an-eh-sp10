#include "Material.h"

Material::Material(BRDF *brdf) 
{
	constantBRDF = new BRDF(brdf->myKd, brdf->myKs, brdf->myKa, brdf->myKr, brdf->myKe, brdf->myShininess);
}

Material::Material(void)
{
}

Material::~Material(void)
{
}

void Material::materialBRDF(LocalGeo &local, BRDF *brdf)
{
	brdf->myKa = constantBRDF->myKa;
	brdf->myKd = constantBRDF->myKd;
	brdf->myKr = constantBRDF->myKr;
	brdf->myKs = constantBRDF->myKs;
	brdf->myKe = constantBRDF->myKe;
	brdf->myShininess = constantBRDF->myShininess;
 }
