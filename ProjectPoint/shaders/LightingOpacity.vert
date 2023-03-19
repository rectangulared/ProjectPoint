#version 460 core
// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// Texture Coordinates
layout (location = 2) in vec2 aTex;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec3 camPos;

uniform vec3 _camPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 physicsTransform;

void main()
{
    camPos = _camPos;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTex;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}