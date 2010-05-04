#include "BRDF.h"

BRDF::BRDF(Color kd, Color ks, Color ka, Color kr, Color ke, float shininess, float rindex, Color krefract)
{
	(this->myKd).setEqual(kd);
	(this->myKs).setEqual(ks);
	(this->myKa).setEqual(ka);
	(this->myKr).setEqual(kr);
	(this->myKe).setEqual(ke);
	(this->myKrefract).setEqual(krefract);
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
	(this->myKrefract).setEqual(dest.myKr);
	myShininess = dest.myShininess;
	myRIndex = dest.myRIndex;
}
