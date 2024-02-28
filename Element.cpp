#include "Element.h"
#include "Matrix.h"
#include "Scence.h"
#include "Texture.h"
#include "Ray.h"
#include <cmath>
#include <GL/glut.h>

#include "MatrixException.h"

Element::Element(int x) :tran(4, 4), positions(4, x), colors(4, x), UV(2, x), normal(4, x)
{
	n = x;
	tran.get(0, 0) = 1;
	tran.get(1, 1) = 1;
	tran.get(2, 2) = 1;
	tran.get(3, 3) = 1;

	for (int i = 0; i < n; ++i)
	{
		positions.get(3, i) = 1;
	}

	texture = nullptr;

}

Element::~Element()
{
	scence.unregisterElement(this);
	if (parm != nullptr)
		delete[] parm;

	if (__TAKEGLOBEVALUE)
	{
		delete texture;
	}
}

void Element::setUV(float* uv)
{
	UV = uv;
}

void Element::setNormals(float* normals)
{
	for (int i = 0; i < n; ++i)
	{
		normal.get(0, i) = normals[i * 3 + 0];
		normal.get(1, i) = normals[i * 3 + 1];
		normal.get(2, i) = normals[i * 3 + 2];
	}
}

void Element::setTexture(Texture* t)
{
	texture = t;
}

void Element::tranform(const Matrix& b)
{
	if (b.col() != 4 && b.row() != 4)
		throw MatrixException(b, "can't change tranform matrix with improper matrix.");
	tran = b;
}

void Element::move(float x, float y, float z)
{
	Matrix m(4, 4);

	m.get(0, 0) = 1;
	m.get(1, 1) = 1;
	m.get(2, 2) = 1;
	m.get(3, 3) = 1;

	m.get(0, 3) = x;
	m.get(1, 3) = y;
	m.get(2, 3) = z;

	tran ^= m;
}

void Element::rotate(float x, float y, float z)
{
	Matrix m(4, 4);

	float sina = sin(x), cosa = cos(x);
	float sinb = sin(y), cosb = cos(y);
	float sinc = sin(z), cosc = cos(z);

	m.get(0, 0) = cosa * cosb;
	m.get(0, 1) = cosa * cosb;
	m.get(0, 2) = sinb;
	m.get(1, 0) = -sina * cosc + cosa * -sinb * sinc;
	m.get(1, 1) = sina * cosc + cosa * -sinb * sinc;
	m.get(1, 2) = cosb * sinc;
	m.get(2, 0) = -sina * -sinc + cosa * -sinb * cosc;
	m.get(2, 1) = sina * -sinc + cosa * -sinb * cosc;
	m.get(2, 2) = cosb * cosc;
	m.get(3, 3) = 1;

	tran ^= m;

}

void Element::rotate(float x, float y, float z, float w)
{
	float len = sqrt(x * x + y * y + z * z + w * w);
	x = x / len;
	y = y / len;
	z = z / len;
	w = w / len;

	Matrix m(4, 4);
	m.get(0, 0) = 1 - 2 * y * y - 2 * z * z;
	m.get(0, 1) = 2 * x * y - 2 * w * z;
	m.get(0, 2) = 2 * x * z + 2 * w * y;
	m.get(1, 0) = 2 * x * y + 2 * w * z;
	m.get(1, 1) = 1 - 2 * x * x - 2 * z * z;
	m.get(1, 2) = 2 * y * z - 2 * w * x;
	m.get(2, 0) = 2 * x * z - 2 * w * y;
	m.get(2, 1) = 2 * y * z + 2 * w * x;
	m.get(2, 2) = 1 - 2 * x * x - 2 * y * y;
	m.get(3, 3) = 1;

	tran ^= m;
}

void Element::axisRotate(float n, float a, float b, float c)
{
	if (a == 0 && b == 0 && c == 0)//如果旋转轴向量为0向量，那么什么事都不干
		return;

	float len = sqrtf(a * a + b * b + c * c);
	float cosa = cos(n);
	float sina = sin(n);

	a /= len;
	b /= len;
	c /= len;

	Matrix m(4, 4);
	m.get(0, 0) = (1 - cosa) * a * a + cosa;
	m.get(1, 0) = (1 - cosa) * a * b - sina * c;
	m.get(2, 0) = (1 - cosa) * a * c + sina * b;
	m.get(0, 1) = (1 - cosa) * a * b + sina * c;
	m.get(1, 1) = (1 - cosa) * b * b + cosa;
	m.get(2, 1) = (1 - cosa) * b * c - sina * a;
	m.get(0, 2) = (1 - cosa) * a * c - sina * b;
	m.get(1, 2) = (1 - cosa) * b * c + sina * a;
	m.get(2, 2) = (1 - cosa) * c * c + cosa;

	m.get(3, 3) = 1;

	tran ^= m;
}

void Element::setColors(float* cs)
{
	memcpy(colors.get(), cs, sizeof(float) * colors.col() * 4);
}

void Element::setColors(const Matrix& cs)
{
	colors = cs;
}

Matrix Element::tranform() const
{
	Matrix m = scence.topStack() ^ tran ^ positions;
	for (int i = 0; i < m.col(); ++i)
		for (int j = 0; j < 3; ++j)
			m.get(j, i) /= abs(m.get(3, i));
	return m;
}

float Element::averageAlpha()const
{
	float ans = 0;

	for (int i = 0; i < n; ++i)
	{
		ans += colors.get(3, i);
	}

	return ans / n;
}

void Element::setParm(int m, float* p)
{
	if (parm != nullptr)delete[] parm;
	parmm = m;
	parm = new float[parmm * n + 5];
	memcpy(parm, p, (parmm * n) * sizeof(float));

}

void Element::scale(float x, float y, float z)
{
	tran.get(0, 0) *= x;
	tran.get(0, 1) *= x;
	tran.get(0, 2) *= x;
	tran.get(0, 3) *= x;
	tran.get(1, 0) *= y;
	tran.get(1, 1) *= y;
	tran.get(1, 2) *= y;
	tran.get(1, 3) *= y;
	tran.get(2, 0) *= z;
	tran.get(2, 1) *= z;
	tran.get(2, 2) *= z;
	tran.get(2, 3) *= z;
}

bool Element::triangleIntersect(const Ray& ray, float* out) const { return false; }