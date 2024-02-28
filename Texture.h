#pragma once
#include <fstream>
using std::string;
class Texture
{
private:
	int high, width;
	int type;
	float* data;
public:
	Texture();
	Texture(string path);
	~Texture();

	void readImg(string path);
	void getColor(float u, float v, float* colors);
	void clear();
	int getTpye() { return type; }
};

