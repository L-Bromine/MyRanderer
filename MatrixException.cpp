#include "MatrixException.h"
#include "Matrix.h"

MatrixException::MatrixException(const string& msg)
{
	ap = NULL;
	bp = NULL;
	info = msg;
}

MatrixException::MatrixException(const Matrix& a, const string& msg)
{
	ap = &a;
	bp = NULL;
	info = msg;
}

MatrixException::MatrixException(const Matrix& a, const Matrix& b, const string& msg)
{
	ap = &a;
	bp = &b;
	info = msg;
}

string MatrixException::ToString()
{
	if (ap == NULL)
		return info;
	else if (bp == NULL)
		return info + std::format("by operating matrix %dx%d.", ap->_row, ap->_col);
	else
		return info + std::format("by operating matrix %dx%d and matrix %dx%d.", ap->_row, ap->_col, bp->_row, bp->_col);
}
