#pragma once

#include <string>
#include <vector>

#include "glad/glad.h"
#include "stb_image.h"

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