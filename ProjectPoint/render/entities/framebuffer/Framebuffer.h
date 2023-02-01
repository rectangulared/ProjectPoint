#pragma once

#include <glad/glad.h>

class Framebuffer
{
	GLuint _id;
	GLuint _framebufferWidth;
	GLuint _framebufferHeight;

public:

	void bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
	}

	void unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLuint renderToTexture()
	{
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _framebufferWidth, _framebufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		return texture;
	}

	Framebuffer() : _framebufferWidth(1920), _framebufferHeight(1080)
	{
		glGenFramebuffers(1, &_id);
	}

	Framebuffer(GLuint framebufferWidth, GLuint framebufferHeight) : _framebufferWidth(framebufferWidth), _framebufferHeight(framebufferHeight)
	{
		glGenFramebuffers(1, &_id);
	}

	~Framebuffer()
	{
		glDeleteFramebuffers(1, &_id);
	}

};