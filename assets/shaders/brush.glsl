// NOTE: no #version here on purpose. The loader prepends "#version 330 core"

// ------------- Vertex ---------------
#ifdef VERTEX_SHADER

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 vertUV;

uniform mat4 Model;
out vec3 worldPos;
void main() {
    worldPos    = (Model * vec4(vertPos, 1.0)).xyz;
    gl_Position = vec4(vertUV * 2.0 - 1.0, 0.0, 1.0);
}

#endif

// ------------- Fragment ---------------

#ifdef FRAGMENT_SHADER

in vec3 worldPos;

uniform vec3  center;
uniform float radius;
uniform float hardness;
uniform vec4  color;

out vec4 FragColor;

void main() {
	float h = min(hardness, 0.999);
	float d = distance(worldPos, center) / radius;
	float a = 1.0 - smoothstep(h, 1.0, d);
	FragColor = vec4(color.rgb, color.a * a);
}

#endif
