#version 460 core

layout (location = 0) in vec3 vertPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 vertNormal;

layout (location = 2) in vec3 vertColor;
// Texture Coordinates
layout (location = 3) in vec2 sourceTexCoords;

out vec2 texCoords;

void main()
{
    texCoords = sourceTexCoords;    
    gl_Position = vec4(vertPos, 1.0);
}