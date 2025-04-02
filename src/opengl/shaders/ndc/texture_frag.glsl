#version 330 core 

uniform sampler2D myTexture;

in vec2 textCoord;

out vec4 FragColor;
void main()
{
    vec4 textColor = texture(myTexture, textCoord);

    // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    FragColor = textColor;
}