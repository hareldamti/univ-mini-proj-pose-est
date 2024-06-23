#version 330 core
in vec2 TexCoord;
out vec4 FragColor;
//uniform vec4 tintColor;
void main()
{
    FragColor = vec4(
        0.5 + 0.5 * sin(TexCoord.x * 10),
        0.5 + 0.5 * sin(TexCoord.y * 10),
        0.5 + 0.5 * sin((TexCoord.x - TexCoord.y) * 4),
        1.0);
}