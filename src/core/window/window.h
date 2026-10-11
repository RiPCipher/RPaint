#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "services/render/viewport.h"

class Window {
public:
	Window() {
		init();
	}

	void init() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		void createWindow();
	}

	void createWindow() {
		Viewport viewport;

		GLFWwindow* window = glfwCreateWindow((int)viewport.size.x, (int)viewport.size.y, "RPaint", NULL, NULL);
		glfwMakeContextCurrent(window);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		glfwSwapInterval(1);
	}

};