#pragma once

#include "core/math/vector.h"

struct Viewport {

	Vector2<float> size{ 900, 600 };

	inline float getAspect() const {
		return size.x / size.y;
	};

};
