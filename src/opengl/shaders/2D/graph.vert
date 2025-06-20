#version 430 core

layout(location = 0) in float X;
layout(location = 1) in float Y;

uniform mat3 camera2D_mat;


void main()
{

    vec3 pos_scene =  camera2D_mat * vec3(X, Y, 1.0f);

    gl_Position = vec4(pos_scene.x, pos_scene.y, 0.0, 1.0);

}