#include "function.h"
#include <gl/glut.h>
#include "Element.h"
#include "Scence.h" 
#include <random>

void defShader(float* output)
{
	float color_temp[5];
	if (scence.texture)
	{
		scence.texture->getColor(output[7], output[8], color_temp);
		output[3] *= color_temp[0];
		output[4] *= color_temp[1];
		output[5] *= color_temp[2];
	}
}

#define abs(a) ((a)>0?(a):-(a))
#define min(a1,a2) ((a1)<(a2)?(a1):(a2))
#define max(a1,a2) ((a1)>(a2)?(a1):(a2))
#define max3(a1,a2,a3) max(a1,max(a2,a3))
#define min3(a1,a2,a3) min(a1,min(a2,a3))
#define swap(type,a1,a2) {type temp = a1;a1 = a2;a2 = temp;}
#define swapPoint(x1,y1,x2,y2) {swap(int,x1,x2)swap(int,y1,y2)}
#define swapColor(r1,g1,b1,r2,g2,b2) {swap(float,r1,r2)swap(float,g1,g2)swap(float,b1,b2)}
#define setColor(cl,base,i,r,g,b,a) {(cl)[(i)]=(r);(cl)[(base)+(i)]=(g);(cl)[((base)<<1)+(i)]=(b);(cl)[((base)<<1)+base+(i)]=(a);}

int getArea(float x, float y, float z, float Xmin, float Xmax, float Ymin, float Ymax, float Zmin, float Zmax)
{
	int out = 0;
	if (x < Xmin)out |= 1;
	if (x > Xmax)out |= 2;
	if (y < Ymin)out |= 4;
	if (y > Ymax)out |= 8;
	if (z < Zmin)out |= 16;
	if (z > Zmax)out |= 32;
	return out;
}

int cutLine(float& x0, float& y0, float& z0, float& x1, float& y1, float& z1,
	float Xmin, float Xmax, float Ymin, float Ymax, float Zmin, float Zmax)
{
	float tx0 = x0, tx1 = x1, ty0 = y0, ty1 = y1, tz0 = z0, tz1 = z1;
	bool chf = 0;
	while (1)
	{
		int c0 = getArea(tx0, ty0, tz0, Xmin, Xmax, Ymin, Ymax, Zmin, Zmax);
		int c1 = getArea(tx1, ty1, tz1, Xmin, Xmax, Ymin, Ymax, Zmin, Zmax);

		if ((c0 | c1) == 0)
		{
			if (chf)
			{
				x1 = tx0;
				x0 = tx1;
				y1 = ty0;
				y0 = ty1;
				z1 = tz0;
				z0 = tz1;
			}
			else
			{
				x0 = tx0;
				x1 = tx1;
				y0 = ty0;
				y1 = ty1;
				z0 = tz0;
				z1 = tz1;
			}

			return 1;
		}
		else if ((c0 & c1) != 0)
		{
			return 0;
		}
		else
		{
			if (c0 == 0)
			{
				swap(int, c0, c1);
				swap(float, tx0, tx1);
				swap(float, ty0, ty1);
				swap(float, tz0, tz1);
				chf = !chf;
			}

			if (c0 & 1)
			{
				ty0 = ty0 + ((Xmin - tx0) * (ty0 - ty1) / (tx0 - tx1));
				tz0 = tz0 + ((Xmin - tx0) * (tz0 - tz1) / (tx0 - tx1));
				tx0 = Xmin;
			}
			if (c0 & 2)
			{
				ty0 = ty0 + ((Xmax - tx0) * (ty0 - ty1) / (tx0 - tx1));
				tz0 = tz0 + ((Xmax - tx0) * (tz0 - tz1) / (tx0 - tx1));
				tx0 = Xmax;
			}
			if (c0 & 4)
			{
				tx0 = tx0 + ((Ymin - ty0) * (tx0 - tx1) / (ty0 - ty1));
				tz0 = tz0 + ((Ymin - ty0) * (tz0 - tz1) / (ty0 - ty1));
				ty0 = Ymin;
			}
			if (c0 & 8)
			{
				tx0 = tx0 + ((Ymax - ty0) * (tx0 - tx1) / (ty0 - ty1));
				tz0 = tz0 + ((Ymax - ty0) * (tz0 - tz1) / (ty0 - ty1));
				ty0 = Ymax;
			}
			if (c0 & 16)
			{
				tx0 = tx0 + ((Zmin - tz0) * (tx0 - tx1) / (tz0 - tz1));
				ty0 = ty0 + ((Zmin - tz0) * (ty0 - ty1) / (tz0 - tz1));
				tz0 = Zmin;
			}
			if (c0 & 32)
			{
				tx0 = tx0 + ((Zmax - tz0) * (tx0 - tx1) / (tz0 - tz1));
				ty0 = ty0 + ((Zmax - tz0) * (ty0 - ty1) / (tz0 - tz1));
				tz0 = Zmax;
			}

			//if (c0)
			//{
			//	swap(int, c0, c1);
			//	swap(float, tx0, tx1);
			//	swap(float, ty0, ty1);
			//}
		}
	}
}

