#pragma once

#include "core/math/transform.h"

class Mesh;
class Material;
struct Camera;

struct RenderObject {
	Mesh*     mesh = nullptr;
	Material* material = nullptr;
	Transform transform;
};

void draw(RenderObject& rob, Camera& camera, const Matrix::Matrix4& projection);
