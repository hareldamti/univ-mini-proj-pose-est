#version 330 core
in vec2 uv;
out vec4 FragColor;
void main()
{
    FragColor = vec4(uv.x * vec3(1.0, 0.0, 0.0) + uv.y * vec3(0.0, 0.0, 1.0), 1.0);
}