#include "opengl/program.hh"
#include "opengl/color.hh"

#include "math/vecmat.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

#include "shader_mesh.hh"


void ShaderMesh::
init()
{
    build();

    glUseProgram(id);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}


void ShaderMesh::
set_camera_view_projection(m4f4 persective_mat, m4f4 view_mat)
{
    glUseProgram(id);
    // opengl::gpu_use_program(opengl::ProgramName::ModelTexture);


    glUniformMatrix4fv(glGetUniformLocation(id, "projection"), 1, GL_TRUE, (float*) &persective_mat);
    glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_TRUE, (float*) &view_mat);
}


void ShaderMesh::
set_color(f3 color)
{
    glUseProgram(id);
    f4 f4_color = {color, 1.0f};
    glUniform4fv(glGetUniformLocation(id, "mesh_color"), 1, f4_color.pointer());
}



void ShaderMesh::
render(Mesh& mesh, unsigned int color)
{
    m4f4 model_matrix;

    glUseProgram(id);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.verts.size() * sizeof(Vertex), mesh.verts.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.faces.size() * sizeof(TriangleFaceIndeces), mesh.faces.data(), GL_STATIC_DRAW);


    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // glBindVertexArray(0);

    glBindVertexArray(VAO);
    glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_FALSE, model_matrix.pointer());


    glUniform4fv(glGetUniformLocation(id, "mesh_color"), 1, Color::uint_to_f4(color).pointer());


    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glLineWidth(2);

    glDrawElements(GL_TRIANGLES, mesh.faces.size() * 3, GL_UNSIGNED_INT, 0);

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}


void ShaderMesh::
render(const m4f4& model_matrix, Mesh& mesh, unsigned int color)
{
    glUseProgram(id);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.verts.size() * sizeof(Vertex), mesh.verts.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.faces.size() * sizeof(TriangleFaceIndeces), mesh.faces.data(), GL_DYNAMIC_DRAW);


    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // glBindVertexArray(0);

    glBindVertexArray(VAO);
    glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_TRUE, model_matrix.pointer());


    glUniform4fv(glGetUniformLocation(id, "mesh_color"), 1, Color::uint_to_f4(color).pointer());


    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glLineWidth(2);

    glDrawElements(GL_TRIANGLES, mesh.faces.size() * 3, GL_UNSIGNED_INT, 0);

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}


void ShaderMesh::
render(const m4f4& model_matrix, Mesh& mesh)
{
    glUseProgram(id);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.verts.size() * sizeof(Vertex), mesh.verts.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.faces.size() * sizeof(TriangleFaceIndeces), mesh.faces.data(), GL_DYNAMIC_DRAW);


    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // glBindVertexArray(0);

    glBindVertexArray(VAO);
    glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_TRUE, model_matrix.pointer());


    glUniform4fv(glGetUniformLocation(id, "mesh_color"), 1, Color::uint_to_f4(mesh.color).pointer());


    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glLineWidth(2);

    glDrawElements(GL_TRIANGLES, mesh.faces.size() * 3, GL_UNSIGNED_INT, 0);

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}



void ShaderMesh::
render_filled(const m4f4 & model_matrix, Mesh & mesh)
{
    glUseProgram(id);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.verts.size() * sizeof(Vertex), mesh.verts.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.faces.size() * sizeof(TriangleFaceIndeces), mesh.faces.data(), GL_DYNAMIC_DRAW);


    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // glBindVertexArray(0);

    glBindVertexArray(VAO);
    glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_TRUE, model_matrix.pointer());


    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    glDrawElements(GL_TRIANGLES, mesh.faces.size() * 3, GL_UNSIGNED_INT, 0);

}