int cutLine(float& x0, float& y0, float& z0, float& x1, float& y1, float& z1,
	int n, float* parm0, float* parm1, float Xmin, float Xmax, float Ymin, float Ymax, float Zmin, float Zmax)
{
	float tx0 = x0, tx1 = x1, ty0 = y0, ty1 = y1, tz0 = z0, tz1 = z1;
	bool chf = 0;
	while (1)
	{
		int c0 = getArea(tx0, ty0, tz0, Xmin, Xmax, Ymin, Ymax, Zmin, Zmax);
		int c1 = getArea(tx1, ty1, tz1, Xmin, Xmax, Ymin, Ymax, Zmin, Zmax);

		if ((c0 | c1) == 0)
		{
			float k0, k1;
			if (chf)
			{
				if (abs(x0 - x1) > 1e-6)
				{
					k0 = (tx1 - x1) / (x0 - x1);
					k1 = (x0 - tx0) / (x0 - x1);
				}
				else if (abs(y0 - y1) > 1e-6)
				{
					k0 = (ty1 - y1) / (y0 - y1);
					k1 = (y0 - ty0) / (y0 - y1);
				}
				else
				{
					return 2;
				}
				x1 = tx0;
				x0 = tx1;
				y1 = ty0;
				y0 = ty1;
				z1 = tz0;
				z0 = tz1;
			}
			else
			{
				if (abs(x0 - x1) > 1e-6)
				{
					k0 = (tx0 - x1) / (x0 - x1);
					k1 = (x0 - tx1) / (x0 - x1);
				}
				else if (abs(y0 - y1) > 1e-6)
				{
					k0 = (ty0 - y1) / (y0 - y1);
					k1 = (y0 - ty1) / (y0 - y1);
				}
				else
				{
					return 2;
				}
				x0 = tx0;
				x1 = tx1;
				y0 = ty0;
				y1 = ty1;
				z0 = tz0;
				z1 = tz1;
			}
			float p0, p1;

			for (int i = 0; i < n; ++i)
			{
				p0 = parm0[i];
				p1 = parm1[i];

				parm0[i] = k0 * p0 + (1 - k0) * p1;
				parm1[i] = (1 - k1) * p0 + k1 * p1;
			}
			return 1;
		}
		else if ((c0 & c1) != 0)
		{
			return 0;
		}
		else
		{
			if (c0 == 0)
			{
				swap(int, c0, c1);
				swap(float, tx0, tx1);
				swap(float, ty0, ty1);
				swap(float, tz0, tz1);
				chf = !chf;
			}

			if (c0 & 1)
			{
				ty0 = ty0 + ((Xmin - tx0) * (ty0 - ty1) / (tx0 - tx1));
				tz0 = tz0 + ((Xmin - tx0) * (tz0 - tz1) / (tx0 - tx1));
				tx0 = Xmin;
			}
			if (c0 & 2)
			{
				ty0 = ty0 + ((Xmax - tx0) * (ty0 - ty1) / (tx0 - tx1));
				tz0 = tz0 + ((Xmax - tx0) * (tz0 - tz1) / (tx0 - tx1));
				tx0 = Xmax;
			}
			if (c0 & 4)
			{
				tx0 = tx0 + ((Ymin - ty0) * (tx0 - tx1) / (ty0 - ty1));
				tz0 = tz0 + ((Ymin - ty0) * (tz0 - tz1) / (ty0 - ty1));
				ty0 = Ymin;
			}
			if (c0 & 8)
			{
				tx0 = tx0 + ((Ymax - ty0) * (tx0 - tx1) / (ty0 - ty1));
				tz0 = tz0 + ((Ymax - ty0) * (tz0 - tz1) / (ty0 - ty1));
				ty0 = Ymax;
			}
			if (c0 & 16)
			{
				tx0 = tx0 + ((Zmin - tz0) * (tx0 - tx1) / (tz0 - tz1));
				ty0 = ty0 + ((Zmin - tz0) * (ty0 - ty1) / (tz0 - tz1));
				tz0 = Zmin;
			}
			if (c0 & 32)
			{
				tx0 = tx0 + ((Zmax - tz0) * (tx0 - tx1) / (tz0 - tz1));
				ty0 = ty0 + ((Zmax - tz0) * (ty0 - ty1) / (tz0 - tz1));
				tz0 = Zmax;
			}
		}
	}
}

