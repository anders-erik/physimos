#include <iostream>
#include <cmath>
#include <cstring> // memcpy

#include "glad/glad.h"

#include "opengl/program.hh"
#include "opengl/color.hh"

#include "math/vecmat.hh"


#include "scene/mesh.hh"
#include "scene/mesh_texture.hh"

#include "program_quad.hh"



void ProgramQuad::init()
{
    glUseProgram(id);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // MESH
    Mesh mesh_quad;
    mesh_quad.create_quad();
    size_t vert_size_bytes = mesh_quad.vert_size_bytes();

    glGenBuffers(1, &vbo_vert);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
    glBufferData(GL_ARRAY_BUFFER, vert_size_bytes, mesh_quad.verts.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);


    // TEXTURE
    MeshTexture mesh_texture_quad;
    mesh_texture_quad.create_quad();
    size_t text_size_bytes = mesh_texture_quad.vert_size_bytes();

    glGenBuffers(1, &vbo_text);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_text);
    glBufferData(GL_ARRAY_BUFFER, text_size_bytes, mesh_texture_quad.verts.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1); // Texture Coord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(f2), (void*)0);
    

    // FACES
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_quad.faces.size() * sizeof(TriangleFaceIndeces), mesh_quad.faces.data(), GL_STATIC_DRAW);


    model_matrix_LOC = glGetUniformLocation(id, "model");
    projection_matrix_LOC = glGetUniformLocation(id, "projection");
    view_matrix_LOC = glGetUniformLocation(id, "view");

    texture_0_LOC = glGetUniformLocation(id, "texture_0");
    glUniform1i(texture_0_LOC, 0);
}



void ProgramQuad::
set_camera_view_projection(m4f4 persective_mat, m4f4 view_mat)
{
    glUseProgram(id);

    glUniformMatrix4fv(view_matrix_LOC, 1, GL_TRUE, (float*) &view_mat);
    glUniformMatrix4fv(projection_matrix_LOC, 1, GL_TRUE, (float*) &persective_mat);
}




void ProgramQuad::
render(const m4f4& model_matrix)
{
    glUseProgram(id);

    glUniformMatrix4fv(model_matrix_LOC, 1, GL_FALSE, model_matrix.pointer());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, opengl::texture_get_id(opengl::Textures::Grass));

    glBindVertexArray(vao);

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0);

}


