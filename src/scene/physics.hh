#pragma once

#include <vector>

#include "math/vecmat.hh"
#include "math/aabb.hh"

#include "scene/mesh.hh"


struct Physics
{
    AABBf aabb_base;
    f3 pos;
    bool colliding = false;

    void set_AABB_mesh(const Mesh& mesh)
    {
        for(auto& vert : mesh.verts)
        {
            if(vert.x < aabb_base.min.x)
                aabb_base.min.x = vert.x;
            if(vert.x > aabb_base.max.x)
                aabb_base.max.x = vert.x;

            if(vert.y < aabb_base.min.y)
                aabb_base.min.y = vert.y;
            if(vert.y > aabb_base.max.y)
                aabb_base.max.y = vert.y;

            if(vert.z < aabb_base.min.z)
                aabb_base.min.z = vert.z;
            if(vert.z > aabb_base.max.z)
                aabb_base.max.z = vert.z;
        }

        // Make sure all values are non-zero
        if(aabb_base.min.x == aabb_base.max.x)
            aabb_base.max.x += 0.0001;
        if(aabb_base.min.y == aabb_base.max.y)
            aabb_base.max.y += 0.0001;
        if(aabb_base.min.z == aabb_base.max.z)
            aabb_base.max.z += 0.0001;
    }
};