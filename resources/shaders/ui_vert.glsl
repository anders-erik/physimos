#version 330 core

layout(location = 0) in vec3 posIn;
layout (location = 1) in vec3 colorIn;
layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in float isChar_f;

uniform mat4 uiTransform;


out vec3 colorUpdated;
out vec2 TexCoord;

out float isCharOut_f;



void main()
{

    isCharOut_f = isChar_f;

    gl_Position = uiTransform * vec4(posIn, 1.0);

    
    colorUpdated = colorIn;
    TexCoord = aTexCoord;
}