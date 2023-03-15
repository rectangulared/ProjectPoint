#pragma once

#include <glad/glad.h>

#include "../VBO/VBO.h"

class VAO
{
public:
	GLuint _id;

	VAO();

	void linkAttrib(VBO& VBO, const GLuint layout, const GLuint numComponents, const GLenum type, const GLsizeiptr stride, const void* offset);

	void bind();

	void unbind();

	void destroy();
};
