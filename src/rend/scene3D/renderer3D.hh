
#pragma once

#include <array>

#include "math/vecmat.hh"
#include "math/transform.hh"
#include "math/shape.hh"

#include "opengl/program.hh"
#include "opengl/texture_fb.hh"

#include "window/auxevent.hh"

#include "rend/scene3D/shader_vector.hh"
#include "rend/scene3D/shader_axes.hh"

#include "rend/scene3D/shader_model_texture.hh"
#include "rend/scene3D/shader_model_color.hh"

#include "rend/scene3D/shader_mesh.hh"
#include "rend/scene3D/shader_quad.hh"

#include "rend/scene3D/shader_object_ids.hh"


namespace opengl {
    class TextureFB;
    class TextureFBMS;
}

struct Scene3D;
struct SceneState;
struct Manager3D;


class RendererScene3D 
{
    f2 window_fb_size;

    ShaderAxes program_axes;
    ShaderVector program_vector;
    
    ShaderModelTexture program_model_texture;
    ShaderModelColor program_model_color;

    ShaderMesh program_mesh;
    ShaderQuad program_quad;

    ShaderObjectIDs program_object_ids;

    opengl::TextureFB fb_object_ids;

public:

    RendererScene3D() = default;

    void init(f2 window_fb_size);

    void bind_window_fb(i2 window_scene_i);

    void set_window_fb_size(window::WindowResizeEvent& window_resize_event);


    /** Render scene objects. */
    void render_scene_3d(Scene3D& scene3D, Manager3D& manager_3D);

    /** Object id outlines for cursor interaction */
    void render_object_ids(Scene3D& scene3D, Manager3D& manager_3D);
    TagO sample_oid_tag(const std::vector<TagO>& scene_tags, const f2 cursor_pos_sane);


private:

    void render_texure_model(model::ModelT model_texture);
    void render_color_model(model::ModelColor model);

};


