#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aSide;
layout (location = 3) in vec3 offset;
layout (location = 4) in float scale;
layout (location = 5) in float type;
layout (location = 6) in float ind;

out vec2 TexCoord;
out float Side;
out float atype;
out float distance;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 rot[6];

void main()
{
    vec4 pos = rot[int(ind)] * vec4(aPos, 1.0);
    gl_Position = projection * view * vec4(pos.xyz * scale + offset, 1.0);
    TexCoord = aTexCoord*scale;
    Side = ind;
    if (ind == 2) {
        Side = 1;
    }
    else if (ind == 3) {
        Side = 2;
    }
    else {
        Side = 0;
    }
    atype = type;
    distance = gl_Position.z;
}