
#pragma once

#include <vector>

#include "opengl/program.hh"
#include "opengl/texture.hh"


#include "math/vecmat.hh"
#include "math/shape.hh"
#include "math/box2D.hh"


#include "rend/scene2D/renderer2D.hh"


namespace scene {


class ShapeS2D {

    void create_point_vertices(f2 point); // Create unit square around point
    void create_line_vertices(f2 p1, f2 p2);
    void create_fan_vertices(std::vector<f2>& points);

    m3f3 M_m_s;
    Shape shape;

private: 
public: // Keep this while moving objects to private above
    
    m3f3 get_matrix();

    opengl::TextureColors texture_color = opengl::TextureColors::LightGray;
    f2 text_coord; // Texture coordinate for coloring texture

    std::vector<VertexQuad2D> verts_render; // Render vertices
    std::array<VertexQuad2D, 6> verts_6;   // Old quad verts
    Transform2D transform_2d;



public:
    RenderContextQuadS2D render_context;

    ShapeS2D();
    ShapeS2D(Shape &shape);


    bool is_point();
    bool is_line();

    void set_texture(unsigned int texture_id);


    void set_pos(f2 pos);
    void set_size(f2 size);



    // Generate unit square centered at zero and scale and shift according to parameters.
    static std::array<VertexQuad2D,6> generate_quad_verts_c0(f2 center, float scale, f2 texture_coord);
    static std::array<VertexQuad2D,8> generate_quad_line_frame_verts_0505(f2 texture_coord);

};




}