#include "Matrix.h"
#include <string.h>
#include <stdio.h>

Matrix::Matrix(int len)
{
	_row = len;
	_col = 1;
	a = new float[len];
	memset(a, 0, sizeof(float) * len);
}

Matrix::Matrix(int r, int l)
{
	_row = r;
	_col = l;
	a = new float[r * l + 2];
	memset(a, 0, sizeof(float) * r * l);
}

Matrix::Matrix(int len, float* b)
{
	_row = len;
	_col = 1;

	a = new float[_row * _col];
	memcpy(a, b, sizeof(float) * len);
}

Matrix::Matrix(int r, int l, float* b)
{
	_row = r;
	_col = l;

	a = new float[_row * _col];
	memcpy(a, b, sizeof(float) * r * l);
}

Matrix::Matrix(const Matrix& m)
{
	_row = m._row;
	_col = m._col;

	a = new float[_row * _col];
	memcpy(a, m.a, sizeof(float) * _row * _col);
}

Matrix::~Matrix()
{
	delete[] a;
}

void Matrix::output() const
{
	printf("[");
	for (int i = 0; i < _col; ++i)
	{
		printf("[");
		for (int j = 0; j < _row; ++j)
		{
			printf("%.3f, ", get(j, i));
		}
		printf("],\n");
	}
	printf("]\n");
}

Matrix& Matrix::operator=(const Matrix& b)
{
	if (&b == this)
		return *this;

	delete[] a;
	_row = b._row;
	_col = b._col;

	a = new float[_row * _col];
	memcpy(a, b.a, sizeof(float) * _row * _col);

	return *this;
}

Matrix& Matrix::operator=(float* b)
{
	memcpy(a, b, sizeof(float) * _row * _col);
	return *this;
}

Matrix Matrix::dot(const Matrix& a, const Matrix& b)
{
	if (a._col != b._row)
		throw MatrixException(a, b, "can't dot matrixs.");

	Matrix m(a._row, b._col);
	for (int i = 0; i < a._row; ++i)
		for (int j = 0; j < b._col; ++j)
			for (int c = 0; c < a._col; ++c)
			{
				m.get(i, j) += a.get(i, c) * b.get(c, j);
			}

	return m;
}

Matrix Matrix::operator+(const Matrix& b)
{
	if (_row != b._row || _col != b._col)
		throw MatrixException(*this, b, "can't add matrixs.");

	Matrix m(*this);
	for (int i = 0; i < _row * _col; ++i)
		m.a[i] += b.a[i];

	return m;
}

Matrix& Matrix::operator+=(const Matrix& b)
{
	if (_row != b._row || _col != b._col)
		throw MatrixException(*this, b, "can't subtract matrixs.");

	for (int i = 0; i < _row * _col; ++i)
		a[i] += b.a[i];

	return *this;
}

Matrix Matrix::operator-(const Matrix& b)
{
	if (_row != b._row || _col != b._col)
		throw MatrixException(*this, b, "can't subtract matrixs.");

	Matrix m(*this);
	for (int i = 0; i < _row * _col; ++i)
		m.a[i] -= b.a[i];

	return m;
}

Matrix& Matrix::operator-=(const Matrix& b)
{
	if (_row != b._row || _col != b._col)
		throw MatrixException(*this, b, "can't subtract matrixs.");

	for (int i = 0; i < _row * _col; ++i)
		a[i] -= b.a[i];

	return *this;
}

Matrix Matrix::operator*(const Matrix& b)
{
	if (_row != b._row || _col != b._col)
		throw MatrixException(*this, b, "can't multiply matrixs.");

	Matrix m(*this);
	for (int i = 0; i < _row * _col; ++i)
		m.a[i] *= b.a[i];

	return m;
}

Matrix& Matrix::operator*=(const Matrix& b)
{
	if (_row != b._row || _col != b._col)
		throw MatrixException(*this, b, "can't subtract matrixs.");

	for (int i = 0; i < _row * _col; ++i)
		a[i] *= b.a[i];

	return *this;
}

Matrix Matrix::operator/(const Matrix& b)
{
	if (_row != b._row || _col != b._col)
		throw MatrixException(*this, b, "can't divide matrixs.");

	Matrix m(*this);
	for (int i = 0; i < _row * _col; ++i)
		m.a[i] /= b.a[i];

	return m;
}

