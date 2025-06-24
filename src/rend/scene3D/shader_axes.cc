#include "glad/glad.h"
#include "scene/camera.hh"

#include "rend/scene3D/shader_axes.hh"

#include "scene/model.hh"
#include "math/vecmat.hh"

// #include "rubik.hh"

#include <iostream>
#include <cmath>



// RendererModel renderer_model;
// RendererAxes renderer_axes;

// CameraOrbital camera;

// Model model;



void ShaderAxes::init()
{
    build();

    glUseProgram(id);

    transform_location = glGetUniformLocation(id, "transform");
    view_location = glGetUniformLocation(id, "view");
    perspective_location = glGetUniformLocation(id, "perspective");


    AxesVertex vertices[6];

    float scale = 1.1f;

    f3 _000 = {0.0f,  0.0f,  0.0f};
    f3 _100 = {scale, 0.0f,  0.0f};
    f3 _010 = {0.0f,  scale, 0.0f};
    f3 _001 = {0.0f,  0.0f,  scale};

    f3 red   = {1.0f, 0.0f, 0.0f};
    f3 green = {0.0f, 1.0f, 0.0f};
    f3 blue  = {0.0f, 0.0f, 1.0f};


    vertices[0] = { _000, red};
    vertices[1] = { _100, red};

    vertices[2] = { _000, green};
    vertices[3] = { _010, green};
    
    vertices[4] = { _000, blue};
    vertices[5] = { _001, blue};

    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(AxesVertex) * 6, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AxesVertex), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AxesVertex), (void*) sizeof(f3));
    glEnableVertexAttribArray(1);

}
void ShaderAxes::set_uniforms(m4f4 transform_mat, m4f4 view_mat, m4f4 pers_mat){

    glUseProgram(id);

    glUniformMatrix4fv(transform_location, 1, GL_TRUE, (float*) &transform_mat);
    glUniformMatrix4fv(view_location, 1, GL_TRUE, (float*) &view_mat);
    glUniformMatrix4fv(perspective_location, 1, GL_TRUE, (float*) &pers_mat);

}
void ShaderAxes::render(){

    glUseProgram(id);

    glBindVertexArray(vao);

    glDrawArrays(GL_LINES, 0, 6);
}


