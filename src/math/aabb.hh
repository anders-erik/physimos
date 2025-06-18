
#pragma once


template <typename F>
struct F2
{
    F x;
    F y;
};


/** 2D */
template <typename F>
struct AABB 
{
    F2<F> pos;
    F2<F> size;
};
