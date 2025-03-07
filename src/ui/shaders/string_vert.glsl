#version 330 core

layout(location = 0) in vec3 posIn;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 viewportTransform;
uniform mat4 primitiveTransform;

out vec2 TexCoord;


void main()
{
    gl_Position = viewportTransform * primitiveTransform * vec4(posIn, 1.0);

    // aTexCoord.x += 0.05;

    TexCoord = vec2(aTexCoord.x, aTexCoord.y+0.05);
}