#version 460 core
layout (location = 0) in vec3 vertPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(vertPos, 1.0);
}