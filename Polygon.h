#pragma once
#include "Element.h"

class Polygon
	:public Element
{

public:
	Polygon(int n, float* fl);
	virtual void draw() const;
};

