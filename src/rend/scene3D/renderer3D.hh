
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
#include "rend/scene3D/shader_color_light.hh"
#include "rend/scene3D/shader_color_light_inst.hh"

#include "rend/scene3D/shader_object_ids.hh"


namespace opengl {
    class TextureFB;
    class TextureFBMS;
}

struct Manager3D;
struct ManagerProperty;
struct Physics;
struct Scene3D;
struct SceneState;
struct CameraObject;
struct Lamp;


class RendererScene3D 
{
    f2 window_fb_size;

    ShaderAxes program_axes;
    ShaderVector program_vector;
    
    ShaderModelTexture program_model_texture;
    ShaderModelColor program_model_color;

    ShaderMesh program_mesh;
    ShaderQuad program_quad;

    ShaderColorLight        program_color_light;
    ShaderColorLightInst    shader_color_light_inst;

    ShaderObjectIDs program_object_ids;

    opengl::TextureFB fb_object_ids;

public:

    RendererScene3D() = default;

    void init(f2 window_fb_size);

    void bind_window_fb(i2 window_scene_i);
    void set_window_fb_size(window::WindowResizeEvent& window_resize_event);

    void set_camera(CameraObject& camera);
    void set_lamps(std::vector<TagO> lamp_tags, Manager3D& manager3D);
    void render_scene(Scene3D& scene3D, Manager3D& manager_3D);

    /** Object id outlines for cursor interaction */
    void render_object_ids(Scene3D& scene3D, Manager3D& manager_3D);
    TagO sample_oid_tag(const Scene3D& scene, const f2 cursor_pos_sane);


private:

    /** Returns false on invalid object tag. */
    bool render_tag(TagO tag, Manager3D& manager_3D);
    void render_quad(Object& object, Quad& quad, Manager3D& manager_3D);
    void render_lamp(Object & object, Lamp & lamp);
    void render_physics(Physics* physics, ManagerProperty& manager_p);

};


