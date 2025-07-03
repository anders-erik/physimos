#include "opengl/program.hh"
#include "opengl/color.hh"

#include "math/vecmat.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

#include "shader_color_light.hh"


void ShaderColorLight::
init()
{
    build();

    glUseProgram(id);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vbo_pos);
    glGenBuffers(1, &vbo_norm);
    glGenBuffers(1, &EBO);

    light_pos_LOC       = glGetUniformLocation(id, "light_pos");
    view_mat_LOC        = glGetUniformLocation(id, "view");
    perspective_mat_LOC = glGetUniformLocation(id, "perspective");
}


void ShaderColorLight::
set_camera_view_perspective(m4f4 view_mat, m4f4 perspective_mat)
{
    glUseProgram(id);
    glUniformMatrix4fv(view_mat_LOC, 1, GL_TRUE, (float*) &view_mat);
    glUniformMatrix4fv(perspective_mat_LOC, 1, GL_TRUE, (float*) &perspective_mat);
}

void ShaderColorLight::set_light_pos(f3 light_pos)
{
    glUniform3fv(light_pos_LOC, 1, light_pos.pointer());
}



void ShaderColorLight::
render(const m4f4& model_matrix, Mesh& mesh)
{
    glUseProgram(id);

    glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_TRUE, model_matrix.pointer());
    glUniform4fv(glGetUniformLocation(id, "mesh_color"), 1, Color::uint_to_f4(mesh.color).pointer());

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

    // Elements
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.faces.size() * sizeof(TriangleFaceIndeces), mesh.faces.data(), GL_DYNAMIC_DRAW);


    glBindVertexArray(VAO);

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    // glLineWidth(2);

    glDrawElements(GL_TRIANGLES, mesh.faces.size() * 3, GL_UNSIGNED_INT, 0);
}


