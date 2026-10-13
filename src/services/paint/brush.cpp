#include <glad/glad.h>

#include "services/paint/brush.h"
#include "services/paint/paint.h"
#include "services/render/mesh.h"
#include "services/render/render.h"

Brush::Brush()
	: m_shader("assets/shaders/brush.glsl")
{
}

void Brush::stampAll(PaintTarget& target, const RenderObject& obj, const std::vector<Vector3<float>>& centers) {
	if (centers.empty()) return;

	glBindFramebuffer(GL_FRAMEBUFFER, target.FBO);
	glViewport(0, 0, target.size(), target.size());

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_shader.use();
	m_shader.setMat4("Model", obj.transform.modelMatrix());
	m_shader.setFloat("radius", radius);
	m_shader.setFloat("hardness", hardness);
	m_shader.setVec4("color", color.x, color.y, color.z, color.w);

	glBindVertexArray(obj.mesh->VAO);
	for (const Vector3<float>& c : centers) { m_shader.setVec3("center", c.x, c.y, c.z);
		glDrawElements(GL_TRIANGLES, obj.mesh->indexCount, GL_UNSIGNED_INT, 0);
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}