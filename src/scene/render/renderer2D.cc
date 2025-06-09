
#include <iostream>
#include <cmath>

#include "glad/glad.h"

#include "math/vecmat.hh"

#include "opengl/texture.hh"

#include "scene/quadS2D.hh"

#include "renderer2D.hh"

#include "program_quad2D.hh"

namespace scene {




std::array<VertexQuad2D, 6> RenderContextQuadS2D::generate_quad_verts_c05(){
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


void RenderContextQuadS2D::delete_contents(){
    glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO);
}


void RenderContextQuadS2D::init(){

    std::array<VertexQuad2D, 6> verts = generate_quad_verts_c05();

    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
    // glGenBuffers(1, &quad.render_context.EBO);

	glBindVertexArray(VAO);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.render_context.EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, quad.faces.size() * sizeof(i3), quad.faces.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Sizeof OK because its an array
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts.data(), GL_STATIC_DRAW);
    

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexQuad2D), (void *)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexQuad2D), (void *)(sizeof(f3)));
	glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


RenderContextQuadS2D::RenderContextQuadS2D(){

    init();

}

RenderContextQuadS2D::RenderContextQuadS2D(unsigned int texture_id){

    init();
    set_texture_id(texture_id);

}

void RenderContextQuadS2D::set_texture_id(unsigned int texture_id){
    texture = texture_id;
}





RendererScene2D::RendererScene2D(){

    program_quad_2D.init();

    // program = opengl::build_program_vert_frag(opengl::Programs::phont_texture);
    program = opengl::build_program_vert_frag(program_name_enum);

    init_frame_wire_quad_context_t();
}

void RendererScene2D::create_shape_context_t(RenderContextQuadS2D& render_context, std::vector<VertexQuad2D> verts){

    render_context.texture = opengl::texture_get_id(opengl::Textures::Colors);

	glGenVertexArrays(1, &render_context.VAO);
	glGenBuffers(1, &render_context.VBO);

	glBindVertexArray(render_context.VAO);


    size_t buffer_size = verts.size() *sizeof(VertexQuad2D);
	glBindBuffer(GL_ARRAY_BUFFER, render_context.VBO);
    glBufferData(GL_ARRAY_BUFFER, buffer_size, verts.data(), GL_STATIC_DRAW);
    

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexQuad2D), (void *)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexQuad2D), (void *)(sizeof(f3)));
	glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
}



void RendererScene2D::activate(){
    glUseProgram(program);
}
void RendererScene2D::set_camera(m3f3 camera){

    program_quad_2D.set_camera_matrix(camera);

    glUseProgram(program);
    glUniformMatrix3fv(glGetUniformLocation(program, "camera2D_mat"), 1, GL_TRUE, (float*) &camera);
}
void RendererScene2D::set_model(m3f3 model_mat){
    glUseProgram(program);
    glUniformMatrix3fv(glGetUniformLocation(program, "model_mat"), 1, GL_TRUE, (float*) &model_mat);
}


void RendererScene2D::render_point(RenderContextQuadS2D context){

    this->activate();

    glPointSize(10);
    glLineWidth(3);

    glBindTexture(GL_TEXTURE_2D, context.texture);
    
    glBindVertexArray(context.VAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    // glDrawArrays(GL_LINES, 0, 6);
    // glDrawArrays(GL_POINTS, 0, 6);
}

void RendererScene2D::render_line(RenderContextQuadS2D context){
    this->activate();

    glPointSize(10);
    glLineWidth(3);

    glBindTexture(GL_TEXTURE_2D, context.texture);
    
    glBindVertexArray(context.VAO);

    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawArrays(GL_LINES, 0, 2);
    // glDrawArrays(GL_POINTS, 0, 6);
}




void RendererScene2D::render_quad(scene::QuadS2D& quad){


    program_quad_2D.set_model_texture(quad.get_model_matrix(), quad.get_texture_id());
    program_quad_2D.draw();


    return;


}



void RendererScene2D::render_multisample_texture(RenderContextQuadS2D context){

    glUseProgram(program);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, context.texture);

    glBindVertexArray(context.VAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}




void RendererScene2D::init_frame_wire_quad_context_t(){

    std::array<VertexQuad2D, 8> verts;
    f2 frame_color_coords = opengl::texture_get_color_coordinate(opengl::TextureColors::Green);
    verts = generate_quad_line_frame_verts_0101(frame_color_coords);

    render_context_frame.texture = opengl::texture_get_id(opengl::Textures::Colors);

	glGenVertexArrays(1, &render_context_frame.VAO);
	glGenBuffers(1, &render_context_frame.VBO);

	glBindVertexArray(render_context_frame.VAO);


    size_t buffer_size = verts.size() *sizeof(VertexQuad2D);
	glBindBuffer(GL_ARRAY_BUFFER, render_context_frame.VBO);
    glBufferData(GL_ARRAY_BUFFER, buffer_size, verts.data(), GL_STATIC_DRAW);
    

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexQuad2D), (void *)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexQuad2D), (void *)(sizeof(f3)));
	glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void RendererScene2D::render_frame(m3f3 M_m_s, bool selected, int line_width){
    this->activate();

    glPointSize(10);

    glLineWidth(line_width);

    set_model(M_m_s);

    

    if(selected)
    {
        render_context_frame.texture = opengl::texture_get_id(opengl::Textures::Colors);
    }
    else
    {
        opengl::set_texture_checker_2x2(render_context_frame.texture);
    }
    glBindTexture(GL_TEXTURE_2D, render_context_frame.texture);

    glBindVertexArray(render_context_frame.VAO);

    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawArrays(GL_LINES, 0, 8);
    // glDrawArrays(GL_POINTS, 0, 6);
}

std::array<VertexQuad2D,8> RendererScene2D::generate_quad_line_frame_verts_0101(f2 texture_coord){

    std::array<VertexQuad2D, 8> verts;

    f2 center = {0.5f, 0.5f};
    float scale = 1;

    VertexQuad2D v0;  // Lower left
    v0.pos.x = center.x - 0.5*scale;
    v0.pos.y = center.y - 0.5f*scale;
    v0.pos.z = 0.0f;
    v0.tex = texture_coord;

    VertexQuad2D v1;  // Lower right
    v1.pos.x = center.x + 0.5f*scale;
    v1.pos.y = center.y - 0.5f*scale;
    v1.pos.z = 0.0f;
    v1.tex = texture_coord;

    VertexQuad2D v2;  // Upper right
    v2.pos.x = center.x + 0.5f*scale;
    v2.pos.y = center.y + 0.5f*scale;
    v2.pos.z = 0.0f;
    v2.tex = texture_coord;

    VertexQuad2D v3;  // Upper left
    v3.pos.x = center.x - 0.5f*scale;
    v3.pos.y = center.y + 0.5f*scale;
    v3.pos.z = 0.0f;
    v3.tex = texture_coord;

    

    verts[0] = v0;
    verts[1] = v1;

    verts[2] = v1;
    verts[3] = v2;
    
    verts[4] = v2;
    verts[5] = v3;

    verts[6] = v3;
    verts[7] = v0;

    return verts;
}



}
