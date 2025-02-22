#version 330 core

uniform sampler2D charTexture;

uniform vec4 color4;
uniform float darknessShift;

// in vec4 color4;
// in float darknessShift;

out vec4 FragColor;

void main()
{
    // FragColor = vec4(color4.x - darknessShift, color4.y - darknessShift, color4.z - darknessShift, color4.w - darknessShift);
    FragColor = color4;



    // vec4 texColor = texture(charTexture, TexCoord);
    // if(texColor.a < 0.1) 
    //     discard;

    // Play around with texture coordinates
    // FragColor = vec4(texColor.x, texColor.y, texColor.z, 1.0f);
    // FragColor = vec4(texColor.x, texColor.y, texColor.z, texColor.w);
    // FragColor = vec4(texColor.x, texColor.y, texColor.z, 0.5);

    // float darken = 0.0;
    // Darken
    // FragColor = vec4(texColor.x - darken, texColor.y - darken, texColor.z - darken, texColor.w);
    
    // Render texture as is
    
    
    // FragColor = texture(charTexture, TexCoord);

    // FragColor.rb = (1.0 - FragColor.aa);

    // FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
