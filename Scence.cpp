#include "Scence.h"
#include <memory.h>
#include <gl/glut.h>
#include "MyException.h"
#include "Element.h"
#include "Camera.h"
#include "Ray.h"

#define getdepth(x,y) (depth[(x) + (y) * width])
#define getcolor(x,y) (data + ((x) + (y) * width) * 3)
#define RAND(min,max) ((rand()%((min)-(max)+1))+(min))
#define MAY(x) (RAND(0,10000)<=(x)*100)

Scence* Scence::__SCENCE = new Scence();
std::vector<Scence*> Scence::scence_list = std::vector<Scence*>(1, Scence::__SCENCE);


int Scence::windowHigh = 0;
int Scence::windowWidth = 0;

void Scence::setPoint(int x, int y, float z, float r, float g, float b, float a)
{
	//超出屏幕，取消绘制
	if (x > width || y > high || y < 0 || x < 0)
		return;

	//深度比当前大 或 深度为负，取消绘制
	if (getdepth(x, y) < z || z < 0)
		return;

	getdepth(x, y) = z;

	getcolor(x, y)[0] = (r * a) * 255 + getcolor(x, y)[0] * (1 - a);
	getcolor(x, y)[1] = (g * a) * 255 + getcolor(x, y)[1] * (1 - a);
	getcolor(x, y)[2] = (b * a) * 255 + getcolor(x, y)[2] * (1 - a);
}

Scence::Scence()
{
	printf("a scence created.\n");

	Matrix m(4, 4);
	m.get(0, 0) = 1;
	m.get(1, 1) = 1;
	m.get(2, 2) = 1;
	m.get(3, 3) = 1;
	matrixStack.push(m);

	len = 0;
	maxView = 2000;

	Ia[0] = 0.2;
	Ia[1] = 0.2;
	Ia[2] = 0.2;
}

Scence::~Scence()
{
	if (len > 0)
	{
		delete[] data;
		delete[] depth;
		delete[] stencl;
	}
	if (__TAKEGLOBEVALUE)
	{
		delete mainCamera;
		for (auto i : elementSet)
			delete i;

	}
}

void Scence::resize(int w, int h)
{
	Scence::windowHigh = h;
	Scence::windowWidth = w;

	width = w;
	width -= w % 4;
	high = h;
	//printf("width=%d high=%d\n", w, h);
	if (w * h > len)
	{
		if (len > 0)
		{
			delete[] data;
			delete[] depth;
			delete[] stencl;
		}
		len = w * h * 2;


		data = new unsigned char[len * 3 + 100];
		depth = new float[len + 100];
		stencl = new unsigned char[len + 100];
	}
	clear();
}

void Scence::clear()
{
	memset(data, 0, width * high * 3);
	memset(stencl, 0, width * high);

	for (int i = 0; i < width * high; ++i)
		depth[i] = maxView;
}

void Scence::setRGB(float _x, float _y, float z, float r, float g, float b)
{
	int x = (_x + 1) / 2 * Scence::windowWidth;
	int y = (_y + 1) / 2 * Scence::windowHigh;

	if (x > width || y > high || y < 0 || x < 0)
		return;


	//深度比当前大，取消绘制
	if (getdepth(x, y) < z || z < 0)
		return;

	getdepth(x, y) = z;

	getcolor(x, y)[0] = r * 255;
	getcolor(x, y)[1] = g * 255;
	getcolor(x, y)[2] = b * 255;
}

void Scence::setRGB(float* ps, float* colors)
{
	int x = (ps[0] + 1) / 2 * Scence::windowWidth;
	int y = (ps[1] + 1) / 2 * Scence::windowHigh;

	if (x > width || y > high || y < 0 || x < 0)
		return;

	//深度比当前大，取消绘制
	if (getdepth(x, y) < ps[2] || ps[2] < 0)
		return;

	getdepth(x, y) = ps[2];

	getcolor(x, y)[0] = colors[0] * 255;
	getcolor(x, y)[1] = colors[1] * 255;
	getcolor(x, y)[2] = colors[2] * 255;
}

void Scence::setRGBA(float _x, float _y, float z, float r, float g, float b, float a)
{
	int x = (_x + 1) / 2 * Scence::windowWidth;
	int y = (_y + 1) / 2 * Scence::windowHigh;

	if (x > width || y > high || y < 0 || x < 0)
		return;
	//深度比当前大，取消绘制
	if (getdepth(x, y) < z || z < 0)
		return;

	getdepth(x, y) = z;

	getcolor(x, y)[0] = (int)((r * 255 * a) + ((1 - a) * getcolor(x, y)[0]));
	getcolor(x, y)[1] = (int)((g * 255 * a) + ((1 - a) * getcolor(x, y)[1]));
	getcolor(x, y)[2] = (int)((b * 255 * a) + ((1 - a) * getcolor(x, y)[2]));
}

