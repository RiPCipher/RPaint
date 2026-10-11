#pragma once

#include "core/io/iimage.h"

class StbImageLoader : public IImageLoader {
public:
	bool canLoad(std::string_view ext) const override;
	ImageData load(std::span<const uint8_t> bytes) override;

private:

};