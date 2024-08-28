#version 330 core 

out vec4 FragColor;
uniform vec4 vertexColor; 

in vec4 colorOut;

void main()
{
    
    FragColor = colorOut;
}