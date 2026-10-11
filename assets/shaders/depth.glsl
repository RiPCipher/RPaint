#ifdef VERTEX_SHADER

layout (location = 0) in vec3 vertPos;

uniform mat4 Model;
uniform mat4 lightSpaceMatrix;

void main() { gl_Position = lightSpaceMatrix * Model * vec4(vertPos, 1.0); }

#endif

#ifdef FRAGMENT_SHADER

void main() {}

#endif