#pragma once
#include "BRDF.h"
#include "LocalGeo.h"

class Material
{
public:
	
	BRDF *constantBRDF;

	Material(BRDF *brdf);
	Material(void);
	~Material(void);

	void materialBRDF(LocalGeo &local, BRDF *brdf);
};
