#version 330 core

uniform sampler2D texture_0; 

in vec2 TexCoords;

out vec4 FragColor;

void main() 
{
    vec4 text_color = texture(texture_0, TexCoords);

    FragColor = text_color;
    // FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}
