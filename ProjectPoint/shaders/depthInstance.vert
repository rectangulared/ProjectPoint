#version 460 core
layout (location = 0) in vec3 vertPos;

layout (location = 4) in mat4 instanceMatrix;

uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * instanceMatrix * vec4(vertPos, 1.0);
}