#pragma once

#include "services/render/texture.h"

class PaintTarget {
public:
	Texture texture;
	unsigned int FBO = 0;

	PaintTarget(int size, bool srgb);
	~PaintTarget();

	PaintTarget(const PaintTarget&) = delete;
	PaintTarget& operator=(const PaintTarget&) = delete;

	void clear(float r, float g, float b, float a);

	int size() const { return m_size; };

private:
	int m_size;
};

class PickBuffer {
public:
	unsigned int FBO = 0;
	unsigned int colorRBO = 0;
	unsigned int depthRBO = 0;

	PickBuffer(int width, int height);
	~PickBuffer();

	PickBuffer(const PickBuffer&) = delete;
	PickBuffer& operator=(const PickBuffer&) = delete;

	void resize(int width, int height);
	void clear();

	int width()  const { return m_width; }
	int height() const { return m_height; }

private:
	void allocate();

	int m_width;
	int m_height;
};