int cutPolygon(int n, int m, float* f, float Xmin, float Xmax, float Ymin, float Ymax, float Zmin, float Zmax)
{
	int size = n * m;
	float* tf = new float[size * 2 + 5];

	memcpy(tf, f, size * sizeof(float));
	memcpy(tf + size, f, size * sizeof(float));

	int count = 0;
	for (int i = 0; i < n; ++i)
	{
		float x1, y1, x2, y2, z1, z2;
		int nexti = i + 1 >= n ? 0 : i + 1;

		x1 = tf[i * m + 0];
		y1 = tf[i * m + 1];
		z1 = tf[i * m + 2];
		x2 = tf[nexti * m + 0];
		y2 = tf[nexti * m + 1];
		z2 = tf[nexti * m + 2];

		int flg = cutLine(x1, y1, z1, x2, y2, z2,
			m, tf + i * m, tf + nexti * m + size, Xmin, Xmax, Ymin, Ymax, Zmin, Zmax);


		if (flg == 1)
		{
			if (count == 0)
			{
				memcpy(f, tf + m * i, m * sizeof(float));
				memcpy(f + m, tf + m * nexti + size, m * sizeof(float));
				count += 2;
			}
			else
			{
				int j = count - 1;
				if (abs(f[m * j + 0] - x1) > 1e-4 ||
					abs(f[m * j + 1] - y1) > 1e-4 ||
					abs(f[m * j + 2] - z1) > 1e-4)
				{
					memcpy(f + m * count, tf + m * i, m * sizeof(float));
					++count;
				}
				memcpy(f + m * count, tf + m * nexti + size, m * sizeof(float));
				++count;
			}
		}
		else if (flg == 2)
		{
			memcpy(f + m * count, tf + m * nexti + size, m * sizeof(float));
			++count;
		}
	}
	if (count != 0)
		if (abs(f[m * (count - 1) + 0] - f[0]) < 1e-4 &&
			abs(f[m * (count - 1) + 1] - f[1]) < 1e-4 &&
			abs(f[m * (count - 1) + 2] - f[2]) < 1e-4)
			--count;
	delete[] tf;
	return count;
}


void lineColor(int x1, int y1, int x2, int y2, float r1, float g1, float b1, float r2, float g2, float b2)
{
	int x, y, dx, dy, d, d1, d2, xm, ym;
	float dr, dg, db, r, g, b;

	if (x1 > x2)
	{
		xm = x1;
		ym = y1;

		dr = r1 - r2;
		dg = g1 - g2;
		db = b1 - b2;

		r = r2;
		g = g2;
		b = b2;

		x = x2;
		y = y2;

		dx = x1 - x2;
		dy = y1 - y2;
	}
	else
	{
		xm = x2;
		ym = y2;

		dr = r2 - r1;
		dg = g2 - g1;
		db = b2 - b1;

		r = r1;
		g = g1;
		b = b1;

		x = x1;
		y = y1;

		dx = x2 - x1;
		dy = y2 - y1;
	}

	dr /= (dx > dy ? dx : dy);
	dg /= (dx > dy ? dx : dy);
	db /= (dx > dy ? dx : dy);


	glBegin(GL_POINTS);
	if (dy < 0)
		if (dx > -dy)
		{
			d = (-dy << 1) - dx;
			d1 = -dy << 1;
			d2 = (-dy - dx) << 1;

			glColor3f(r, g, b);
			glVertex2d(x, y);
			while (x < xm)
			{
				++x;

				r += dr;
				b += db;
				g += dg;

				if (d < 0)
					d += d1;
				else
				{
					--y;
					d += d2;
				}
				glColor3f(r, g, b);
				glVertex2d(x, y);
			}
		}
		else
		{
			d = (dx << 1) + dy;
			d1 = dx << 1;
			d2 = (dx + dy) << 1;

			glColor3f(r, g, b);
			glVertex2d(x, y);
			while (y > ym)
			{
				--y;

				r += dr;
				b += db;
				g += dg;

				if (d < 0)
					d += d1;
				else
				{
					++x;
					d += d2;
				}
				glColor3f(r, g, b);
				glVertex2d(x, y);
			}
		}
	else
		if (dx > dy)
		{
			d = (dy << 1) - dx;
			d1 = dy << 1;
			d2 = (dy - dx) << 1;

			glColor3f(r, g, b);
			glVertex2d(x, y);
			while (x < xm)
			{
				++x;

				r += dr;
				b += db;
				g += dg;

				if (d < 0)
					d += d1;
				else
				{
					++y;
					d += d2;
				}
				glColor3f(r, g, b);
				glVertex2d(x, y);
			}
		}
		else
		{
			d = (dx << 1) - dy;
			d1 = dx << 1;
			d2 = (dx - dy) << 1;

			glColor3f(r, g, b);
			glVertex2d(x, y);
			while (y < ym)
			{
				++y;

				r += dr;
				b += db;
				g += dg;

				if (d < 0)
					d += d1;
				else
				{
					++x;
					d += d2;
				}
				glColor3f(r, g, b);
				glVertex2d(x, y);
			}
		}
	glEnd();
}


