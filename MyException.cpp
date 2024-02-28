#include "MyException.h"

MyException::MyException()
{
	info = "Unknowing Exception!";
}

MyException::MyException(const string& msg)
{
	info = msg;
}

string MyException::ToString()
{
	return info;
}
