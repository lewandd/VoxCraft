#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2DArray tex;
in float Side;
in float atype;
in float distance;

void main()
{
    FragColor = texture(tex, vec3(TexCoord, (atype-1)*3 + Side));
    FragColor.xyz = mix(FragColor.xyz, vec3(0.55f, 0.75f, 0.85f), distance*distance*distance/1000000.0f);
} 