//void setlineinT(int* l, float* cl, float* zl, int bx, int base,
//	int x1, int y1, float z1,
//	int x2, int y2, float z2,
//	float r1, float g1, float b1, float a1,
//	float r2, float g2, float b2, float a2)
#define setlineinT(l,cl,zl,bx,base,					\
			x1,y1,z1,										\
			x2,y2,z2,										\
			r1,g1,b1,a1,									\
			r2,g2,b2,a2)									\
{															\
	int x, y, dx, dy, d, d1, d2;							\
	float dr, dg, db, r, g, b;								\
	float dz, z, da, a;										\
															\
	dr = r2 - r1;											\
	dg = g2 - g1;											\
	db = b2 - b1;											\
	dz = z2 - z1;											\
	da = a2 - a1;											\
															\
	r = r1;													\
	g = g1;													\
	b = b1;													\
	z = z1;													\
	a = a1;													\
															\
	dx = x2 - x1;											\
	dy = y2 - y1;											\
															\
	dr /= max(dx, abs(dy));									\
	dg /= max(dx, abs(dy));									\
	db /= max(dx, abs(dy));									\
	dz /= max(dx, abs(dy));									\
	da /= max(dx, abs(dy));									\
															\
	x = x1;													\
	y = y1;													\
															\
	if (dy < 0)												\
		if (dx > -dy)										\
		{													\
			d = (-dy << 1) - dx;							\
			d1 = -dy << 1;									\
			d2 = (-dy - dx) << 1;							\
															\
			setColor(cl, base, x - bx, r, g, b, a);			\
			l[x - bx] = y;									\
			zl[x - bx] = z;									\
															\
			while (x < x2)									\
			{												\
				++x;										\
															\
				r += dr;									\
				b += db;									\
				g += dg;									\
				z += dz;									\
				a += da;									\
															\
				if (d < 0)									\
					d += d1;								\
				else										\
				{											\
					--y;									\
					d += d2;								\
				}											\
				setColor(cl, base, x - bx, r, g, b, a);		\
				l[x - bx] = y;								\
				zl[x - bx] = z;								\
															\
			}												\
		}													\
		else												\
		{													\
			d = (dx << 1) + dy;								\
			d1 = dx << 1;									\
			d2 = (dx + dy) << 1;							\
															\
			setColor(cl, base, x - bx, r, g, b, a);			\
			l[x - bx] = y;									\
			zl[x - bx] = z;									\
															\
			while (y > y2)									\
			{												\
				--y;										\
															\
				r += dr;									\
				b += db;									\
				g += dg;									\
				z += dz;									\
				a += da;									\
															\
				if (d < 0)									\
					d += d1;								\
				else										\
				{											\
					++x;									\
					d += d2;								\
				}											\
				setColor(cl, base, x - bx, r, g, b, a);		\
				l[x - bx] = y;								\
				zl[x - bx] = z;								\
															\
			}												\
		}													\
	else													\
		if (dx > dy)										\
		{													\
			d = (dy << 1) - dx;								\
			d1 = dy << 1;									\
			d2 = (dy - dx) << 1;							\
															\
			setColor(cl, base, x - bx, r, g, b, a);			\
			l[x - bx] = y;									\
			zl[x - bx] = z;									\
															\
			while (x < x2)									\
			{												\
				++x;										\
															\
				r += dr;									\
				b += db;									\
				g += dg;									\
				z += dz;									\
				a += da;									\
															\
				if (d < 0)									\
					d += d1;								\
				else										\
				{											\
					++y;									\
					d += d2;								\
				}											\
				setColor(cl, base, x - bx, r, g, b, a);		\
				l[x - bx] = y;								\
				zl[x - bx] = z;								\
															\
			}												\
		}													\
		else												\
		{													\
			d = (dx << 1) - dy;								\
			d1 = dx << 1;									\
			d2 = (dx - dy) << 1;							\
															\
			setColor(cl, base, x - bx, r, g, b, a);			\
			l[x - bx] = y;									\
			zl[x - bx] = z;									\
															\
			while (y < y2)									\
			{												\
				++y;										\
															\
				r += dr;									\
				b += db;									\
				g += dg;									\
				z += dz;									\
				a += da;									\
															\
				if (d < 0)									\
					d += d1;								\
				else										\
				{											\
					++x;									\
					d += d2;								\
				}											\
				setColor(cl, base, x - bx, r, g, b, a);		\
				l[x - bx] = y;								\
				zl[x - bx] = z;								\
															\
			}												\
		}													\
}

bool firstShow = true;
int globeListSize = 0;

int* l1, * l2, * ltemp;
float* zl1, * zl2;
float* cl1, * cl2;

