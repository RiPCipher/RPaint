#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <iostream>

namespace io {

	inline std::vector<uint8_t> readBytes(const char* path) {
		std::ifstream file(path, std::ios::binary | std::ios::ate);
		if (!file) {
			std::cerr << "Failed to open file: " << path << std::endl;
			return {};
		}
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<uint8_t> bytes(size);
		file.read(reinterpret_cast<char*>(bytes.data()), size);

		return bytes;
	}

	inline std::string readText(const char* path) {
		std::vector<uint8_t> bytes = readBytes(path);
		return std::string(bytes.begin(), bytes.end());
	}

}