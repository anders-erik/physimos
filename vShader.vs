#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 transform;
uniform mat4 perspective;

out vec4 grayscaleColor;

void main()
{
    vec4 clipCoordinates = perspective * transform * vec4(position, 1.0);
    grayscaleColor = vec4( clipCoordinates.z / 100.0, clipCoordinates.z / 100.0,  clipCoordinates.z / 100.0, 1.0);

    gl_Position = clipCoordinates;
}