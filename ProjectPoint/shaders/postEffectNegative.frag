#version 460 core

out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    fragColor = texture(texture_diffuse1, TexCoords);
    float average = (fragColor.r + fragColor.g + fragColor.b) / 3.0;
    fragColor = vec4(average, average, average, 1.0);
}