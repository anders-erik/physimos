#include <iostream>
#include <cmath>

#include "math/vecmat.hh"
#include "math/transform.hh"

#include "opengl/texture.hh"

#include "glad/glad.h"

#include "shader_vector.hh"






void ShaderVector::
init()
{
    build();

    glUseProgram(id);

    // GET UNIFORM LOCATIONS
    // camera2D_mat_LOC = glGetUniformLocation(id, "camera2D_mat");
    // model_mat_LOC = glGetUniformLocation(id, "model_mat");


    // glUseProgram(program);

    glUniform1f( glGetUniformLocation(id, "vector_scale"), scale);
    glUniform3fv( glGetUniformLocation(id, "vector_color"), 1, (float*) &color);
    // glUniform3f( glGetUniformLocation(program, "vector_color"), color.x, color.y, color.z);
    
    float length = 1.0f;
    float pyramid_width = 0.05f;
    float pyramid_height = 0.10f;
    float pyramid_base_loc = length - pyramid_height;

    f3 base = {0.0f, 0.0f, 0.0f};
    f3 head = {length, 0.0f, 0.0f};

    f3 pyramid_y_min = {pyramid_base_loc, -pyramid_width, 0.0f};
    f3 pyramid_y_max = {pyramid_base_loc, pyramid_width, 0.0f};
    f3 pyramid_z_min = {pyramid_base_loc, 0.0f, -pyramid_width};
    f3 pyramid_z_max = {pyramid_base_loc, 0.0f, pyramid_width};

    // std::vector<f3> arrow;

    arrow.push_back(base);
    arrow.push_back(head);

    // Connect pyramid base vertices
    arrow.push_back(pyramid_y_max);
    arrow.push_back(pyramid_z_max);
    arrow.push_back(pyramid_z_max);
    arrow.push_back(pyramid_y_min);
    arrow.push_back(pyramid_y_min);
    arrow.push_back(pyramid_z_min);
    arrow.push_back(pyramid_z_min);
    arrow.push_back(pyramid_y_max);

    // connect pyramid base with arrow tip
    arrow.push_back(pyramid_y_max);
    arrow.push_back(head);
    arrow.push_back(pyramid_z_max);
    arrow.push_back(head);
    arrow.push_back(pyramid_y_min);
    arrow.push_back(head);
    arrow.push_back(pyramid_z_min);
    arrow.push_back(head);

    

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, arrow.size()*sizeof(f3), arrow.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // glBindVertexArray(vao);
    // glDrawArrays(GL_LINES, 0, arrow.size());
}

void ShaderVector::set_project_view_matrix(m4f4 project_mat, m4f4 view_mat)
{
    glUseProgram(id);

    glUniformMatrix4fv(glGetUniformLocation(id, "projection"), 1, GL_TRUE, (float*) &project_mat);
    glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_TRUE, (float*) &view_mat);

}

void ShaderVector::set_color(f3 new_color)
{
    glUseProgram(id);
    color = new_color;
    glUniform3fv( glGetUniformLocation(id, "vector_color"), 1, (float*) &color);
}

void ShaderVector::render(f3 pos, f3 vector)
{
    // PROGRAM
    glUseProgram(id);

    // UNIFORMS
    glUniform3fv(glGetUniformLocation(id, "vector"), 3, (float*) &vector);
    glUniform1f( glGetUniformLocation(id, "vector_scale"), vector.norm());

    // ROTATION
    YawPitch yaw_pitch = Transform::to_yaw_pitch(vector);
    m4f4 vec_rotation_mat = Transform::yaw_pitch_matrix(yaw_pitch);
    glUniformMatrix4fv(glGetUniformLocation(id, "rotation_mat"), 1, GL_TRUE, (float*) &vec_rotation_mat);

    // TRANSLATION 
    m4f4 translation_mat = m4f4::translation(pos);
    glUniformMatrix4fv(glGetUniformLocation(id, "translation_mat"), 1, GL_TRUE, (float*) &translation_mat );


    glBindVertexArray(vao);    
    glDrawArrays(GL_LINES, 0, arrow.size());
}


