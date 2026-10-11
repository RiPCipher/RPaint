#pragma once

#include <vector>

#include "services/render/shader.h"
#include "services/render/mesh.h"
#include "services/render/texture.h"
#include "services/render/material.h"
#include "services/render/light.h"
#include "services/render/shadow.h"
#include "services/paint/paint_target.h"
#include "services/render/render.h"

struct Camera;
struct DefaultTextures;

// Hard-coded test scene: a row of hearts on a ground slab, lit by a shadow-casting sun + two point lights.
// NOTE: makes GL calls in its constructor, so create it after the context exists.
class DemoScene {
public:
	explicit DemoScene(const DefaultTextures& defaults);

	DemoScene(const DemoScene&) = delete;
	DemoScene& operator=(const DemoScene&) = delete;

	void update(float dt);
	void render(Camera& camera, int width, int height);

private:
	void shadowPass(const Matrix::Matrix4& lightSpace);

	Shader m_pbr;
	Shader m_depth;

	Mesh m_heartMesh;
	Mesh m_groundMesh;

	Texture m_testAlbedo, m_testNormal, m_testORM;
	Texture m_roughAlbedo, m_roughNormal, m_roughORM;

	PaintTarget m_paint; // must be declared before the materials that point at its texture

	Material m_redPlastic;
	Material m_gold;
	Material m_textured;
	Material m_roughTest;
	Material m_ground;

	Lights    m_lights;
	ShadowMap m_shadow;

	std::vector<RenderObject> m_objects; // [0] = ground, the rest are hearts
	float m_time = 0.0f;
};
