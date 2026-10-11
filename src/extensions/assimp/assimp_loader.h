#pragma once

#include "core/io/imesh.h"

class AssimpLoader : public IMeshLoader {
public:
	bool canLoad(std::string_view ext) const override;
	MeshData loadFile(const std::string& path) override;
	MeshData load(std::span<const uint8_t> bytes, std::string_view ext) override;
};
