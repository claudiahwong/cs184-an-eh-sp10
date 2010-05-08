#include "Ray.h"

Ray::Ray(void) {
	pos.myX = 0;
	pos.myY = 0;
	pos.myZ = 0;
	dir = vec3(0, 0, 0);
	t_min = 0;
	t_max = 8000;
}

Ray::Ray(Point position, vec3 direction, double min, double max)
{
	pos.myX = position.myX;
	pos.myY = position.myY;
	pos.myZ = position.myZ;
	dir = direction;
	t_min = min;
	t_max = max;
}

Ray::~Ray(void)
{
}
