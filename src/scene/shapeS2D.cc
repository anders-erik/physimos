#include <iostream>

#include "shapeS2D.hh"

#include "math/vecmat.hh"

#include "math/transform.hh"

namespace scene {

using namespace opengl;

ShapeS2D::ShapeS2D() : shape {Shape::create(shape_t::triangle)}{


    verts_6 = generate_quad();


    // float width_pixels  = 20.0f;
    // float height_pixels = 30.0f;

    // set_dims(window_width, window_height, width_pixels, height_pixels);
    
    
}

ShapeS2D::ShapeS2D(Shape& shape) : shape {shape} {
    
    Vertex2DT v_tmp;
    
    std::vector<f2>& shape_points = shape.get_points();
    
    text_coord = opengl::texture_get_color_coordinate(texture_color);

    // Create vertices
    if(shape.is(shape_t::point))
        create_point(shape_points[0]);
    else if(shape.is(shape_t::line))
        create_line(shape_points[0], shape_points[1]);
    else
        create_fan(shape_points);

}

void ShapeS2D::create_point(f2 point){
    Vertex2DT v_tmp = {{point.x, point.y, 0.0f}, this->text_coord};
    verts.push_back(v_tmp);
}
void ShapeS2D::create_line(f2 p1, f2 p2){
    Vertex2DT v1 = {{p1.x, p1.y, 0.0f}, {0.0f, 0.0f}};
    verts.push_back(v1);
    Vertex2DT v2 = {{p2.x, p2.y, 0.0f}, {0.0f, 0.0f}};
    verts.push_back(v2);
}
void ShapeS2D::create_fan(std::vector<f2>& points){

    Vertex2DT v0 = {{points[0].x, points[0].y, 0.0f}, {0.0f, 0.0f}};

    // create triangles with v0 as the connecting vertex
    for(size_t i = 1; i < points.size(); i++){

        Vertex2DT v_i =  {{points[i].x, points[i].y, 0.0f}, {0.0f, 0.0f}};
        Vertex2DT v_ip1 =  {{points[i+1].x, points[i+1].y, 0.0f}, {0.0f, 0.0f}};
        
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

std::array<Vertex2DT, 6> ShapeS2D::generate_quad(){
    std::array<Vertex2DT, 6> verts;

    Vertex2DT v0;  // Lower left
    v0.pos.x = 0.0f;
    v0.pos.y = 0.0f;
    v0.pos.z = 0.0f;
    v0.tex.x = 0.0f;
    v0.tex.y = 0.0f;

    Vertex2DT v1;  // Lower right
    v1.pos.x = 1.0f;
    v1.pos.y = 0.0f;
    v1.pos.z = 0.0f;
    v1.tex.x = 1.0f;
    v1.tex.y = 0.0f;

    Vertex2DT v2;  // Upper right
    v2.pos.x = 1.0f;
    v2.pos.y = 1.0f;
    v2.pos.z = 0.0f;
    v2.tex.x = 1.0f;
    v2.tex.y = 1.0f;

    Vertex2DT v3;  // Upper left
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






QuadS2D::QuadS2D(){

    verts_6 = QuadS2D::generate_quad();

}

m3f3 QuadS2D::get_matrix(){
    return M_m_s;
}

void QuadS2D::set_dims(f2 pos, f2 size){
    M_m_s.x.z = pos.x;
    M_m_s.y.z = pos.y;

    M_m_s.x.x = size.x;
    M_m_s.y.y = size.y;
}

void QuadS2D::set_texture(opengl::Textures texture){
    render_context.texture = opengl::texture_get_id(texture);
}

void QuadS2D::set_texture_id(unsigned int id){
    render_context.texture = id;
}
opengl::ShapeS2DRenderContext& QuadS2D::get_rendering_context(){
    return render_context;
}

std::array<opengl::Vertex2DT,6>& QuadS2D::get_verts(){
    return verts_6;
}

std::array<Vertex2DT, 6> QuadS2D::generate_quad(){
    std::array<Vertex2DT, 6> verts;

    Vertex2DT v0;  // Lower left
    v0.pos.x = 0.0f;
    v0.pos.y = 0.0f;
    v0.pos.z = 0.0f;
    v0.tex.x = 0.0f;
    v0.tex.y = 0.0f;

    Vertex2DT v1;  // Lower right
    v1.pos.x = 1.0f;
    v1.pos.y = 0.0f;
    v1.pos.z = 0.0f;
    v1.tex.x = 1.0f;
    v1.tex.y = 0.0f;

    Vertex2DT v2;  // Upper right
    v2.pos.x = 1.0f;
    v2.pos.y = 1.0f;
    v2.pos.z = 0.0f;
    v2.tex.x = 1.0f;
    v2.tex.y = 1.0f;

    Vertex2DT v3;  // Upper left
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

}