Matrix& Matrix::operator/=(const Matrix& b)
{
	if (_row != b._row || _col != b._col)
		throw MatrixException(*this, b, "can't subtract matrixs.");

	for (int i = 0; i < _row * _col; ++i)
		a[i] /= b.a[i];

	return *this;
}

Matrix Matrix::operator+(const float n)
{
	Matrix m(*this);
	for (int i = 0; i < _row * _col; ++i)
		m.a[i] += n;

	return m;
}

Matrix& Matrix::operator+=(const float n)
{
	for (int i = 0; i < _row * _col; ++i)
		a[i] += n;

	return *this;
}

Matrix Matrix::operator-(const float n)
{
	Matrix m(*this);
	for (int i = 0; i < _row * _col; ++i)
		m.a[i] -= n;

	return m;
}

Matrix& Matrix::operator-=(const float n)
{
	for (int i = 0; i < _row * _col; ++i)
		a[i] -= n;

	return *this;
}

Matrix Matrix::operator*(const float n)
{
	Matrix m(*this);
	for (int i = 0; i < _row * _col; ++i)
		m.a[i] *= n;

	return m;
}

Matrix& Matrix::operator*=(const float n)
{
	for (int i = 0; i < _row * _col; ++i)
		a[i] *= n;

	return *this;
}

Matrix Matrix::operator/(const float n)
{
	Matrix m(*this);
	for (int i = 0; i < _row * _col; ++i)
		m.a[i] /= n;

	return m;
}

Matrix& Matrix::operator/=(const float n)
{
	for (int i = 0; i < _row * _col; ++i)
		a[i] /= n;

	return *this;
}

Matrix& Matrix::E()
{
	if (_col != _row)
		throw MatrixException(*this, "can't make a E with different col and row.");

	memset(a, 0, sizeof(float) * _row * _col);
	for (int i = 0; i < _col; ++i)
		get(i, i) = 1;

	return *this;
}

const int MAXMATRIX = 100;
float W[MAXMATRIX][MAXMATRIX],
L[MAXMATRIX][MAXMATRIX], U[MAXMATRIX][MAXMATRIX],
L_n[MAXMATRIX][MAXMATRIX], U_n[MAXMATRIX][MAXMATRIX];

Matrix Matrix::Inverse()
{
	int N = this->_row;
	int i, j, k, d;
	float s;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			W[i][j] = this->get(i, j);
			L[i][j] = 0;
			U[i][j] = 0;
			L_n[i][j] = 0;
			U_n[i][j] = 0;
		}
	}

	for (i = 0; i < N; i++)  // L�Խ���1
	{
		L[i][i] = 1.0;
	}

	for (j = 0; j < N; j++)
	{
		U[0][j] = W[0][j];
	}

	for (i = 1; i < N; i++)
	{
		L[i][0] = W[i][0] / U[0][0];
	}

	for (i = 1; i < N; i++)
	{
		for (j = i; j < N; j++) // ��U
		{
			s = 0;
			for (k = 0; k < i; k++)
			{
				s += L[i][k] * U[k][j];
			}
			U[i][j] = W[i][j] - s;
		}

		for (d = i; d < N; d++) // ��L
		{
			s = 0;
			for (k = 0; k < i; k++)
			{
				s += L[d][k] * U[k][i];
			}
			L[d][i] = (W[d][i] - s) / U[i][i];
		}
	}

	for (j = 0; j < N; j++)  //��L����
	{
		for (i = j; i < N; i++)
		{
			if (i == j)
				L_n[i][j] = 1 / L[i][j];
			else if (i < j)
				L_n[i][j] = 0;
			else
			{
				s = 0.;
				for (k = j; k < i; k++)
				{
					s += L[i][k] * L_n[k][j];
				}
				L_n[i][j] = -L_n[j][j] * s;
			}
		}
	}

	for (i = 0; i < N; i++)  //��U����
	{
		for (j = i; j >= 0; j--)
		{
			if (i == j)
				U_n[j][i] = 1 / U[j][i];
			else if (j > i)
				U_n[j][i] = 0;
			else
			{
				s = 0.;
				for (k = j + 1; k <= i; k++)
				{
					s += U[j][k] * U_n[k][i];
				}
				U_n[j][i] = -1 / U[j][j] * s;
			}
		}
	}

	Matrix m(_row, _col);

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			for (k = 0; k < N; k++)
			{
				m.get(i, j) += U_n[i][k] * L_n[k][j];
			}
		}
	}

	return m;
}
