#include <iostream>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "math/math.hh"

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

    float width_pixels  = 320.0f;
    float height_pixels = 320.0f;
    
	float x_center_ndc = 0.0f;
    float y_center_ndc = 0.0f;


    float width_ndc_per_pixel = (1.0f / window_width);
    float height_ndc_per_pixel = (1.0f / window_height);

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

void QuadRenderer::render_context(QuadRenderContext context){

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

unsigned int get_texture_F(){

    // FRAMEBUFFER
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);


    // TEXTURE
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 32, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0); 


    // RENDER BUFFER OBJECT
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 32, 32);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);


    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    else 
        std::cout << "FRAMEBUFFER OK!" << std::endl;

    

    // RENDER F
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // SETUP
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    // glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
    // glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT); 
    // glDrawArrays(GL_TRIANGLES, 0, 0);



    // VERTEX

    // QUAD

    float width_glyph_px = 32.0f;
    float height_glyph_px = 32.0f;

    std::array<Vertex, 4> quad;
    std::array<i3, 2> faces;

    float width_pixels  = 32.0f;
    float height_pixels = 32.0f;
    
	float x_center_ndc = 0.0f;
    float y_center_ndc = 0.0f;


    float width_ndc_per_pixel = (1.0f / width_glyph_px);
    float height_ndc_per_pixel = (1.0f / height_glyph_px);

    float width_ndc = width_pixels * width_ndc_per_pixel;
    float height_ndc = height_pixels * height_ndc_per_pixel;


    // Lower left
    quad[0].pos.x = x_center_ndc - width_ndc / 2;
    quad[0].pos.y = y_center_ndc - height_ndc / 2;
    quad[0].pos.z = 0.0f;
    quad[0].tex.x = 0.0f;
    quad[0].tex.y = 0.0f;

    // Lower right
    quad[1].pos.x = x_center_ndc + width_ndc / 2;
    quad[1].pos.y = y_center_ndc - height_ndc / 2;
    quad[1].pos.z = 0.0f;
    quad[1].tex.x = 1.0f;
    quad[1].tex.y = 0.0f;

    // Upper right
    quad[2].pos.x = x_center_ndc + width_ndc / 2;
    quad[2].pos.y = y_center_ndc + height_ndc / 2;
    quad[2].pos.z = 0.0f;
    quad[2].tex.x = 1.0f;
    quad[2].tex.y = 1.0f;

    // Upper left
    quad[3].pos.x = x_center_ndc - width_ndc / 2;
    quad[3].pos.y = y_center_ndc + height_ndc / 2;
    quad[3].pos.z = 0.0f;
    quad[3].tex.x = 0.0f;
    quad[3].tex.y = 1.0f;
    

    faces[0] = {0, 1, 2};
    faces[1] = {0, 2, 3};



	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(i3), faces.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad.data(), GL_STATIC_DRAW);
    

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(f3)));
	glEnableVertexAttribArray(1);


    // TEXTURE
    unsigned int texture_checker = 0;
    phont::set_texture_checker(texture_checker);



    glBindVertexArray(VAO); 
    glBindTexture(GL_TEXTURE_2D, texture_checker);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, faces.size() * 3 , GL_UNSIGNED_INT, 0);



    glBindVertexArray(0); 



    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // RENDER F END


    // texture_id = textureColorbuffer;

    

    return textureColorbuffer;

    // glBindFramebuffer(GL_FRAMEBUFFER, 0);  

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
