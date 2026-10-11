// NOTE: no #version here on purpose. The loader prepends "#version 330 core"

// ------------- Vertex ---------------
#ifdef VERTEX_SHADER

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 vertUV;

uniform vec2  center;
uniform float radius;

out vec2 localUV;

void main() {
	vec2 uv = center + vertPos.xy * 2.0 * radius;
	gl_Position = vec4(uv * 2.0 - 1.0, 0.0, 1.0);
	localUV = vertUV;
}

#endif

// ------------- Fragment ---------------

#ifdef FRAGMENT_SHADER

in vec2 localUV;

uniform vec4  color;
uniform float hardness;

out vec4 FragColor;

void main() {
	float d = length(localUV - 0.5) * 2.0;
	float a = 1.0 - smoothstep(hardness, 1.0, d);
	FragColor = vec4(color.rgb, color.a * a);
}

#endif
