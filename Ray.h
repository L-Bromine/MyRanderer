#pragma once
class Matrix;
class Ray
{
private:
	float dx, dy, dz;//射线方向向量
	float x, y, z;//起始点
public:
	Ray(float _x, float _y, float _z, float _dx, float _dy, float _dz);

	bool triangleIntersect(float* triangle, float* out) const;
};

