#pragma once

#include <vector>
#include <cmath>

#include "math/vecmat.hh"
#include "math/aabb.hh"

#include "scene/mesh.hh"

/** Bounding Box */
enum class BBType
{
    Point,
    AABB,
    Sphere,
};

struct Sphere
{
    f3 pos;
    float r;

    static bool intersect(const Sphere& A, const Sphere& B)
    {
        return fabs((A.pos-B.pos).norm()) < (A.r+B.r);
    }
};

constexpr bool intersect(const AABBf& A, const Sphere& S)
{
    // Algorithm below can fail if sphere-center is inside AABB
    if(A.contains_point(S.pos))
        return true;

    float x_closest;
    if(fabs(A.max.x - S.pos.x) < fabs(A.min.x - S.pos.x))
        x_closest = A.max.x;
    else
        x_closest = A.min.x;

    float y_closest;
    if(fabs(A.max.y - S.pos.y) < fabs(A.min.y - S.pos.y))
        y_closest = A.max.y;
    else
        y_closest = A.min.y;

    float z_closest;
    if(fabs(A.max.z - S.pos.z) < fabs(A.min.z - S.pos.z))
        z_closest = A.max.z;
    else
        z_closest = A.min.z;

    f3 p_closest = {    x_closest,
                        y_closest,
                        z_closest   };
    
    float dist = (p_closest - S.pos).norm();

    return dist < S.r;
}

struct Physics
{
    BBType type = BBType::Point;

    AABBf aabb;
    Sphere sphere;

    bool intersecting = false;
    f3 model_size;

    bool is_aabb() {return type == BBType::AABB;}
    bool is_sphere() {return type == BBType::Sphere;}
};