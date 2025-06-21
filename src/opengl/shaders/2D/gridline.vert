#version 430 core

layout(location = 0) in vec2 point;

uniform mat3 camera2D_mat;

void main()
{

    vec3 pos_scene =  camera2D_mat * vec3(point, 1.0f);

    gl_Position = vec4(pos_scene.x, pos_scene.y, 0.0, 1.0);

}