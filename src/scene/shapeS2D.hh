
#pragma once

#include <vector>

#include "opengl/program.hh"
#include "opengl/renderer2D.hh"
#include "opengl/texture.hh"

#include "window/auxwin.hh"

#include "math/vecmat.hh"
#include "math/shape.hh"
#include "math/box2D.hh"

#include "scene/camera2D.hh"

#include "phont/phont.hh"



namespace scene {


struct ShapeS2D {
    m3f3 M_m_s;
    std::vector<opengl::Vertex2DT> verts_render;
    Shape shape;
    opengl::TextureColors texture_color = opengl::TextureColors::LightGray;
    f2 text_coord; // Texture coordinate for coloring texture
    opengl::ShapeS2DRenderContext render_context;

    std::array<opengl::Vertex2DT, 6> verts_6;
    Transform2D transform_2d;



    ShapeS2D();
    ShapeS2D(Shape &shape);

    void create_point_vertices(f2 point); // Create unit square around point
    void create_line_vertices(f2 p1, f2 p2);
    void create_fan_vertices(std::vector<f2>& points);

    bool is_point();
    bool is_line();

    void set_texture(f2 text_coord);

    m3f3 get_matrix();


    void set_dims(float window_width, float window_height, float width_pixels, float height_pixels);

    // Unit Quad verts centered at 0.5. ([0,1]x[0,1])
    static std::array<opengl::Vertex2DT, 6> generate_quad_verts_c05();
    // Generate unit square centered at zero and scale and shift according to parameters.
    static std::array<opengl::Vertex2DT,6> generate_quad_verts_c0(f2 center, float scale, f2 texture_coord);
    static std::array<opengl::Vertex2DT,8> generate_quad_line_frame_verts_0505(f2 texture_coord);

};


struct QuadS2D {
    Box2D box;
    m3f3 M_m_s;
    opengl::ShapeS2DRenderContext render_context;

    std::array<opengl::Vertex2DT, 6> verts_6;


    QuadS2D();

    m3f3 get_matrix();
    opengl::ShapeS2DRenderContext& get_rendering_context();
    std::array<opengl::Vertex2DT, 6>& get_verts();

    // Compares provided point to Box2D of quad in scene coordinates.
    bool contains_cursor(f2  cursor_pos_scene_coords);
    // Converts provided scene-point to quad-coordinates. Quad domain = [0,0]x[1,1].
    f2 get_normalized_from_point(f2 cursor_pos_scene_coords);

    void set_box(f2 pos, f2 size);
    void set_texture(opengl::Textures texture);
    void set_texture_id(unsigned int id);

    static std::array<opengl::Vertex2DT, 6> generate_quad_verts_c05();

};



}