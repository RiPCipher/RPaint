#pragma once

#include <cmath>
#include "core/math/matrix.h"
#include "core/math/vector.h"
#include "core/math/math.h"
#include "services/input/keybinds.h"

#include <GLFW/glfw3.h>

struct Camera {
	Vector3<float> position = { 0.0f, 0.0f, 3.0f };
	float yaw = -90.0f;
	float pitch = 0.0f;
	float speed = 1.5f;
	float sensitivity = 0.1f;

	Vector3<float> worldUp = { 0.0f, 1.0f, 0.0f };

	Vector3<float> forward() const {
		const float rad = Constant::PIf / 180.0f;
		Vector3<float> f;
		f.x = std::cos(yaw * rad) * std::cos(pitch * rad);
		f.y = std::sin(pitch * rad);
		f.z = std::sin(yaw * rad) * std::cos(pitch * rad);
		return f.normalized();
	}

	Matrix::Matrix4 viewMatrix() const {
		Vector3<float> f = forward();
		return Matrix::lookAt(position, position + f, worldUp);
	}

	void processKeyboard(const Input& input, float deltaTime) {
		Vector3<float> f = forward();
		Vector3<float> right = f.cross(worldUp).normalized();
		float velocity = speed * deltaTime;

		if (input.isDown(Action::KeyW)) position += f * velocity;
		if (input.isDown(Action::KeyS))    position -= f * velocity;
		if (input.isDown(Action::KeyA))    position -= right * velocity;
		if (input.isDown(Action::KeyD))   position += right * velocity;
		if (input.isDown(Action::MoveUp))      position += worldUp * velocity;
		if (input.isDown(Action::MoveDown))    position -= worldUp * velocity;
	}

	void processMouse(float xOffset, float yOffset) {
		yaw += xOffset * sensitivity;
		pitch += yOffset * sensitivity;
		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
	}
};