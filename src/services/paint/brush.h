#pragma once

#include <vector>

#include "core/math/vector.h"
#include "services/render/shader.h"

class PaintTarget;
struct RenderObject;

struct BrushHit {
	Vector3<float> position;
	Vector3<float> normal;
};

class Brush {
public:
	float radius = 0.1f;
	float hardness = 0.5f;
	Vector4<float> color{ 1.0f, 0.0f, 0.0f, 1.0f };

	Brush();
	~Brush();

	Brush(const Brush&) = delete;
	Brush& operator=(const Brush&) = delete;

	void stampAll(PaintTarget& strokeBuffer, const RenderObject& obj, const std::vector<BrushHit>& hits);
	void commit(PaintTarget& canvas, const PaintTarget& strokeBuffer);

	Vector4<float> linearColor() const;

private:
	Shader m_shader;
	Shader m_commitShader;
	unsigned int m_emptyVAO = 0;
};