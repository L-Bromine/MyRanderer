#pragma once
#include <exception>
#include <string>
using std::string;

class MyException : std::exception
{
protected:
	string info;
public:
	MyException();
	MyException(const string& msg);

	virtual string ToString();
};

