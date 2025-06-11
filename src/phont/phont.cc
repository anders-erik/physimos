#include <iostream>
#include <array>

#include <glad/glad.h>

#include "math/vecmat.hh"

#include "opengl/program.hh"
#include "opengl/texture.hh"

#include "phont.hh"


namespace phont {


Glyph::Glyph(char ch, i2 _size)
    :   text_framebuff { opengl::TextureFB(_size) },
        text_framebuff_multi { opengl::TextureFrameBufferMultisample(_size, 8) }
{
    set_char(ch);
    set_texture_size(_size);
}

void Glyph::set_char(char ch){
    char_value = ch;
    
    if(char_value == 'F'){
        get_mesh_F(mesh_glyph);
    }
    else if(char_value == 'A'){
        get_mesh_A(mesh_glyph);
    }
}
void Glyph::set_texture_size(i2 _size){
    
    
}
opengl::Texture Glyph::get_texture(){
    return text_framebuff.texture;
}

void Glyph::generate(){

    opengl::gpu_use_program(opengl::ProgramName::ndc_black);

    render_context.vertex_count = mesh_glyph.faces.size() * 3;

	glGenVertexArrays(1, &render_context.VAO);
	glGenBuffers(1, &render_context.VBO);
    glGenBuffers(1, &render_context.EBO);

	glBindVertexArray(render_context.VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_context.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_glyph.faces.size() * sizeof(i3), mesh_glyph.faces.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, render_context.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(f3) * mesh_glyph.verts.size(), mesh_glyph.verts.data(), GL_STATIC_DRAW);
    
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(f3), (void *)0);
	glEnableVertexAttribArray(0);


    text_framebuff.set_clear_color( {0.8f, 0.8f, 0.8f, 1.0f} );

    text_framebuff.framebuffer_bind();
    text_framebuff.clear();

    glBindVertexArray(render_context.VAO);
    glDrawElements(GL_TRIANGLES, render_context.vertex_count, GL_UNSIGNED_INT, 0);


    text_framebuff.framebuffer_unbind(800, 600);
}

void Glyph::generate_multisample(){

    opengl::gpu_use_program(opengl::ProgramName::ndc_black);

    render_context.vertex_count = mesh_glyph.faces.size() * 3;

	glGenVertexArrays(1, &render_context.VAO);
	glGenBuffers(1, &render_context.VBO);
    glGenBuffers(1, &render_context.EBO);

	glBindVertexArray(render_context.VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_context.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_glyph.faces.size() * sizeof(i3), mesh_glyph.faces.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, render_context.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(f3) * mesh_glyph.verts.size(), mesh_glyph.verts.data(), GL_STATIC_DRAW);
    
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(f3), (void *)0);
	glEnableVertexAttribArray(0);


    text_framebuff_multi.clear_color = {0.8f, 0.8f, 0.8f, 1.0f};
    text_framebuff_multi.multisample_fbo_bind();
    text_framebuff_multi.multisample_fbo_clear();

    glBindVertexArray(render_context.VAO);
    glDrawElements(GL_TRIANGLES, render_context.vertex_count, GL_UNSIGNED_INT, 0);

    text_framebuff_multi.blit();
    text_framebuff_multi.multisample_fbo_unbind();
}



