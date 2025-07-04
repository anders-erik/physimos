
#pragma once


#include <vector>

#include "lib/str.hh"

#include "math/transform.hh"

#include "opengl/texture.hh"

#include "scene/mesh.hh"

#include "scene/sid.hh"

#include "scene/object.hh"

#include "scene/quad.hh"


#include "scene/lamp.hh"

#include "scene/tagp.hh"

#include "lib/pair.hh"

struct ManagerProperty
{
    PID pid_count = 1;

    std::vector<Pair<TagP, Lamp>> lamps;

    PID new_pid();
    TagP new_tagp();
    TagP new_tagp(TagP::Type type);

    TagP push_lamp(const Lamp& lamp);
    Lamp* find_lamp(TagP lamp_tagp);

};




