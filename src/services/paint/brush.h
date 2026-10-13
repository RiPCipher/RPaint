#pragma once

#include <vector>

#include "core/math/vector.h"
#include "services/render/shader.h"

class PaintTarget;
struct RenderObject;

class Brush {
public:
	float radius = 0.1f;
	float hardness = 0.5f;
	Vector4<float> color{ 1.0f, 0.0f, 0.0f, 1.0f };

	Brush();

	Brush(const Brush&) = delete;
	Brush& operator=(const Brush&) = delete;

	void stampAll(PaintTarget& target, const RenderObject& obj, const std::vector<Vector3<float>>& centers);

private:
	Shader m_shader;
};