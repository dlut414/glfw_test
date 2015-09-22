// glfw_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>
#include <glfw_testDll.h>

using namespace glfw_testFunc;

int _tmain(int argc, _TCHAR* argv[])
{
	glfw_test obj;
	obj.init();
	obj.mainLoop();
	obj.terminate();
	return 0;
}

