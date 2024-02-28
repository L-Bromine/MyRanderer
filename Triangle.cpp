#include "Triangle.h"
#include "function.h"
#include "Ray.h"
#include <gl/glut.h>

Triangle::Triangle(float x1, float y1, float x2, float y2, float x3, float y3) : Element(3)
{
	positions.get(0, 0) = x1;
	positions.get(1, 0) = y1;
	positions.get(2, 0) = 0;
	positions.get(0, 1) = x2;
	positions.get(1, 1) = y2;
	positions.get(2, 1) = 0;
	positions.get(0, 2) = x3;
	positions.get(1, 2) = y3;
	positions.get(2, 2) = 0;
}

Triangle::Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) : Element(3)
{
	positions.get(0, 0) = x1;
	positions.get(1, 0) = y1;
	positions.get(2, 0) = z1;
	positions.get(0, 1) = x2;
	positions.get(1, 1) = y2;
	positions.get(2, 1) = z2;
	positions.get(0, 2) = x3;
	positions.get(1, 2) = y3;
	positions.get(2, 2) = z3;
}

Triangle::Triangle(POINT p1, POINT p2, POINT p3) : Element(3)
{
	positions.get(0, 0) = p1.x;
	positions.get(1, 0) = p1.y;
	positions.get(2, 0) = p1.z;
	positions.get(0, 1) = p2.x;
	positions.get(1, 1) = p2.y;
	positions.get(2, 1) = p2.z;
	positions.get(0, 2) = p3.x;
	positions.get(1, 2) = p3.y;
	positions.get(2, 2) = p3.z;
}

Triangle::Triangle(float* xs, float* ys) : Element(3)
{
	positions.get(0, 0) = xs[0];
	positions.get(1, 0) = ys[0];
	positions.get(2, 0) = 0;
	positions.get(0, 1) = xs[1];
	positions.get(1, 1) = ys[1];
	positions.get(2, 1) = 0;
	positions.get(0, 2) = xs[2];
	positions.get(1, 2) = ys[2];
	positions.get(2, 2) = 0;
}

Triangle::Triangle(float* xs, float* ys, float* zs) : Element(3)
{
	positions.get(0, 0) = xs[0];
	positions.get(1, 0) = ys[0];
	positions.get(2, 0) = zs[0];
	positions.get(0, 1) = xs[1];
	positions.get(1, 1) = ys[1];
	positions.get(2, 1) = zs[1];
	positions.get(0, 2) = xs[2];
	positions.get(1, 2) = ys[2];
	positions.get(2, 2) = zs[2];
}

Triangle::Triangle(POINT* ps) : Element(3)
{
	positions.get(0, 0) = ps[0].x;
	positions.get(1, 0) = ps[0].y;
	positions.get(2, 0) = ps[0].z;
	positions.get(0, 1) = ps[1].x;
	positions.get(1, 1) = ps[1].y;
	positions.get(2, 1) = ps[1].z;
	positions.get(0, 2) = ps[2].x;
	positions.get(1, 2) = ps[2].y;
	positions.get(2, 2) = ps[2].z;
}


bool Triangle::triangleIntersect(const Ray& ray, float* out) const
{
	return ray.triangleIntersect(tranform().get(), out);
}

void Triangle::draw() const
{
	scence.texture = texture;

	Matrix m = tranform();

	//int x1, x2, x3, y1, y2, y3;
	//
	//x1 = m.get(0, 0) * Scence::windowWidth;
	//x2 = m.get(0, 1) * Scence::windowWidth;
	//x3 = m.get(0, 2) * Scence::windowWidth;
	//
	//y1 = m.get(1, 0) * Scence::windowHigh;
	//y2 = m.get(1, 1) * Scence::windowHigh;
	//y3 = m.get(1, 2) * Scence::windowHigh;
	//
	//printf("[(%d,%d,%.3f)(%d,%d,%.3f)(%d,%d,%.3f)]\n", x1, y1, m.get(2, 0), x2, y2, m.get(2, 1), x3, y3, m.get(2, 2));
	//positions.output();
	//scence.topStack().output();
	//tran.output();
	//m.output();
	//float fl[] = {
	//	m.get(2, 0),colors.get(0, 0), colors.get(1, 0), colors.get(2, 0), colors.get(3, 0),
	//	UV.get(0, 0), UV.get(1, 0), normal.get(0, 0), normal.get(1, 0), normal.get(2, 0),
	//	m.get(2, 1),colors.get(0, 1), colors.get(1, 1), colors.get(2, 1), colors.get(3, 1),
	//	UV.get(0, 1), UV.get(1, 1), normal.get(0, 1), normal.get(1, 1), normal.get(2, 1),
	//	m.get(2, 2),colors.get(0, 2), colors.get(1, 2), colors.get(2, 2), colors.get(3, 2),
	//	UV.get(0, 2), UV.get(1, 2), normal.get(0, 2), normal.get(1, 2), normal.get(2, 2)
	//};
	//
	//triangleShader(10, defShader,
	//	x1, y1, fl + 0,
	//	x2, y2, fl + 10,
	//	x3, y3, fl + 20);

	float fl[] = {
	m.get(0, 0), m.get(1, 0), m.get(2, 0), colors.get(0, 0), colors.get(1, 0), colors.get(2, 0), colors.get(3, 0),
	UV.get(0, 0), UV.get(1, 0), normal.get(0, 0), normal.get(1, 0), normal.get(2, 0),
	m.get(0, 1), m.get(1, 1), m.get(2, 1), colors.get(0, 1), colors.get(1, 1), colors.get(2, 1), colors.get(3, 1),
	UV.get(0, 1), UV.get(1, 1), normal.get(0, 1), normal.get(1, 1), normal.get(2, 1),
	m.get(0, 2), m.get(1, 2), m.get(2, 2), colors.get(0, 2), colors.get(1, 2), colors.get(2, 2), colors.get(3, 2),
	UV.get(0, 2), UV.get(1, 2), normal.get(0, 2), normal.get(1, 2), normal.get(2, 2)
	};

	polygonShader(3, 12, defShader, fl);
}

