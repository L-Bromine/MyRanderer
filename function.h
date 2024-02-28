#pragma once
#include <string>
class Matrix;
struct POINT
{
	float x;
	float y;
	float z;
};
struct COLOR
{
	float r;
	float g;
	float b;
	float a = 1;
};

int cutPolygon(int n, int m, float* f, float Xmin, float Xmax, float Ymin, float Ymax, float Zmin, float Zmax);

void defShader(float* output);

void lineColor(int x1, int y1, int x2, int y2, float r1, float g1, float b1, float r2, float g2, float b2);

void triangleColor(int x1, int y1, float z1,
	int x2, int y2, float z2,
	int x3, int y3, float z3,
	float r1, float g1, float b1, float a1,
	float r2, float g2, float b2, float a2,
	float r3, float g3, float b3, float a3);

void triangleShader(int n, void (*func)(float*),
	int x1, int y1, float* parm1,
	int x2, int y2, float* parm2,
	int x3, int y3, float* parm3);

void polygonShader(int m, int n, void (*func)(float*), float* parm, float randf = 1);
