#include "EBO.h"

EBO::EBO(const std::vector<GLuint>& indices, GLint usage)
{
	glGenBuffers(1, &_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), usage);
}

void EBO::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void EBO::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::destroy()
{
	glDeleteBuffers(1, &_id);
}