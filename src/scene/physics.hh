#pragma once

#include <vector>
#include <map>
#include <cstring>
#include <cmath>


#include "math/vecmat.hh"


#include "scene/mesh.hh"

#include "scene/isector.hh"








struct Physics
{
    f3 p;
    f3 v;

    bool static_flag = true;
    bool isect_flag = false;

    IsectBits isect_bits = ISECT_NONE;
    Isector   isector;
    
    // int afsize = sizeof(f3);
    // int isize = sizeof(Isector);
    // int psize = sizeof(Physics);

    f3 update_dynamic(float dt)
    {
        v.z += -9.8f * dt;
        p.z += v.z * dt;

        isect_flag = false;

        if(is_aabb())
            isector.aabb.update(p);
        else if(is_sphere())
            isector.sphere.update(p);

        return p;
    }

    /** Position is driven by */
    void update_static(f3 new_pos)
    {
        isect_flag = false;

        if(is_aabb())
            isector.aabb.update(new_pos);
        else if(is_sphere())
            isector.sphere.update(new_pos);
    }

    void update_isector(f3 new_pos)
    {
        if(is_aabb())
            isector.aabb.update(new_pos);
        else if(is_sphere())
            isector.sphere.update(new_pos);
    }

    void set_static_aabb(f3 pos, f3 half_size)
    {
        static_flag = true;
        isect_bits = ISECT_AABB;
        isector.aabb.half_size = half_size;
        isector.aabb.update(pos);
    }

    void set_static_sphere(f3 pos, float radius)
    {
        static_flag = true;
        isect_bits = ISECT_SPHERE;
        isector.sphere.r = radius;
        isector.sphere.update(pos);
    }

    void set_dynamic_sphere(f3 pos_0, float radius)
    {
        p = pos_0;

        static_flag = false;
        isect_bits = ISECT_SPHERE;
        isector.sphere.r = radius;
        isector.sphere.update(p);
    }

    bool is_dynamic() {return !static_flag;}
    bool is_static() {return static_flag;}
    bool has_collider()  { return !ISECT_NONE; }
    bool is_aabb()      {return isect_bits == ISECT_AABB;     }
    bool is_sphere()    {return isect_bits == ISECT_SPHERE;   }
};



static bool isect_physics(Physics& PA, Physics& PB)
{
    // IsectBits bits = (PA.isect_bits << 16) | PB.isect_bits;
    switch ( (PA.isect_bits << 16) | PB.isect_bits )
    {

    case ISECT_AABB_AABB:
        return AABBf::isect_aabb_aabb(  PA.isector.aabb, PB.isector.aabb);
        break;

    case ISECT_AABB_SPHERE:
        return isect_aabb_sphere(PA.isector.aabb, PB.isector.sphere);
        break;

    case ISECT_SPHERE_AABB:
        return isect_aabb_sphere(PB.isector.aabb, PA.isector.sphere);
        break;

    case ISECT_SPHERE_SPHERE:
        return isect_sphere_sphere(PA.isector.sphere, PB.isector.sphere);
        break;

    default:
        throw "Unmatched switch in isect_physics.";
        break;
    }
}