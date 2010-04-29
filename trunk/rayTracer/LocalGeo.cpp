#include "LocalGeo.h"

LocalGeo::LocalGeo(Point p, vec3 norm)
{
	pos.myX = p.myX;
	pos.myY = p.myY;
	pos.myZ = p.myZ;
	normal = norm;
}

LocalGeo::LocalGeo(void)
{
}

LocalGeo::~LocalGeo(void)
{
}
