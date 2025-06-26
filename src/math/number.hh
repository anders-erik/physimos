
#pragma once


/** Real 2D point.
    R^2 = (R, R) */
template <typename F>
struct R2
{
    F x;
    F y;

    F* data() { return (F*) this; }
};


