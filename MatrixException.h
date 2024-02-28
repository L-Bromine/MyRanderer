#pragma once
#include "MyException.h"
#include <format>

class Matrix;

class MatrixException : MyException
{
private:
	const Matrix* ap;
	const Matrix* bp;
public:
	MatrixException(const string& msg);
	MatrixException(const Matrix& a, const string& msg);
	MatrixException(const Matrix& a, const Matrix& b, const string& msg);

	virtual string ToString();
};

