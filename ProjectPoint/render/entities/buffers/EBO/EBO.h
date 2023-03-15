#pragma once

#include <vector>

#include "glad/glad.h"

class EBO
{
public:
	GLuint _id;

	EBO(const std::vector<GLuint>& indices, GLint usage);

	void bind();

	void unbind();

	void destroy();
};