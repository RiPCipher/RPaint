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

	int size() const { return m_size; }

private:
	int m_size;
};