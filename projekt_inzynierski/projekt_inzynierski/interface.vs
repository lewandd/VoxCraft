#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform vec3 translate;

void main()
{
    gl_Position = vec4(aPos+translate, 1.0f);
    TexCoord = aTexCoord;
}