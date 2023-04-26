#version 460 core
// Positions/Coordinates
layout (location = 0) in vec3 vertPos;

layout (std140, binding = 1) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    vec3 fragPos = vec3(model * vec4(vertPos, 1.0));
    gl_Position = projection * view * vec4(fragPos, 1.0);
}