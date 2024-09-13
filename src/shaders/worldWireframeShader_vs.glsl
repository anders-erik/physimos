#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 sanityTransform;
uniform mat4 perspective;

void main()
{

    vec4 clipCoordinates = perspective * sanityTransform * view * model * vec4(position, 1.0);
    gl_Position = clipCoordinates;

}