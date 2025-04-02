#include <iostream>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "math/vecmat.hh"

#include "opengl/program.hh"
#include "opengl/texture.hh"

#include "phont.hh"


namespace phont {


void get_mesh_F(GlyphMesh& mesh){

    // float size = 0.9f;
    
    // VERTICAL BAR
    float l_vert_bar    = -0.9;
    float r_vert_bar    = -0.5;
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
    float l_top_hori_bar    = -0.5;
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
    float l_bottom_hori_bar    = -0.5;
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

opengl::TextureFrameBuffer get_texture_F(){

    int texture_width   = 200;
    int texture_height  = 300;


    // PROGRAM
    unsigned int program_char = opengl::build_program_vert_frag(opengl::Programs::ndc_black);
    glUseProgram(program_char);

    // RENDERER CONTEXT

    GlyphMesh mesh_F;
    get_mesh_F(mesh_F);

    unsigned int VAO, VBO, EBO;
    int triangle_count;

    triangle_count = mesh_F.faces.size() * 3;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_F.faces.size() * sizeof(i3), mesh_F.faces.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(f3) * mesh_F.verts.size(), mesh_F.verts.data(), GL_STATIC_DRAW);
    
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(f3), (void *)0);
	glEnableVertexAttribArray(0);

    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(f3), (void *)(sizeof(f3)));
	// glEnableVertexAttribArray(1);



    // TEXTURE FRAMEBUFFER 
    opengl::TextureFrameBuffer texture_framebuffer (texture_width, texture_height);
    texture_framebuffer.set_clear_color( {0.8f, 0.8f, 0.8f, 1.0f} );



    
    texture_framebuffer.framebuffer_bind();
    texture_framebuffer.clear();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, triangle_count, GL_UNSIGNED_INT, 0);


    texture_framebuffer.framebuffer_unbind(800, 600);
    



    // FOUR RED PIXELS
    float pixels_4_red[16] = {  1.0f, 0.0f, 0.0f, 1.0f, 
                                1.0f, 0.0f, 0.0f, 1.0f, 
                                1.0f, 0.0f, 0.0f, 1.0f, 
                                1.0f, 0.0f, 0.0f, 1.0f
    };
    glBindTexture(GL_TEXTURE_2D, texture_framebuffer.texture_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 2, 2,  GL_RGBA,  GL_FLOAT, pixels_4_red);


    


    return texture_framebuffer;

}

void set_texture_checker(unsigned int& texture_id){

    // unsigned char bitmap[16] = {255, 255, 255, 255,
    //                             0,   0,   0,   255,
    //                             0,   0,   0,   255,
    //                             255, 255, 255, 255,
    // };

    float bitmap[16] = {    1.0f, 1.0f, 1.0f, 1.0f,
                            0.0f, 0.0f, 0.0f, 1.0f,
                            0.0f, 0.0f, 0.0f, 1.0f,
                            1.0f, 1.0f, 1.0f, 1.0f,
    };

    
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Not necessary
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT, bitmap);
    glGenerateMipmap(GL_TEXTURE_2D);


}


}
