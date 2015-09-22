// glfw_testDll.h : Defines the exported functions for the DLL application.
//

#include <windows.h>
#include <cstdlib>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#ifdef GLFW_TESTDLL_EXPORTS
#define GLFW_TESTDLL_API __declspec(dllexport)
#else
#define GLFW_TESTDLL_API __declspec(dllimport)
#endif

namespace glfw_testFunc {
	
	typedef GLFWwindow* hGl;

	class glfw_test {
	public:
		GLFW_TESTDLL_API hGl WINAPI init();
		GLFW_TESTDLL_API void WINAPI terminate();

		GLFW_TESTDLL_API void WINAPI mainLoop();

	public:
		hGl window;
	};

}