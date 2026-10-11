#include <glad/glad.h>

#include "services/paint/brush.h"
#include "services/paint/paint_target.h"
#include "services/render/mesh_primitives.h"

Brush::Brush()
	: m_shader("assets/shaders/brush.glsl")
	, m_quad(Primitives::plane())
{
}

void Brush::stamp(PaintTarget& target, Vector2<float> uv) {
	glBindFramebuffer(GL_FRAMEBUFFER, target.FBO);
	glViewport(0, 0, target.size(), target.size());

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_shader.use();
	m_shader.setVec2("center", uv.x, uv.y);
	m_shader.setFloat("radius", radius);
	m_shader.setFloat("hardness", hardness);
	m_shader.setVec4("color", color.x, color.y, color.z, color.w);

	glBindVertexArray(m_quad.VAO);
	glDrawElements(GL_TRIANGLES, m_quad.indexCount, GL_UNSIGNED_INT, 0);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}