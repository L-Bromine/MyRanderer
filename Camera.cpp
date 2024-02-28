#include "Camera.h"
#include "Ray.h"

Camera::Camera() : rot(4, 4), looktran(4, 4), mov(4, 4)
{
	looktran.get(0, 0) = 1;
	looktran.get(1, 1) = 1;
	looktran.get(2, 2) = 1;
	looktran.get(3, 3) = 1;

	rot.get(0, 0) = 1;
	rot.get(1, 1) = 1;
	rot.get(2, 2) = 1;
	rot.get(3, 3) = 1;

	mov.get(0, 0) = 1;
	mov.get(1, 1) = 1;
	mov.get(2, 2) = 1;
	mov.get(3, 3) = 1;
}

void Camera::lookdBox(float n, float f, float size)
{
	nearFace = n;
	viewSize = size;
	looktran.get(0, 0) = 2 * n / size;
	looktran.get(1, 1) = 2 * n / scence.windowHigh * scence.windowWidth / size;
	looktran.get(2, 2) = (n + f) / (n - f);
	looktran.get(3, 3) = 0;
	looktran.get(3, 2) = -1;
	looktran.get(2, 3) = (2 * n * f) / (n - f);
}

void Camera::move(float y, float z, float x)
{
	float sina = sin(angleY);
	float cosa = cos(angleY);

	float xx = x * cosa + y * sina;
	float yy = y * cosa - x * sina;

	tranformPosition(-yy, -z, xx);
}

void Camera::tranformPosition(float x, float y, float z)
{
	//Element::move(-x, -y, -z);
	//x = -x;
	//y = -y;
	//z = -z;

	this->x += x;
	this->y += y;
	this->z += z;

	Matrix m(4, 4);

	m.get(0, 0) = 1;
	m.get(1, 1) = 1;
	m.get(2, 2) = 1;
	m.get(3, 3) = 1;

	m.get(0, 3) = x;
	m.get(1, 3) = y;
	m.get(2, 3) = z;

	mov ^= m;
}

void Camera::rotate(float x, float y, float z)
{
	//Element::rotate(-x, -y, -z);

	bool flg = false;
	angleY -= y;
	if (angleX - x < 1.5 && angleX - x > -1.5)
	{
		angleX -= x;
		flg = true;
	}


	float cosa = cos(angleY), sina = sin(angleY);

	tranformAxisRotate(y, 0, 1, 0);

	if (flg)
		tranformAxisRotate(x, cosa, 0, -sina);

	tranformAxisRotate(z, sina, 0, cosa);


}

void Camera::tranformAxisRotate(float n, float a, float b, float c)
{
	//Element::axisRotate(-n, x, y, z);

	n = -n;

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

	rot ^= m;
}

void Camera::scale(float x, float y, float z)
{
	//Element::scale(1 / x, 1 / y, 1 / z);
	x = 1 / x;
	y = 1 / y;
	z = 1 / y;

	mov.get(0, 0) *= x;
	mov.get(0, 1) *= x;
	mov.get(0, 2) *= x;
	mov.get(0, 3) *= x;
	mov.get(1, 0) *= y;
	mov.get(1, 1) *= y;
	mov.get(1, 2) *= y;
	mov.get(1, 3) *= y;
	mov.get(2, 0) *= z;
	mov.get(2, 1) *= z;
	mov.get(2, 2) *= z;
	mov.get(2, 3) *= z;
}

