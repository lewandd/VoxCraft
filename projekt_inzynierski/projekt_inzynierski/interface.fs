#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2DArray tex;
uniform int layer;
uniform float selected;

void main()
{
    FragColor = texture(tex, vec3(TexCoord, layer)) * selected;
    FragColor.w = 1.0f;
} 