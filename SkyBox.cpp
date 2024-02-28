#include "SkyBox.h"
#include "Camera.h"
#include "Texture.h"

SkyBox::SkyBox(float size) : Element(24), bkimg("skybox.dat")
{
	this->texture = &bkimg;
	this->size = size;
}

void SkyBox::draw() const
{
	scence.texture = this->texture;
	int w = scence.windowWidth;
	int h = scence.windowHigh;
	float color_temp[5];

	float u, v;
	for (int i = -w / 2; i < w / 2; ++i)
		for (int j = -h / 2; j < h / 2; ++j)
		{
			u = (i * 1.0 / w * size * 3.14159 - scence.mainCamera->AngleY()) / 2 / 3.14159 + 0.5;
			v = ((-j * 1.0 / h * size) * 3.14159 + 3.14159 / 2 - scence.mainCamera->AngleX()) / 3.1415926;

			if (v > 1 || v < 0)u += 0.5;
			if (v > 1)v = 1 - (v - (int)v);
			if (v < 0)v = -(v - (int)v);

			if (u > 1 || u < 0)u -= (int)u;
			if (u < 0)u += 1;

			scence.texture->getColor(u, v, color_temp);
			//printf("drawing backimg vex u = %.2f v = %.2f ,rgb = %.2f,%.2f,%.2f\n", u, v, color_temp[0], color_temp[1], color_temp[2]);
			scence.setRGBAd(i, j, 100, color_temp[0], color_temp[1], color_temp[2], 1);
		}
}
