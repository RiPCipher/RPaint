#pragma once

#include "core/io/imesh.h"

namespace Primitives {
	//// Plane ////
	constexpr float planeVertices[] = {
		// Pos           // UV        // Normals        // Tangent + W
		-0.5,0.5, 0.0,   0.0, 0.0,    0.0, 0.0, 1.0,    1.0,0.0,0.0,  -1.0f, //tl
		0.5,0.5, 0.0,    1.0, 0.0,    0.0, 0.0, 1.0,    1.0,0.0,0.0,  -1.0f, //tr
		-0.5,-0.5, 0.0,  0.0, 1.0,    0.0, 0.0, 1.0,    1.0,0.0,0.0,  -1.0f, //bl
		0.5,-0.5, 0.0,   1.0, 1.0,    0.0, 0.0, 1.0,    1.0,0.0,0.0,  -1.0f, //br
	};

	constexpr unsigned int planeIndices[6] = {
		0, 1, 3,
		0, 2, 3
	};

	//// Cube ////
	constexpr float cubeVertices[] = {
		// Pos               UV     Normal    Tangent  W  
		-0.5f,-0.5f, 0.5f,   0,0,   0,0, 1,   1,0,0,   1.0f, // Front  (+1)
		 0.5f,-0.5f, 0.5f,   1,0,   0,0, 1,   1,0,0,   1.0f,
		 0.5f, 0.5f, 0.5f,   1,1,   0,0, 1,   1,0,0,   1.0f,
		-0.5f, 0.5f, 0.5f,   0,1,   0,0, 1,   1,0,0,   1.0f,

		-0.5f,-0.5f,-0.5f,   0,0,   0,0,-1,   1,0,0,  -1.0f, // Back   (-1)
		 0.5f,-0.5f,-0.5f,   1,0,   0,0,-1,   1,0,0,  -1.0f,
		 0.5f, 0.5f,-0.5f,   1,1,   0,0,-1,   1,0,0,  -1.0f,
		-0.5f, 0.5f,-0.5f,   0,1,   0,0,-1,   1,0,0,  -1.0f,

		-0.5f, 0.5f, 0.5f,   0,0,   0, 1,0,   1,0,0,   1.0f, // Top    (+1)
		 0.5f, 0.5f, 0.5f,   1,0,   0, 1,0,   1,0,0,   1.0f,
		 0.5f, 0.5f,-0.5f,   1,1,   0, 1,0,   1,0,0,   1.0f,
		-0.5f, 0.5f,-0.5f,   0,1,   0, 1,0,   1,0,0,   1.0f,

		-0.5f,-0.5f, 0.5f,   0,0,   0,-1,0,   1,0,0,  -1.0f, // Bottom (-1)
		 0.5f,-0.5f, 0.5f,   1,0,   0,-1,0,   1,0,0,  -1.0f,
		 0.5f,-0.5f,-0.5f,   1,1,   0,-1,0,   1,0,0,  -1.0f,
		-0.5f,-0.5f,-0.5f,   0,1,   0,-1,0,   1,0,0,  -1.0f,

		 0.5f,-0.5f, 0.5f,   0,0,   1,0,0,   0,1,0,   -1.0f, // Right  (-1)
		 0.5f, 0.5f, 0.5f,   1,0,   1,0,0,   0,1,0,   -1.0f,
		 0.5f, 0.5f,-0.5f,   1,1,   1,0,0,   0,1,0,   -1.0f,
		 0.5f,-0.5f,-0.5f,   0,1,   1,0,0,   0,1,0,   -1.0f,

		-0.5f,-0.5f, 0.5f,   0,0,  -1,0,0,   0,1,0,    1.0f, // Left   (+1)
		-0.5f, 0.5f, 0.5f,   1,0,  -1,0,0,   0,1,0,    1.0f,
		-0.5f, 0.5f,-0.5f,   1,1,  -1,0,0,   0,1,0,    1.0f,
		-0.5f,-0.5f, 0.5f,   0,1,  -1,0,0,   0,1,0,    1.0f,
	};

	constexpr unsigned int cubeIndices[] = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	};
	//// Cylinder ////


	///////////////// Accessors ////////////////////
	inline MeshData cube() {
		return MeshData{
			std::vector<float>(cubeVertices, cubeVertices + sizeof(cubeVertices) / sizeof(float)),
			std::vector<unsigned int>(cubeIndices, cubeIndices + sizeof(cubeIndices) / sizeof(unsigned int))
		};
	}

	inline MeshData plane() {
		return MeshData{
			std::vector<float>(planeVertices, planeVertices + sizeof(planeVertices) / sizeof(float)),
			std::vector<unsigned int>(planeIndices, planeIndices + sizeof(planeIndices) / sizeof(unsigned int))
		};
	}
}