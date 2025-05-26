
#include "math/vecmat.hh"
#include "renderer_quad_2d.hh"
#include "opengl/texture.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

namespace opengl {



ShapeS2D::ShapeS2D() : shape {Shape::create(shape_t::triangle)}{


    verts_6 = generate_quad();


    // float width_pixels  = 20.0f;
    // float height_pixels = 30.0f;

    // set_dims(window_width, window_height, width_pixels, height_pixels);
    
    
}

ShapeS2D::ShapeS2D(Shape& shape) : shape {shape} {
    
    VertexShape2D v_tmp;
    
    std::vector<f2>& shape_points = shape.get_points();
    
    text_coord = opengl::texture_get_color_coordinate(opengl::TextureColors::Red);

    // Create vertices
    if(shape.is(shape_t::point))
        create_point(shape_points[0]);
    else if(shape.is(shape_t::line))
        create_line(shape_points[0], shape_points[1]);
    else
        create_fan(shape_points);

}

void ShapeS2D::create_point(f2 point){
    VertexShape2D v_tmp = {{point.x, point.y, 0.0f}, this->text_coord};
    verts.push_back(v_tmp);
}
void ShapeS2D::create_line(f2 p1, f2 p2){
    VertexShape2D v1 = {{p1.x, p1.y, 0.0f}, {0.0f, 0.0f}};
    verts.push_back(v1);
    VertexShape2D v2 = {{p2.x, p2.y, 0.0f}, {0.0f, 0.0f}};
    verts.push_back(v2);
}
void ShapeS2D::create_fan(std::vector<f2>& points){

    VertexShape2D v0 = {{points[0].x, points[0].y, 0.0f}, {0.0f, 0.0f}};

    // create triangles with v0 as the connecting vertex
    for(size_t i = 1; i < points.size(); i++){

        VertexShape2D v_i =  {{points[i].x, points[i].y, 0.0f}, {0.0f, 0.0f}};
        VertexShape2D v_ip1 =  {{points[i+1].x, points[i+1].y, 0.0f}, {0.0f, 0.0f}};
        
        verts.push_back(v0);
        verts.push_back(v_i);
        verts.push_back(v_ip1);
    }

}

bool ShapeS2D::is_point(){
    return shape.is(shape_t::point) ? true : false;
}
bool ShapeS2D::is_line(){
    return shape.is(shape_t::line) ? true : false;
}


void ShapeS2D::set_texture(f2 text_coord){


}

m3f3 ShapeS2D::get_matrix(){
    return M_m_s;
}


void ShapeS2D::set_dims(float window_width, float window_height, float width_pixels, float height_pixels){



    float x_center_ndc = 0.0f;
    float y_center_ndc = 0.0f;

    float width_ndc_per_pixel = (2.0f / window_width);
    float height_ndc_per_pixel = (2.0f / window_height);

    float width_ndc = width_pixels * width_ndc_per_pixel;
    float height_ndc = height_pixels * height_ndc_per_pixel;


    // Lower left
    verts_6[0].pos.x = x_center_ndc - width_ndc / 2;
    verts_6[0].pos.y = y_center_ndc - height_ndc / 2;
    // Lower right
    verts_6[1].pos.x = x_center_ndc + width_ndc / 2;
    verts_6[1].pos.y = y_center_ndc - height_ndc / 2;
    // Upper right
    verts_6[2].pos.x = x_center_ndc + width_ndc / 2;
    verts_6[2].pos.y = y_center_ndc + height_ndc / 2;

    // Lower left
    verts_6[3].pos.x = x_center_ndc - width_ndc / 2;
    verts_6[3].pos.y = y_center_ndc - height_ndc / 2;
    // Upper right
    verts_6[4].pos.x = x_center_ndc + width_ndc / 2;
    verts_6[4].pos.y = y_center_ndc + height_ndc / 2;
    // Upper left
    verts_6[5].pos.x = x_center_ndc - width_ndc / 2;
    verts_6[5].pos.y = y_center_ndc + height_ndc / 2;
    
}

std::array<VertexShape2D, 6> ShapeS2D::generate_quad(){
    std::array<VertexShape2D, 6> verts;

    VertexShape2D v0;  // Lower left
    v0.pos.x = 0.0f;
    v0.pos.y = 0.0f;
    v0.pos.z = 0.0f;
    v0.tex.x = 0.0f;
    v0.tex.y = 0.0f;

    VertexShape2D v1;  // Lower right
    v1.pos.x = 1.0f;
    v1.pos.y = 0.0f;
    v1.pos.z = 0.0f;
    v1.tex.x = 1.0f;
    v1.tex.y = 0.0f;

    VertexShape2D v2;  // Upper right
    v2.pos.x = 1.0f;
    v2.pos.y = 1.0f;
    v2.pos.z = 0.0f;
    v2.tex.x = 1.0f;
    v2.tex.y = 1.0f;

    VertexShape2D v3;  // Upper left
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



void ShapeS2DRenderContext::delete_contents(){
    glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO);
}


Scene2DRenderer::Scene2DRenderer(){
    // program = opengl::build_program_vert_frag(opengl::Programs::phont_texture);
    program = opengl::build_program_vert_frag(shader);
}

void Scene2DRenderer::create_context(ShapeS2D& shape){

    shape.render_context.texture = opengl::texture_get_id(opengl::Textures::Colors);

	glGenVertexArrays(1, &shape.render_context.VAO);
	glGenBuffers(1, &shape.render_context.VBO);

	glBindVertexArray(shape.render_context.VAO);


	glBindBuffer(GL_ARRAY_BUFFER, shape.render_context.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shape.verts), shape.verts.data(), GL_STATIC_DRAW);
    

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexShape2D), (void *)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexShape2D), (void *)(sizeof(f3)));
	glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Scene2DRenderer::create_context_quad(ShapeS2D& quad){

    // unsigned int render_context.VAO, VBO, EBO;

    // quad.render_context.element_count = quad.faces.size() * 3;

	glGenVertexArrays(1, &quad.render_context.VAO);
	glGenBuffers(1, &quad.render_context.VBO);
    // glGenBuffers(1, &quad.render_context.EBO);

	glBindVertexArray(quad.render_context.VAO);


    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.render_context.EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, quad.faces.size() * sizeof(i3), quad.faces.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, quad.render_context.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad.verts_6), quad.verts_6.data(), GL_STATIC_DRAW);
    

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexShape2D), (void *)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexShape2D), (void *)(sizeof(f3)));
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
