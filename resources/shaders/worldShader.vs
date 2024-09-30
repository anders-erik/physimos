#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 sanityTransform;
uniform mat4 perspective;

out vec4 colorOut;
out vec2 TexCoord;

void main()
{

    TexCoord = aTexCoord;

    vec4 clipCoordinates = perspective * sanityTransform * view * transform * vec4(position, 1.0);
    gl_Position = clipCoordinates;

    // colorOut = vec4( clipCoordinates.z / 100.0, clipCoordinates.z / 100.0,  clipCoordinates.z / 100.0, 1.0);
    colorOut = vec4(color , 1.0);
}