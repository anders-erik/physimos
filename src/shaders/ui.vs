#version 330 core

layout(location = 0) in vec3 posIn;
layout (location = 1) in vec3 colorIn;

out vec3 colorOut;

uniform mat4 uiTransform;


void main()
{

    gl_Position = uiTransform * vec4(posIn, 1.0);

    
    colorOut = colorIn;
}