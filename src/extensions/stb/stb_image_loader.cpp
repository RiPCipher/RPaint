#include "headers/stb_image.h"
#include "stb_image_loader.h"


bool StbImageLoader::canLoad(std::string_view ext) const {
	return ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "bmp" || ext == "tga";
}

ImageData StbImageLoader::load(std::span<const uint8_t> bytes) {
	int w = 0, h = 0, srcChannels = 0;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load_from_memory(
		bytes.data(), static_cast<int>(bytes.size()),
		&w, &h, &srcChannels, 4);
	
	ImageData out;
	if (!data) return out;

	out.width = w;
	out.height = h;
	out.channels = 4;
	out.format = ImageData::Format::RGBA8;
	out.pixels.assign(data, data + (w * h * 4));

	stbi_image_free(data);
	return out;
}