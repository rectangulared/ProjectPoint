#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh/Mesh.h"
#include "render/ShaderProgram.h"

#include <fstream>
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

class Model
{
public:

    Model();
    Model(std::string const& path, bool _opaque = true, bool _gamma = false);
    Model(std::string const& path, const std::vector<glm::mat4>& instanceMatrix, const int instancing, bool _opaque = true, bool gamma = false);

    void draw(ShaderProgram& shaderProgram);

    bool isOpaque();

private:
    bool opaque;
    bool gammaCorrection;
    std::string directory;
    std::vector<Texture> loadedTextures;
    std::vector<Mesh> meshes;

    unsigned int _instancing = 0;
    std::vector<glm::mat4> _instanceMatrix;

    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
