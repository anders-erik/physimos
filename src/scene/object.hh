#pragma once

#include "math/vecmat.hh"
#include "math/quarternion.hh"

#include "lib/str.hh"

#include "scene/tago.hh"
#include "scene/tagp.hh"
#include "scene/tag_phy.hh"

#include "scene/mesh.hh"
// #include "scene/physics.hh"

struct InstanceContext
{
    bool enabled = false;
    uint instance_count = 0;
};

struct ObjectRenderContext
{
    enum Shading
    {
        ColorLight,
        Wireframe,
    };

    Shading shading     = ColorLight;
    bool    visible     = true;
    InstanceContext     instance_ctx;
    bool    normals     = false;
    bool    physics     = true;
    bool    axes        = false;

    void set_shading_color_light()  { shading = ColorLight; }
    void set_shading_wireframe()    { shading = Wireframe; }
};

struct Object
{
    TagO        tag;
    Str         name;
    Mesh        mesh;
    
    f3          pos;
    Quarternion rot     = {1.0f, 0.0f, 0.0f, 0.0f};
    float       scale = 1.0f;

    TagP        tagp;
    IDPhy       id_phy = 0;

    ObjectRenderContext rend_cxt;

    Object() = default;
    Object(TagO tag, Str name, Mesh& mesh)
        :   tag {tag},
            name {name},
            mesh {mesh}
    {}

    m4f4 get_model_matrix()
    {
        m4f4 scale_matrix       = m4f4::scale(scale);
        m4f4 translation_matrix = m4f4::translation(pos);
        m4f4 rotation_matrix    = rot.matrix();
        return translation_matrix * rotation_matrix * scale_matrix;
    }
};
