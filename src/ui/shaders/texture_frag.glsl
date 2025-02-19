#version 330 core

uniform sampler2D charTexture;

in vec2 TexCoord;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(charTexture, TexCoord);
    // if(texColor.a < 0.1)
    //     discard;
    FragColor = texColor;
    
    
    // FragColor = texture(charTexture, TexCoord);

    // FragColor.rb = (1.0 - FragColor.aa);

    // FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
