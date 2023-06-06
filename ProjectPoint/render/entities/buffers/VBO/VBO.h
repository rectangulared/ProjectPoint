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
    glm::vec3 tangent;
    glm::vec3 bitangent;

    Vertex() 
    {
        position = glm::vec3(0.0);
        normal = glm::vec3(0.0);
        color = glm::vec3(0.0);
        textureUV = glm::vec2(0.0);
        tangent = glm::vec3(0.0);
        bitangent = glm::vec3(0.0);
    };

    Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& color, const glm::vec2& textureUV, const glm::vec3& tangent, const glm::vec3& bitangent)
    {
        this->position = position;
        this->normal = normal;
        this->color = color;
        this->textureUV = textureUV;
        this->tangent = tangent;
        this->bitangent = bitangent;
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
