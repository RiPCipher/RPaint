#pragma once

#include "core/math/vector.h"
#include "services/render/shader.h"
#include "services/render/mesh.h"

class PaintTarget;

class Brush {
public:
	float radius = 0.05f;
	float hardness = 0.5f;
	Vector4<float> color{ 1.0f, 0.0f, 0.0f, 1.0f };

	Brush();

	Brush(const Brush&) = delete;
	Brush& operator=(const Brush&) = delete;

	void stamp(PaintTarget& target, Vector2<float> uv);

private:
	Shader m_shader;
	Mesh   m_quad;
};