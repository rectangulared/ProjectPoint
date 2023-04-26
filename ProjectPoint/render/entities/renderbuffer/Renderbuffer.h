#pragma once

#include "glad/glad.h"

class Renderbuffer
{
private:
	GLuint _id;
	GLuint _rbWidth;
	GLuint _rbHeight;

public:
	Renderbuffer(const GLenum& internalFormat, const GLenum& attachment, const GLuint& width, const GLuint& height);

	Renderbuffer(const GLenum& internalFormat, const GLenum& attachment, const GLuint& width, const GLuint& height, const GLuint& samples);

	~Renderbuffer();

	void bind();

	void unbind();

	void destroy();
};
