#pragma once
#include "Element.h"
class Point :public Element
{
private:

public:
	/// <summary>
	/// 无参构造函数
	/// 全部初始化为0
	/// </summary>
	Point();

	/// <summary>
	/// 2D点构造
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	Point(float x, float y);

	/// <summary>
	/// 3D点构造
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	Point(float x, float y, float z);


	/// <summary>
	/// 通过POINT结构体初始化
	/// </summary>
	/// <param name="p"></param>
	Point(POINT);

	virtual void draw()const;
};