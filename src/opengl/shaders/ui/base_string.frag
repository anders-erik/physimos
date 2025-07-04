#version 330 core

uniform sampler2D font_unit;

in vec2 TexCoord;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(font_unit, TexCoord);
    // if(texColor.a < 0.1) 
    //     discard;

    // Play around with texture coordinates
    // FragColor = vec4(texColor.x, texColor.y, texColor.z, 1.0f);
    // FragColor = vec4(texColor.x, texColor.y, texColor.z, texColor.w);
    // FragColor = vec4(texColor.x, texColor.y, texColor.z, 0.5);

    // Darken
    float darken = 0.0;
    FragColor = vec4(texColor.x - darken, texColor.y - darken, texColor.z - darken, texColor.w);
    // Draw black to be able to see render
    // FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    
    
    // Render texture as is
    // FragColor = texColor;
    
    
    // FragColor = texture(charTexture, TexCoord);

    // FragColor.rb = (1.0 - FragColor.aa);

    // FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
