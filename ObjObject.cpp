#include "ObjObject.h"
#include "Camera.h"
#include <fstream>
#include <sstream>
#include <map>


void shaderobj(float* output)
{
	float color_temp[5];
	if (scence.texture)
	{
		scence.texture->getColor(output[7], 0.99-output[8], color_temp);
		output[3] *= color_temp[0];
		output[4] *= color_temp[1];
		output[5] *= color_temp[2];
		if(scence.texture->getTpye() == 4)
			output[6] *= color_temp[3];
	}

	float x = output[9];
	float y = output[10];
	float z = output[11];

	float nx = -output[12];
	float ny = -output[13];
	float nz = -output[14];

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

	float ker = output[24];
	float keg = output[25];
	float keb = output[26];

	float n = output[27];

	float r, g, b;

	float iar = scence.Ia[0];
	float iag = scence.Ia[1];
	float iab = scence.Ia[2];

	float lx, ly, lz;
	float hx, hy, hz;

	float cos0, cosna = 0;

	float tr, tg, tb;

	r = iar * ar + ker;
	g = iag * ag + keg;
	b = iab * ab + keb;

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


ObjObject::ObjObject(string path, string objname) :Element(1)
{
	readobj(path, objname);
}

ObjObject::ObjObject(string objname) : Element(1)
{
	readobj("", objname);
}

void ObjObject::readobj(string path, string objname)
{
	std::ifstream fobj, fmtl;
	string line;
	std::stringstream ss;
	fobj.open(path + "\\" + objname, std::ios::in);
	std::vector<float> lx, ly, lz;
	std::vector<float> uvx, uvy;
	std::vector<float> nx, ny, nz;
	int face_count = 0;
	//std::map<string, Matrix> materials;
	std::map<string, Texture*> materials_Kd_map;
	string now_material = "";

	materials.insert(std::map<string, Matrix>::value_type("", Matrix(20)));

	if (!fobj.is_open())
	{
		printf("%s is not found!", objname.c_str());
		return;
	}

	printf("object>>%s\n", objname.c_str());

	while (std::getline(fobj, line))
	{
		ss.clear();
		ss.str(line);
		string word;

		ss >> word;
		//printf("%s %s\n", line.c_str(), word.c_str());
		if (word == "v")
		{
			float x, y, z;
			ss >> x >> y >> z;

			lx.push_back(x);
			ly.push_back(y);
			lz.push_back(z);
		}
		else if (word == "vn")
		{
			float x, y, z;
			ss >> x >> y >> z;

			nx.push_back(x);
			ny.push_back(y);
			nz.push_back(z);
		}
		else if (word == "vt")
		{
			float x, y;
			ss >> x >> y;

			uvx.push_back(x);
			uvy.push_back(y);
		}
		else if (word == "f")
		{
			std::stringstream facep;
			int count = 0;
			std::vector<int> p, uv, n;
			while (ss >> word)
			{
				for (char& c : word)if (c == '/')c = ' ';
				facep.clear();
				facep.str(word);
				int pi, uvi, ni;
				facep >> pi >> uvi >> ni;
				p.push_back(pi - 1);
				uv.push_back(uvi - 1);
				n.push_back(ni - 1);
				++count;
			}

			if (count > 0)
			{
				face_count += 1;

				Matrix mp(4, count), muv(2, count), mn(4, count);
				for (int i = 0; i < count; ++i)
				{
					mp.get(0, i) = lx[p[i]];
					mp.get(1, i) = ly[p[i]];
					mp.get(2, i) = lz[p[i]];
					mp.get(3, i) = 1;

					muv.get(0, i) = uvx[uv[i]];
					muv.get(1, i) = uvy[uv[i]];

					mn.get(0, i) = nx[n[i]];
					mn.get(1, i) = ny[n[i]];
					mn.get(2, i) = nz[n[i]];
				}
				faces_postions.push_back(mp);
				faces_uv.push_back(muv);
				faces_normal.push_back(mn);
				faces_material.push_back(&materials.at(now_material));
				faces_texture.push_back(materials_Kd_map.at(now_material));
			}
		}
		else if (word == "usemtl")
		{
			ss >> now_material;
		}
		else if (word == "mtllib")
		{
			ss >> word;
			printf("mtllib>>%s\n", word.c_str());
			fmtl.open(path + "\\" + word, std::ios::in);
			string reading_material = "";
			string mword;
			string mline;
			std::stringstream ms;
			//Matrix material(20);

			//第一次遍历，存储图像文件
			while (std::getline(fmtl, mline))
			{
				ms.clear();
				ms.str(mline);
				ms >> mword;

				if (mword == "map_Kd")
				{
					ms >> mword;
					printf("map>>%s\n", mword.c_str());
					textures_map[mword] = Texture();
					textures_map[mword].readImg(path + "\\" + mword);
				}
			}

			fmtl.clear();
			fmtl.seekg(0, std::ios::beg);

			//遍历每一行
			while (std::getline(fmtl, mline))
			{
				ms.clear();
				ms.str(mline);
				ms >> mword;
				//printf("mtllib>>%s %s\n", mline.c_str(), mword.c_str());
				if (mword == "newmtl")
				{
					ms >> reading_material;
					materials.insert(std::map<string, Matrix>::value_type(reading_material, Matrix(20)));
					materials_Kd_map[reading_material] = nullptr;
					//if (reading_material != "")
					//{
					//	materials.insert(std::map<string, Matrix>::value_type(reading_material, material));
					//}
					//ms >> reading_material;
				}
				if (mword == "Ka")
				{
					Matrix& material = materials.at(reading_material);
					ms >> material.get(0) >> material.get(1) >> material.get(2);
				}
				if (mword == "Kd")
				{
					Matrix& material = materials.at(reading_material);
					ms >> material.get(3) >> material.get(4) >> material.get(5);
				}
				if (mword == "Ks")
				{
					Matrix& material = materials.at(reading_material);
					ms >> material.get(6) >> material.get(7) >> material.get(8);
				}
				if (mword == "Ks")
				{
					Matrix& material = materials.at(reading_material);
					ms >> material.get(9) >> material.get(10) >> material.get(11);
				}
				if (mword == "Ns")
				{
					Matrix& material = materials.at(reading_material);
					ms >> material.get(12);
				}
				if (mword == "d")
				{
					Matrix& material = materials.at(reading_material);
					ms >> material.get(13);
				}
				if (mword == "map_Kd")
				{
					ms >> mword;
					materials_Kd_map[reading_material] = &textures_map[mword];
				}
			}
			//materials.insert(std::map<string, Matrix>::value_type(reading_material, material));
			fmtl.close();
		}
	}
	n = face_count;
	fobj.close();
}

float ObjObject::averageAlpha() const
{
	return 1;
}

Matrix ObjObject::tranform(const Matrix& pos) const
{
	Matrix m = scence.topStack() ^ tran ^ pos;
	for (int i = 0; i < m.col(); ++i)
		for (int j = 0; j < 3; ++j)
			m.get(j, i) /= abs(m.get(3, i));
	return m;
}

void ObjObject::draw() const
{
	int ploym = 15 + 12;
	float* fl = new float[(n + 1) * ploym * 2];
	for (int f = 0; f < n; ++f)
	{
		Matrix m = tranform(faces_postions[f]);

		float x1, y1, x2, y2;
		x1 = m.get(0, 1) - m.get(0, 0);
		x2 = m.get(0, 2) - m.get(0, 1);
		y1 = m.get(1, 1) - m.get(1, 0);
		y2 = m.get(1, 2) - m.get(1, 1);

		if (x1 * y2 - x2 * y1 < 0)
		{
			continue;
		}

		Matrix p = tran ^ faces_postions[f];
		Matrix np = tran ^ faces_normal[f];
		int facen = faces_postions[f].col();

		scence.texture = faces_texture[f];

		//printf("facen = %d\n", facen);
		//printf("alp = %.3f\n", faces_material[f].get(13));

		for (int i = 0; i < facen; ++i)
		{
			fl[i * ploym + 0] = m.get(0, i);
			fl[i * ploym + 1] = m.get(1, i);
			fl[i * ploym + 2] = m.get(2, i);
			fl[i * ploym + 3] = 1;
			fl[i * ploym + 4] = 1;
			fl[i * ploym + 5] = 1;
			fl[i * ploym + 6] = faces_material[f]->get(13);
			fl[i * ploym + 7] = faces_uv[f].get(0, i);
			fl[i * ploym + 8] = faces_uv[f].get(1, i);
			fl[i * ploym + 9] = p.get(0, i);
			fl[i * ploym + 10] = p.get(1, i);
			fl[i * ploym + 11] = p.get(2, i);
			fl[i * ploym + 12] = np.get(0, i);
			fl[i * ploym + 13] = np.get(1, i);
			fl[i * ploym + 14] = np.get(2, i);

			for (int j = 0; j < 13; ++j)
			{
				fl[i * ploym + 15 + j] = faces_material[f]->get(j);
			}
		}



		//m.output();

		int cutedN = facen;
		cutedN = cutPolygon(cutedN, ploym, fl, -999, 999, -999, 999, -1, 1);
		if (cutedN > 2)
			cutedN = cutPolygon(cutedN, ploym, fl, -1, 1, -999, 999, -999, 999);
		if (cutedN > 2)
			cutedN = cutPolygon(cutedN, ploym, fl, -999, 999, -1, 1, -999, 999);

		//printf("\nthe n %d\n", cutedN);


		if (cutedN > 2)
		{
			//printf("\n");
			//for (int i = 0; i < cutedN; ++i)
			//{
			//	for (int j = 0; j < ploym; ++j)printf("%9.3f|", fl[i * ploym + j]);
			//	printf("\n");
			//}
			//printf("\n");
			polygonShader(cutedN, ploym, shaderobj, fl, 1);
		}
	}
	delete[] fl;
}
