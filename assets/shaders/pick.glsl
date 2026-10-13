// NOTE: no #version here on purpose. The loader prepends "#version 330 core"

// ------------- Vertex ---------------
#ifdef VERTEX_SHADER

layout (location = 0) in vec3 vertPos;

out vec3 worldPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;


void main() {
	vec4 world = Model * vec4(vertPos, 1.0);
	gl_Position = Projection * View * world;

	worldPos = world.xyz; 

}

#endif

// ------------- Fragment ---------------

#ifdef FRAGMENT_SHADER

in vec3 worldPos;

uniform float objectId;
out vec4 PickOut;

void main() { PickOut = vec4(worldPos, objectId); }

#endif
