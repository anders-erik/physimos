
#include "math/vecmat.hh"
#include "opengl/texture.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

#include "program_quad2D.hh"





void ProgramQuad2D::
init()
{
    glUseProgram(id);

    // GET UNIFORM LOCATIONS
    camera2D_mat_LOC = glGetUniformLocation(id, "camera2D_mat");
    model_mat_LOC = glGetUniformLocation(id, "model_mat");


    std::array<VertexQuad2D, 6> verts = generate_quad_verts_c05();

    glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
    // glGenBuffers(1, &quad.render_context.EBO);

	glBindVertexArray(vao);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.render_context.EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, quad.faces.size() * sizeof(i3), quad.faces.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Sizeof OK because its an array
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts.data(), GL_STATIC_DRAW);
    

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexQuad2D), (void *)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexQuad2D), (void *)(sizeof(f3)));
	glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void ProgramQuad2D::set_camera_matrix(m3f3 _camera_matrix){

    glUseProgram(id);

    glUniformMatrix3fv(camera2D_mat_LOC, 1, GL_TRUE, _camera_matrix.pointer() );

}


void ProgramQuad2D::set_model_texture(const m3f3 model_matrix, const unsigned int texture) const {
    
    glUseProgram(id);
    
    glUniformMatrix3fv(model_mat_LOC, 1, GL_TRUE, model_matrix.pointer_const());

    glBindTexture(GL_TEXTURE_2D, texture);

}


void ProgramQuad2D::draw() const {

    glUseProgram(id);

    glBindVertexArray(vao);
    

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisable(GL_BLEND);

}





std::array<VertexQuad2D, 6> ProgramQuad2D::generate_quad_verts_c05()
{
    std::array<VertexQuad2D, 6> verts;

    VertexQuad2D v0;  // Lower left
    v0.pos.x = 0.0f;
    v0.pos.y = 0.0f;
    v0.pos.z = 0.0f;
    v0.tex.x = 0.0f;
    v0.tex.y = 0.0f;

    VertexQuad2D v1;  // Lower right
    v1.pos.x = 1.0f;
    v1.pos.y = 0.0f;
    v1.pos.z = 0.0f;
    v1.tex.x = 1.0f;
    v1.tex.y = 0.0f;

    VertexQuad2D v2;  // Upper right
    v2.pos.x = 1.0f;
    v2.pos.y = 1.0f;
    v2.pos.z = 0.0f;
    v2.tex.x = 1.0f;
    v2.tex.y = 1.0f;

    VertexQuad2D v3;  // Upper left
    v3.pos.x = 0.0f;
    v3.pos.y = 1.0f;
    v3.pos.z = 0.0f;
    v3.tex.x = 0.0f;
    v3.tex.y = 1.0f;

    // Low Right triangle
    verts[0] = v0;
    verts[1] = v1;
    verts[2] = v2;
    // Upper left triangle
    verts[3] = v0;
    verts[4] = v2;
    verts[5] = v3;

    return verts;
}


