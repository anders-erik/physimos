#version 460 core

layout (location = 0 ) in vec3 aPos;
layout (location = 1 ) in vec3 aColor;

uniform mat4 view;
uniform mat4 perspective;

out vec3 color;

void main() {

    color = aColor;
    // color = vec3(1.0, 1.0, 1.0);

    gl_Position = perspective * view * vec4(aPos, 1.0);
    // gl_Position =  vec4(aPos, 1.0);


}


