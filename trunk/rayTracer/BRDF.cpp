#include "BRDF.h"

BRDF::BRDF(Color kd, Color ks, Color ka, Color kr, Color ke, float shininess, float rindex, Color krefract, unsigned int type)
{
	(this->myKd).setEqual(kd);
	(this->myKs).setEqual(ks);
	(this->myKa).setEqual(ka);
	(this->myKr).setEqual(kr);
	(this->myKe).setEqual(ke);
	(this->myKrefract).setEqual(krefract);
	myType = type;
	myRIndex = rindex;
	myShininess = shininess;
}

BRDF::BRDF(void)
{
	myKd = Color(0, 0, 0);
	myKs = Color(0, 0, 0);
	myKa = Color(0, 0, 0);
	myKr = Color(0, 0, 0);
	myKe = Color(0, 0, 0);
	myC = Color(0, 0, 0);
	myType = 0;
	myKrefract = Color(0, 0, 0);
	myRIndex = 1;
	myShininess = 0;
}

BRDF::~BRDF(void)
{
}

void BRDF::setEqual(BRDF dest) {
	(this->myKd).setEqual(dest.myKd);
	(this->myKs).setEqual(dest.myKs);
	(this->myKa).setEqual(dest.myKa);
	(this->myKr).setEqual(dest.myKr);
	(this->myKe).setEqual(dest.myKe);
	(this->myC).setEqual(dest.myC);
	(this->myKrefract).setEqual(dest.myKr);
	myType = dest.myType;
	myShininess = dest.myShininess;
	myRIndex = dest.myRIndex;
}
