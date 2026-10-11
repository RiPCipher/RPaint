#pragma once

struct ImageData;

class Texture {
public:
	unsigned int TID;

	Texture(const ImageData& image, bool srgb = false);  //Texture(const ImageData& image, bool srgb = false);
	Texture(int width, int height, bool srgb = false);

	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

private:
	//void initBuffers(const ImageData& image, int width, int height, bool srgb);

};