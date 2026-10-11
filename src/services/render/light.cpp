#include <glad/glad.h>

#include "services/render/light.h"
#include "core/math/vector.h"

Lights::Lights() {
	initBuffers();
}

Lights::~Lights() {
	glDeleteBuffers(1, &UBO);
}

void Lights::initBuffers() {
	const int UBO_SIZE =
		sizeof(DirectionalLight) + sizeof(int) * 4 + sizeof(OmniLight) 
		* MAX_POINT_LIGHTS + sizeof(SpotLight) * MAX_SPOT_LIGHTS;

	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);

	glBufferData(GL_UNIFORM_BUFFER, UBO_SIZE, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);

	glBindBuffer(GL_UNIFORM_BUFFER, 0); // unbind
}

void Lights::upload() {
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);

	const int SUN_OFFSET    = 0;
	const int COUNTS_OFFSET = SUN_OFFSET    + sizeof(DirectionalLight);
	const int POINTS_OFFSET = COUNTS_OFFSET + sizeof(int) * 4;
	const int SPOTS_OFFSET  = POINTS_OFFSET + sizeof(OmniLight) * MAX_POINT_LIGHTS;

	int counts[4]{ (int)l_pointLights.size(), (int)l_spotLights.size(), 0, 0 };

	glBufferSubData(GL_UNIFORM_BUFFER, SUN_OFFSET,    sizeof(DirectionalLight),                  &l_sun);
	glBufferSubData(GL_UNIFORM_BUFFER, COUNTS_OFFSET, sizeof(counts),                            counts);
	glBufferSubData(GL_UNIFORM_BUFFER, POINTS_OFFSET, l_pointLights.size() * sizeof(OmniLight),  l_pointLights.data());
	glBufferSubData(GL_UNIFORM_BUFFER, SPOTS_OFFSET,  l_spotLights.size() * sizeof(SpotLight),   l_spotLights.data());
}

void Lights::addPointLight(const OmniLight& light) {
	if (l_pointLights.size() >= MAX_POINT_LIGHTS) return;
	l_pointLights.push_back(light);
}

void Lights::addSpotLight(const SpotLight& light) {
	if (l_spotLights.size() >= MAX_SPOT_LIGHTS) return;
	l_spotLights.push_back(light);
}

void Lights::addDirectionalLight(const DirectionalLight& light) {
	l_sun = light;
}
