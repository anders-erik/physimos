
#pragma once

#include <array>
#include <vector>

#include "math/vecmat.hh"
#include "math/transform.hh"
#include "math/shape.hh"

#include "opengl/program.hh"
#include "opengl/texture_fb.hh"

#include "scene/sid.hh"

#include "rend/scene2D/shader_quad2D.hh"

#include "rend/scene2D/shader_graph_point.hh"
#include "rend/scene2D/shader_graph_line.hh"

#include "rend/scene2D/shader_gridline.hh"

#include "rend/defs.hh"

template <typename T>
struct Graph2D;

namespace opengl {
    class TextureFB;
    class TextureFBMS;
}

class Scene2D;
class QuadS2D;
struct Manager2D;



class RenderContextQuadS2D {


public:
    void init();

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int texture;

    void delete_contents();

    RenderContextQuadS2D() = default;
    RenderContextQuadS2D(unsigned int texture_id);

    void set_texture_id(unsigned int texture_id);

    // Quad as two triangles of unit dimensions ( [0,1]x[0,1] ), thus center at (0.5, 0.5).
    static std::array<VertexQuad2D, 6> generate_quad_verts_c05();
};


struct RenderContextShapeS2D {
    unsigned int VAO;
    unsigned int VBO;

    unsigned int texture;
    f2 tex_coord;

    int element_count;

};


/** Associiates a Scene ID with a framebuffer.
    One is usually created automatically by scene manager. */
struct SceneFramebuffer
{
    SID sid;
    opengl::TextureFB framebuffer;
};



class RendererScene2D 
{
    ShaderQuad2D program_quad_2D;
    Shader2DGraphPoint shader_graph_point;
    Shader2DGraphLine shader_graph_line;
    Shader2DGridline shader_gridline;

    // Old enum for quad shader program -- only instance of specific program
    opengl::ProgramName program_name_enum = opengl::ProgramName::Quad2D;
    unsigned int program; // opengl id for renderer -- only one program currently

    RenderContextQuadS2D render_context_frame; // frame context does not need updating

    std::vector<SceneFramebuffer> scene_framebuffers;

public:

    RendererScene2D();

    void init();

    // THREE CALLS TO MANAGE SCENE FRAMEBUFFERS
    void create_scene_framebuffer(SID sid, ui2 framebuffer_size);
    opengl::TextureFB& get_scene_fb(SID sid);
    uint get_scene_fb_texture_id(SID sid);
    void render_scene_framebuffer(SID sid);
    /** All 2D scenes - usually quad-textures */
    void render_all_scene2D_to_frambuffers(Manager2D& manager_2D);


    void create_shape_context_t(RenderContextQuadS2D& render_context, std::vector<VertexQuad2D> verts);


    void activate();
    void set_camera(m3f3 camera);
    void set_model(m3f3 model_mat);

    // Render real quads
    void render_quad(const QuadS2D& quad);

    void render_scene(Scene2D& scene, Manager2D& manager_2D);
    unsigned int render_scene_FB(Scene2D& scene, Manager2D& manager_2D, opengl::TextureFB& framebuffer);
    unsigned int render_scene_FBMS(Scene2D& scene, Manager2D& manager_2D, opengl::TextureFBMS& framebuffer_ms);

    void render_point(RenderContextQuadS2D context);
    void render_line(RenderContextQuadS2D context);

    void render_graph2D(Graph2D<float>& graph);

    void render_frame(m3f3 M_m_s, bool selected, int line_width);

    // OLD UNUSED
    void render_multisample_texture(RenderContextQuadS2D context);

private:

    // Frame
    void init_frame_wire_quad_context_t();
    static std::array<VertexQuad2D,8> generate_quad_line_frame_verts_0101(f2 texture_coord);

};


