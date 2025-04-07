#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTextCoord;

uniform mat3 model_mat;
uniform mat3 camera2D_mat;

out vec2 textCoord;

void main()
{
    float val = model_mat[2][0] / 200;

    vec3 pos_local = vec3(aPos.x, aPos.y, 1.0f);

    vec3 pos_scene =  camera2D_mat * model_mat * pos_local;
    // vec3 pos_scene = model_mat * pos_local;
    // vec3 pos_scene = pos_local;

    pos_scene.z = 0.0;


    gl_Position = vec4(pos_scene, 1.0);

    textCoord = aTextCoord;
}