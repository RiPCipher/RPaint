#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


const int bufferSize = 512;

// Shader Errors
bool checkShaderCompile(GLuint shader, const char* label) {
	int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS ,&result);
	if (result == GL_FALSE) {
		char buffer[bufferSize];
		glGetShaderInfoLog(shader, bufferSize, NULL, buffer);

		std::cerr << label << " Shader Failed to Compile: " << buffer;

		return false;
	}
	return true;
}

bool checkProgramLink(GLuint program, const char* label) {
	int result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		char buffer[bufferSize];
		glGetProgramInfoLog(program, bufferSize, NULL, buffer);

		std::cerr << label << " Program failed to link: " << buffer;

		return false;
	}
	return true;
}