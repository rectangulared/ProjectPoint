#pragma once

#include <glad/glad.h>

class Framebuffer
{
private:
	GLuint _id;
	GLuint _framebufferWidth;
	GLuint _framebufferHeight;

public:

	void bind(const GLenum& target);
	void unbind(const GLenum& target);
	Framebuffer();
	Framebuffer(GLuint framebufferWidth, GLuint framebufferHeight);
	~Framebuffer();
};