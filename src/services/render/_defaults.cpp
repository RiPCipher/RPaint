#include "core/io/iimage.h"
#include "services/render/_defaults.h"


//// Defaulkt image

static ImageData solidPx(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
	ImageData image;
	image.width = 1;
	image.height = 1;
	image.channels = 4;
	image.format = ImageData::Format::RGBA8;
	image.pixels = { r, g, b, a };
	return image;
}

DefaultTextures::DefaultTextures() : 
	albedo(solidPx(255,255,255), true),
	normal(solidPx(128,128,255), false),
	orm(solidPx(255,255,255), false) {}