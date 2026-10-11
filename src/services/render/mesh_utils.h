#pragma once

#include "core/io/imesh.h"
#include <vector>
#include <cmath>

namespace MeshUtils {
	inline void computeTangents(MeshData& mesh) {
		const int STRIDE = 12;
		const int POS = 0, UV = 3, NRM = 5, TAN = 8;

		size_t vertexCount = mesh.vertices.size() / STRIDE;

		std::vector<float> tan(vertexCount * 3, 0.0f);
		std::vector<float> bit(vertexCount * 3, 0.0f);

		for (size_t i = 0; i + 2 < mesh.indices.size(); i += 3) {
			unsigned int i0 = mesh.indices[i + 0];
			unsigned int i1 = mesh.indices[i + 1];
			unsigned int i2 = mesh.indices[i + 2];

			float* v0 = &mesh.vertices[i0 * STRIDE];
			float* v1 = &mesh.vertices[i1 * STRIDE];
			float* v2 = &mesh.vertices[i2 * STRIDE];

			float e1x = v1[POS + 0] - v0[POS + 0], e1y = v1[POS + 1] - v0[POS + 1], e1z = v1[POS + 2] - v0[POS + 2];
			float e2x = v2[POS + 0] - v0[POS + 0], e2y = v2[POS + 1] - v0[POS + 1], e2z = v2[POS + 2] - v0[POS + 2];

			float du1 = v1[UV + 0] - v0[UV + 0], dv1 = v1[UV + 1] - v0[UV + 1];
			float du2 = v2[UV + 0] - v0[UV + 0], dv2 = v2[UV + 1] - v0[UV + 1];

			float denom = du1 * dv2 - du2 * dv1;
			float r = (fabsf(denom) < 1e-8f) ? 0.0f : 1.0f / denom;

			float Tx = (e1x * dv2 - e2x * dv1) * r;
			float Ty = (e1y * dv2 - e2y * dv1) * r;
			float Tz = (e1z * dv2 - e2z * dv1) * r;
			float Bx = (e2x * du1 - e1x * du2) * r;
			float By = (e2y * du1 - e1y * du2) * r;
			float Bz = (e2z * du1 - e1z * du2) * r;

			for (unsigned int idx : { i0, i1, i2 }) {
				tan[idx * 3 + 0] += Tx; tan[idx * 3 + 1] += Ty; tan[idx * 3 + 2] += Tz;
				bit[idx * 3 + 0] += Bx; bit[idx * 3 + 1] += By; bit[idx * 3 + 2] += Bz;
			}
		}


		for (size_t v = 0; v < vertexCount; ++v) {
			float* vert = &mesh.vertices[v * STRIDE];
			float Nx = vert[NRM + 0], Ny = vert[NRM + 1], Nz = vert[NRM + 2];
			float Tx = tan[v * 3 + 0], Ty = tan[v * 3 + 1], Tz = tan[v * 3 + 2];

			float ndt = Nx * Tx + Ny * Ty + Nz * Tz;
			Tx -= Nx * ndt; Ty -= Ny * ndt; Tz -= Nz * ndt;

			float len = sqrtf(Tx * Tx + Ty * Ty + Tz * Tz);
			if (len > 1e-8f) { Tx /= len; Ty /= len; Tz /= len; }
			else { Tx = 1.0f; Ty = 0.0f; Tz = 0.0f; }

			float cx = Ny * Tz - Nz * Ty;
			float cy = Nz * Tx - Nx * Tz;
			float cz = Nx * Ty - Ny * Tx;
			float dotB = cx * bit[v * 3 + 0] + cy * bit[v * 3 + 1] + cz * bit[v * 3 + 2];
			float w = (dotB < 0.0f) ? -1.0f : 1.0f;

			vert[TAN + 0] = Tx; vert[TAN + 1] = Ty; vert[TAN + 2] = Tz; vert[TAN + 3] = w;
		}
	}
}