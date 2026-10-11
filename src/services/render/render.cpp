#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "debug/debug.h"
#include "services/render/render.h"
#include "services/render/shader.h"
#include "services/render/mesh.h"
#include "services/render/texture.h"
#include "services/render/material.h"

#include "core/math/transform.h"
#include "core/math/view.h"
#include "core/math/camera.h"



void draw(RenderObject& rob, Camera& camera, const Matrix::Matrix4& projection) {
	rob.material->bind();

	Shader* shader = rob.material->shader;
	shader->setMat4("Model", rob.transform.modelMatrix());
	shader->setMat4("View", camera.viewMatrix());
	shader->setMat4("Projection", projection);
	shader->setVec3("camPos", camera.position.x, camera.position.y, camera.position.z);

	glBindVertexArray(rob.mesh->VAO);
	glDrawElements(GL_TRIANGLES, rob.mesh->indexCount, GL_UNSIGNED_INT, 0);
}

void clear() {

}
