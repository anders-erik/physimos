#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 sanityTransform;
uniform mat4 perspective;

uniform mat4 modelViewSanityPersp_mat4;

out vec2 TexCoord;

void main()
{

    TexCoord = aTexCoord;

    vec4 clipCoordinates = perspective * sanityTransform * view * model * vec4(position, 1.0);
    gl_Position = clipCoordinates;

    // colorOut = vec4( clipCoordinates.z / 100.0, clipCoordinates.z / 100.0,  clipCoordinates.z / 100.0, 1.0);
    // colorOut = vec4(color , 1.0);
}