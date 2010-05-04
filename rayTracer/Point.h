#pragma once
#include <Math.h>

class Point
{
public:
	double myX, myY, myZ;

	double dist(Point p);

	Point(void);
	Point(double x, double y, double z);
	~Point(void);
};
