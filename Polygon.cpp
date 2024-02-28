#include "Polygon.h"
#include "Camera.h"

Polygon::Polygon(int n, float* fl) : Element(n)
{
	positions = fl;
}

void shader2(float* output)
{
	float color_temp[5];
	if (scence.texture)
	{
		scence.texture->getColor(output[7], output[8], color_temp);
		output[3] *= color_temp[0];
		output[4] *= color_temp[1];
		output[5] *= color_temp[2];
	}

	float x = output[9];
	float y = output[10];
	float z = output[11];

	float nx = output[12];
	float ny = output[13];
	float nz = output[14];

	float len = sqrt(nx * nx + ny * ny + nz * nz);

	nx /= len;
	ny /= len;
	nz /= len;

	float ex = scence.mainCamera->X() - x;
	float ey = scence.mainCamera->Y() - y;
	float ez = scence.mainCamera->Z() - z;

	len = sqrt(ex * ex + ey * ey + ez * ez);

	ex /= len;
	ey /= len;
	ez /= len;

	float ar = output[15];
	float ag = output[16];
	float ab = output[17];

	float dr = output[18];
	float dg = output[19];
	float db = output[20];

	float sr = output[21];
	float sg = output[22];
	float sb = output[23];

	float n = output[24];

	float r, g, b;

	float iar = scence.Ia[0];
	float iag = scence.Ia[1];
	float iab = scence.Ia[2];

	float lx, ly, lz;
	float hx, hy, hz;

	float cos0, cosna = 0;

	float tr, tg, tb;

	r = iar * ar;
	g = iag * ag;
	b = iab * ab;

	for (const Light& l : scence.lights)
	{
		lx = l.x - x;
		ly = l.y - y;
		lz = l.z - z;

		float len = sqrt(lx * lx + ly * ly + lz * lz);

		lx /= len;
		ly /= len;
		lz /= len;

		hx = (ex + lx) / 2;
		hy = (ey + ly) / 2;
		hz = (ez + lz) / 2;

		len = sqrt(hx * hx + hy * hy + hz * hz);

		hx /= len;
		hy /= len;
		hz /= len;

		cosna = pow(hx * nx + hy * ny + hz * nz, n);
		if (cosna < 0)cosna = 0;

		cos0 = lx * nx + ly * ny + lz * nz;
		if (cos0 < 0)cos0 = 0;

		tr = dr * l.idr * cos0;
		tg = dg * l.idg * cos0;
		tb = db * l.idb * cos0;

		tr = tr > 0 ? tr : 0;
		tg = tg > 0 ? tg : 0;
		tb = tb > 0 ? tb : 0;

		r += tr;
		g += tg;
		b += tb;

		tr = sr * l.ipr * cosna;
		tg = sg * l.ipg * cosna;
		tb = sb * l.ipb * cosna;

		tr = tr > 0 ? tr : 0;
		tg = tg > 0 ? tg : 0;
		tb = tb > 0 ? tb : 0;

		r += tr;
		g += tg;
		b += tb;
	}

	output[3] *= r > 1 ? 1 : r;
	output[4] *= g > 1 ? 1 : g;
	output[5] *= b > 1 ? 1 : b;
}

void Polygon::draw() const
{
	scence.texture = this->texture;

	Matrix m = tranform();//positions;
	Matrix p = tran ^ positions;
	Matrix np = tran ^ normal;

	int ploym = 15 + parmm;
	float* fl = new float[(n + 1) * ploym * 2];

	for (int i = 0; i < n; ++i)
	{
		fl[i * ploym + 0] = m.get(0, i);
		fl[i * ploym + 1] = m.get(1, i);
		fl[i * ploym + 2] = m.get(2, i);
		fl[i * ploym + 3] = colors.get(0, i);
		fl[i * ploym + 4] = colors.get(1, i);
		fl[i * ploym + 5] = colors.get(2, i);
		fl[i * ploym + 6] = colors.get(3, i);
		fl[i * ploym + 7] = UV.get(0, i);
		fl[i * ploym + 8] = UV.get(1, i);
		fl[i * ploym + 9] = p.get(0, i);
		fl[i * ploym + 10] = p.get(1, i);
		fl[i * ploym + 11] = p.get(2, i);
		fl[i * ploym + 12] = np.get(0, i);
		fl[i * ploym + 13] = np.get(1, i);
		fl[i * ploym + 14] = np.get(2, i);

		if (parm != nullptr)
			for (int j = 0; j < parmm; ++j)
			{
				fl[i * ploym + 15 + j] = parm[i * parmm + j];
			}
	}

	float x1, y1, x2, y2;
	x1 = m.get(0, 1) - m.get(0, 0);
	x2 = m.get(0, 2) - m.get(0, 1);
	y1 = m.get(1, 1) - m.get(1, 0);
	y2 = m.get(1, 2) - m.get(1, 1);

	//if (x1 * y2 - x2 * y1 < 0)
	//{
	//	delete[]fl;
	//	return;
	//}

	//m.output();

	int cutedN = n;
	cutedN = cutPolygon(cutedN, ploym, fl, -999, 999, -999, 999, -1, 1);
	//printf("\nthe 0n %d\n", cutedN);
	//printf("\n");
	//for (int i = 0; i < cutedN; ++i)
	//{
	//	for (int j = 0; j < ploym; ++j)printf("%9.3f|", fl[i * ploym + j]);
	//	printf("\n");
	//}
	//printf("\n");


	if (cutedN > 2)
		cutedN = cutPolygon(cutedN, ploym, fl, -1, 1, -999, 999, -999, 999);


	//printf("\nthe 1n %d\n", cutedN);
	//printf("\n");
	//for (int i = 0; i < cutedN; ++i)
	//{
	//	for (int j = 0; j < ploym; ++j)printf("%9.3f|", fl[i * ploym + j]);
	//	printf("\n");
	//}
	//printf("\n");


	if (cutedN > 2)
		cutedN = cutPolygon(cutedN, ploym, fl, -999, 999, -1, 1, -999, 999);

	//printf("\nthe 2n %d\n", cutedN);
	//printf("\n");
	//for (int i = 0; i < cutedN; ++i)
	//{
	//	for (int j = 0; j < ploym; ++j)printf("%9.3f|", fl[i * ploym + j]);
	//	printf("\n");
	//}
	//printf("\n");
	//printf(" %d,%d", cutedN, parmm);

	if (cutedN > 2)
	{
		if (parm != nullptr)
			polygonShader(cutedN, ploym, shader2, fl, 1);
		else
			polygonShader(cutedN, ploym, defShader, fl);
	}

	delete[] fl;
}