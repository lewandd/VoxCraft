#version 330 core
out vec4 FragColor;

in float out_selected;
in vec2 TexCoord;
uniform sampler2D ourTexture;
in float Side;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * out_selected;
} 