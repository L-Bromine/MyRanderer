#pragma once
#include "Element.h"
#include <time.h>
class Particle :
	public Element
{
private:
	Matrix points_v;
	Matrix orgin_v;
	Matrix color;

	double* born_times;

	float size = 10;
	float light = 0.1f;

	float g = 9.8f;
	float random = 0.1f;
	float death_time = 2;

public:
	Particle(int n);
	~Particle();

	void setColor(float r, float b, float g);
	void setG(float _g);
	void setRand(float ran);
	void setDeathTime(float t);
	void setSize(float s);
	void setOrginV(float x, float y, float z);

	virtual float averageAlpha()const { return 0; };
	virtual void draw() const;
};