void triangleColor(int x1, int y1, float z1,
	int x2, int y2, float z2,
	int x3, int y3, float z3,
	float r1, float g1, float b1, float a1,
	float r2, float g2, float b2, float a2,
	float r3, float g3, float b3, float a3)
{
	int bx, maxx, i, size;

	bx = min3(x1, x2, x3);
	maxx = max3(x1, x2, x3);
	size = maxx - bx;

	if (maxx > globeListSize)
	{
		if (firstShow)
		{
			firstShow = false;
		}
		else
		{
			delete[] l1;
			delete[] l2;
			delete[] zl1;
			delete[] zl2;
			delete[] cl1;
			delete[] cl2;
		}
		globeListSize = maxx;
		l1 = new int[maxx + 5];
		l2 = new int[maxx + 5];
		zl1 = new float[maxx + 5];
		zl2 = new float[maxx + 5];
		cl1 = new float[maxx * 4 + 5];
		cl2 = new float[maxx * 4 + 5];
	}

	if (x1 > x3) { swapPoint(x1, y1, x3, y3)swapColor(r1, g1, b1, r3, g3, b3)swap(float, z1, z3)swap(float, a1, a3) }
	if (x2 > x3) { swapPoint(x2, y2, x3, y3)swapColor(r2, g2, b2, r3, g3, b3)swap(float, z2, z3)swap(float, a2, a3) }
	if (x1 > x2) { swapPoint(x1, y1, x2, y2)swapColor(r1, g1, b1, r2, g2, b2)swap(float, z1, z2)swap(float, a1, a2) }

	if (y1 > y2)
	{
		setlineinT(l2, cl2, zl2, bx, size, x1, y1, z1, x3, y3, z3, r1, g1, b1, a1, r3, g3, b3, a3);
		setlineinT(l1, cl1, zl1, bx, size, x1, y1, z1, x2, y2, z2, r1, g1, b1, a1, r2, g2, b2, a2);
		setlineinT(l1, cl1, zl1, bx, size, x2, y2, z2, x3, y3, z3, r2, g2, b2, a2, r3, g3, b3, a3);
	}
	else
	{
		setlineinT(l1, cl1, zl1, bx, size, x1, y1, z1, x3, y3, z3, r1, g1, b1, a1, r3, g3, b3, a3);
		setlineinT(l2, cl2, zl2, bx, size, x1, y1, z1, x2, y2, z2, r1, g1, b1, a1, r2, g2, b2, a2);
		setlineinT(l2, cl2, zl2, bx, size, x2, y2, z2, x3, y3, z3, r2, g2, b2, a2, r3, g3, b3, a3);
	}

	//float y0, yn;
	//float r0, g0, b0, rn, gn, bn;

	//glBegin(GL_LINES);
	//for (i = 0; i < size; ++i)
	//{
	//	y0 = l1[i];
	//	yn = l2[i];

	//	r0 = cl1[i];
	//	g0 = cl1[i + size];
	//	b0 = cl1[i + (size << 1)];

	//	rn = cl2[i];
	//	gn = cl2[i + size];
	//	bn = cl2[i + (size << 1)];

	//	glColor3f(r0, g0, b0);
	//	glVertex2f((i + bx) * 1.0 / Scence::windowWidth, y0 / Scence::windowHigh);
	//	glColor3f(rn, gn, bn);
	//	glVertex2f((i + bx) * 1.0 / Scence::windowWidth, yn / Scence::windowHigh);

	//}
	int y0, yn, j;
	float r0, g0, b0, a0, rn, gn, bn, an, dr, dg, db, da, r, g, b, a;
	float z0, zn, dz, z;

	for (i = 0; i < size; ++i)
	{
		y0 = l1[i];
		yn = l2[i];

		r0 = cl1[i];
		g0 = cl1[i + size];
		b0 = cl1[i + (size << 1)];
		a0 = cl1[i + (size << 1) + size];
		z0 = zl1[i];

		rn = cl2[i];
		gn = cl2[i + size];
		bn = cl2[i + (size << 1)];
		an = cl2[i + (size << 1) + size];
		zn = zl2[i];

		dr = (rn - r0) / (yn - y0);
		dg = (gn - g0) / (yn - y0);
		db = (bn - b0) / (yn - y0);
		da = (an - a0) / (yn - y0);
		dz = (zn - z0) / (yn - y0);

		r = r0;
		g = g0;
		b = b0;
		z = z0;
		a = a0;
		//printf("(%d,%d)(%.3f,%.3f,%.3f,%.3f=>%.3f,%.3f,%.3f,%.3f)\n", y0, yn, r0, g0, b0, z0, rn, g0, b0, z0);
		//printf("(%d,%d)\n", y0, yn);


		if (y0 < yn)
		{
			for (j = y0; j < yn; ++j)
			{
				scence.setRGBAd((i + bx), j, z, r, g, b, a);
				r += dr;
				b += db;
				g += dg;
				z += dz;
				a += da;
			}
		}
		else
		{
			for (j = y0; j > yn; --j)
			{
				scence.setRGBAd((i + bx), j, z, r, g, b, a);
				r -= dr;
				b -= db;
				g -= dg;
				z -= dz;
				a -= da;
			}
		}
	}
}


