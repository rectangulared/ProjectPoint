#pragma once

#include "glad/glad.h"

#include "stb_image.h"

#include "render/ShaderProgram.h"

class Texture
{
public:
	GLboolean isMultisampled;
	GLuint _id;
	GLuint _unit = 0;
	std::string type;
	std::string path;

	Texture(const GLuint& width, const GLuint& height, const GLenum& internalFormat, const GLenum& format);

	Texture(const char* data, const char* textureType, const bool& isGammaCorrected);

	Texture(const GLuint& width, const GLuint& height, const GLenum& format, const GLuint& samples, const GLboolean& fixedSampleLocations);

	void assignTextureUnit(ShaderProgram& shaderProgram, const char* uniform, GLuint unit);

	void attachToCurrentFramebuffer(GLenum attachment);

	void bind();

	void unbind();

	void destroy();
};
