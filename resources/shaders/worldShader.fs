#version 330 core 

out vec4 FragColor;
uniform vec4 vertexColor;
uniform bool hasTexture;

in vec2 TexCoord;
in vec4 colorOut;

uniform sampler2D ourTexture;

void main()
{
    if(hasTexture){
        FragColor = texture(ourTexture, TexCoord);
    }
    else {
        FragColor = colorOut;
    }
}