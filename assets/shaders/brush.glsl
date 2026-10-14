// NOTE: no #version here on purpose. The loader prepends "#version 330 core"

// ------------- Vertex ---------------
#ifdef VERTEX_SHADER

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 vertUV;
layout (location = 2) in vec3 vertNormal;

uniform mat4 Model;
out vec3 worldPos;
out vec3 worldNormal;
void main() {
    worldPos    = (Model * vec4(vertPos, 1.0)).xyz;
    worldNormal = transpose(inverse(mat3(Model))) * vertNormal;
    gl_Position = vec4(vertUV * 2.0 - 1.0, 0.0, 1.0);
}

#endif

// ------------- Fragment ---------------

#ifdef FRAGMENT_SHADER

in vec3 worldPos;
in vec3 worldNormal;

uniform vec3  center;
uniform vec3  hitNormal;
uniform float radius;
uniform float hardness;

out vec4 FragColor;

void main() {
	float h = min(hardness, 0.999);
	float d = distance(worldPos, center) / radius;
	float a = 1.0 - smoothstep(h, 1.0, d);

	float facing = dot(normalize(worldNormal), hitNormal);
	a *= smoothstep(0.0, 0.2, facing);

	FragColor = vec4(a);
}

#endif
