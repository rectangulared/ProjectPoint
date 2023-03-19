#version 460 core

layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;

layout (location = 2) in vec3 aColor;
// Texture Coordinates
layout (location = 3) in vec2 aTex;

out vec2 TexCoords;

void main()
{
    TexCoords = aTex;    
    gl_Position = vec4(aPos, 1.0);
}