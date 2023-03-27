#include "UBO.h"

UBO::UBO(const GLuint& bufferSize, const GLenum& usage)
{
	glGenBuffers(1, &_id);
	glBindBuffer(GL_UNIFORM_BUFFER, _id);
	glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, usage);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UBO::~UBO()
{
	destroy();
}

void UBO::bind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, _id);
}

void UBO::assignBindingPoint(const GLuint& uniformBlockBinding, const GLuint& shaderProgramID, const std::string& uniformBlockName)
{
	GLuint uniformBlockIndexMatrices = glGetUniformBlockIndex(shaderProgramID, uniformBlockName.c_str());
	glUniformBlockBinding(shaderProgramID, uniformBlockIndexMatrices, uniformBlockBinding);
}

void UBO::bindToTargetBase(const GLuint& uniformBlockBinding)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, uniformBlockBinding, _id);
}

void UBO::updateSubsetData(const void* data, const GLuint& size, const GLuint& offset)
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void UBO::unbind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UBO::destroy()
{
	glDeleteBuffers(1, &_id);
}