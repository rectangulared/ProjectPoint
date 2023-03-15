#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &_id);
}

void VAO::linkAttrib(VBO& VBO, const GLuint layout, const GLuint numComponents, const GLenum type, const GLsizeiptr stride, const void* offset)
{
	VBO.bind();
	glEnableVertexAttribArray(layout);
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	VBO.unbind();
}

void VAO::bind()
{
	glBindVertexArray(_id);
}

void VAO::unbind()
{
	glBindVertexArray(0);
}

void VAO::destroy()
{
	glDeleteVertexArrays(1, &_id);
}
