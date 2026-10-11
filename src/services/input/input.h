#pragma once

#include <unordered_map>
#include <GLFW/glfw3.h>

enum class Action {
	MoveUp, MoveDown, ToggleCursor,

	KeyA, KeyB, KeyC, KeyD, KeyE, KeyF, KeyG, KeyH, KeyI, KeyJ, KeyK, KeyL, KeyM,KeyN, KeyO, KeyP, KeyQ, KeyR, KeyS, KeyT, KeyU, KeyV, KeyW, KeyX, KeyY, KeyZ,
	Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9,

	KeyEsc, KeySpace, KeyLShift, KeyLAlt,

	//Mouse Input
	MouseLeft, MouseRight, MouseMiddle,

};

class Input {
public:

	void bind(Action action, int glfwKey) { bindings[action] = glfwKey; }

	void update(GLFWwindow* window) {
		previous = current;
		for (auto& [action, key] : bindings) current[action] = (glfwGetKey(window, key) == GLFW_PRESS);
	}

	bool isDown(Action a) const { return get(current, a); }
	bool wasPressed(Action a) const { return get(current, a) && !get(previous, a); }



	// Mouse Input
	//void readPixel(int mX,  int mY, int height, int pixels) {
	//	glReadPixels(mX, height - mY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &pixels);
	//}

private:
	std::unordered_map<Action, int> bindings;
	std::unordered_map<Action, bool> current, previous;

	static bool get(const std::unordered_map<Action, bool>& m, Action a) {
		auto it = m.find(a);
		return it != m.end() && it->second;
	}
};