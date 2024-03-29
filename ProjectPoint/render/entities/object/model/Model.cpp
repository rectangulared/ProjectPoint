#include "Model.h"

Model::Model() {}

Model::Model(std::string const& path, bool _opaque, bool _gamma) : gammaCorrection(_gamma), opaque(_opaque)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        //TODO: Add model import error checking logic
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

Model::Model(std::string const& path, const std::vector<glm::mat4>& instanceMatrix, const int instancing, bool _opaque, bool _gamma)
{
    this->_instancing = instancing < 1 ? 1 : instancing;
    this->_instanceMatrix = instanceMatrix;
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        //TODO: Add model import error checking logic
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::draw(ShaderProgram& shaderProgram)
{
    for (size_t i = 0; i < meshes.size(); i++)
        meshes[i].draw(shaderProgram);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (size_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.textureUV = vec;
        }
        else
            vertex.textureUV = glm::vec2(0.0f);

        if (mesh->HasTangentsAndBitangents())
        {
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.tangent = vector;

            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.bitangent = vector;
        }
        else
        {
            vertex.tangent = glm::vec3(0.0f);
            vertex.bitangent = glm::vec3(0.0f);
        }

        vertices.push_back(vertex);
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (size_t j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    if (opaque)
    {
        std::vector<Texture> opacityMaps = loadMaterialTextures(material, aiTextureType_OPACITY, "texture_opacity");
        textures.insert(textures.end(), opacityMaps.begin(), opacityMaps.end());
    }

    return Mesh(vertices, indices, textures, _instanceMatrix, _instancing);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        Texture* texture;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (size_t j = 0; j < loadedTextures.size(); j++)
        {
            if (std::strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(loadedTextures[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            std::string fullPath = directory + "/" + str.C_Str();
            texture = new Texture(fullPath.c_str(), typeName.c_str(), true);
            texture->path = str.C_Str();
            textures.push_back(*texture);
            loadedTextures.push_back(*texture);
        }
    }
    return textures;
}
bool Model::isOpaque()
{
    return opaque;
}