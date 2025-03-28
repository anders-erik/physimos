
#include <cmath>

#include "glad/glad.h"

#include "opengl/renderer_vector.hh"

#include "opengl/program.hh"



namespace opengl {




void RendererVector::init() {



    program = build_program_vert_frag(Programs::Vector);


    glUseProgram(program);

    glUniform1f( glGetUniformLocation(program, "vector_scale"), scale);
    glUniform3fv( glGetUniformLocation(program, "vector_color"), 1, (float*) &color);
    // glUniform3f( glGetUniformLocation(program, "vector_color"), color.x, color.y, color.z);
    


    f3 base = {0.0f, 0.0f, 0.0f};
    f3 head = {1.0f, 0.0f, 0.0f};

    float pyramid_width = 0.05f;
    f3 pyramid_y_min = {0.9f, -pyramid_width, 0.0f};
    f3 pyramid_y_max = {0.9f, pyramid_width, 0.0f};
    f3 pyramid_z_min = {0.9f, 0.0f, -pyramid_width};
    f3 pyramid_z_max = {0.9f, 0.0f, pyramid_width};

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
    glDrawArrays(GL_LINES, 0, 18);

}


void RendererVector::set_project_view_matrix(m4f4 project_mat, m4f4 view_mat){

    // glUseProgram(shader);
    // unsigned int program = opengl::gpu_get_program(Programs::Vector);
    glUseProgram(program);

    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, (float*) &project_mat);
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (float*) &view_mat);

}


void RendererVector::render(f3 vector, f3 translation){
    // NOTE: ANTIPATTERN
    if(vector.x == 0.0f && vector.y == 0.0f)
        vector.x = 0.0001f;

    // glUseProgram(program);
    opengl::gpu_use_program(Programs::Vector);

    glUniform3fv(glGetUniformLocation(program, "vector"), 3, (float*) &vector);

    glUniform3fv( glGetUniformLocation(program, "vector_color"), 1, (float*) &color);


    glUniform1f( glGetUniformLocation(program, "vector_scale"), sqrtf( vector.x*vector.x + vector.y*vector.y + vector.z*vector.z));

    // ROTATION MATRIX
    // Calculate pitch + yaw using passed vector, then use extrinsic rotation to construct rotation vector

    f3 vector_xy = vector;
    // vector_xy.z = 0.0f;

    float vector_xy_mag = sqrtf( vector_xy.x*vector_xy.x + vector_xy.y*vector_xy.y);


    float yaw = atanf(vector_xy.y / vector_xy.x);
    float pitch = atanf( vector.z / vector_xy_mag);
    //  atan only outputs -pi/2 <= atan <= pi/2, thus rotate to other two quadrant if vector facing that direction
    if(vector_xy.x < 0.0f)
        yaw += 3.141592;



    m4f4 rotation_mat;// = glm::mat4(1.0f);

    // rotation_mat = glm::rotate(rotation_mat, yaw, f3(0.0f, 0.0f, 1.0f));
    // rotation_mat = glm::rotate(rotation_mat, -pitch, f3(0.0f, 1.0f, 0.0f));
    
    rotation_mat.rotate_z(-yaw);    // NOTE: Not sure why the signs should both be negative, 
    rotation_mat.rotate_y(-pitch);  // Yet the vectors appear to be displayed correctly when this is the case...

    // rotation_mat = glm::translate(rotation_mat, translation);
    glUniformMatrix4fv(glGetUniformLocation(program, "rotation_mat"), 1, GL_TRUE, (float*) &rotation_mat);


    // TRANSLATION MATRIX
    m4f4 translation_mat; // = glm::mat4(1.0f);
    // translation_mat = glm::translate(translation_mat, translation);
    translation_mat.translate(translation);

    glUniformMatrix4fv(glGetUniformLocation(program, "translation_mat"), 1, GL_TRUE, (float*) &translation_mat );

    // DRAW
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 18);

}


}
