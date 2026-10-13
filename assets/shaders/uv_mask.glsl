// NOTE: no #version here on purpose. The loader prepends "#version 330 core"

// ------------- Vertex ---------------
#ifdef VERTEX_SHADER

layout (location = 1) in vec2 vertUV;

void main() {
	gl_Position = vec4(vertUV * 2.0 - 1.0, 0.0, 1.0);
}

#endif

// ------------- Fragment ---------------

#ifdef FRAGMENT_SHADER

out vec4 MaskOut;

void main() { MaskOut = vec4(1.0); }

#endif
