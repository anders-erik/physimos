#include "glad/glad.h"
#include "scene/scene.hh"
#include "scene/camera.hh"

#include "rend/scene3D/program_axes.hh"

#include "model/model.hh"
#include "math/vecmat.hh"

// #include "rubik.hh"

#include <iostream>
#include <cmath>



// RendererModel renderer_model;
// RendererAxes renderer_axes;

// CameraOrbital camera;

// Model model;



void ProgramAxes::init()
{
    glUseProgram(id);

    transform_location = glGetUniformLocation(id, "transform");
    view_location = glGetUniformLocation(id, "view");
    perspective_location = glGetUniformLocation(id, "perspective");


    AxesVertex vertices[6];

    float axes_scale = 1.0f;

    vertices[0].pos.x = 0.0f;
    vertices[0].color.x = 1.0f;

    vertices[1].pos.x = axes_scale;
    vertices[1].color.x = 1.0f;

    vertices[2].pos.y = 0.0f;
    vertices[2].color.y = 1.0f;

    vertices[3].pos.y = axes_scale;
    vertices[3].color.y = 1.0f;

    vertices[4].pos.z = 0.0f;
    vertices[4].color.z = 1.0f;

    vertices[5].pos.z = axes_scale;
    vertices[5].color.z = 1.0f;

    
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
void ProgramAxes::set_uniforms(m4f4 transform_mat, m4f4 view_mat, m4f4 pers_mat){

    glUseProgram(id);

    glUniformMatrix4fv(transform_location, 1, GL_TRUE, (float*) &transform_mat);
    glUniformMatrix4fv(view_location, 1, GL_TRUE, (float*) &view_mat);
    glUniformMatrix4fv(perspective_location, 1, GL_TRUE, (float*) &pers_mat);

}
void ProgramAxes::render(){

    glUseProgram(id);

    glBindVertexArray(vao);

    glDrawArrays(GL_LINES, 0, 6);
}


