#version 330 core
in vec2 TexCoord;
out vec4 FragColor;
uniform vec4 color;
void main()
{
    // 0, 0: white
    // 0.5, 0: orange
    // 1, 0: gray
    FragColor = TexCoord.x < 0.01 ? color :
                TexCoord.x > 0.99 ? vec4(1,0.5,0,1) :
                TexCoord.x > 0.6  ? vec4(0.3,0.3,0.3,1) :
                                    vec4(0.5,0.5,0.5,1);
}