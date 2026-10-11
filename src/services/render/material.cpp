#include <glad/glad.h>
#include <cmath>

#include "material.h"
#include "_defaults.h"

static void bindSlot(int unit, const Texture* tex, const Texture& fallback) {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, tex ? tex->TID : fallback.TID);
}

static float srgbToLinear(float c) {
	return c <= 0.04045f ? c / 12.92f : std::pow((c + 0.055f) / 1.055f, 2.4f);
}

void Material::setBaseColorSRGB(float r, float g, float b, float a) {
	baseColorFactor = { srgbToLinear(r), srgbToLinear(g), srgbToLinear(b), a };
}


Material::Material(Shader* shader, const DefaultTextures& defaults,
	Texture* albedo, Texture* normal, Texture* orm)
	: shader(shader), albedo(albedo), normal(normal), orm(orm), m_defaults(defaults) {
}

void Material::bind() {
	shader->use();

	shader->setInt("aTexture", 0);
	shader->setInt("aNormalTexture", 1);
	shader->setInt("aORMTexture", 2);

	bindSlot(0, albedo, m_defaults.albedo);
	bindSlot(1, normal, m_defaults.normal);
	bindSlot(2, orm, m_defaults.orm);

	shader->setVec4("baseColorFactor", baseColorFactor.x, baseColorFactor.y, baseColorFactor.z, baseColorFactor.w);
	shader->setFloat("roughnessFactor", roughnessFactor);
	shader->setFloat("metallicFactor", metallicFactor);
	shader->setFloat("occlusionFactor", occlusionFactor);
}