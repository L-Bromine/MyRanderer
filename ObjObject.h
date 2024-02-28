#pragma once
#include "Element.h"
#include <vector>
#include <map>
class ObjObject :
	public Element
{
private:
	std::vector<Matrix> faces_postions;
	std::vector<Matrix> faces_normal;
	std::vector<Matrix> faces_uv;
	std::vector<Matrix*> faces_material;
	std::vector<Texture*> faces_texture;
	std::map<string, Matrix> materials;
	std::map<string, Texture> textures_map;
public:
	ObjObject(string path, string objname);
	ObjObject(string objname);
	Matrix tranform(const Matrix& pos) const;
	void readobj(string path, string objname);
	virtual float averageAlpha()const;
	virtual void draw() const;
};

