#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 color;

uniform vec3 aColor;
uniform float aSize;

void main()
{
    gl_Position = vec4(aPos*aSize, 1.0f);
    color = aColor;
}