
#pragma once


#include <vector>

#include "lib/str.hh"

#include "math/transform.hh"

#include "opengl/texture.hh"

#include "scene/mesh.hh"

#include "scene/sid.hh"

#include "scene/object.hh"

#include "scene/quad.hh"


struct ManagerObject
{
    OID oid_index = 1;

    std::vector<Object> objects;

    OID new_oid();
    TagO new_tag();
    TagO new_tag(TagO::Type type);

    TagO push_object(Object& object);
    Object* get_object(OID oid);
    Object* get_object(TagO tago);

    Object& new_object();
};




