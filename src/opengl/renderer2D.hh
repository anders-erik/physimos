
#pragma once

#include <array>

#include "math/vecmat.hh"
#include "math/transform.hh"
#include "math/shape.hh"

#include "opengl/program.hh"


namespace scene {

class QuadS2D;


}

namespace opengl {

/** Represents a wireframe used to highlight scene objects. */
// class BoxFrame2D : Box2D {

// public:
//     m3f3 M_m_s;
//     std::array<opengl::VertexQuad2D, 8> verts; // Vertices for wireframe highlighting
//     opengl::RenderContextQuadS2D render_context;  // Rendering context for wireframe highlighting

// };

struct VertexQuad2D {
    f3 pos;
    f2 tex;
};

class RenderContextQuadS2D {

    void init();

public:

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int texture;

    void delete_contents();

    RenderContextQuadS2D();
    RenderContextQuadS2D(unsigned int texture_id);

    void set_texture_id(unsigned int texture_id);

    // Quad as two triangles of unit dimensions ( [0,1]x[0,1] ), thus center at (0.5, 0.5).
    static std::array<opengl::VertexQuad2D, 6> generate_quad_verts_c05();
};


struct RenderContextShapeS2D {
    unsigned int VAO;
    unsigned int VBO;

    unsigned int texture;
    f2 tex_coord;

    int element_count;

};




class Scene2DRenderer {

    opengl::ProgramName program_name_enum = opengl::ProgramName::Texture2D;
    unsigned int program; // opengl id for renderer -- only one program currently

    opengl::RenderContextQuadS2D render_context_frame; // frame context does not need updating

public:

    Scene2DRenderer();

    void create_shape_context_t(RenderContextQuadS2D& render_context, std::vector<VertexQuad2D> verts);


    void activate();
    void set_camera(m3f3 camera);
    void set_model(m3f3 model_mat);

    // Render real quads
    void render_quad(scene::QuadS2D& quad);


    void render_point(RenderContextQuadS2D context);
    void render_line(RenderContextQuadS2D context);

    void render_frame(m3f3 M_m_s);

    // OLD UNUSED
    void render_multisample_texture(RenderContextQuadS2D context);

private:

    // Frame
    void init_frame_wire_quad_context_t();
    static std::array<opengl::VertexQuad2D,8> generate_quad_line_frame_verts_0101(f2 texture_coord);

};


}