#include "VBO.h"

VBO::VBO(const std::vector<Vertex>& vertices, GLint usage)
{
	glGenBuffers(1, &_id);
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), usage);
}

VBO::VBO(const std::vector<glm::mat4>& mat4s, GLint usage)
{
	glGenBuffers(1, &_id);
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferData(GL_ARRAY_BUFFER, mat4s.size() * sizeof(glm::mat4), mat4s.data(), usage);
}

void VBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void VBO::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::destoy()
{
	glDeleteBuffers(1, &_id);
}