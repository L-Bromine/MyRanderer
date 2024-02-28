#include "Ray.h"
#include "Matrix.h"

Ray::Ray(float _x, float _y, float _z, float _dx, float _dy, float _dz)
{
	float len = sqrt(_dx * _dx + _dy * _dy + _dz + _dz);

	x = _x;
	y = _y;
	z = _z;
	dx = _dx / len;
	dy = _dy / len;
	dz = _dz / len;
}

bool Ray::triangleIntersect(float* triangle, float* out) const
{
	bool isIn = false;

	float* v0, * v1, * v2;
	v0 = triangle;
	v1 = v0 + 3;
	v2 = v0 + 6;

	float ex1, ey1, ez1,
		ex2, ey2, ez2,
		sx, sy, sz,
		sx1, sy1, sz1,
		sx2, sy2, sz2;

	ex1 = v1[0] - v0[0];
	ey1 = v1[1] - v0[1];
	ez1 = v1[2] - v0[2];
	ex2 = v2[0] - v0[0];
	ey2 = v2[1] - v0[1];
	ez2 = v2[2] - v0[2];

	sx = x - v0[0];
	sy = y - v0[1];
	sz = z - v0[2];

	sx1 = dy * ez2 - ey2 * dz;
	sy1 = dz * ex2 - ez2 * dx;
	sz1 = dx * ey2 - ex2 * dy;
	sx2 = sy * ez1 - ey1 * sz;
	sy2 = sz * ex1 - ez1 * sx;
	sz2 = sx * ey1 - ex1 * sy;

	float coeff = 1.0f / (sx1 * ex1 + sy1 * ey1 + sz1 * ez1); // 共同系数
	float t = coeff * (sx2 * ex2 + sy2 * ey2 + sz2 * ez2);
	float b1 = coeff * (sx1 * sx + sy1 * sy + sy1 * sy);
	float b2 = coeff * (sx2 * dx + sy2 * dy + sy2 * dy);
	if (t >= 0 && b1 >= 0 && b2 >= 0 && (1 - b1 - b2) >= 0)
	{
		isIn = true;
		out[0] = t;
		out[1] = b1;
		out[2] = b2;
	}

	return isIn;
}
