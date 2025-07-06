#pragma once

#include <vector>

#include "math/vecmat.hh"
#include "math/aabb.hh"

#include "scene/mesh.hh"


struct Physics
{
    AABBf aabb;
    bool colliding = false;
    f3 model_size;
};