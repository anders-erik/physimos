#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 perspective;

out vec4 colorOut;

void main()
{

    // mat4 sanityTransform;
    // sanityTransform[0] = vec4(1.0, 0.0, 0.0, 0.0);
    // sanityTransform[1] = vec4(0.0, 1.0, 0.0, 0.0);
    // sanityTransform[2] = vec4(0.0, 0.0, 1.0, 0.0);
    // sanityTransform[3] = vec4(0.0, 0.0, 0.0, 1.0);

    vec4 clipCoordinates = perspective * view * transform * vec4(position, 1.0);
    gl_Position = clipCoordinates;

    // colorOut = vec4( clipCoordinates.z / 100.0, clipCoordinates.z / 100.0,  clipCoordinates.z / 100.0, 1.0);
    colorOut = vec4(color , 1.0);
}