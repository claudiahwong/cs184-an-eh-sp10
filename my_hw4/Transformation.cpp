#include "Transformation.h"

Transformation::Transformation(mat4& matrix)
{
	M = mat4(matrix);
	mat4 tempInverse;
	invert(tempInverse, matrix);
	transpose(M_inv_t, tempInverse);
}

Transformation::Transformation(void)
{
}

Transformation::~Transformation(void)
{
}
