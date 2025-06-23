#pragma once

#include "lib/str.hh"

#include "scene/tago.hh"
#include "scene/mesh.hh"


struct Object
{
    TagO tag;
    Str name;
    Mesh mesh;
    
    f3 pos;

    Object() = default;
    Object(TagO tag, Str name, Mesh& mesh)
        :   tag {tag},
            name {name},
            mesh {mesh}
    {
    }
};