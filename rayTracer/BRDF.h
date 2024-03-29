#pragma once
#include "Color.h"

class BRDF
{
public:
	Color myKd, myKs, myKa, myKr, myKe, myKrefract, myC;
	unsigned int myType;
	float myShininess;
	float myRIndex;
	BRDF(Color kd, Color ks, Color ka, Color kr, Color ke, float shininess, float rindex, Color krefract, unsigned int type);
	BRDF(void);
	~BRDF(void);
	void setEqual(BRDF dest);
};
