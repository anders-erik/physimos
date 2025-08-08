
#pragma once


#include <vector>

#include "lib/str.hh"


#include "scene/physics.hh"

#include "scene/tag_phy.hh"

#include "lib/pair.hh"

struct ManagerPhysics
{
    IDPhy phy_id_count = 1;

    std::vector<Pair<IDPhy, Physics>> data;

    IDPhy       new_id();
    TagPhy      new_tagp();

    IDPhy       push_physics(const Physics& physics);
    Physics*    find_physics(IDPhy physics_id);
};




