#version 330 core
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D TexImage;
uniform vec4 tint;
void main()
{
    FragColor = tint * texture(TexImage, TexCoord);
}