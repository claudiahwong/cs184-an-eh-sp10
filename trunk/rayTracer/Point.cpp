#include "Point.h"

Point::Point(void)
{
}

Point::Point(double x, double y, double z)
{
	myX = x;
	myY = y;
	myZ = z;
}

Point::~Point(void)
{
}

double Point::dist(Point p) {

	return sqrt(pow(myX-p.myX, 2) + pow(myY-p.myY, 2) + pow(myZ-p.myZ, 2));

}