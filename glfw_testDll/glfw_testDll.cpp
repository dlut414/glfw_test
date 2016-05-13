// glfw_testDll.cpp : Defines the exported functions for the DLL application.
//

#include <cstdio>
#include "glfw_testDll.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

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

	#define BW  64
	#define BH  64

	unsigned char buffer[24 << 20];
	unsigned char screen[BH][BW];
	unsigned char screen_processed[BH][BW][4];

	void glfw_test::mainLoop() {
		while (!glfwWindowShouldClose(this->window)) {

			///font staffs
			stbtt_fontinfo font;
			int ascent, baseline, ch = 0;
			float scale, xpos = 2; // leave a little padding in case the character extends left
			char *text = "A10BDC"; // intentionally misspelled to show 'lj' brokenness

			fread(buffer, 1, 1000000, fopen("c:/windows/fonts/arialbd.ttf", "rb"));
			stbtt_InitFont(&font, buffer, 0);

			scale = stbtt_ScaleForPixelHeight(&font, 15);
			stbtt_GetFontVMetrics(&font, &ascent, 0, 0);
			baseline = (int)(ascent*scale);

			while (text[ch]) {
				int advance, lsb, x0, y0, x1, y1;
				float x_shift = xpos - (float)floor(xpos);
				stbtt_GetCodepointHMetrics(&font, text[ch], &advance, &lsb);
				stbtt_GetCodepointBitmapBoxSubpixel(&font, text[ch], scale, scale, x_shift, 0, &x0, &y0, &x1, &y1);
				stbtt_MakeCodepointBitmapSubpixel(&font, &screen[baseline + y0][(int)xpos + x0], x1 - x0, y1 - y0, BW, scale, scale, x_shift, 0, text[ch]);
				// note that this stomps the old data, so where character boxes overlap (e.g. 'lj') it's wrong
				// because this API is really for baking character bitmaps into textures. if you want to render
				// a sequence of characters, you really need to render each bitmap to a temp buffer, then
				// "alpha blend" that into the working buffer
				xpos += (advance * scale);
				if (text[ch + 1])
					xpos += scale*stbtt_GetCodepointKernAdvance(&font, text[ch], text[ch + 1]);
				++ch;
			}
			for (int i = 0; i < BH; ++i) {
				for (int j = 0; j < BW; ++j) {
					putchar(" .:ioVM@"[screen[i][j] >> 5]);
					screen_processed[i][j][3] = screen[BW - i - 1][j];
					screen_processed[i][j][0] = 0;
					screen_processed[i][j][1] = 0;
					screen_processed[i][j][2] = 0;
				}
				putchar('\n');
			}

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
			//glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
			//glBegin(GL_TRIANGLES);
			//glColor3f(1.f, 0.f, 0.f);
			//glVertex3f(-0.6f, -0.4f, 0.f);
			//glColor3f(0.f, 1.f, 0.f);
			//glVertex3f(0.6f, -0.4f, 0.f);
			//glColor3f(0.f, 0.f, 1.f);
			//glVertex3f(0.f, 0.6f, 0.f);
			//glEnd();
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			//glDisable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glClearColor(1, 1, 1, 0);
			GLuint texid;
			glGenTextures(1, &texid);
			glBindTexture(GL_TEXTURE_2D, texid);
		 // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, BW, BH, 0, GL_RGBA, GL_UNSIGNED_BYTE, screen_processed);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glBegin(GL_QUADS);
				glTexCoord3f(0.0f, 0.0f, 0.0f);
				glVertex3f(-0.5f, -0.5f, 0.0f);
				glTexCoord3f(1.0f, 0.0f, 0.0f);
				glVertex3f(0.5f, -0.5f, 0.0f);
				glTexCoord3f(1.0f, 1.0f, 0.0f);
				glVertex3f(0.5f, 0.5f, 0.0f);
				glTexCoord3f(0.0f, 1.0f, 0.0f);
				glVertex3f(-0.5f, 0.5f, 0.0f);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);

			glfwSwapBuffers(this->window);
			glfwPollEvents();
			//glfwWaitEvents();
		}
	}

}