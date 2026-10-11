#include <glad/glad.h>
#include <iostream>
#include "paint.h"

PaintTarget::PaintTarget(int size, bool srgb) : texture(size, size, srgb), m_size(size) {
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.TID, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Paint Target FBO incomplete: 0x" << std::hex << status << std::dec << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

PaintTarget::~PaintTarget() {
	glDeleteFramebuffers(1, &FBO);
}

void PaintTarget::clear(float r, float g, float b, float a) {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, m_size, m_size);

	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

PickBuffer::PickBuffer(int width, int height) : m_width(width), m_height(height) {
	glGenFramebuffers(1, &FBO);
	glGenRenderbuffers(1, &colorRBO);
	glGenRenderbuffers(1, &depthRBO);

	allocate();

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRBO);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Pick Buffer FBO incomplete: 0x" << std::hex << status << std::dec << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

PickBuffer::~PickBuffer() {
	glDeleteFramebuffers(1, &FBO);
	glDeleteRenderbuffers(1, &colorRBO);
	glDeleteRenderbuffers(1, &depthRBO);
}

void PickBuffer::allocate() {
	glBindRenderbuffer(GL_RENDERBUFFER, colorRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32F, m_width, m_height);

	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void PickBuffer::resize(int width, int height) {
	if (width == m_width && height == m_height) return;

	m_width  = width;
	m_height = height;
	allocate();
}

void PickBuffer::clear() {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, m_width, m_height);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
