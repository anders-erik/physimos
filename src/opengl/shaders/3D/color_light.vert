#version 460 core

layout (location = 0 ) in vec3 aPos;
layout (location = 1 ) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

out vec3 frag_pos;
out vec3 frag_normal;

void main()
{
    gl_Position = perspective * view * model * vec4(aPos, 1.0);

    vec3 normal_model = normalize(aPos);

    frag_normal = mat3(transpose(inverse(model))) * aNormal;
    frag_pos = vec3(model * vec4(aPos, 1.0));
}


