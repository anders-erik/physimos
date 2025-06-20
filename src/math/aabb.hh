
#pragma once


/** Real 2D point.
    R^2 = (R, R) */
template <typename F>
struct R2
{
    F x;
    F y;
};


/** 2D */
template <typename F>
struct AABB 
{
    R2<F> pos;
    R2<F> size;
};
