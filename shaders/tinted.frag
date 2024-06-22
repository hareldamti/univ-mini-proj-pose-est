#version 330 core
in vec2 uv;
out vec4 FragColor;
uniform vec4 tintColor;
void main()
{
    FragColor = vec4(
        0.5 + 0.5 * sin(uv.x * 10),
        0.5 + 0.5 * sin(uv.y * 10),
        0.5 + 0.5 * sin((uv.x - uv.y) * 4),
        1.0);
    FragColor *= tintColor;
}