bool firstShowN = true;
int globeListSizeN = 0;
float* fl1, * fl2, * fltemp;
float l0[505], ln[505], dl[505], il[505];

void setlineinTl(int* l, int n, float* fl, int bx, int base,
	int x1, int y1, float* p1,
	int x2, int y2, float* p2)
	//#define setlineinTl(l,cl,zl,al,bx,base,					
	//		x1,y1,z1,										
	//		x2,y2,z2,										
	//		r1,g1,b1,a1,									
	//		r2,g2,b2,a2)								
{
	int x, y, dx, dy, d, d1, d2;
	int i;

	dx = x2 - x1;
	dy = y2 - y1;

	for (i = 0; i < n; ++i)
	{
		dl[i] = p2[i] - p1[i];
		il[i] = p1[i];
		dl[i] /= max(dx, abs(dy));
	}

	x = x1;
	y = y1;

	if (dy < 0)
		if (dx > -dy)
		{
			d = (-dy << 1) - dx;
			d1 = -dy << 1;
			d2 = (-dy - dx) << 1;

			for (i = 0; i < n; ++i)fl[(x - bx) * n + i] = il[i];
			l[x - bx] = y;

			while (x < x2)
			{
				++x;
				if (d < 0)
					d += d1;
				else
				{
					--y;
					d += d2;
				}
				for (i = 0; i < n; ++i)
				{
					il[i] += dl[i];
					fl[(x - bx) * n + i] = il[i];
				}
				l[x - bx] = y;

			}
		}
		else
		{
			d = (dx << 1) + dy;
			d1 = dx << 1;
			d2 = (dx + dy) << 1;

			for (i = 0; i < n; ++i)fl[(x - bx) * n + i] = il[i];
			l[x - bx] = y;

			while (y > y2)
			{
				--y;

				if (d < 0)
					d += d1;
				else
				{
					++x;
					d += d2;
				}
				for (i = 0; i < n; ++i)
				{
					il[i] += dl[i];
					fl[(x - bx) * n + i] = il[i];
				}
				l[x - bx] = y;

			}
		}
	else
		if (dx > dy)
		{
			d = (dy << 1) - dx;
			d1 = dy << 1;
			d2 = (dy - dx) << 1;

			for (i = 0; i < n; ++i)fl[(x - bx) * n + i] = il[i];
			l[x - bx] = y;

			while (x < x2)
			{
				++x;

				if (d < 0)
					d += d1;
				else
				{
					++y;
					d += d2;
				}
				for (i = 0; i < n; ++i)
				{
					il[i] += dl[i];
					fl[(x - bx) * n + i] = il[i];
				}
				l[x - bx] = y;

			}
		}
		else
		{
			d = (dx << 1) - dy;
			d1 = dx << 1;
			d2 = (dx - dy) << 1;

			for (i = 0; i < n; ++i)fl[(x - bx) * n + i] = il[i];
			l[x - bx] = y;

			while (y < y2)
			{
				++y;

				if (d < 0)
					d += d1;
				else
				{
					++x;
					d += d2;
				}
				for (i = 0; i < n; ++i)
				{
					il[i] += dl[i];
					fl[(x - bx) * n + i] = il[i];
				}
				l[x - bx] = y;

			}
		}
}

float output_temp[105];

