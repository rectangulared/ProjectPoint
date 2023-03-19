#pragma once

#include <glad/glad.h>

class Framebuffer
{
private:
	GLuint _id;
	GLuint _framebufferWidth;
	GLuint _framebufferHeight;

public:

	void bind();
	void unbind();
	Framebuffer();
	Framebuffer(GLuint framebufferWidth, GLuint framebufferHeight);
	~Framebuffer();
};