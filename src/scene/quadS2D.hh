
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



/** Only ever contains 4 vertices and is always rendered with a texture.  */
class QuadS2D {
    Box2D box;  // Utility object to get good bounds-checking semantics
    m3f3 M_m_s; // quad (model) to scene tranformation matrix

    std::array<opengl::VertexQuad2D, 6> verts_6;

    opengl::RenderContextQuadS2D render_context;

public:

    QuadS2D() = default;


    // GET
    m3f3 get_matrix();
    opengl::RenderContextQuadS2D& get_rendering_context();

    // SET
    void set_box(f2 pos, f2 size);
    void set_texture_id(unsigned int id);

    // QUERY

    // Compares provided point to Box2D of quad in scene coordinates.
    bool contains_cursor(f2 cursor_pos_scene_coords);
    // Converts provided scene-point to quad-coordinates. Quad domain = [0,0]x[1,1].
    f2 get_normalized_from_point(f2 cursor_pos_scene_coords);

};



}