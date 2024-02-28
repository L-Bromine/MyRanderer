#pragma once
#include "Element.h"
class Line :
	public Element
{
private:
public:
	Line(float x, float y);
	Line(float x, float y, float z);
	Line(float x1, float y1, float x2, float y2);
	Line(float x1, float y1, float z1, float x2, float y2, float z2);
	Line(POINT t1, POINT t2);

	virtual void draw()const;
};