void triangleShader(int n, void (*func)(float*),
	int x1, int y1, float* parm1,
	int x2, int y2, float* parm2,
	int x3, int y3, float* parm3)
{
	int bx, maxx, i, size;

	bx = min3(x1, x2, x3);
	maxx = max3(x1, x2, x3);

	size = maxx - bx;

	//printf("size = %d, n = %d, globe size * n = %d\n", size, n, globeListSizeN);
	if (size * n >= globeListSizeN)
	{
		if (firstShowN)
		{
			firstShowN = false;
		}
		else
		{
			delete[] l1;
			delete[] l2;
			delete[] fl1;
			delete[] fl2;
			delete[] ltemp;
			delete[] fltemp;
		}

		globeListSizeN = size * n;

		l1 = new int[size + 5];
		l2 = new int[size + 5];
		ltemp = new int[size + 5];
		fl1 = new float[(size + 1) * n + 5];
		fl2 = new float[(size + 1) * n + 5];
		fltemp = new float[(size + 1) * n + 5];
	}


	if (x1 > x3) { swapPoint(x1, y1, x3, y3)swap(float*, parm1, parm3) }
	if (x2 > x3) { swapPoint(x2, y2, x3, y3)swap(float*, parm2, parm3) }
	if (x1 > x2) { swapPoint(x1, y1, x2, y2)swap(float*, parm1, parm2) }

	if (y1 > y2)
	{
		setlineinTl(l2, n, fl2, bx, size, x1, y1, parm1, x3, y3, parm3);
		setlineinTl(l1, n, fl1, bx, size, x1, y1, parm1, x2, y2, parm2);
		setlineinTl(l1, n, fl1, bx, size, x2, y2, parm2, x3, y3, parm3);
	}
	else
	{
		setlineinTl(l1, n, fl1, bx, size, x1, y1, parm1, x3, y3, parm3);
		setlineinTl(l2, n, fl2, bx, size, x1, y1, parm1, x2, y2, parm2);
		setlineinTl(l2, n, fl2, bx, size, x2, y2, parm2, x3, y3, parm3);
	}
	int y0, yn, j, k;

	i = 0;
	if (size + bx > scence.windowWidth) size = scence.windowWidth - bx;
	if (bx < -scence.windowWidth / 2) i = -bx - scence.windowWidth / 2;

	for (; i < size; ++i)
	{
		y0 = l1[i];
		yn = l2[i];

		for (k = 0; k < n; ++k)
		{
			l0[k] = fl1[i * n + k];
			ln[k] = fl2[i * n + k];
			dl[k] = (ln[k] - l0[k]) / (yn - y0);
			il[k] = l0[k];
		}
		//printf("(%d,%d)(%.3f,%.3f,%.3f,%.3f=>%.3f,%.3f,%.3f,%.3f)\n", y0, yn, r0, g0, b0, z0, rn, g0, b0, z0);
		//printf("(%d,%d)\n", y0, yn);

		if (y0 < yn)
		{
			for (j = y0; j <= yn; ++j)
			{
				memcpy(output_temp, il, sizeof(float) * (n));
				func(output_temp);
				scence.setRGBAd((i + bx), j, output_temp[0],
					output_temp[1], output_temp[2], output_temp[3], output_temp[4]);
				for (k = 0; k < n; ++k)il[k] += dl[k];
			}
		}
		else
		{
			for (j = y0; j >= yn; --j)
			{
				memcpy(output_temp, il, sizeof(float) * (n));
				func(output_temp);
				scence.setRGBAd((i + bx), j, output_temp[0],
					output_temp[1], output_temp[2], output_temp[3], output_temp[4]);
				for (k = 0; k < n; ++k)il[k] -= dl[k];
			}
		}
	}
}


void setlineinNew(int* lmax, int* lmin, int n, float* flmax, float* flmin,
	int bx, int base, int flag,
	int x1, int y1, float* p1,
	int x2, int y2, float* p2)
{
	// 当x1<x2，交换x1x2
	if (x1 > x2)
	{
		int t = x1;
		x1 = x2;
		x2 = t;
		t = y1;
		y1 = y2;
		y2 = t;
		float* tp = p1;
		p1 = p2;
		p2 = tp;
	}

	// 求y,z在x轴上的变化率
	float dx = x2 - x1;
	float dy = y2 - y1;
	for (int k = 0; k < n; ++k)
	{
		dl[k] = p2[k] - p1[k];
		dl[k] /= dx;
		il[k] = p1[k];
	}

	dy /= dx;

	float ty = y1;
	// 逐个存入边表
	for (int j = x1; j < x2; ++j)
	{
		if (lmax[j - bx] == flag || lmax[j - bx] < ty)
		{
			lmax[j - bx] = ty;
			for (int k = 0; k < n; ++k)flmax[(j - bx) * n + k] = il[k];
		}

		if (lmin[j - bx] == flag || lmin[j - bx] > ty)
		{
			lmin[j - bx] = ty;
			for (int k = 0; k < n; ++k)flmin[(j - bx) * n + k] = il[k];
		}
		ty += dy;
		for (int k = 0; k < n; ++k)il[k] += dl[k];
	}
}

#define RAND(mi,mx) ((rand()%((mi)-(mx)+1))+(mi))
#define MAY(x) (RAND(0,10000)<=(x)*10000)

