#pragma once

#include "texture.h"
#include "shader.h"

struct DefaultTextures;

class Material {
public:
	Shader* shader;
	Texture* albedo;
	Texture* normal;
	Texture* orm;

	Vector4<float> baseColorFactor{ 1.0f, 1.0f, 1.0f, 1.0f };
	float roughnessFactor = 1.0f;
	float metallicFactor = 1.0f;
	float occlusionFactor = 1.0f;

	Material(Shader* shader, const DefaultTextures& defaults, Texture* albedo = nullptr, Texture* normal = nullptr, Texture* orm = nullptr);

	void bind();
	void setBaseColorSRGB(float r, float g, float b, float a = 1.0f);

private:
	const DefaultTextures& m_defaults;
};
