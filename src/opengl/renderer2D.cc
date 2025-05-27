
#include "math/vecmat.hh"
#include "renderer2D.hh"
#include "opengl/texture.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

namespace opengl {






void ShapeS2DRenderContext::delete_contents(){
    glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO);
}


Scene2DRenderer::Scene2DRenderer(){
    // program = opengl::build_program_vert_frag(opengl::Programs::phont_texture);
    program = opengl::build_program_vert_frag(shader);
}

void Scene2DRenderer::create_shape_context_t(ShapeS2DRenderContext& render_context, std::vector<Vertex2DT> verts){

    render_context.texture = opengl::texture_get_id(opengl::Textures::Colors);

	glGenVertexArrays(1, &render_context.VAO);
	glGenBuffers(1, &render_context.VBO);

	glBindVertexArray(render_context.VAO);


    size_t buffer_size = verts.size() *sizeof(Vertex2DT);
	glBindBuffer(GL_ARRAY_BUFFER, render_context.VBO);
    glBufferData(GL_ARRAY_BUFFER, buffer_size, verts.data(), GL_STATIC_DRAW);
    

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DT), (void *)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DT), (void *)(sizeof(f3)));
	glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Scene2DRenderer::create_context_quad_t(ShapeS2DRenderContext& render_context, std::array<Vertex2DT, 6> verts){

    // unsigned int render_context.VAO, VBO, EBO;

    // quad.render_context.element_count = quad.faces.size() * 3;

	glGenVertexArrays(1, &render_context.VAO);
	glGenBuffers(1, &render_context.VBO);
    // glGenBuffers(1, &quad.render_context.EBO);

	glBindVertexArray(render_context.VAO);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.render_context.EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, quad.faces.size() * sizeof(i3), quad.faces.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, render_context.VBO);
    // Sizeof OK because its an array
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts.data(), GL_STATIC_DRAW);
    

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DT), (void *)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DT), (void *)(sizeof(f3)));
	glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
}

void Scene2DRenderer::activate(){
    glUseProgram(program);
}
void Scene2DRenderer::set_camera(m3f3 camera){
    glUniformMatrix3fv(glGetUniformLocation(program, "camera2D_mat"), 1, GL_TRUE, (float*) &camera);
}
void Scene2DRenderer::set_model(m3f3 model_mat){
    glUniformMatrix3fv(glGetUniformLocation(program, "model_mat"), 1, GL_TRUE, (float*) &model_mat);
}


void Scene2DRenderer::render_point(ShapeS2DRenderContext context){

    this->activate();

    glPointSize(10);

    glBindTexture(GL_TEXTURE_2D, context.texture);
    
    glBindVertexArray(context.VAO);

    glDrawArrays(GL_POINTS, 0, 1);
}




void Scene2DRenderer::render_quad(ShapeS2DRenderContext context){

    glUseProgram(program);

    // glBindTexture(GL_TEXTURE_2D, texture_checker);
    // glBindTexture(GL_TEXTURE_2D, texture_F);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 3);
    glBindTexture(GL_TEXTURE_2D, context.texture);

    // Visualize the actual rendered pixels of glyph
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    

    glBindVertexArray(context.VAO); 
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawElements(GL_TRIANGLES, context.element_count , GL_UNSIGNED_INT, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}



void Scene2DRenderer::render_multisample_texture(ShapeS2DRenderContext context){

    glUseProgram(program);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, context.texture);

    glBindVertexArray(context.VAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

}
