#include <glad/glad.h>

#include "main/demo_scene.h"

#include "core/io/file_io.h"
#include "core/math/camera.h"
#include "core/math/math.h"
#include "services/render/_defaults.h"
#include "services/render/mesh_primitives.h"
#include "extensions/assimp/assimp_loader.h"
#include "extensions/stb/stb_image_loader.h"

namespace {
	// Direction the sunlight travels (from the sun toward the ground)
	const Vector3<float> SUN_DIR = Vector3<float>(-0.4f, -1.0f, -0.3f).normalized();

	ImageData loadImage(const char* path) {
		StbImageLoader loader;
		std::vector<uint8_t> bytes = io::readBytes(path);
		return loader.load(bytes);
	}

	MeshData loadMesh(const char* path) {
		AssimpLoader loader;
		return loader.loadFile(path);
	}
}

DemoScene::DemoScene(const DefaultTextures& defaults)
	: m_pbr  ("assets/shaders/default.glsl")
	, m_depth("assets/shaders/depth.glsl")

	, m_heartMesh (loadMesh("assets/objects/HighPoly-Heart.obj"))
	, m_groundMesh(Primitives::cube())

	, m_testAlbedo (loadImage("assets/textures/test_texture_1_albedo.png"), true)
	, m_testNormal (loadImage("assets/textures/test_texture_1_normal.png"))
	, m_testORM    (loadImage("assets/textures/test_texture_1_orm.png"))
	, m_roughAlbedo(loadImage("assets/textures/roughness_test_1_albedo.png"), true)
	, m_roughNormal(loadImage("assets/textures/roughness_test_1_normal.png"))
	, m_roughORM   (loadImage("assets/textures/roughness_test_1_orm.png"))

	, m_redPlastic(&m_pbr, defaults)
	, m_gold      (&m_pbr, defaults)
	, m_textured  (&m_pbr, defaults, &m_testAlbedo,  &m_testNormal,  &m_testORM)
	, m_roughTest (&m_pbr, defaults, &m_roughAlbedo, &m_roughNormal, &m_roughORM)
	, m_ground    (&m_pbr, defaults)
{
	// Factor-only materials: no textures, so the 1x1 defaults pass through and the factors decide
	m_redPlastic.setBaseColorSRGB(0.85f, 0.10f, 0.18f);
	m_redPlastic.metallicFactor  = 0.0f;
	m_redPlastic.roughnessFactor = 0.35f;

	m_gold.setBaseColorSRGB(1.00f, 0.77f, 0.34f);
	m_gold.metallicFactor  = 1.0f;
	m_gold.roughnessFactor = 0.25f;

	m_ground.setBaseColorSRGB(0.45f, 0.45f, 0.48f);
	m_ground.metallicFactor  = 0.0f;
	m_ground.roughnessFactor = 0.85f;

	// Ground: a flattened cube whose top face sits at y = 0
	RenderObject ground{ &m_groundMesh, &m_ground };
	ground.transform.position = { 0.0f, -0.1f, 0.0f };
	ground.transform.scale    = { 200.0f, 0.2f, 200.0f };
	m_objects.push_back(ground);

	// Hearts: the model lies flat in XZ with its tip toward +Z, so +90deg about X stands it up
	Material* heartMaterials[] = { &m_redPlastic, &m_gold, &m_textured, &m_roughTest };
	for (int i = 0; i < 4; ++i) {
		RenderObject heart{ &m_heartMesh, heartMaterials[i] };
		heart.transform.position = { -4.5f + 3.0f * i, 1.35f, 0.0f };
		heart.transform.rotation = { Constant::PIf * 0.5f, 0.0f, 0.0f };
		m_objects.push_back(heart);
	}

	// Lighting: warm sun + a red and a blue fill light in front of the hearts
	m_lights.addDirectionalLight({ { SUN_DIR.x, SUN_DIR.y, SUN_DIR.z, 0.0f }, { 1.0f, 0.95f, 0.85f, 3.0f } });
	m_lights.addPointLight({ { -3.0f, 2.5f, 2.5f, 8.0f }, { 1.0f, 0.35f, 0.25f, 4.0f } });
	m_lights.addPointLight({ {  3.0f, 2.5f, 2.5f, 8.0f }, { 0.3f, 0.5f, 1.0f, 4.0f } });
	m_lights.upload(); // static lights, so once is enough
}

void DemoScene::update(float dt) {
	m_time += dt;

	// Spin each heart around world Y (Rx is applied first in modelMatrix, so this turns the upright heart)
	for (size_t i = 1; i < m_objects.size(); ++i) {
		m_objects[i].transform.rotation.y = m_time * 0.5f + (float)i;
	}
}

void DemoScene::render(Camera& camera, int width, int height) {
	// Light-space matrix: an orthographic box looking down the sun direction, covering the hearts
	Vector3<float> sunPos = SUN_DIR * -20.0f;
	Matrix::Matrix4 lightView  = Matrix::lookAt(sunPos, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
	Matrix::Matrix4 lightProj  = Matrix::ortho(-12.0f, 12.0f, -12.0f, 12.0f, 1.0f, 50.0f);
	Matrix::Matrix4 lightSpace = Matrix::multiply(lightProj, lightView);

	shadowPass(lightSpace);

	// Main pass
	glViewport(0, 0, width, height);

	m_pbr.use();
	m_pbr.setMat4("lightSpaceMatrix", lightSpace);
	m_pbr.setInt("shadowMap", 3); // units 0-2 belong to the material
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_shadow.depthTex);

	Matrix::Matrix4 projection = Matrix::perspective(Constant::PIf / 4.0f, (float)width / (float)height, 0.1f, 100.0f);
	for (RenderObject& obj : m_objects) {
		draw(obj, camera, projection);
	}
}

void DemoScene::shadowPass(const Matrix::Matrix4& lightSpace) {
	glViewport(0, 0, ShadowMap::WIDTH, ShadowMap::HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadow.FBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	m_depth.use();
	m_depth.setMat4("lightSpaceMatrix", lightSpace);
	for (RenderObject& obj : m_objects) {
		m_depth.setMat4("Model", obj.transform.modelMatrix());
		glBindVertexArray(obj.mesh->VAO);
		glDrawElements(GL_TRIANGLES, obj.mesh->indexCount, GL_UNSIGNED_INT, 0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
