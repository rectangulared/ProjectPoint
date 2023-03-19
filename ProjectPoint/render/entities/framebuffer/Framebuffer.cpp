#include "Framebuffer.h"

void Framebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
}

void  Framebuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::Framebuffer() : _framebufferWidth(1920), _framebufferHeight(1080)
{
	glGenFramebuffers(1, &_id);
}

Framebuffer::Framebuffer(GLuint framebufferWidth, GLuint framebufferHeight) : _framebufferWidth(framebufferWidth), _framebufferHeight(framebufferHeight)
{
	glGenFramebuffers(1, &_id);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &_id);
}