#include <iostream>

#include "shapeS2D.hh"

#include "math/vecmat.hh"

#include "math/transform.hh"

namespace scene {

using namespace opengl;

ShapeS2D::ShapeS2D() : shape {Shape::create(shape_t::triangle)}{

    
    
}

ShapeS2D::ShapeS2D(Shape& shape) 
    : shape {shape} 
{
    
    std::vector<f2>& shape_points = shape.get_points();
    
    text_coord = opengl::texture_get_color_coordinate(texture_color);

    // Create vertices
    if(shape.is(shape_t::point))
        create_point_vertices(shape_points[0]);
    else if(shape.is(shape_t::line))
        create_line_vertices(shape_points[0], shape_points[1]);
    else
        create_fan_vertices(shape_points);

}

void ShapeS2D::create_point_vertices(f2 point){

    std::array<VertexQuad2D, 6> quad_verts = generate_quad_verts_c0(point, 0.1, this->text_coord);

    verts_render.push_back(quad_verts[0]);
    verts_render.push_back(quad_verts[1]);
    verts_render.push_back(quad_verts[2]);
    verts_render.push_back(quad_verts[3]);
    verts_render.push_back(quad_verts[4]);
    verts_render.push_back(quad_verts[5]);

    // Vertex2DT v_lower_left = {{point.x-0.5, point.y-0.5, 0.0f}, this->text_coord};
    // verts_render.push_back(v_lower_left);
    // Vertex2DT v_lower_right = {{point.x+0.5, point.y-0.5, 0.0f}, this->text_coord};
    // verts_render.push_back(v_lower_right);
    
    // Vertex2DT v_upper_left = {{point.x-0.5, point.y+0.5, 0.0f}, this->text_coord};
    // verts_render.push_back(v_upper_left);
    // Vertex2DT v_upper_right = {{point.x+0.5, point.y+0.5, 0.0f}, this->text_coord};
    // verts_render.push_back(v_upper_right);
    
}
void ShapeS2D::create_line_vertices(f2 p1, f2 p2){

    VertexQuad2D v1 = {{p1.x, p1.y, 0.0f}, this->text_coord};
    VertexQuad2D v2 = {{p2.x, p2.y, 0.0f}, this->text_coord};
    verts_render.push_back(v1);
    verts_render.push_back(v2);
    
}
void ShapeS2D::create_fan_vertices(std::vector<f2>& points){

    VertexQuad2D v0 = {{points[0].x, points[0].y, 0.0f}, {0.0f, 0.0f}};

    // create triangles with v0 as the connecting vertex
    for(size_t i = 1; i < points.size(); i++){

        VertexQuad2D v_i =  {{points[i].x, points[i].y, 0.0f}, {0.0f, 0.0f}};
        VertexQuad2D v_ip1 =  {{points[i+1].x, points[i+1].y, 0.0f}, {0.0f, 0.0f}};
        
        verts_render.push_back(v0);
        verts_render.push_back(v_i);
        verts_render.push_back(v_ip1);
    }

}

bool ShapeS2D::is_point(){
    return shape.is(shape_t::point) ? true : false;
}
bool ShapeS2D::is_line(){
    return shape.is(shape_t::line) ? true : false;
}


void ShapeS2D::set_texture(unsigned int texture_id){
    render_context.texture = texture_id;
}

m3f3 ShapeS2D::get_matrix(){
    return M_m_s;
}

void ShapeS2D::set_pos(f2 pos){
    M_m_s.x.z = pos.x;
    M_m_s.y.z = pos.y;
}

void ShapeS2D::set_size(f2 size){
    M_m_s.x.x = size.x;
    M_m_s.y.y = size.y;
}



std::array<VertexQuad2D, 6> ShapeS2D::generate_quad_verts_c0(f2 center, float scale, f2 texture_coord){

    std::array<VertexQuad2D, 6> verts;

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

std::array<VertexQuad2D,8> ShapeS2D::generate_quad_line_frame_verts_0505(f2 texture_coord){

    std::array<VertexQuad2D, 8> verts;

    f2 center = {0.0f, 0.0f};
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
