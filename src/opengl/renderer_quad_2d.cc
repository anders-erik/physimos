
#include "math/vecmat.hh"
#include "renderer_quad_2d.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

namespace opengl {



Quad2D::Quad2D(){


    verts = generate_quad();


    // float width_pixels  = 20.0f;
    // float height_pixels = 30.0f;

    // set_dims(window_width, window_height, width_pixels, height_pixels);
    
    
}


void Quad2D::set_dims(float window_width, float window_height, float width_pixels, float height_pixels){



    float x_center_ndc = 0.0f;
    float y_center_ndc = 0.0f;

    float width_ndc_per_pixel = (2.0f / window_width);
    float height_ndc_per_pixel = (2.0f / window_height);

    float width_ndc = width_pixels * width_ndc_per_pixel;
    float height_ndc = height_pixels * height_ndc_per_pixel;


    // Lower left
    verts[0].pos.x = x_center_ndc - width_ndc / 2;
    verts[0].pos.y = y_center_ndc - height_ndc / 2;
    // Lower right
    verts[1].pos.x = x_center_ndc + width_ndc / 2;
    verts[1].pos.y = y_center_ndc - height_ndc / 2;
    // Upper right
    verts[2].pos.x = x_center_ndc + width_ndc / 2;
    verts[2].pos.y = y_center_ndc + height_ndc / 2;

    // Lower left
    verts[3].pos.x = x_center_ndc - width_ndc / 2;
    verts[3].pos.y = y_center_ndc - height_ndc / 2;
    // Upper right
    verts[4].pos.x = x_center_ndc + width_ndc / 2;
    verts[4].pos.y = y_center_ndc + height_ndc / 2;
    // Upper left
    verts[5].pos.x = x_center_ndc - width_ndc / 2;
    verts[5].pos.y = y_center_ndc + height_ndc / 2;
    
}

std::array<VertexQuad2D, 6> Quad2D::generate_quad(){
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



void Quad2DRenderContext::delete_contents(){
    glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO);
}


Quad2DRenderer::Quad2DRenderer(){
    // program = opengl::build_program_vert_frag(opengl::Programs::phont_texture);
    program = opengl::build_program_vert_frag(opengl::Programs::Texture2D);
}



void Quad2DRenderer::create_context(Quad2D& quad){

    // unsigned int render_context.VAO, VBO, EBO;

    // quad.render_context.element_count = quad.faces.size() * 3;

	glGenVertexArrays(1, &quad.render_context.VAO);
	glGenBuffers(1, &quad.render_context.VBO);
    // glGenBuffers(1, &quad.render_context.EBO);

	glBindVertexArray(quad.render_context.VAO);


    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.render_context.EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, quad.faces.size() * sizeof(i3), quad.faces.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, quad.render_context.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad.verts), quad.verts.data(), GL_STATIC_DRAW);
    

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexQuad2D), (void *)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexQuad2D), (void *)(sizeof(f3)));
	glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
}

void Quad2DRenderer::set_model_camera(m3f3 model_mat, m3f3 camera){
    glUseProgram(program);

    glUniformMatrix3fv(glGetUniformLocation(program, "model_mat"), 1, GL_TRUE, (float*) &model_mat);
    glUniformMatrix3fv(glGetUniformLocation(program, "camera2D_mat"), 1, GL_TRUE, (float*) &camera);
}

void Quad2DRenderer::render(Quad2DRenderContext context){

    glUseProgram(program);

    // glBindTexture(GL_TEXTURE_2D, texture_checker);
    // glBindTexture(GL_TEXTURE_2D, texture_F);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 3);
    glBindTexture(GL_TEXTURE_2D, context.texture);

    glBindVertexArray(context.VAO); 
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawElements(GL_TRIANGLES, context.element_count , GL_UNSIGNED_INT, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void Quad2DRenderer::render_multisample_texture(Quad2DRenderContext context){

    glUseProgram(program);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, context.texture);

    glBindVertexArray(context.VAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

}
