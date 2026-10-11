#pragma once

#include <memory>
#include "services/render/_defaults.h"
#include "main/demo_scene.h"

#include "services/input/input.h"
#include "services/render/viewport.h"
#include "core/math/camera.h"

struct GLFWwindow;

class App {
public:
	bool initialize();
	void run();

	Camera&         camera()         { return m_camera; }
	Input&          input()          { return m_input; }
	const Viewport& viewport() const { return m_viewport; }
	GLFWwindow*     window()         { return m_window; }

private:
	std::unique_ptr<DefaultTextures> m_defaults;
	std::unique_ptr<DemoScene>       m_scene;

	GLFWwindow* m_window = nullptr;
	Viewport    m_viewport;
	Input       m_input;
	Camera      m_camera;

	void onStart();
	void onUpdate(float dt);
	void onShutdown();

	static void onFramebufferResize(GLFWwindow* window, int width, int height);
	static void onMouseMove(GLFWwindow* window, double xPos, double yPos);
};
