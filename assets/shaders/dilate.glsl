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

uniform sampler2D canvas;
uniform sampler2D mask;
uniform int maxDistance;

out vec4 FragColor;

void main() {
	ivec2 p = ivec2(gl_FragCoord.xy);

	if (texelFetch(mask, p, 0).r > 0.5) {
		FragColor = texelFetch(canvas, p, 0);
		return;
	}

	for (int r = 1; r <= maxDistance; ++r) {
		for (int y = -r; y <= r; ++y) {
			for (int x = -r; x <= r; ++x) {
				if (max(abs(x), abs(y)) != r) continue;
				ivec2 q = p + ivec2(x, y);
				if (texelFetch(mask, q, 0).r > 0.5) {
					FragColor = texelFetch(canvas, q, 0);
					return;
				}
			}
		}
	}

	FragColor = texelFetch(canvas, p, 0);
}

#endif
