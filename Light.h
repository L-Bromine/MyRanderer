#pragma once
class Light
{
public:
	float x, y, z;
	float idr, idg, idb;
	float ipr, ipg, ipb;

	Light();
	void setXYZ(float x, float y, float z);
	void setIdp(float r, float g, float b);
};

