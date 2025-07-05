
#pragma once


#include "math/number.hh"




struct AABBf
{
    f3 min = {__FLT_MAX__, __FLT_MAX__, __FLT_MAX__};
    f3 max = {__FLT_MIN__, __FLT_MIN__, __FLT_MIN__};


    AABBf operator+(const f3& rhs)
    {
        AABBf ret_aabb;
        ret_aabb.min.x = min.x + rhs.x;
        ret_aabb.min.y = min.y + rhs.y;
        ret_aabb.min.z = min.z + rhs.z;
        ret_aabb.max.x = max.x + rhs.x;
        ret_aabb.max.y = max.y + rhs.y;
        ret_aabb.max.z = max.z + rhs.z;
        return ret_aabb;
    }

    static bool collide(AABBf A, AABBf B)
    {
        return  A.min.x <= B.max.x && A.max.x >= B.min.x &&
                A.min.y <= B.max.y && A.max.y >= B.min.y &&
                A.min.z <= B.max.z && A.max.z >= B.min.z    ;
    }

    bool contains_point(const f3& p)
    {
        return  p.x >= min.x && p.x <= max.x  && \
                p.y >= min.y && p.y <= max.y  && \
                p.z >= min.z && p.z <= max.z;
    }

    bool contains_point(f3 offset, const f3& pnt)
    {
        f3 MIN = offset + min;
        f3 MAX = offset + max;

        return  pnt.x >= MIN.x && pnt.x <= MAX.x  && \
                pnt.y >= MIN.y && pnt.y <= MAX.y  && \
                pnt.z >= MIN.z && pnt.z <= MAX.z;
    }
};


/** 2D */
template <typename F>
struct AABB2D 
{
    R2<F> pos;
    R2<F> size;
};
