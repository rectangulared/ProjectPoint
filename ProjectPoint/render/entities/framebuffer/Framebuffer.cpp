#include "Framebuffer.h"

void Framebuffer::bind(const GLenum& target)
{
	glBindFramebuffer(target, _id);
}

void  Framebuffer::unbind(const GLenum& target)
{
	glBindFramebuffer(target, 0);
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