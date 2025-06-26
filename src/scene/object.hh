#pragma once

#include "math/vecmat.hh"
#include "math/quarternion.hh"

#include "lib/str.hh"

#include "scene/tago.hh"
#include "scene/mesh.hh"


struct Object
{
    TagO        tag;
    Str         name;
    Mesh        mesh;
    
    f3          pos;
    Quarternion rot     = {0.0f, 1.0f, 0.0f, 0.0f};
    float       scale = 1.0f;


    Object() = default;
    Object(TagO tag, Str name, Mesh& mesh)
        :   tag {tag},
            name {name},
            mesh {mesh}
    {
    }
};