#include "Light.h"

Light::Light()
{
	x = 0; y = 0; z = 0;
	idr = 0; idg = 0; idb = 0;
	ipr = 0; ipg = 0; ipb = 0;
}

void Light::setXYZ(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Light::setIdp(float r, float g, float b)
{
	idr = ipr = r;
	idg = ipg = g;
	idb = ipb = b;
}
