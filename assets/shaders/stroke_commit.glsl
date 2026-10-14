// NOTE: no #version here on purpose. The loader prepends "#version 330 core"

// ------------- Vertex ---------------
#ifdef VERTEX_SHADER

void main() {
	vec2 pos = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);
	gl_Position = vec4(pos * 2.0 - 1.0, 0.0, 1.0);
}

#endif

// ------------- Fragment ---------------

#ifdef FRAGMENT_SHADER

uniform sampler2D stroke;
uniform vec4 strokeColor;

out vec4 FragColor;

void main() {
	float coverage = texelFetch(stroke, ivec2(gl_FragCoord.xy), 0).r;
	FragColor = vec4(strokeColor.rgb, strokeColor.a * coverage);
}

#endif
