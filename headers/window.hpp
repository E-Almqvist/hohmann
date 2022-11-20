#pragma once
#include "GLFW/glfw3.h"

// Default window dimensions
#define D_WINDOW_TITLE "Euclid Engine"
#define D_WINDOW_WIDTH 640
#define D_WINDOW_HEIGHT 480

class Window {
	public:
		GLFWwindow* win;

		Window(const char* title);
		Window(const char* title, unsigned int w, unsigned int h);

		void spawn();

		unsigned int width()  { return _width; }  // getters and dumb and 
		unsigned int height() { return _height; } // cpp should have readonly fields...

	private:
		const char* _title = D_WINDOW_TITLE;
		unsigned int _width = D_WINDOW_WIDTH;
		unsigned int _height = D_WINDOW_HEIGHT;

		void updateSize(); 
		void setWidth(unsigned int w)  { _width = w; }
		void setHeight(unsigned int h) { _height = h; }
};
