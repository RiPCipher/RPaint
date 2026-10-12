#pragma once

#include <cmath>
#include <vector>
#include "core/math/vector.h"

class PaintState {
public:
	float spacing = 2.0f;

	bool active() const { return m_active; }

	std::vector<Vector2<float>> beginPaint(Vector2<float> pos) {
		m_active  = true;
		m_lastPos = pos;
		m_carry   = 0.0f;
		return { pos };
	}

	std::vector<Vector2<float>> continuePaint(Vector2<float> pos) {
		std::vector<Vector2<float>> out;
		walkSegment(m_lastPos, pos, out);
		m_lastPos = pos;
		return out;
	}

	void endPaint() {
		m_active = false;
		m_carry  = 0.0f;
	}

private:
	void walkSegment(Vector2<float> from, Vector2<float> to, std::vector<Vector2<float>>& out) {
		Vector2<float> distance = to - from;
		float length = std::sqrt(distance.dot(distance));
		if (length <= 0.0f || spacing <= 0.0f) return;

		Vector2<float> direction = distance / length;
		float t = spacing - m_carry;
		while (t <= length) {
			out.push_back(from + direction * t);
			t += spacing;
		}
		m_carry = length - (t - spacing);
	}

	bool m_active = false;
	Vector2<float> m_lastPos;
	float m_carry = 0.0f;
};
