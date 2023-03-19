#pragma once

#include "glad/glad.h"

#include "stb_image.h"

#include "render/ShaderProgram.h"

class Texture
{
public:
	GLuint _id;
	std::string type;
	std::string path;

	Texture(unsigned int width, unsigned int height, GLenum format);
	Texture(const char* data, const char* textureType);

	void assignTextureUnit(ShaderProgram& shaderProgram, const char* uniform, GLuint unit);

	void attachToCurrentFramebuffer(GLenum attachment);

	void bind();

	void unbind();

	void destroy();
};
