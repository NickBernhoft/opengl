// vertex shader

#version 330
in vec2 vPos;
uniform vec2 transformVec;
uniform vec3 colorVec;
out vec3 color;

void main()
{
    gl_Position = vec4(vPos + transformVec, 0.0, 1.0);
    color = colorVec;
};