#include "Particle.h"
#include <random>

#define abs(a) ((a)>0?(a):-(a))
#define min(a1,a2) ((a1)<(a2)?(a1):(a2))
#define max(a1,a2) ((a1)>(a2)?(a1):(a2))
#define RAND(min,max) ((rand()%((min)-(max)+1))+(min))
#define MAY(x) (RAND(0,10000)<=(x)*100)

Particle::Particle(int n) : Element(n), points_v(3, n), orgin_v(3), color(3)
{
	color.get(0) = 1;
	color.get(1) = 1;
	color.get(2) = 1;

	orgin_v.get(1) = 10;
	born_times = new double[n];
	for (int i = 0; i < n; ++i)
	{
		double t;
		t = clock() / 1000.0;
		born_times[i] = (double)t + (death_time * i / n);
		//printf("i=%d t=%.3f\n", i, (death_time * i / n));
	}
}

Particle::~Particle()
{
	delete[] born_times;
}

void Particle::setColor(float r, float b, float g)
{
	color.get(0) = r;
	color.get(1) = g;
	color.get(2) = b;
}

void Particle::setG(float _g)
{
	g = _g;
}

void Particle::setRand(float ran)
{
	random = ran;
}

void Particle::setDeathTime(float t)
{
	death_time = t;
}

void Particle::setSize(float s)
{
	size = s;
}

void Particle::setOrginV(float x, float y, float z)
{
	orgin_v.get(0) = x;
	orgin_v.get(1) = y;
	orgin_v.get(2) = z;
}

void Particle::draw() const
{
	Matrix m = tranform();
	Matrix v = tran ^ normal;
	double t;
	t = clock() / 1000.0;
	//points_v.output();
	//printf("t - death_time = %lld\n", t - death_time);

	for (int i = 0; i < n; ++i)
	{
		float x, y, z;
		x = m.get(0, i);
		y = m.get(1, i);
		z = m.get(2, i);

		//printf("z=%.3f\n", z);
		//printf("t - death_time = %.3f, born_times[%d] = %.3f\n", t - death_time, i, born_times[i]);

		if ((double)t - (double)death_time > born_times[i])
		{
			//printf("boome!");
			born_times[i] += (double)death_time;
			points_v.get(0, i) = orgin_v.get(0) + RAND(-10000, 10000) * 0.001f * random;
			points_v.get(1, i) = orgin_v.get(1) + RAND(-10000, 10000) * 0.001f * random;
			points_v.get(2, i) = orgin_v.get(2) + RAND(-10000, 10000) * 0.001f * random;
			positions.get(0, i) = 0;
			positions.get(1, i) = 0;
			positions.get(2, i) = 0;
		}
		else
		{
			points_v.get(1, i) += -g / 50;
			positions.get(0, i) += points_v.get(0, i) / 300;
			positions.get(1, i) += points_v.get(1, i) / 300;
			positions.get(2, i) += points_v.get(2, i) / 300;
		}

		//printf("%d - %.3f,%.3f,%.3f,%lld\n", i, x, y, z, born_times[i]);


		if (x > 1 || x < -1 || y > 1 || y < -1 || z < -1)
			continue;

		x *= scence.windowWidth;
		y *= scence.windowHigh;

		for (int i1 = (int)(x - size); i1 < (int)(x + size); ++i1)
		{
			for (int i2 = (int)(y - size); i2 < (int)(y + size); ++i2)
			{
				float len = sqrtf((i1 - x) * (i1 - x) + (i2 - y) * (i2 - y));
				scence.setRGBAd(i1, i2, z, color.get(0), color.get(1), color.get(2), max(0.0f, 1.0f - len * light));
			}
		}

	}
}
