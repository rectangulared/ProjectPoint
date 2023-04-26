#version 460 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;

void main()
{
    fragColor = texture(screenTexture, texCoords);
    float gamma = 2.2;
    fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / gamma));
}