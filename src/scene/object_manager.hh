
#pragma once


#include <vector>

#include "lib/str.hh"

#include "math/transform.hh"

#include "opengl/texture.hh"

#include "scene/mesh.hh"

#include "scene/sid.hh"

#include "scene/object.hh"

#include "scene/squad.hh"






namespace ObjectManager 
{

OID new_oid();
TagO new_tag();
TagO new_tag(TagO::Type type);

TagO push_object(Object& object);
Object* get_object(OID oid);
Object* get_object(TagO tago);

TagO push_squado(SQuadO & squado);
SQuadO* get_squado(TagO tago);

Object new_object();
}
