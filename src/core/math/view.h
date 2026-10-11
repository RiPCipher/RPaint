#pragma once

#include "matrix.h"
#include "math.h"

struct View {
	Vector3<float> eye;
	Vector3<float> target;
	Vector3<float> worldUp;

	View() {
		eye = { 2.5f, 2.5f, 2.5f };
		target = { 0.0f, 0.0f, 0.0f };
		worldUp = { 0.0f, 1.0f, 0.0f };
	}

	Matrix::Matrix4 viewMatrix() {
		return Matrix::lookAt(eye, target, worldUp);
	}
};