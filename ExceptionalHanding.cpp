#include "ExceptionalHanding.h"
#include "MatrixException.h"

void ExceptionalHanding::handle(void(*func)())
{
	try
	{
		func();
	}
	catch (MatrixException exc)
	{
		printf("MatrixException: %s", exc.ToString().c_str());
	}
	catch (MyException exc)
	{
		printf("Exception: %s", exc.ToString().c_str());
	}
}
