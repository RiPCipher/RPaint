#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "core/io/iimage.h"
#include "services/render/texture.h"
#include "debug/debug.h" 

Texture::Texture(int width, int height, bool srgb) {
	glGenTextures(1, &TID);
	glBindTexture(GL_TEXTURE_2D, TID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint internalFormat = srgb ? GL_SRGB8_ALPHA8 : GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA8, GL_UNSIGNED_BYTE, nullptr);
}

Texture::Texture(const ImageData& image, bool srgb) {
	glGenTextures(1, &TID);
	glBindTexture(GL_TEXTURE_2D, TID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint internalFormat = srgb ? GL_SRGB8_ALPHA8 : GL_RGBA;

	if (!image.pixels.empty()) {
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixels.data());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture data" << std::endl;
	}
}

Texture::~Texture() {
	glDeleteTextures(1, &TID);
}

//void Texture::initBuffers(const ImageData& image, int width, int height, bool srgb) {
//	glGenTextures(1, &TID);
//	glBindTexture(GL_TEXTURE_2D, TID);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	GLint internalFormat = srgb ? GL_SRGB8_ALPHA8 : GL_RGBA;
//
//	if (!image.pixels.empty()) {
//		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else {
//		std::cout << "Failed to load texture data" << std::endl;
//	}
//}