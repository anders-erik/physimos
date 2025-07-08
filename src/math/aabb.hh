
#pragma once


#include "math/number.hh"




struct AABBf
{
    f3 min = {__FLT_MAX__, __FLT_MAX__, __FLT_MAX__};
    f3 max = {__FLT_MIN__, __FLT_MIN__, __FLT_MIN__};

    f3 half_size;

    void update(f3 pos)
    {
        min = pos - half_size;
        max = pos + half_size;
    }

    static bool isect_aabb_aabb(AABBf A, AABBf B)
    {
        return  A.min.x <= B.max.x && A.max.x >= B.min.x &&
                A.min.y <= B.max.y && A.max.y >= B.min.y &&
                A.min.z <= B.max.z && A.max.z >= B.min.z    ;
    }

    bool contains_point(const f3& p) const
    {
        return  p.x >= min.x && p.x <= max.x  && \
                p.y >= min.y && p.y <= max.y  && \
                p.z >= min.z && p.z <= max.z;
    }

    bool contains_point(f3 offset, const f3& pnt)
    {
        return  pnt.x >= min.x && pnt.x <= min.x  && \
                pnt.y >= min.y && pnt.y <= min.y  && \
                pnt.z >= min.z && pnt.z <= min.z;
    }
};


/** 2D */
template <typename F>
struct AABB2D 
{
    R2<F> pos;
    R2<F> size;
};
