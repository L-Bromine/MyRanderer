#pragma once
#include "Element.h"
class Camera
{
private:
	Matrix looktran, rot, mov;
	float angleY = 0, angleX = 0;
	float nearFace = 0, viewSize = 1;
	float x = 0, y = 0, z = 0;
public:
	Camera();

	/// <summary>
	/// 设置透视变换
	/// </summary>
	void lookdBox(float n, float f, float size);

	/// <summary>
	/// can can need！
	/// 将变换压入scence栈
	/// </summary>
	inline void lookE() { scence.pushStack(looktran ^ rot ^ mov); };
	inline void lookR() { scence.pushStack(rot ^ mov); scence.setCamera(nearFace, viewSize); }

	inline float X() const { return x; }
	inline float Y() const { return y; }
	inline float Z() const { return z; }
	inline float AngleY() const { return angleY; }
	inline float AngleX() const { return angleX; }
	

	/// <summary>
	/// 摄像机漫游
	/// </summary>
	/// <param name="f"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void move(float f, float y, float z);

	/// <summary>
	/// 在3D坐标上移动摄像机
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void tranformPosition(float x, float y, float z);

	/// <summary>
	/// 视角旋转
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void rotate(float x, float y, float z);

	/// <summary>
	/// 在某轴上旋转一个角度（弧度制）
	/// </summary>
	/// <param name="n">弧度</param>
	/// <param name="x">旋转轴x</param>
	/// <param name="y">旋转轴y</param>
	/// <param name="z">旋转轴z</param>
	void tranformAxisRotate(float n, float x, float y, float z);

	/// <summary>
	/// 缩放摄像机
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void scale(float x, float y, float z);

};

