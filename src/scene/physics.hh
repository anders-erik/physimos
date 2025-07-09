#pragma once

#include <vector>
#include <map>
#include <cstring>
#include <cmath>

#include "lib/print.hh"

#include "math/vecmat.hh"
#include "math/quarternion.hh"


#include "scene/mesh.hh"

#include "scene/isector.hh"





const float g = 9.8f;
// const float g = 1.8f;


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
        // "Integrate"
        v.z += -g * dt;
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


    /** If pos is purly above one of AABBs faces, the vector is reflected on that face.
        Returns true if reflected.
    */
    static bool try_reflect_on_face(f3& v_to_reflect, f3 pos, AABBf aabb)
    {
        bool below_x_min = pos.x < aabb.min.x;
        bool below_y_min = pos.y < aabb.min.y;
        bool below_z_min = pos.z < aabb.min.z;

        bool above_x_max = pos.x > aabb.max.x;
        bool above_y_max = pos.y > aabb.max.y;
        bool above_z_max = pos.z > aabb.max.z;

        bool between_x = pos.x > aabb.min.x && pos.x < aabb.max.x;
        bool between_y = pos.y > aabb.min.y && pos.y < aabb.max.y;
        bool between_z = pos.z > aabb.min.z && pos.z < aabb.max.z;

        bool within_x = between_y && between_z;
        bool within_y = between_x && between_z;
        bool within_z = between_x && between_y;

        // Bounce pure face collisions, else reflect dist vector
        if     (within_x && above_x_max)      v_to_reflect.reflect(Axis::x);
        else if(within_x && below_x_min)      v_to_reflect.reflect(Axis::nx);
        else if(within_y && above_y_max)      v_to_reflect.reflect(Axis::y);
        else if(within_y && below_y_min)      v_to_reflect.reflect(Axis::ny);
        else if(within_z && above_z_max)      v_to_reflect.reflect(Axis::z);
        else if(within_z && below_z_min)      v_to_reflect.reflect(Axis::nz);
        else return false; // return false on no match

        return true;
    }


    /** Support:
        aabb-aabb      [O]
        aabb-sphere    [O]
        sphere-aabb    [O]    
        Sphere-sphere  [O]
    */
    static void reflect_dynamic_static(Physics& Dyn, Physics& Stat)
    {
        // IsectBits bits = (PA.isect_bits << 16) | PB.isect_bits;
        switch ( (Dyn.isect_bits << 16) | Stat.isect_bits )
        {

        case ISECT_AABB_AABB:
            if(try_reflect_on_face(Dyn.v, Dyn.p, Stat.isector.aabb))
                return;
            else
                Dyn.v = Quarternion::reflect( Dyn.v, Dyn.p - Stat.isector.aabb.pos());
            break;


        case ISECT_SPHERE_AABB:
            if(try_reflect_on_face(Dyn.v, Dyn.p, Stat.isector.aabb))
                return;
            else
                Dyn.v = Quarternion::reflect( Dyn.v, Dyn.p - Stat.isector.aabb.pos());
            break;

        case ISECT_AABB_SPHERE:
            if(try_reflect_on_face(Dyn.v, Stat.isector.sphere.pos, Dyn.isector.aabb))
                return;
            else
                Dyn.v = Quarternion::reflect( Dyn.v, Dyn.p - Stat.isector.sphere.pos);
            break;

        case ISECT_SPHERE_SPHERE:
            // Reflect across collision plane
            Dyn.v = Quarternion::reflect( Dyn.v, 
                                        Dyn.p - Stat.isector.sphere.pos);
            break;

        default:
            throw "Unmatched switch in isect_physics.";
            break;
        }
    }

};