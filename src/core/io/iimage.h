#pragma once

#include <cstdint>
#include <vector>
#include <optional>
#include <string_view>
#include <span>

struct ImageData {
	int width = 0;
	int height = 0;
	int channels = 0;

	enum class Format { R8, RG8, RGBA8, RGBA16F, RGBA32F, BC7};
	Format format = Format::RGBA8;

	std::vector<uint8_t> pixels;

	bool isCompressed() const {
		return format == Format::BC7;
	}
};

class IImageLoader {
public:
	virtual ~IImageLoader() = default;
	virtual bool canLoad(std::string_view ext) const = 0;
	//virtual std::optional<ImageData> load(std::span<const uint8_t> bytes) = 0;
	virtual ImageData load(std::span<const uint8_t> bytes) = 0;
};