#pragma once

#include "services/input/input.h"

inline void registerDefaultBinds(Input& input) {
	input.bind(Action::MoveUp, GLFW_KEY_SPACE);
	input.bind(Action::MoveDown, GLFW_KEY_LEFT_SHIFT);
	input.bind(Action::ToggleCursor, GLFW_KEY_LEFT_ALT);

	// Generic Keybinds
	input.bind(Action::KeyA, GLFW_KEY_A);
	input.bind(Action::KeyB, GLFW_KEY_B);
	input.bind(Action::KeyC, GLFW_KEY_C);
	input.bind(Action::KeyD, GLFW_KEY_D);
	input.bind(Action::KeyE, GLFW_KEY_E);
	input.bind(Action::KeyF, GLFW_KEY_F);
	input.bind(Action::KeyG, GLFW_KEY_G);
	input.bind(Action::KeyH, GLFW_KEY_H);
	input.bind(Action::KeyI, GLFW_KEY_I);
	input.bind(Action::KeyJ, GLFW_KEY_J);
	input.bind(Action::KeyK, GLFW_KEY_K);
	input.bind(Action::KeyL, GLFW_KEY_L);
	input.bind(Action::KeyM, GLFW_KEY_M);
	input.bind(Action::KeyN, GLFW_KEY_N);
	input.bind(Action::KeyO, GLFW_KEY_O);
	input.bind(Action::KeyP, GLFW_KEY_P);
	input.bind(Action::KeyQ, GLFW_KEY_Q);
	input.bind(Action::KeyR, GLFW_KEY_R);
	input.bind(Action::KeyS, GLFW_KEY_S);
	input.bind(Action::KeyT, GLFW_KEY_T);
	input.bind(Action::KeyU, GLFW_KEY_U);
	input.bind(Action::KeyV, GLFW_KEY_V);
	input.bind(Action::KeyW, GLFW_KEY_W);
	input.bind(Action::KeyX, GLFW_KEY_X);
	input.bind(Action::KeyY, GLFW_KEY_Y);
	input.bind(Action::KeyZ, GLFW_KEY_Z);

	input.bind(Action::Key0, GLFW_KEY_0);
	input.bind(Action::Key1, GLFW_KEY_1);
	input.bind(Action::Key2, GLFW_KEY_2);
	input.bind(Action::Key3, GLFW_KEY_3);
	input.bind(Action::Key4, GLFW_KEY_4);
	input.bind(Action::Key5, GLFW_KEY_5);
	input.bind(Action::Key6, GLFW_KEY_6);
	input.bind(Action::Key7, GLFW_KEY_7);
	input.bind(Action::Key8, GLFW_KEY_8);
	input.bind(Action::Key9, GLFW_KEY_9);

	input.bind(Action::KeyEsc, GLFW_KEY_ESCAPE);
	input.bind(Action::KeySpace, GLFW_KEY_SPACE);

	input.bind(Action::MouseLeft, GLFW_MOUSE_BUTTON_LEFT);
	input.bind(Action::MouseRight, GLFW_MOUSE_BUTTON_RIGHT);
	input.bind(Action::MouseMiddle, GLFW_MOUSE_BUTTON_MIDDLE);
}