void Scence::setRGBA(float* ps, float* colors)
{
	int x = (ps[0] + 1) / 2 * Scence::windowWidth;
	int y = (ps[1] + 1) / 2 * Scence::windowHigh;

	if (x > width || y > high || y < 0 || x < 0)
		return;
	//深度比当前大，取消绘制
	if (getdepth(x, y) < ps[2] || ps[2] < 0)
		return;

	getdepth(x, y) = ps[2];

	getcolor(x, y)[0] = (int)(colors[0] * 255 * colors[3] + ((1 - colors[3]) * getcolor(x, y)[0]));
	getcolor(x, y)[1] = (int)(colors[1] * 255 * colors[3] + ((1 - colors[3]) * getcolor(x, y)[1]));
	getcolor(x, y)[2] = (int)(colors[2] * 255 * colors[3] + ((1 - colors[3]) * getcolor(x, y)[2]));
}

void Scence::setRGBA(float* ps, float* colors, float a)
{
	int x = (ps[0] + 1) / 2 * Scence::windowWidth;
	int y = (ps[1] + 1) / 2 * Scence::windowHigh;

	if (x > width || y > high || y < 0 || x < 0)
		return;

	//深度比当前大，取消绘制
	if (getdepth(x, y) < ps[2] || ps[2] < 0)
		return;

	getdepth(x, y) = ps[2];

	getcolor(x, y)[0] = (int)(colors[0] * 255 * a + ((1 - a) * getcolor(x, y)[0]));
	getcolor(x, y)[1] = (int)(colors[1] * 255 * a + ((1 - a) * getcolor(x, y)[1]));
	getcolor(x, y)[2] = (int)(colors[2] * 255 * a + ((1 - a) * getcolor(x, y)[2]));
}


void Scence::setRGBAd(int x, int y, float z, float r, float g, float b, float a)
{
	x = Scence::windowWidth / 2 + x;
	y = Scence::windowHigh / 2 + y;

	if (x > width || y > high || y < 0 || x < 0)
		return;
	//throw MyException("out of buffer!");

	//如果不是是透明体，消隐
	if (a > 0.999)
	{
		//深度比当前大，取消绘制
		if (getdepth(x, y) < z)
			return;
		getdepth(x, y) = z;

		getcolor(x, y)[0] = r * 255;
		getcolor(x, y)[1] = g * 255;
		getcolor(x, y)[2] = b * 255;
	}
	else
	{
		if (getdepth(x, y) < z)
			return;

		getcolor(x, y)[0] = r * 255 * a + getcolor(x, y)[0] * (1 - a);
		getcolor(x, y)[1] = g * 255 * a + getcolor(x, y)[1] * (1 - a);
		getcolor(x, y)[2] = b * 255 * a + getcolor(x, y)[2] * (1 - a);
	}
}

void Scence::setCamera(float n, float s)
{
	nearFace = n;
	viewSize = s;
}

void Scence::pushStack(const Matrix& m)
{
	matrixStack.push(matrixStack.top() ^ m);
}

void Scence::popStack()
{
	if (matrixStack.size() != 1)
		matrixStack.pop();
}

const Matrix& Scence::topStack()
{
	return matrixStack.top();
}

void Scence::toScence(int i)
{
	if (i > scence_list.size())
		throw MyException("unfound scence!");
	else
	{
		if (i == scence_list.size())
			addScence();
		__SCENCE = scence_list[i];
		scence.resize(Scence::windowWidth, Scence::windowHigh);
	}
}

void Scence::addScence()
{
	scence_list.push_back(new Scence());
}

void Scence::registerElement(Element* e)
{
	Scence::elementSet.push_back(e);
}

void Scence::unregisterElement(Element* e)
{
	bool flg = false;
	std::vector<Element*>::iterator it;
	for (it = Scence::elementSet.begin(); it != Scence::elementSet.end(); ++it)
	{
		if (e == *it)
		{
			flg = true;
			break;
		}
	}
	if (flg)
	{
		Scence::elementSet.erase(it);
		return;
	}
}

float out[105];
void Scence::rayTrace()
{
	mainCamera->lookR();

	for (int i = 0; i < windowWidth; ++i)
		for (int j = 0; j < windowHigh; ++j)
		{
			if (MAY(99.5))continue;

			float x = (i * 1.0f / windowWidth - 0.5f) / viewSize;
			float y = (j * 1.0f / windowHigh - 0.5f) / viewSize;
			Ray ray(0, 0, 0, x, y, nearFace);
			//printf("[%.3f,%.3f]", x, y);

			for (auto e : elementSet)
			{
				if (e->triangleIntersect(ray, out))
				{
					setPoint(i, j, 0, 0, out[1], out[2], 1);
					//printf("[%.3f,%.3f]%.3f\n", x, y, out[0]);
					break;
				}
			}
		}
}

void Scence::drawElement()
{
	if (mainCamera != nullptr)
		mainCamera->lookE();

	//TODO待优化，尽量不要在函数内开空间
	std::vector<Element*> alp;

	for (auto e : elementSet)
	{
		if (e->averageAlpha() < 0.999f)
			alp.push_back(e);
		else
			e->draw();
	}
	for (auto e : alp)
	{
		e->draw();
	}
}

void Scence::show()
{
	if (len == 0)
		throw MyException("Buffer not build!");
	glDrawPixels(width, high, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
	glutSwapBuffers();
	while (matrixStack.size() > 1)
		matrixStack.pop();
	texture = nullptr;
}

void Scence::addLight(const Light& light)
{
	lights.push_back(light);
}

void Scence::setScenceLight(float r, float b, float g)
{
	Ia[0] = r;
	Ia[1] = b;
	Ia[2] = g;
}
