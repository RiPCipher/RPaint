#pragma once

#include <cmath>




namespace Constant {
	// Floats
	constexpr float PIf = 3.14159265358979323846f;
	constexpr float EPSILONf = 0.00000001f;
	constexpr float TAUf = 2.0 * PIf;

	//Doubles
	constexpr double PI = 3.14159265358979323846;
	constexpr double EPSILON = 0.00000001;
	constexpr double TAU = 2.0 * PI;
}

namespace Color {
	inline float srgbToLinear(float c) {
		return c <= 0.04045f ? c / 12.92f : std::pow((c + 0.055f) / 1.055f, 2.4f);
	}
}