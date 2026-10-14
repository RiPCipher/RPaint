// NOTE: no #version here on purpose. The loader prepends "#version 330 core"

// ------------- Vertex ---------------
#ifdef VERTEX_SHADER

layout (location = 0) in vec3 vertPos;
layout (location = 2) in vec3 vertNormal;

out vec3 worldPos;
out vec3 worldNormal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
	vec4 world = Model * vec4(vertPos, 1.0);
	gl_Position = Projection * View * world;

	worldPos    = world.xyz;
	worldNormal = transpose(inverse(mat3(Model))) * vertNormal;
}

#endif

// ------------- Fragment ---------------

#ifdef FRAGMENT_SHADER

in vec3 worldPos;
in vec3 worldNormal;

uniform float objectId;

layout (location = 0) out vec4 PickOut;
layout (location = 1) out vec4 PickNormal;

void main() {
	PickOut    = vec4(worldPos, objectId);
	PickNormal = vec4(normalize(worldNormal), 0.0);
}

#endif
