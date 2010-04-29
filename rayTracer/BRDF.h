#pragma once
#include "Color.h"

class BRDF
{
public:
	Color myKd, myKs, myKa, myKr, myKe;
	float myShininess;
	BRDF(Color kd, Color ks, Color ka, Color kr, Color ke, float shininess);
	BRDF(void);
	~BRDF(void);
	void setEqual(BRDF dest);
};
