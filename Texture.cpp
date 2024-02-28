#include "Texture.h"
#include <gl/glut.h>
#include "MyException.h"

using namespace std;

Texture::Texture()
{
	data = nullptr;
}

Texture::~Texture()
{
	if (data != nullptr)
		delete[] data;
}

Texture::Texture(string path)
{
	data = nullptr;
	readImg(path);
}

void Texture::readImg(string path)
{
	if (data != nullptr)
		delete[] data;
	ifstream f(path);
	if (!f.is_open())
	{
		printf("%s not found!\n", path.c_str());
		data = nullptr;
		return;
	}

	int x, y, t;

	f >> x >> y >> t;
	high = x;
	width = y;

	if (t == 3)type = GL_RGB;
	else if (t == 4)type = GL_RGBA;

	data = new float[x * y * t];

	float point;
	for (int i = 0; i < x * y * t; ++i)
	{
		f >> point;
		if (point <= 1)
			data[i] = point;
		else

			data[i] = point / 255.0f;
	}

	f.close();
}

#define getmap(x,y,i,b) (data[((y)*width+(x))*(b)+(i)]) 
void Texture::getColor(float u, float v, float* colors)
{
	if (data == nullptr)
	{
		colors[0] = 1;
		colors[1] = 1;
		colors[2] = 1;
		colors[3] = 1;
		return;
	}

	u = u < 0 ? 0 : u > 1 ? 1 : u;
	v = v < 0 ? 0 : v > 1 ? 1 : v;

	u *= width;
	v *= high;

	int intu = (int)u;
	int intv = (int)v;

	float du = u - intu;
	float dv = v - intv;

	int x1, x2, y1, y2, t;
	x1 = intu < 0 ? 1 : 0;
	x2 = intu > width ? 0 : 1;
	y1 = intv < 0 ? 1 : 0;
	y2 = intv > high ? 0 : 1;

	switch (type)
	{
	case GL_RGB: t = 3; break;
	case GL_RGBA:t = 4; break;
	default:
		//throw MyException("unknowing color type! texture");
		t = 3;
		break;
	}

	for (int i = 0; i < t; ++i)
	{
		colors[i] = (getmap(intu + x1, intv + y1, i, t) * (1 - du) + getmap(intu + x2, intv + y1, i, t) * (du)) * (1 - dv) +
					(getmap(intu + x1, intv + y2, i, t) * (1 - du) + getmap(intu + x2, intv + y2, i, t) * (du)) * (dv);
	}
}
#undef getmap


void Texture::clear()
{
	delete[] data;
	data = nullptr;
}

