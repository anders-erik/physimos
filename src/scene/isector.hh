#pragma once

#include <cmath>

#include "math/vecmat.hh"
#include "math/aabb.hh"
#include "math/sphere.hh"




using IsectBits = uint;

static constexpr IsectBits ISECT_NONE             = 0x0;

static constexpr IsectBits ISECT_PARTICLE         = 0x0001;
static constexpr IsectBits ISECT_AABB             = 0x0002;
static constexpr IsectBits ISECT_SPHERE           = 0x0004;

static constexpr IsectBits ISECT_AABB_AABB        = 0x00020002;
static constexpr IsectBits ISECT_AABB_SPHERE      = 0x00020004;
static constexpr IsectBits ISECT_SPHERE_AABB      = 0x00040002;
static constexpr IsectBits ISECT_SPHERE_SPHERE    = 0x00040004;


union Isector
{
    AABBf aabb;
    Sphere sphere;

    Isector() {};

    Isector& operator=(const Isector& other)
    {
        // We don't know the type, but the owner does.
        // If call-overhead is too high, cast as 64-bit pointer and copy indeces
        memcpy( static_cast<void*>(this), 
                static_cast<const void*>(&other), 
                sizeof(Isector)                 );
        return *this;
    }
};




static bool isect_sphere_sphere(const Sphere& A, const Sphere& B)
{
    return fabs((A.pos-B.pos).norm()) < (A.r+B.r);
}


static bool isect_aabb_sphere(const AABBf& B, const Sphere& S)
{
    float nearest_x = 0.0f;
    if      ( S.pos.x < B.min.x )
        nearest_x += S.pos.x - B.min.x ; 
    else if ( S.pos.x > B.max.x )
        nearest_x += S.pos.x - B.max.x ;

    float nearest_y = 0.0f;
    if      ( S.pos.y < B.min.y )
        nearest_y +=  S.pos.y - B.min.y;
    else if ( S.pos.y > B.max.y )
        nearest_y +=  S.pos.y - B.max.y;

    float nearest_z = 0.0f;
    if      ( S.pos.z < B.min.z )
        nearest_z +=  S.pos.z - B.min.z;
    else if ( S.pos.z > B.max.z )
        nearest_z +=  S.pos.z - B.max.z;
    
    f3 dist_to_box = {nearest_x, nearest_y, nearest_z};


    return  dist_to_box.norm_sq() <= S.r*S.r ? true : false;
}
