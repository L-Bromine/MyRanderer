#pragma once
#include "Scence.h"
#include "Matrix.h"


class Texture;
class Ray;

class Element
{
protected:
	int n;
	Matrix tran;
	Matrix positions;
	Matrix colors;
	Matrix UV;
	Matrix normal;

	int parmm = 0;
	float* parm = nullptr;

	Texture* texture;

	friend Scence;
public:

	/// <summary>
	/// 初始化图元变换及顶点组
	/// </summary>
	Element(int x);
	~Element();

	/// <summary>
	/// 设置uv
	/// </summary>
	/// <param name="uv"></param>
	void setUV(float* uv);

	/// <summary>
	/// 设置法向
	/// </summary>
	/// <param name="normals"></param>
	void setNormals(float* normals);

	/// <summary>
	/// 设置材质
	/// </summary>
	void setTexture(Texture*);

	/// <summary>
	/// 通过4x4矩阵直接重置tranform
	/// </summary>
	/// <param name="matrix"></param>
	void tranform(const Matrix&);

	/// <summary>
	/// 在3D坐标上移动图元
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void move(float x, float y, float z = 0);

	/// <summary>
	/// 欧拉角旋转
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void rotate(float x, float y, float z);

	/// <summary>
	/// 四元数旋转
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	/// <param name="z"></param>
	void rotate(float x, float y, float z, float w);

	/// <summary>
	/// 在某轴上旋转一个角度（弧度制）
	/// </summary>
	/// <param name="n">弧度</param>
	/// <param name="x">旋转轴x</param>
	/// <param name="y">旋转轴y</param>
	/// <param name="z">旋转轴z</param>
	void axisRotate(float n, float x, float y, float z);

	/// <summary>
	/// 缩放物体
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void scale(float x, float y, float z);

	/// <summary>
	/// 设置颜色数组
	/// </summary>
	void setColors(float* cs);

	/// <summary>
	/// 拷贝颜色矩阵
	/// </summary>
	void setColors(const Matrix& cs);

	/// <summary>
	/// 以scence为父级变换
	/// </summary>
	Matrix tranform() const;

	/// <summary>
	/// 返回平均透明度
	/// </summary>
	/// <returns></returns>
	virtual float averageAlpha()const;

	/// <summary>
	/// 设置额外参数
	/// </summary>
	void setParm(int parmm, float* parm);

	/// <summary>
	/// 射线三角形相交接口
	/// </summary>
	virtual bool triangleIntersect(const Ray& ray, float* out) const;

	/// <summary>
	/// 绘制接口
	/// </summary>
	virtual void draw() const = 0;
};