void get_mesh_F(GlyphMesh& mesh){

    // float size = 0.9f;
    
    // VERTICAL BAR
    float l_vert_bar    = -0.8;
    float r_vert_bar    = -0.4;
    float t_vert_bar    =  0.9;
    float b_vert_bar    = -0.6;
    f3 v0 (l_vert_bar, b_vert_bar,  0.0f); // Lower left
    f3 v1 (r_vert_bar, b_vert_bar,  0.0f); // Lower right
    f3 v2 (r_vert_bar, t_vert_bar,  0.0f); // Upper right
    f3 v3 (l_vert_bar, t_vert_bar,  0.0f); // Upper left

    mesh.verts.push_back(v0);
    mesh.verts.push_back(v1);
    mesh.verts.push_back(v2);
    mesh.verts.push_back(v3);
    
    i3 _f0 (0, 1, 2);
    i3 _f1 (0, 2, 3);
    mesh.faces.push_back(_f0);
    mesh.faces.push_back(_f1);



    // TOP HORIZONAL BAR
    float l_top_hori_bar    = -0.4;
    float r_top_hori_bar    =  0.9;
    float t_top_hori_bar    =  0.9;
    float b_top_hori_bar    =  0.6;
    f3 v4 (l_top_hori_bar, b_top_hori_bar,  0.0f); // Lower left
    f3 v5 (r_top_hori_bar, b_top_hori_bar,  0.0f); // Lower right
    f3 v6 (r_top_hori_bar, t_top_hori_bar,  0.0f); // Upper right
    f3 v7 (l_top_hori_bar, t_top_hori_bar,  0.0f); // Upper left

    mesh.verts.push_back(v4);
    mesh.verts.push_back(v5);
    mesh.verts.push_back(v6);
    mesh.verts.push_back(v7);

    i3 _f2 (4, 5, 6);
    i3 _f3 (4, 6, 7);
    mesh.faces.push_back(_f2);
    mesh.faces.push_back(_f3);



    // BOTTOM HORIZONAL BAR
    float l_bottom_hori_bar    = -0.4;
    float r_bottom_hori_bar    =  0.6;
    float t_bottom_hori_bar    =  0.3;
    float b_bottom_hori_bar    =  0.0;
    f3 v8  (l_bottom_hori_bar, b_bottom_hori_bar,  0.0f); // Lower left
    f3 v9  (r_bottom_hori_bar, b_bottom_hori_bar,  0.0f); // Lower right
    f3 v10 (r_bottom_hori_bar, t_bottom_hori_bar,  0.0f); // Upper right
    f3 v11 (l_bottom_hori_bar, t_bottom_hori_bar,  0.0f); // Upper left

    mesh.verts.push_back(v8 );
    mesh.verts.push_back(v9 );
    mesh.verts.push_back(v10);
    mesh.verts.push_back(v11);

    i3 f4 (8, 9, 10);
    i3 f5 (8, 10, 11);
    mesh.faces.push_back(f4);
    mesh.faces.push_back(f5);
    


    
}

void get_mesh_A(GlyphMesh& mesh){


    // VERTICAL BAR LEFT
    float bottom_left_x = -0.8f;
    float bottom_y = -0.7f;
    float width = 0.4f;
    float height = 1.6f;
    float dx_slope = 0.6f;

    f3 v0 (bottom_left_x                    , bottom_y          ,  0.0f); // Lower left
    f3 v1 (bottom_left_x+width              , bottom_y          ,  0.0f); // Lower right
    f3 v2 (bottom_left_x + dx_slope + width , bottom_y+height   ,  0.0f); // Upper right
    f3 v3 (bottom_left_x + dx_slope         , bottom_y+height   ,  0.0f); // Upper left

    mesh.verts.push_back(v0);
    mesh.verts.push_back(v1);
    mesh.verts.push_back(v2);
    mesh.verts.push_back(v3);
    
    i3 _f0 (0, 1, 2);
    i3 _f1 (0, 2, 3);
    mesh.faces.push_back(_f0);
    mesh.faces.push_back(_f1);



    // VERTICAL BAR RIGHT
    bottom_left_x = 0.5f;

    f3 v4 (bottom_left_x                    , bottom_y          ,  0.0f); // Lower left
    f3 v5 (bottom_left_x + width            , bottom_y          ,  0.0f); // Lower right
    f3 v6 (bottom_left_x - dx_slope + width , bottom_y+height   ,  0.0f); // Upper right
    f3 v7 (bottom_left_x - dx_slope         , bottom_y+height   ,  0.0f); // Upper left

    mesh.verts.push_back(v4);
    mesh.verts.push_back(v5);
    mesh.verts.push_back(v6);
    mesh.verts.push_back(v7);

    i3 _f2 (4, 5, 6);
    i3 _f3 (4, 6, 7);
    mesh.faces.push_back(_f2);
    mesh.faces.push_back(_f3);



    // BOTTOM HORIZONAL BAR
    float bar_x = -0.4f;
    float bar_y = -0.3f;
    float bar_w =  0.8f;
    float bar_h =  0.25f;
    f3 v8  (bar_x, bar_y,  0.0f); // Lower left
    f3 v9  (bar_x+bar_w, bar_y,  0.0f); // Lower right
    f3 v10 (bar_x+bar_w, bar_y+bar_h,  0.0f); // Upper right
    f3 v11 (bar_x, bar_y+bar_h,  0.0f); // Upper left

    mesh.verts.push_back(v8 );
    mesh.verts.push_back(v9 );
    mesh.verts.push_back(v10);
    mesh.verts.push_back(v11);

    i3 f4 (8, 9, 10);
    i3 f5 (8, 10, 11);
    mesh.faces.push_back(f4);
    mesh.faces.push_back(f5);
    


    
}


}
