#pragma once

#include "math/box2D.hh"

#include "math/aabb.hh"
#include "math/vec.hh"
#include "math/polynomial.hh"

/** Polynomial graph for floating point values. */
template <typename T>
class Graph2D
{
public:

    size_t point_count = 0;

    AABB<T> viewbox;
    T step_size;

    Polynomial<T> polynomial { {0} }; // default is zero-order with constant equal to 0

    Vec<T> x;
    Vec<T> y;


    // Graph2D() = default;

    Graph2D(size_t point_count, AABB<T> viewbox)
        :   point_count {point_count},
            viewbox     {viewbox},
            x           {Vec<T> {point_count, 0.0}},
            y           {Vec<T> {point_count, 0.0}}
    {
        calculate_step_size();
    };

    Graph2D(size_t point_count, AABB<T> viewbox, Polynomial<T> polynomial)
        :   point_count {point_count},
            viewbox     {viewbox},
            polynomial  {polynomial},
            x           {Vec<T> {point_count, 0.0}},
            y           {Vec<T> {point_count, 0.0}}
    {
        calculate_step_size();
    };


    void set_polynomial(Polynomial<T>& polynomial)
    {
        this->polynomial = polynomial;
    }

    void update()
    {
        for(size_t i = 0; i < point_count; i++)
        {
            T i_f = (T) i;

            x[i] = viewbox.pos.x + i_f * step_size;
            y[i] = polynomial(x[i]);
        }
    }

    R2<T> at(size_t index)
    {
        return {x[index], y[index]};
    }

private:

    void calculate_step_size()
    {
        T point_count_float = (T) point_count;
        step_size = viewbox.size.x / (point_count_float - 1.0);
    }

};