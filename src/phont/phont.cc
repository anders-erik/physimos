#include <iostream>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "math/vecmat.hh"

#include "opengl/program.hh"

#include "phont.hh"

namespace phont {

QuadRenderer::QuadRenderer(){
    program = opengl::build_program_vert_frag(opengl::Programs::phont_texture);
}

void QuadRenderContext::delete_contents(){
    glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO);
}

Quad::Quad(float window_width, float window_height){

    // WINDOW_WIDTH = 

    float width_pixels  = 20.0f;
    float height_pixels = 30.0f;
    
	float x_center_ndc = 0.0f;
    float y_center_ndc = 0.0f;


    float width_ndc_per_pixel = (2.0f / window_width);
    float height_ndc_per_pixel = (2.0f / window_height);

    float width_ndc = width_pixels * width_ndc_per_pixel;
    float height_ndc = height_pixels * height_ndc_per_pixel;

    // Lower left
    verts[0].pos.x = x_center_ndc - width_ndc / 2;
    verts[0].pos.y = y_center_ndc - height_ndc / 2;
    verts[0].pos.z = 0.0f;
    verts[0].tex.x = 0.0f;
    verts[0].tex.y = 0.0f;

    // Lower right
    verts[1].pos.x = x_center_ndc + width_ndc / 2;
    verts[1].pos.y = y_center_ndc - height_ndc / 2;
    verts[1].pos.z = 0.0f;
    verts[1].tex.x = 1.0f;
    verts[1].tex.y = 0.0f;

    // Upper right
    verts[2].pos.x = x_center_ndc + width_ndc / 2;
    verts[2].pos.y = y_center_ndc + height_ndc / 2;
    verts[2].pos.z = 0.0f;
    verts[2].tex.x = 1.0f;
    verts[2].tex.y = 1.0f;

    // Upper left
    verts[3].pos.x = x_center_ndc - width_ndc / 2;
    verts[3].pos.y = y_center_ndc + height_ndc / 2;
    verts[3].pos.z = 0.0f;
    verts[3].tex.x = 0.0f;
    verts[3].tex.y = 1.0f;
    

    faces[0] = {0, 1, 2};
    faces[1] = {0, 2, 3};

}

void QuadRenderer::create_context(phont::Quad& quad){

    // unsigned int render_context.VAO, VBO, EBO;

    quad.render_context.element_count = quad.faces.size() * 3;

	glGenVertexArrays(1, &quad.render_context.VAO);
	glGenBuffers(1, &quad.render_context.VBO);
    glGenBuffers(1, &quad.render_context.EBO);

	glBindVertexArray(quad.render_context.VAO);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.render_context.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, quad.faces.size() * sizeof(i3), quad.faces.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, quad.render_context.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad.verts), quad.verts.data(), GL_STATIC_DRAW);
    

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(phont::Vertex), (void *)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(phont::Vertex), (void *)(sizeof(f3)));
	glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
}

void QuadRenderer::render(QuadRenderContext context){

    glUseProgram(program);

    // glBindTexture(GL_TEXTURE_2D, texture_checker);
    // glBindTexture(GL_TEXTURE_2D, texture_F);
    glBindTexture(GL_TEXTURE_2D, context.texture);

    glBindVertexArray(context.VAO); 
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, context.element_count , GL_UNSIGNED_INT, 0);

}

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

unsigned int get_texture_F(){

    int texture_width   = 200;
    int texture_height  = 300;

    // MESH

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


    // FRAMEBUFFER 

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);    

    // generate texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);


    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    else 
        std::cout << "FRAMEBUFFER OK!" << std::endl;




    // RENDER


    unsigned int program_char = opengl::build_program_vert_frag(opengl::Programs::phont_char);
    glUseProgram(program_char);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT); 



    glViewport(0,0, texture_width, texture_height);

    glBindVertexArray(VAO); 
    glDrawElements(GL_TRIANGLES, triangle_count, GL_UNSIGNED_INT, 0);

    glViewport(0,0, 800, 600);




    // FOUR RED PIXELS
    float pixels_4_red[16] = {  1.0f, 0.0f, 0.0f, 1.0f, 
                                1.0f, 0.0f, 0.0f, 1.0f, 
                                1.0f, 0.0f, 0.0f, 1.0f, 
                                1.0f, 0.0f, 0.0f, 1.0f
    };
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 2, 2,  GL_RGBA,  GL_FLOAT, pixels_4_red);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    return textureColorbuffer;

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
