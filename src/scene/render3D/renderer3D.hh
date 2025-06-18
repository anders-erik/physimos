
#pragma once

#include <array>

#include "math/vecmat.hh"
#include "math/transform.hh"
#include "math/shape.hh"

#include "opengl/program.hh"
#include "opengl/texture_fb.hh"

#include "window/auxevent.hh"

#include "render3D/program_vector.hh"
#include "render3D/program_axes.hh"

#include "render3D/program_model_texture.hh"
#include "render3D/program_model_color.hh"

#include "render3D/program_mesh.hh"
#include "render3D/program_object_ids.hh"


namespace opengl {
    class TextureFB;
    class TextureFBMS;
}





class RendererScene3D 
{
    f2 window_fb_size;

    ProgramAxes program_axes;
    ProgramVector program_vector;
    
    ProgramModelTexture program_model_texture;
    ProgramModelColor program_model_color;

    ProgramMesh program_mesh;
    ProgramObjectIDs program_object_ids;

    opengl::TextureFB fb_object_ids;

public:

    RendererScene3D(f2 window_fb_size);

    void set_window_fb_size(window::WindowResizeEvent& window_resize_event);

    OID sample_object_id_fb(f2 cursor_pos_sane);

    void render_scene_3d(SceneBase& scene);

private:

    void render_texure_model(model::ModelT model_texture);
    void render_color_model(model::ModelColor model);

};


