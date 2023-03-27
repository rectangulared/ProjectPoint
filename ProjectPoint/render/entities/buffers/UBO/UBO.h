#pragma once

#include <string>

#include "glad/glad.h"

class UBO
{
	GLuint _id;

public:
	UBO(const GLuint& bufferSize, const GLenum& usage);

	~UBO();

	void bind();

	void assignBindingPoint(const GLuint& uniformBlockBinding, const GLuint& shaderProgramID, const std::string& uniformBlockName);

	void bindToTargetBase(const GLuint& uniformBlockBinding);

	void updateSubsetData(const void* data, const GLuint& size, const GLuint& offset);

	void unbind();

	void destroy();
};