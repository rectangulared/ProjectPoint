#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh/Mesh.h"
#include "../../ShaderProgram.h"

#include <fstream>
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

class Model
{
public:
    bool gammaCorrection;
    std::string directory;
    std::vector<Texture> loadedTextures;
    std::vector<Mesh> meshes;

    Model(std::string const& path, bool gamma = false);

    void draw(ShaderProgram& shaderProgram);

private:
    void loadModel(std::string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    GLuint TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
};
