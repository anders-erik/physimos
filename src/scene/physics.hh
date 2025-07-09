#pragma once

#include <vector>
#include <map>
#include <cstring>
#include <cmath>


#include "math/vecmat.hh"
#include "math/quarternion.hh"


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
        p += v * dt;

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

    void set_dynamic_aabb(f3 pos_0, f3 half_size)
    {
        p = pos_0;

        static_flag = false;
        isect_bits = ISECT_AABB;
        isector.aabb.half_size = half_size;
        isector.aabb.update(pos_0);
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



    static bool isect(Physics& PA, Physics& PB)
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

    /** Support:
        aabb-aabb      [X]
        aabb-sphere    [X]
        sphere-aabb    [max-z only]    
        Sphere-sphere  [O]
    */
    static void reflect_dynamic_static(Physics& D, Physics& S)
    {
        // IsectBits bits = (PA.isect_bits << 16) | PB.isect_bits;
        switch ( (D.isect_bits << 16) | S.isect_bits )
        {

        case ISECT_AABB_AABB:
            D.v = -D.v;
            break;

        case ISECT_AABB_SPHERE:
            D.v = -D.v;
            break;

        case ISECT_SPHERE_AABB:
            {
                AABBf& aabb = S.isector.aabb;
                Sphere& sphere = D.isector.sphere;

                if(sphere.pos.z > aabb.max.z)
                {
                    // z-max face collision, no edge/corner
                    if (       sphere.pos.x > aabb.min.x
                            && sphere.pos.x < aabb.max.x
                            && sphere.pos.y > aabb.min.y
                            && sphere.pos.y > aabb.min.y
                        )
                    {
                        D.v.reflect_z();
                    }
                    else
                    {
                        D.v = -D.v;
                    }
                }
                else
                {
                    D.v = -D.v;
                }
                
            }
            break;

        case ISECT_SPHERE_SPHERE:
            // Reflect across collision plane
            D.v = Quarternion::reflect( D.v, 
                                        D.p - S.isector.sphere.pos);
            break;

        default:
            throw "Unmatched switch in isect_physics.";
            break;
        }
    }

};