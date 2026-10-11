#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <span>
#include <cstdint>

// Contiguous index range inside MeshData::indices that came from one source mesh.
struct SubMesh {
	std::string name;
	unsigned int indexOffset = 0;
	unsigned int indexCount = 0;
	unsigned int materialIndex = 0;
};

struct MeshData {
	// Interleaved, 12 floats per vertex: Pos(3) UV(2) Normal(3) Tangent+W(4)
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<SubMesh> submeshes;
};

class IMeshLoader {
public:
	virtual ~IMeshLoader() = default;
	virtual bool canLoad(std::string_view ext) const = 0;

	// Preferred: lets the loader resolve sidecar files (.mtl, .bin, ...) relative to the path.
	virtual MeshData loadFile(const std::string& path) = 0;

	// ext is a format hint without the dot ("obj", "fbx", "gltf", ...).
	virtual MeshData load(std::span<const uint8_t> bytes, std::string_view ext) = 0;
};
