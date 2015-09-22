// glfw_testDll.cpp : Defines the exported functions for the DLL application.
//

#include <cstdio>
#include "glfw_testDll.h"

namespace glfw_testFunc {

	static void error_callback(int error, const char* message) {
		fputs(message, stderr);
	}

	static void key_callback(hGl win, int key, int code, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(win, GL_TRUE);
	}

	hGl glfw_test::init() {
		if (!glfwInit()) exit(EXIT_FAILURE);
		this->window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
		if (!this->window) {
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);
		glfwSetErrorCallback(error_callback);
		glfwSetKeyCallback(this->window, key_callback);
		return this->window;
	}

	void glfw_test::terminate() {
		glfwDestroyWindow(this->window);
		glfwTerminate();
	}

	void glfw_test::mainLoop() {
		while (!glfwWindowShouldClose(this->window)) {
			int width, height;
			glfwGetFramebufferSize(this->window, &width, &height);

			const float ratio = width / (float)height;
			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
			glBegin(GL_TRIANGLES);
			glColor3f(1.f, 0.f, 0.f);
			glVertex3f(-0.6f, -0.4f, 0.f);
			glColor3f(0.f, 1.f, 0.f);
			glVertex3f(0.6f, -0.4f, 0.f);
			glColor3f(0.f, 0.f, 1.f);
			glVertex3f(0.f, 0.6f, 0.f);
			glEnd();

			glfwSwapBuffers(this->window);
			glfwPollEvents();
			//glfwWaitEvents();
		}
	}

}