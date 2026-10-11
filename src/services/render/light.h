#pragma once

#include "core/math/vector.h"
#include <vector>

struct DirectionalLight {
	Vector4<float> direction; // w = unused
	Vector4<float> color;     // w = intensity
};

struct OmniLight {
	Vector4<float> position; // w = range
	Vector4<float> color;    // w = intensity
};

struct SpotLight {
	Vector4<float> position;  // w = range
	Vector4<float> direction; // w = cosInner
	Vector4<float> color;     // w = intensity
};

class Lights {
public:
	static constexpr int MAX_POINT_LIGHTS = 32; // GLSL should mirror these ;)
	static constexpr int MAX_SPOT_LIGHTS  = 32;

	Lights();
	~Lights();

	Lights(const Lights&) = delete;
	Lights& operator=(const Lights&) = delete;

	void addPointLight(const OmniLight& light);
	void addSpotLight(const SpotLight & light);
	void addDirectionalLight(const DirectionalLight& light);

	void upload();

private:
	void initBuffers();

	DirectionalLight l_sun;
	std::vector<OmniLight> l_pointLights;
	std::vector<SpotLight> l_spotLights;

	unsigned int UBO;

};
