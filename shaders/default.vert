#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
uniform mat4 camera;
out vec2 uv;
void main()
{
    gl_Position = camera * vec4(aPos, 1);
    uv = aUV;
}