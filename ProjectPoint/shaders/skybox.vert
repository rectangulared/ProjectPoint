#version 460 core
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;

layout (location = 2) in vec3 aColor;
// Texture Coordinates
layout (location = 3) in vec2 aTex;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
} 