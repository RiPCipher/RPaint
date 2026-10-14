#include <glad/glad.h>

#include "services/paint/brush.h"
#include "services/paint/paint.h"
#include "services/render/mesh.h"
#include "services/render/render.h"
#include "core/math/math.h"

Brush::Brush()
	: m_shader("assets/shaders/brush.glsl")
	, m_commitShader("assets/shaders/stroke_commit.glsl")
{
	glGenVertexArrays(1, &m_emptyVAO);
}

Brush::~Brush() {
	glDeleteVertexArrays(1, &m_emptyVAO);
}

void Brush::stampAll(PaintTarget& strokeBuffer, const RenderObject& obj, const std::vector<BrushHit>& hits) {
	if (hits.empty()) return;

	glBindFramebuffer(GL_FRAMEBUFFER, strokeBuffer.FBO);
	glViewport(0, 0, strokeBuffer.size(), strokeBuffer.size());

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_MAX);

	m_shader.use();
	m_shader.setMat4("Model", obj.transform.modelMatrix());
	m_shader.setFloat("radius", radius);
	m_shader.setFloat("hardness", hardness);

	glBindVertexArray(obj.mesh->VAO);
	for (const BrushHit& hit : hits) {
		m_shader.setVec3("center", hit.position.x, hit.position.y, hit.position.z);
		m_shader.setVec3("hitNormal", hit.normal.x, hit.normal.y, hit.normal.z);
		glDrawElements(GL_TRIANGLES, obj.mesh->indexCount, GL_UNSIGNED_INT, 0);
	}

	glBlendEquation(GL_FUNC_ADD);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Brush::commit(PaintTarget& canvas, const PaintTarget& strokeBuffer) {
	glBindFramebuffer(GL_FRAMEBUFFER, canvas.FBO);
	glViewport(0, 0, canvas.size(), canvas.size());

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
	glEnable(GL_FRAMEBUFFER_SRGB);

	Vector4<float> c = linearColor();
	m_commitShader.use();
	m_commitShader.setInt("stroke", 0);
	m_commitShader.setVec4("strokeColor", c.x, c.y, c.z, c.w);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, strokeBuffer.texture.TID);

	glBindVertexArray(m_emptyVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisable(GL_FRAMEBUFFER_SRGB);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Vector4<float> Brush::linearColor() const {
	return { Color::srgbToLinear(color.x), Color::srgbToLinear(color.y), Color::srgbToLinear(color.z), color.w };
}
