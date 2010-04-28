#include "Ray.h"

Ray::Ray(void) {

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
