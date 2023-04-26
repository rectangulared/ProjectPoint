#version 460 core
// Positions/Coordinates
layout (location = 0) in vec3 vertPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 vertNormal;

layout (location = 2) in vec3 vertColor;
// Texture Coordinates
layout (location = 3) in vec2 sourceTexCoords;

out VS_OUT
{
    vec2 texCoords;
    vec3 normal;
    vec3 fragPos;
    vec3 camPos;
    vec4 fragPosLightSpace;
} vs_out;

layout (std140, binding = 1) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform vec3 camPos;
uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    vs_out.camPos = camPos;
    vs_out.fragPos = vec3(model * vec4(vertPos, 1.0));
    vs_out.normal = transpose(inverse(mat3(model))) * vertNormal;
    vs_out.texCoords = sourceTexCoords;
    vs_out.fragPosLightSpace = lightSpaceMatrix * vec4(vs_out.fragPos, 1.0);
    gl_Position = projection * view * vec4(vs_out.fragPos, 1.0);
}