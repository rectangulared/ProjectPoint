#version 460 core
// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;

layout (location = 2) in vec3 aColor;
// Texture Coordinates
layout (location = 3) in vec2 aTex;

out VS_OUT
{
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragPos;
    vec3 camPos;
} vs_out;

layout (std140, binding = 1) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform vec3 _camPos;
uniform mat4 model;

void main()
{
    vs_out.camPos = _camPos;
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.TexCoord = aTex;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}