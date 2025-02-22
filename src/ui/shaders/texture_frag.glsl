#version 330 core

uniform sampler2D charTexture;

in vec2 TexCoord;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(charTexture, TexCoord);
    // if(texColor.a < 0.1) 
    //     discard;

    // Play around with texture coordinates
    // FragColor = vec4(texColor.x, texColor.y, texColor.z, 1.0f);
    // FragColor = vec4(texColor.x, texColor.y, texColor.z, texColor.w);
    // FragColor = vec4(texColor.x, texColor.y, texColor.z, 0.5);

    float darken = 0.0;
    // Darken
    FragColor = vec4(texColor.x - darken, texColor.y - darken, texColor.z - darken, texColor.w);
    
    // Render texture as is
    // FragColor = texColor;
    
    
    // FragColor = texture(charTexture, TexCoord);

    // FragColor.rb = (1.0 - FragColor.aa);

    // FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
