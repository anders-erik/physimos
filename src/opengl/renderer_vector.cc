
#include <cmath>

#include "glad/glad.h"

#include "opengl/program.hh"
#include "opengl/renderer_vector.hh"

#include "math/transform.hh"


namespace opengl {




void RendererVector::init() {



    program = build_program_vert_frag(ProgramName::Vector);


    glUseProgram(program);

    glUniform1f( glGetUniformLocation(program, "vector_scale"), scale);
    glUniform3fv( glGetUniformLocation(program, "vector_color"), 1, (float*) &color);
    // glUniform3f( glGetUniformLocation(program, "vector_color"), color.x, color.y, color.z);
    
    float length = 1.0f;
    float pyramid_width = 0.07f;
    float pyramid_height = 0.07f;
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

    

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vectorVertices), vectorVertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, arrow.size()*sizeof(f3), arrow.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, arrow.size());

}


void RendererVector::set_project_view_matrix(m4f4 project_mat, m4f4 view_mat){

    // glUseProgram(shader);
    // unsigned int program = opengl::gpu_get_program(Programs::Vector);
    glUseProgram(program);

    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, (float*) &project_mat);
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (float*) &view_mat);

}


void RendererVector::render(f3 vector, f3 translation){

    // PROGRAM
    opengl::gpu_use_program(ProgramName::Vector);

    // UNIFORMS
    glUniform3fv(glGetUniformLocation(program, "vector"), 3, (float*) &vector);
    glUniform3fv( glGetUniformLocation(program, "vector_color"), 1, (float*) &color);
    glUniform1f( glGetUniformLocation(program, "vector_scale"), sqrtf( vector.x*vector.x + vector.y*vector.y + vector.z*vector.z));

    // ROTATION
    m4f4 rotation_mat = math::rect_f3_to_m4f4(vector);
    glUniformMatrix4fv(glGetUniformLocation(program, "rotation_mat"), 1, GL_TRUE, (float*) &rotation_mat);


    // TRANSLATION 
    m4f4 translation_mat;
    translation_mat.translate(translation);
    glUniformMatrix4fv(glGetUniformLocation(program, "translation_mat"), 1, GL_TRUE, (float*) &translation_mat );


    // DRAW
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, arrow.size());

}


}
