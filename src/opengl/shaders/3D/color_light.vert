#version 460 core

layout (location = 0 ) in vec3 a_pos;
layout (location = 1 ) in vec3 a_normal;
layout (location = 2 ) in vec3 a_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

out vec3 frag_pos;
out vec3 frag_normal;

out vec3 color;

void main()
{
    gl_Position = perspective * view * model * vec4(a_pos, 1.0);

    vec3 normal_model = normalize(a_pos);

    frag_normal = mat3(transpose(inverse(model))) * a_normal;
    frag_pos = vec3(model * vec4(a_pos, 1.0));
    
    color = a_color;
}


