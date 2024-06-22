#version 330 core
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D TexImage;
void main()
{
    FragColor = texture(TexImage, TexCoord);
}