#version 330 core 

out vec4 FragColor;

in vec2 TexCoord;
in vec4 colorOut;

uniform sampler2D ourTexture;

void main()
{
        FragColor = texture(ourTexture, TexCoord);
}