#pragma once

#include "math/vecmat.hh"
#include "math/quarternion.hh"

#include "lib/str.hh"

#include "scene/tago.hh"
#include "scene/tagp.hh"

#include "scene/mesh.hh"
// #include "scene/physics.hh"


struct Object
{
    TagO        tag;
    Str         name;
    Mesh        mesh;
    
    f3          pos;
    Quarternion rot     = {1.0f, 0.0f, 0.0f, 0.0f};
    float       scale = 1.0f;

    TagP        tagp;
    TagP        pyh_tag;

    Object() = default;
    Object(TagO tag, Str name, Mesh& mesh)
        :   tag {tag},
            name {name},
            mesh {mesh}
    {}
};
