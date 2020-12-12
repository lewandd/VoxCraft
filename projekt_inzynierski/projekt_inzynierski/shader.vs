#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aSide;

out float out_selected;
out vec2 TexCoord;
out float Side;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float scale;
uniform float selected;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord*scale;
    out_selected = selected;
    Side = aSide;
}