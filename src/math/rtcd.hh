#pragma once

#include <cmath>
#include "math/vecmat.hh"

/**  */
struct Line3D
{
    f3 a;
    f3 b;

    Line3D() = default;
    Line3D(f3 a, f3 b) 
        : a {a}
        , b {b}
    {}

    f3 direction() const
    {
        return (b - a).unit();
    }

};

struct Plane3D
{
    f3 n;
    float d; // = \vec{} \cdot \vec{Point} 
};


/**
    If returns positive, C is to the left of line from A to B
*/
float Orient2D(f2 A, f2 B, f2 C)
{
    m2f2 mat { 
        {A.x - C.x, A.y - C.y}, 
        {B.x - C.x, B.y - C.y}
    };
    return mat.det();
}

void barycentric_2D(f2 a, f2 b, f2 c, f2 p, float& u, float& v, float& w)
{
    f2 v0 = b - a;
    f2 v1 = c - a;
    f2 v2 = p - a;

    float d00 = v0.dot(v0);
    float d01 = v0.dot(v1);
    float d11 = v1.dot(v1);
    float d20 = v2.dot(v0);
    float d21 = v2.dot(v1);

    float denom = d00*d11 - d01*d01;

    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;
}


/** Area of a triangle in 3D */
float tri_signed_area(f3 A, f3 B, f3 C)
{
    return 0.5f * (B - A).cross(C - A).norm();
}

/** three points in one plane */
float triangle_area_2D(float x1, float y1, float x2, float y2, float x3, float y3)
{
    return (x1-x2)*(y2-y3) - (x2-x3)*(y1-y2);
}

void barycentric_3D(f3 a, f3 b, f3 c, f3 p, float& u, float& v, float& w)
{
    f3 m = (b-a).cross(c-a);

    float nu, nv; // nominators (numerators?)
    float ood; // one over denominator

    // We are only interested in the primary plane of the triangle, thus the axis with the largest component is found
    float x = fabsf(m.x);
    float y = fabsf(m.y);
    float z = fabsf(m.z);

    // We are intrested in the plane with the largest projection area
    // The direction of the cross product is used is used to determine which axis to not project onto
    if(x >= y && x >= z) // project yz as normal is in x direction
    {
        nu = triangle_area_2D(p.y, p.z, b.y, b.z, c.y, c.z);
        nv = triangle_area_2D(p.y, p.z, c.y, c.z, a.y, a.z);
        ood = 1.0f / m.x;
    }
    else if(y >= x && y >= z) // project xz as normal is in y direction
    {
        nu = triangle_area_2D(p.x, p.z, b.x, b.z, c.x, c.z);
        nv = triangle_area_2D(p.x, p.z, c.x, c.z, a.x, a.z);
        ood = 1.0f / -m.y;
    }
    else // project xy
    {
        nu = triangle_area_2D(p.x, p.y, b.x, b.y, c.x, c.y);
        nv = triangle_area_2D(p.x, p.y, c.x, c.y, a.x, a.y);
        ood = 1.0f / m.z;
    }

    u = nu * ood;
    v = nv * ood;
    w = 1.0f - u - v;
}

float test_point_in_triangle_2D(f2 p, f2 a, f2 b, f2 c)
{
    float u, v, w;
    barycentric_2D(a, b, c, p, u, v, w);
    return v >= 0.0f && w >= 0.0f && (v+w) <= 1.0f;
}