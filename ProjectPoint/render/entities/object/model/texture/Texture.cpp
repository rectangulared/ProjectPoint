#include "Texture.h"

Texture::Texture(const char* path, const char* textureType)
{
    this->path = path;
	this->type = textureType;

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrComponents;

    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

    if (data)
    {
        GLenum format{ 0 };
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Texture::assignTextureUnit(ShaderProgram& shaderProgram, const char* uniform, GLuint unit)
{
    GLuint textureUnit = glGetUniformLocation(shaderProgram.getProgramID(), uniform);

    shaderProgram.use();

    glUniform1i(textureUnit, unit);
}

void Texture::bind()
{
    //glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::destroy()
{
    glDeleteTextures(1, &_id);
}