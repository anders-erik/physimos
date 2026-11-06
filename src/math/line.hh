
#pragma once

#include "math/vecmat.hh"


// y = kx + m
class Line
{
public:

    d2 p1;
    d2 p2;

    double k;
    double m;

    Line(d2 _p1, d2 _p2) : p1 {_p1}, p2 {_p2}
    {
        // y = kx + m
        k = (p2.y - p1.y)/(p2.x - p1.x);
        m = p1.y - p1.x*k;
    }

    double operator[](double _x)
    {
        return k*_x + m;
    }

};



/*
    Line object used for drawing lines
    p1 is automatically set to the be the point with the smaller x-value during construction
*/
class LineRend
{
public:

    enum StepDirection
    {
        X,
        Y,
    } step_direction;

    i2 p1; // point with smaller x-value
    i2 p2; // point with larger x-value


    LineRend(i2 _p1, i2 _p2) 
    {
        p1 = _p1.x < _p2.x ? _p1 : _p2; // point with smaller x-value
        p2 = _p1.x < _p2.x ? _p2 : _p1;

        step_direction = Dx() > Dy() ? StepDirection::X : StepDirection::Y;
    }

    int get_major_direction_pixels()
    {
        return (p2.x-p1.x) + 1;
    }

    // int operator[](int major_index)
    // {

    // }

    int y(int major_index)
    {
        
    }




    //  /* t := [0, 1] */
    // i2 lerp(double t)
    // {
    //     if(t < 0 || t > 1)
    //     {
    //         Print::ln("t-value out of bounds in LineVec. ");
    //     }
    //     return i2 { p1.x + t*(p2.x-p1.x),
    //                 p1.y + t*(p2.y-p1.y) };
    // }

    /* Total delta-x */
    double Dx()
    {
        return fabs(p2.x - p1.x);
    }

    /* Total delta-y */
    double Dy()
    {
        return fabs(p2.y - p1.y);
    }

};


