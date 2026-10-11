#pragma once

#include <stdint.h>
#include <array>
#include <cstring>
#include "vector.h"
#include "math.h"

namespace Matrix {

	// ------------------- Matrix4 ------------------- //
	struct Matrix4 {
		std::array<float, 16>m;

		Matrix4() {
			m.fill(0.0f);
			m[0] = 1.0f;
			m[5] = 1.0f;
			m[10] = 1.0f;
			m[15] = 1.0f;
		};

		const float* data() const { return m.data(); }

		float operator()(int row, int col) const {
			return m[col * 4 + row];
		}

		float& operator()(int row, int col) {
			return m[col * 4 + row];
		}

	};

	inline Matrix4 multiply(const Matrix4& a, const Matrix4& b) {

		Matrix4 result;

		for (int col = 0; col < 4; ++col)
			for (int row = 0; row < 4; ++row) {
				float sum = 0.0f;
				for (int k = 0; k < 4; ++k) {
					sum += a(row, k) * b(k, col);
				}
				result(row, col) = sum;
			}

		return result;

	}

	inline Matrix4 translation(Vector3<float> t) {
		Matrix4 result;

		result(0, 3) = t.x;
		result(1, 3) = t.y;
		result(2, 3) = t.z;


		return result;
	}

	inline Matrix4 scale(Vector3<float> s) {
		Matrix4 result;

		result(0, 0) = s.x;
		result(1, 1) = s.y;
		result(2, 2) = s.z;


		return result;
	}

	inline Matrix4 rotationX(float angle) {
		Matrix4 result;
		result(1, 1) = cos(angle);
		result(1, 2) = -sin(angle);
		result(2, 1) = sin(angle);
		result(2, 2) = cos(angle);

		return result;
	}

	inline Matrix4 rotationY(float angle) {
		Matrix4 result;
		result(0, 0) = cos(angle);
		result(0, 2) = sin(angle);
		result(2, 0) = -sin(angle);
		result(2, 2) = cos(angle);

		return result;
	}

	inline Matrix4 rotationZ(float angle) {
		Matrix4 result;
		result(0, 0) = cos(angle);
		result(0, 1) = -sin(angle);
		result(1, 0) = sin(angle);
		result(1, 1) = cos(angle);

		return result;
	}

	inline Matrix4 lookAt(Vector3<float> eye, Vector3<float> target, Vector3<float> worldUp) {
		Matrix4 result;
		Vector3<float> forward = (target - eye).normalized();
		Vector3<float> right = (forward.cross(worldUp).normalized());
		Vector3<float> up = (right.cross(forward));

		result(0, 0) = right.x;
		result(0, 1) = right.y;
		result(0, 2) = right.z;
		result(0, 3) = -right.dot(eye);

		result(1, 0) = up.x;
		result(1, 1) = up.y;
		result(1, 2) = up.z;
		result(1, 3) = -up.dot(eye);

		result(2, 0) = -forward.x;
		result(2, 1) = -forward.y;
		result(2, 2) = -forward.z;
		result(2, 3) = forward.dot(eye);

		return result;
	}


	inline Matrix4 perspective(float fovy, float aspect, float near, float far) {
		Matrix4 result;
		float f = 1 / tan(fovy / 2);

		result(0, 0) = f / aspect;
		result(1, 1) = f;
		result(2, 2) = (far + near) / (near - far);
		result(2, 3) = (2 * far * near) / (near - far);
		result(3, 2) = -1;
		result(3, 3) = 0;

		return result;

	}

	inline Matrix4 ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
		Matrix4 result;
		result(0, 0) = 2.0f / (right - left);
		result(1, 1) = 2.0f / (top - bottom);
		result(2, 2) = -2.0f / (zFar - zNear);
		result(0, 3) = -(right + left) / (right - left);
		result(1, 3) = -(top + bottom) / (top - bottom);
		result(2, 3) = -(zFar + zNear) / (zFar - zNear);
		result(3, 3) = 1.0f;
		return result;
	}

	// ------------------- Matrix3 ------------------- //
	struct Matrix3 {
		std::array<float, 16>m;

		Matrix3() {
			m.fill(0.0f);
			m[0] = 1.0f;
			m[5] = 1.0f;
			m[10] = 1.0f;
			m[15] = 1.0f;
		};

		const float* data() const { return m.data(); }

		float operator()(int row, int col) const {
			return m[col * 4 + row];
		}

		float& operator()(int row, int col) {
			return m[col * 4 + row];
		}

	};

}
