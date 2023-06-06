#include "Mesh.h"

    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures, const std::vector<glm::mat4>& instanceMatrix, const int instancing)
    {
        this->_vertices = vertices;
        this->_indices = indices;
        this->_textures = textures;
        this->_instanceMatrix = instanceMatrix;
        this->_instancing = instancing;
        if (_instancing < 1) { _instancing = 1; }

        VAO.bind();

        VBO instanceVBO(_instanceMatrix, GL_STATIC_DRAW);
        VBO VBO(_vertices, GL_STATIC_DRAW);
        EBO EBO(_indices, GL_STATIC_DRAW);

        VAO.linkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
        VAO.linkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(1 * sizeof(glm::vec3)));
        VAO.linkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3)));
        VAO.linkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(glm::vec3)));
        VAO.linkAttrib(VBO, 4, 3, GL_FLOAT, sizeof(Vertex), (void*)(sizeof(glm::vec2) + 3 * sizeof(glm::vec3)));
        VAO.linkAttrib(VBO, 5, 3, GL_FLOAT, sizeof(Vertex), (void*)(sizeof(glm::vec2) + 4 * sizeof(glm::vec3)));

        if (_instancing != 1)
        {
            // Can't link to a mat4 so you need to link four vec4s
            VAO.linkAttrib(instanceVBO, 6, 4, GL_FLOAT, sizeof(glm::mat4), (void*)0);
            VAO.linkAttrib(instanceVBO, 7, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
            VAO.linkAttrib(instanceVBO, 8, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
            VAO.linkAttrib(instanceVBO, 9, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
            // Makes it so the transform is only switched when drawing the next instance
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);
            glVertexAttribDivisor(7, 1);
        }
        // Unbind all to prevent accidentally modifying them
        VAO.unbind();
        VBO.unbind();
        instanceVBO.unbind();
        EBO.unbind();
    }

    void Mesh::draw(ShaderProgram& shaderProgram)
    {
        GLuint diffuseNr = 1;
        GLuint specularNr = 1;
        GLuint opacityNr = 1;
        GLuint normalNr = 1;
        for (size_t i = 0; i < _textures.size(); i++)
        {
            std::string number;
            std::string name = _textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);
            else if (name == "texture_opacity")
                number = std::to_string(opacityNr++);
            else if (name == "texture_normal")
                number = std::to_string(normalNr++);

            _textures[i].assignTextureUnit(shaderProgram, ("material." + name + number).c_str(), i);
            _textures[i].bind();
        }

        VAO.bind();
        if (_instancing == 1)
        {
            glDrawElements(GL_TRIANGLES, static_cast<GLuint>(_indices.size()), GL_UNSIGNED_INT, 0);
        }
        else
        {
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLuint>(_indices.size()), GL_UNSIGNED_INT, 0, _instancing);
        }

        for (auto t : _textures)
        {
            t.unbind();
        }
        VAO.unbind();
    }