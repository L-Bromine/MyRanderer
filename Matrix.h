#pragma once
#include "function.h"
#include "MatrixException.h"
class Matrix
{
private:
	float* a;
	int _row, _col;

	friend MatrixException;
public:
	Matrix(int len);
	Matrix(int r, int l);
	Matrix(int len, float* b);
	Matrix(int r, int l, float* b);
	Matrix(const Matrix&);
	Matrix();
	~Matrix();

	inline float& get(int x)const { return a[x]; }
	inline float& get(int x, int y) const { return a[y * _row + x]; }
	inline float* get()const { return a; }

	inline int row() const { return _row; }
	inline int col() const { return _col; }

	void output() const;

	Matrix& operator = (const Matrix& b);
	Matrix& operator = (float* b);

	/// <summary>
	/// 矩阵点乘
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	static Matrix dot(const Matrix& a, const Matrix& b);

	/// <summary>
	/// 矩阵点乘
	/// 引用matrix dot
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	inline Matrix operator^(const Matrix& b) { return dot(*this, b); }
	inline Matrix& operator^=(const Matrix& b) { return *this = dot(*this, b); }
	inline friend Matrix operator^(const Matrix& a, const Matrix& b) { return dot(a, b); }

	/// <summary>
	/// 矩阵逐元素相加
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	Matrix operator+(const Matrix& b);
	Matrix& operator+=(const Matrix& b);

	/// <summary>
	/// 矩阵逐元素相减
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	Matrix operator-(const Matrix& b);
	Matrix& operator-=(const Matrix& b);

	/// <summary>
	/// 矩阵逐元素相乘（正片叠底）
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	Matrix operator*(const Matrix& b);
	Matrix& operator*=(const Matrix& b);

	/// <summary>
	/// 矩阵逐元素相除
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	Matrix operator/(const Matrix& b);
	Matrix& operator/=(const Matrix& b);


	/// <summary>
	/// 矩阵正置偏
	/// </summary>
	/// <param name="a"></param>
	/// <param name="n"></param>
	/// <returns></returns>
	Matrix operator+(const float n);
	Matrix& operator+=(const float n);

	/// <summary>
	/// 矩阵负置偏
	/// </summary>
	/// <param name="a"></param>
	/// <param name="n"></param>
	/// <returns></returns>
	Matrix operator-(const float n);
	Matrix& operator-=(const float n);

	/// <summary>
	/// 矩阵正缩放
	/// </summary>
	/// <param name="a"></param>
	/// <param name="n"></param>
	/// <returns></returns>
	Matrix operator*(const float n);
	Matrix& operator*=(const float n);

	/// <summary>
	/// 矩阵负缩放
	/// </summary>
	/// <param name="a"></param>
	/// <param name="n"></param>
	/// <returns></returns>
	Matrix operator/(const float n);
	Matrix& operator/=(const float n);

	/// <summary>
	/// 将自身转换为单位矩阵
	/// </summary>
	/// <returns></returns>
	Matrix& E();

	/// <summary>
	/// 矩阵求逆
	/// </summary>
	/// <returns></returns>
	Matrix Inverse();

};

