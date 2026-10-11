#pragma once

#include "matrix.h"
#include "math.h"

struct Transform {
	Vector3<float> position;
	Vector3<float> rotation;
	Vector3<float> scale;


	Transform() {
		position = { 0.0f, 0.0f, 0.0f };
		rotation = { 0.0f, 0.0f, 0.0f };
		scale = { 1.0f, 1.0f, 1.0f };

	}

	Matrix::Matrix4 modelMatrix() {
		Matrix::Matrix4 T = Matrix::translation(position);

		Matrix::Matrix4 Rx = Matrix::rotationX(rotation.x);
		Matrix::Matrix4 Ry = Matrix::rotationY(rotation.y);
		Matrix::Matrix4 Rz = Matrix::rotationZ(rotation.z);
		Matrix::Matrix4 R = Matrix::multiply(Rz, Matrix::multiply(Ry, Rx));

		Matrix::Matrix4 S = Matrix::scale(scale);

		return Matrix::multiply(T, Matrix::multiply(R, S));
	}
};