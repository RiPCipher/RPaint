#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "core/app/app.h"
#include "services/input/keybinds.h"

bool App::initialize() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	m_window = glfwCreateWindow((int)m_viewport.size.x, (int)m_viewport.size.y, "RPaint", nullptr, nullptr);
	if (!m_window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}

	m_defaults = std::make_unique<DefaultTextures>();

	glfwSwapInterval(1);

	registerDefaultBinds(m_input);

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(m_window, this);
	glfwSetCursorPosCallback(m_window, onMouseMove);
	glfwSetFramebufferSizeCallback(m_window, onFramebufferResize);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	return true;
}

void App::run() {
	onStart();

	float lastFrame = 0.0f;
	while (!glfwWindowShouldClose(m_window)) {
		float now = (float)glfwGetTime();
		float dt = now - lastFrame;
		lastFrame = now;

		m_input.update(m_window);
		m_camera.processKeyboard(m_input, dt);
		if (m_input.wasPressed(Action::KeyEsc)) glfwSetWindowShouldClose(m_window, true);

		glClearColor(0.55f, 0.65f, 0.78f, 1.0f); // sky, roughly matches the shader's ambient skyColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		onUpdate(dt);

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	onShutdown();
	m_defaults.reset();
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

//////////////// Program //////////////// :)

void App::onStart() {
	m_scene = std::make_unique<DemoScene>(*m_defaults);

	m_camera.position = { 0.0f, 2.0f, 12.0f };
	m_camera.speed    = 4.0f;
}

void App::onUpdate(float dt) {
	int width = 0, height = 0;
	glfwGetFramebufferSize(m_window, &width, &height);
	if (width == 0 || height == 0) return; // minimized

	m_scene->update(dt);
	m_scene->render(m_camera, width, height);
}

void App::onShutdown() {
	m_scene.reset(); // before m_defaults and the GL context go away
}

void App::onFramebufferResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);

	App* app = (App*)glfwGetWindowUserPointer(window);
	app->m_viewport.size = { (float)width, (float)height };
}

void App::onMouseMove(GLFWwindow* window, double xPos, double yPos) {
	static float lastX = 0.0f, lastY = 0.0f;
	static bool firstMouse = true;

	if (firstMouse) {
		lastX = (float)xPos;
		lastY = (float)yPos;
		firstMouse = false;
	}

	float xOffset = (float)xPos - lastX;
	float yOffset = lastY - (float)yPos;
	lastX = (float)xPos;
	lastY = (float)yPos;

	App* app = (App*)glfwGetWindowUserPointer(window);
	app->m_camera.processMouse(xOffset, yOffset);
}