void polygonShader(int m, int n, void (*func)(float*), float* parm, float randf)
{
	int flg = 0xcfcfcfcf;
	int bx, maxx, i, size;
	int w = scence.windowWidth / 2;
	int h = scence.windowHigh / 2;

	maxx = parm[0] * w;
	bx = parm[0] * w;
	for (int i = 0; i < m; ++i)
	{
		if (parm[i * n] * w > maxx)maxx = parm[i * n] * w;
		if (parm[i * n] * w < bx)bx = parm[i * n] * w;
	}

	size = maxx - bx;

	//printf("maxx = %d, bx = %d, n = %d, globe size * n = %d\n", maxx, bx, n, globeListSizeN);
	if (size * n > globeListSizeN || firstShow)
	{
		if (firstShowN)
		{
			firstShowN = false;
		}
		else
		{
			delete[] l1;
			delete[] l2;
			delete[] fl1;
			delete[] fl2;
			delete[] ltemp;
			delete[] fltemp;
		}

		globeListSizeN = size * n;

		l1 = new int[size + 10];
		l2 = new int[size + 10];
		ltemp = new int[size + 10];
		fl1 = new float[(size + 2) * n + 10];
		fl2 = new float[(size + 2) * n + 10];
		fltemp = new float[(size + 2) * n + 10];
	}

	memset(l1, 0xcf, size * sizeof(int));
	memset(l2, 0xcf, size * sizeof(int));

	//if (parm[0] < parm[n])
	//{
	//	setlineinTl(l1, n, fl1, bx, size, parm[0] * w, parm[1] * h, parm,
	//		parm[n] * w, parm[n + 1] * h, parm + n);
	//}
	//else
	//{
	//	setlineinTl(l1, n, fl1, bx, size, parm[n] * w, parm[n + 1] * h, parm + n,
	//		parm[0] * w, parm[1] * h, parm);
	//}


	//memcpy(l2, l1, (size + 1) * sizeof(int));
	//memcpy(fl2, fl1, n * (size + 1) * sizeof(float));

	//printf("\nbegin\n%d->%d\n", (int)(parm[0] * w), (int)(parm[n] * w));
	//printf("\nl1:");
	//for (int i = 0; i < size; ++i)printf(",%d ", l1[i]);
	//printf("\nl2:");
	//for (int i = 0; i < size; ++i)printf(",%d ", l2[i]);
	//printf("\nend\n");

	for (int i = 0; i < m; ++i)
	{
		int next = i + 1 == m ? 0 : i + 1;
		int t, b;
		setlineinNew(l2, l1, n, fl2, fl1, bx, size, flg,
			parm[i * n] * w, parm[i * n + 1] * h, parm + (i * n),
			parm[next * n] * w, parm[next * n + 1] * h, parm + (next * n));
		if (parm[i * n] < parm[next * n])
		{
			t = parm[next * n] * w - bx;
			b = parm[i * n] * w - bx;
		}
		else
		{
			b = parm[next * n] * w - bx;
			t = parm[i * n] * w - bx;
		}
		//if (parm[i * n] < parm[next * n])
		//{
		//	setlineinTl(ltemp, n, fltemp, bx, size, parm[i * n] * w, parm[i * n + 1] * h, parm + (i * n),
		//		parm[next * n] * w, parm[next * n + 1] * h, parm + (next * n));
		//	t = parm[next * n] * w - bx;
		//	b = parm[i * n] * w - bx;
		//}
		//else
		//{
		//	setlineinTl(ltemp, n, fltemp, bx, size, parm[next * n] * w, parm[next * n + 1] * h, parm + (next * n),
		//		parm[i * n] * w, parm[i * n + 1] * h, parm + (i * n));
		//	b = parm[next * n] * w - bx;
		//	t = parm[i * n] * w - bx;
		//}
		////printf("\n%d, %d->%d\n", m, b, t);
		////printf("\nltemp:");
		////for (int i = 0; i < size; ++i)printf(",%d ", ltemp[i]);
		////printf("\nend\n");

		//for (int i = b + 1; i <= t; ++i)
		//{
		//	if (l2[i] == flg || ltemp[i] > l2[i])
		//	{
		//		l2[i] = ltemp[i];
		//		for (int j = 0; j < n; ++j)
		//			fl2[i * n + j] = fltemp[i * n + j];
		//	}
		//	if (l1[i] == flg || ltemp[i] < l1[i])
		//	{
		//		l1[i] = ltemp[i];
		//		for (int j = 0; j < n; ++j)
		//			fl1[i * n + j] = fltemp[i * n + j];
		//	}
		//}
	}
	//printf("\nl1:");
	//for (int i = 0; i < size; ++i)printf(",%d ", l1[i]);
	//printf("\nl2:");
	//for (int i = 0; i < size; ++i)printf(",%d ", l2[i]);
	//printf("\nend\n");
	int y0, yn, j, k;


	//if (size + bx > w) size = w - bx;
	//if (bx < -w / 2)i = -bx + w / 2;


	for (i = 0; i < size; ++i)
	{
		y0 = l1[i];
		yn = l2[i];

		//if (y0 < -h / 2)y0 = -h / 2;
		//if (yn > h / 2)yn = h / 2;

		for (k = 0; k < n; ++k)
		{
			l0[k] = fl1[i * n + k];
			ln[k] = fl2[i * n + k];
			dl[k] = (ln[k] - l0[k]) / (yn - y0);
			il[k] = l0[k];
		}
		//printf("(%d,%d)(%.3f,%.3f,%.3f,%.3f=>%.3f,%.3f,%.3f,%.3f)\n", y0, yn, r0, g0, b0, z0, rn, g0, b0, z0);
		//printf("(%d,%d)\n", y0, yn);


		for (j = y0; j <= yn; ++j)
		{
			if (MAY(randf))
			{
				memcpy(output_temp, il, sizeof(float) * (n));
				func(output_temp);
				scence.setRGBAd((i + bx), j, output_temp[2],
					output_temp[3], output_temp[4], output_temp[5], output_temp[6]);
			}
			for (k = 0; k < n; ++k)il[k] += dl[k];
		}
	}
}


