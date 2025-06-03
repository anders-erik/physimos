#version 330 core

layout(location = 0) in vec3 posIn;
// layout (location = 1) in vec2 aTexCoord;

uniform mat4 viewportTransform;
uniform mat4 primitiveTransform;

// uniform vec4 color4;
// uniform float darknessShift;

// out vec4 color4;
// out float darknessShift;

void main()
{
    gl_Position = viewportTransform * primitiveTransform * vec4(posIn, 1.0);

    // TexCoord = aTexCoord;
}