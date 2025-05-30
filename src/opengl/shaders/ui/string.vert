#version 330 core

layout (location = 0) in vec3 posIn;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float xOffset;

uniform mat4 viewportTransform;
uniform mat4 primitiveTransform;

out vec2 TexCoord;


void main()
{
    
    mat4 primitiveTransform_shifted = primitiveTransform;

    // Shift the given char offset in the positive x direction
    // NOTE: Column major!
    primitiveTransform_shifted[3].x += xOffset;

    gl_Position = viewportTransform * primitiveTransform_shifted * vec4(posIn, 1.0);

    // aTexCoord.x += 0.05;

    TexCoord = vec2(aTexCoord.x, aTexCoord.y+0.05);
}