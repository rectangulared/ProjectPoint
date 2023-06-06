#version 460 core

layout (location = 0) in vec3 vertPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 vertNormal;

layout (location = 2) in vec3 vertColor;
// Texture Coordinates
layout (location = 3) in vec2 sourceTexCoords;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = vertPos;
    vec4 pos = projection * view * vec4(vertPos, 1.0);
    gl_Position = pos.xyww;
} 