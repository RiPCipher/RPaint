// NOTE: no #version here on purpose. The loader prepends "#version 330 core"

// ------------- Vertex ---------------
#ifdef VERTEX_SHADER

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 aTexPos;

out vec2 texPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;


void main() {
	vec4 world = Model * vec4(vertPos, 1.0);
	gl_Position = Projection * View * world;

	texPos   = aTexPos;

}

#endif

// ------------- Fragment ---------------

#ifdef FRAGMENT_SHADER

in vec2 texPos;

uniform float objectId;
out vec4 PickOut;

void main() { PickOut = vec4(texPos, objectId, 1.0); }

#endif
