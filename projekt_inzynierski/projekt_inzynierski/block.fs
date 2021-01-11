#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2DArray tex;
in float Side;
in float atype;

void main()
{
    FragColor = texture(tex, vec3(TexCoord, (atype-1)*3 + Side));
} 