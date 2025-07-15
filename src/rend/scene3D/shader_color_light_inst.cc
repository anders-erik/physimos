#include "opengl/program.hh"
#include "opengl/color.hh"

#include "opengl/texture_unit.hh"

#include "math/vecmat.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

#include "shader_color_light_inst.hh"


void ShaderColorLightInst::
init()
{
    build();

    glUseProgram(id);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vbo_pos);
    glGenBuffers(1, &vbo_norm);
    glGenBuffers(1, &vbo_color);
    glGenBuffers(1, &vbo_model_mats);
    glGenBuffers(1, &EBO);

    view_mat_LOC        = glGetUniformLocation(id, "view");
    perspective_mat_LOC = glGetUniformLocation(id, "perspective");

    light_pos_LOC       = glGetUniformLocation(id, "light_pos");
    light_color_LOC     = glGetUniformLocation(id, "light_color");
}


void ShaderColorLightInst::
set_camera_view_perspective(m4f4 view_mat, m4f4 perspective_mat)
{
    glUseProgram(id);
    glUniformMatrix4fv(view_mat_LOC, 1, GL_TRUE, (float*) &view_mat);
    glUniformMatrix4fv(perspective_mat_LOC, 1, GL_TRUE, (float*) &perspective_mat);
}

void ShaderColorLightInst::set_light_pos(f3 light_pos)
{
    glUniform3fv(light_pos_LOC, 1, light_pos.pointer());
}

void ShaderColorLightInst::set_light_color(f3 light_color)
{
    glUniform3fv(light_color_LOC, 1, light_color.pointer());
}

void ShaderColorLightInst::set_data(Mesh & mesh)
{
    glUseProgram(id);

    glBindVertexArray(VAO);

    // Position
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
    glBufferData(GL_ARRAY_BUFFER, mesh.verts.size() * sizeof(Vertex), mesh.verts.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Normals
    glBindBuffer(GL_ARRAY_BUFFER, vbo_norm);
    glBufferData(GL_ARRAY_BUFFER, mesh.normals.size() * sizeof(f3), mesh.normals.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(f3), (void*)0);
    glEnableVertexAttribArray(1);

    // Texture Coords
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    glBufferData(GL_ARRAY_BUFFER, mesh.colors.size() * sizeof(f3), mesh.colors.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(f3), (void*)0);
    glEnableVertexAttribArray(2);


    // Elements
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.faces.size() * sizeof(TriangleFaceIndeces), mesh.faces.data(), GL_DYNAMIC_DRAW);

    tri_face_count = mesh.faces.size();
}




void ShaderColorLightInst::
render_fill(uint instance_count)
{
    glUseProgram(id);

    glBindVertexArray(VAO);

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    // glDrawElements( GL_TRIANGLES, 
    //                 3 * tri_face_count, 
    //                 GL_UNSIGNED_INT,
    //                 0                   );
    
    glDrawElementsInstanced(    GL_TRIANGLES, 
                                3 * tri_face_count, 
                                GL_UNSIGNED_INT,
                                0,
                                instance_count      );
}




