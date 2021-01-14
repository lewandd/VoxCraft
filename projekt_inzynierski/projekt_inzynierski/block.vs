#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aSide;
layout (location = 3) in vec3 offset;
layout (location = 4) in float scale;
layout (location = 5) in float type;

out vec2 TexCoord;
out float Side;
out float atype;
out float distance;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4((aPos*scale + offset), 1.0);
    TexCoord = aTexCoord*scale;
    Side = aSide;
    atype = type;
    distance = gl_Position.z;
}