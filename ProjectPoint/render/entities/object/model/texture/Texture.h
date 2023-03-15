#pragma once

#include "glad/glad.h"

#include "stb_image.h"

#include "render/ShaderProgram.h"

class Texture
{
public:
	GLuint _id;
	//GLuint unit = 0;
	std::string type;
	std::string path;

	Texture(const char* data, const char* textureType);

	void assignTextureUnit(ShaderProgram& shaderProgram, const char* uniform, GLuint unit);

	void bind();

	void unbind();

	void destroy();
};
