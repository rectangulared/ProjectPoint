#pragma once

#include <string>
#include <vector>

#include "glad/glad.h"

class Cubemap
{
	GLuint _id;
public:
	Cubemap(const std::vector<std::string>& faces);

	void bind();

	void unbind();

	void destroy();

	void attachToCurrentFramebuffer() = delete;
};