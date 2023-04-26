#include "Renderbuffer.h"

Renderbuffer::Renderbuffer(const GLenum& internalFormat, const GLenum& attachment,const GLuint& width, const GLuint& height)
{
	this->_rbWidth = width;
	this->_rbHeight = height;

	glGenRenderbuffers(1, &_id);
	glBindRenderbuffer(GL_RENDERBUFFER, _id);
	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, _id);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

Renderbuffer::Renderbuffer(const GLenum& internalFormat, const GLenum& attachment, const GLuint& width, const GLuint& height, const GLuint& samples)
{
	this->_rbWidth = width;
	this->_rbHeight = height;

	glGenRenderbuffers(1, &_id);
	glBindRenderbuffer(GL_RENDERBUFFER, _id);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, _id);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

Renderbuffer::~Renderbuffer()
{
	this->destroy();
}

void Renderbuffer::bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, _id);
}

void Renderbuffer::unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Renderbuffer::destroy()
{
	glDeleteRenderbuffers(1, &_id);
}