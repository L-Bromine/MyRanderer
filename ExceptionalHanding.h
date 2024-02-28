#pragma once
#include "MyException.h"
class ExceptionalHanding
{
public:
	static void handle(void(*func)());
};

