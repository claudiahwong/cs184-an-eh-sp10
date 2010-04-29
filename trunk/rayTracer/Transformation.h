#pragma once
#include "nv_math.h"
#include "nv_algebra.h"
#include "nv_mathdecl.h"
#include "Ray.h"
#include "LocalGeo.h"
#include <stdio.h>

class Transformation
{
public:

	mat4 M, M_inv_t;

	Transformation(mat4& matrix);
	Transformation(void);
	~Transformation(void);

	const Point operator*(const Point &p) 
	{
		vec4 result;
		mult(result, M, vec4(p.myX, p.myY, p.myZ, 1));
		return Point(result.x, result.y, result.z);

	}

	/*const vec3 operator*(const vec3 &v) 
	{
		vec4 result;
		mult(result, M, vec4(v));
		return vec3(result);
	}*/

	const Ray operator*(const Ray &r)
	{
		Point tempPos = r.pos;
		Point pos = (*this) * tempPos;
		vec3 tempDir = r.dir;
		vec4 dir4;
		mult(dir4, M, vec4(tempDir.x, tempDir.y, tempDir.z, 0));
		vec3 dir = vec3(dir4);
		return Ray(pos, dir, r.t_min, r.t_max);
	}

	const LocalGeo operator*(const LocalGeo &l)
	{
		Point pos = (*this) * l.pos;
		vec4 normal4;
		mult(normal4, M_inv_t, vec4((l.normal).x, (l.normal).y, (l.normal).z, 0));
		vec3 normal = vec3(normal4);
		return LocalGeo(pos, normal);
	}

};
