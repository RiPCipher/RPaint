#pragma once

#include <string>
#include "core/math/matrix.h"

class Shader {
public:
	unsigned int PID;

	Shader(const char* path);
	~Shader();

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;
	void setMat4(const std::string& name, Matrix::Matrix4 value) const;

private:
	std::string path = "assets/shaders/default.glsl";
	std::string readShader(const char* path);
	unsigned int compileStage(unsigned int type, const char* stageDefine, const std::string& body);
	void compileShaders();
};