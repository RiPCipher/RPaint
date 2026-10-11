#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


#include "services/render/shader.h"
#include "debug/debug.h" 

Shader::Shader(const char* p) : path(p) {
	compileShaders();
}
Shader::~Shader() {
	glDeleteProgram(PID);
}
void Shader::use() {
	glUseProgram(PID);
}
void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(PID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(PID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(PID, name.c_str()), value);
}
void Shader::setVec2(const std::string& name, float x, float y) const {
	glUniform2f(glGetUniformLocation(PID, name.c_str()), x, y);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(PID, name.c_str()), x, y, z);
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const {
	glUniform4f(glGetUniformLocation(PID, name.c_str()), x, y, z, w);
}
void Shader::setMat4(const std::string& name, Matrix::Matrix4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(PID, name.c_str()), 1, GL_FALSE, value.data());
}

// priv
std::string Shader::readShader(const char* path) {
	std::ifstream file(path);
	if (file.is_open()) {
		std::cout << "File Opened Successfully" << std::endl;
		std::stringstream buffer;
		buffer << file.rdbuf();
		file.close();
		return buffer.str();
	}
	else {
		std::cout << "Error opening File" << std::endl;
	}
	return "";
}

unsigned int Shader::compileStage(unsigned int type, const char* stageDefine, const std::string& body) {
	const char* strings[3] = {
		"#version 330 core\n",
		stageDefine,
		body.c_str()
	};

	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 3, strings, NULL);
	glCompileShader(shader);
	checkShaderCompile(shader, type == GL_VERTEX_SHADER ? "Vertex" : "Fragment");
	return shader;
}

void Shader::compileShaders() {
	std::string code = readShader(path.c_str());

	unsigned int vertex   = compileStage(GL_VERTEX_SHADER,   "#define VERTEX_SHADER\n",   code);
	unsigned int fragment = compileStage(GL_FRAGMENT_SHADER, "#define FRAGMENT_SHADER\n", code);

	PID = glCreateProgram();
	glAttachShader(PID, vertex);
	glAttachShader(PID, fragment);
	glLinkProgram(PID);

	checkProgramLink(PID, "Program");
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	unsigned int lightBlock = glGetUniformBlockIndex(PID, "LightBlock");
	if (lightBlock != GL_INVALID_INDEX) {
		glUniformBlockBinding(PID, lightBlock, 0);
	}
}
