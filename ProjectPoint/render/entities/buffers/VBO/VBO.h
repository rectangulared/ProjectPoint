#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 textureUV;

    Vertex() {};

    Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& color, const glm::vec2& textureUV)
    {
        this->position = position;
        this->normal = normal;
        this->color = color;
        this->textureUV = textureUV;
    }
};

class VBO
{
public:
	GLuint _id;

    VBO(const std::vector<Vertex>& vertices, GLint usage);
    VBO(const std::vector<glm::mat4>& mat4s, GLint usage);

    void bind();

    void unbind();

    void destoy();
};
