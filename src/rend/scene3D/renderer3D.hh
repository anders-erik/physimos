
#pragma once

#include <array>

#include "math/vecmat.hh"
#include "math/transform.hh"
#include "math/shape.hh"

#include "opengl/program.hh"
#include "opengl/texture_fb.hh"

#include "window/auxevent.hh"

#include "rend/scene3D/program_vector.hh"
#include "rend/scene3D/program_axes.hh"

#include "rend/scene3D/program_model_texture.hh"
#include "rend/scene3D/program_model_color.hh"

#include "rend/scene3D/program_mesh.hh"
#include "rend/scene3D/program_quad.hh"

#include "rend/scene3D/program_object_ids.hh"


namespace opengl {
    class TextureFB;
    class TextureFBMS;
}

struct Scene3D;
struct SceneState;


class RendererScene3D 
{
    f2 window_fb_size;

    ProgramAxes program_axes;
    ProgramVector program_vector;
    
    ProgramModelTexture program_model_texture;
    ProgramModelColor program_model_color;

    ProgramMesh program_mesh;
    ProgramQuad program_quad;

    ProgramObjectIDs program_object_ids;

    opengl::TextureFB fb_object_ids;

public:

    RendererScene3D(f2 window_fb_size);

    void set_window_fb_size(window::WindowResizeEvent& window_resize_event);


    /** Render scene objects. */
    void render_scene_3d(Scene3D& scene3D, SceneState& state);

    /** Object id outlines for cursor interaction */
    void render_object_ids(Scene3D& scene3D);

    /** Samples a location in the object id framebuffer */
    Object* sample_object_in_fb(f2 cursor_pos_sane);

private:

    void render_texure_model(model::ModelT model_texture);
    void render_color_model(model::ModelColor model);

};


