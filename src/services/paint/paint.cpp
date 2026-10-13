#include <glad/glad.h>
#include <iostream>
#include "paint.h"
#include "services/render/mesh.h"

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

	glClearColor(0.0f, 0.0f, 0.0f, -1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

SeamPadding::SeamPadding()
	: m_maskShader("assets/shaders/uv_mask.glsl")
	, m_dilateShader("assets/shaders/dilate.glsl")
{
	glGenVertexArrays(1, &m_emptyVAO);
}

SeamPadding::~SeamPadding() {
	glDeleteVertexArrays(1, &m_emptyVAO);
}

void SeamPadding::buildMask(PaintTarget& mask, const Mesh& mesh) {
	mask.clear(0.0f, 0.0f, 0.0f, 0.0f);

	glBindFramebuffer(GL_FRAMEBUFFER, mask.FBO);
	glViewport(0, 0, mask.size(), mask.size());

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	m_maskShader.use();
	glBindVertexArray(mesh.VAO);
	glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SeamPadding::apply(const PaintTarget& canvas, const PaintTarget& mask, PaintTarget& display) {
	glBindFramebuffer(GL_FRAMEBUFFER, display.FBO);
	glViewport(0, 0, display.size(), display.size());

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_FRAMEBUFFER_SRGB);

	m_dilateShader.use();
	m_dilateShader.setInt("canvas", 0);
	m_dilateShader.setInt("mask", 1);
	m_dilateShader.setInt("maxDistance", maxDistance);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, canvas.texture.TID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mask.texture.TID);

	glBindVertexArray(m_emptyVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}