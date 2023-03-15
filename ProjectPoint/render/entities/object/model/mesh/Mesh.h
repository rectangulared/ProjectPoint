#pragma once



#include <string>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "render/ShaderProgram.h"

#include "render/entities/object/model/texture/Texture.h"

#include "render/entities/buffers/VAO/VAO.h"
#include "render/entities/buffers/VBO/VBO.h"
#include "render/entities/buffers/EBO/EBO.h"

class Mesh 
{
public:

    Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures);
    Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures, const std::vector<glm::mat4>& instanceMatrix, const int instances);

    void draw(ShaderProgram& shaderProgram);

private:
    std::vector<Vertex> _vertices;
    std::vector<GLuint> _indices;
    std::vector<Texture> _textures;
    std::vector<glm::mat4> _instanceMatrix;

    VAO VAO;

    unsigned int _instancing = 0;
};