#pragma once

#include "core/io/imesh.h"

class Mesh {
public:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int indexCount;

	Mesh();
	explicit Mesh(const MeshData& data);
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

private:
	void initBuffers(const MeshData& data);
};