#pragma once

#include <cmath>

#include "math/vecmat.hh"
#include "math/transform.hh"

struct Quarternion
{
    float q0 = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Quarternion() = default;
    Quarternion(float q0, const f3 pure_q)
        :   q0 {q0},
            x {pure_q.x},
            y {pure_q.y},
            z {pure_q.z}
    {
    }
    Quarternion(float q0, float x, float y, float z)
        :   q0 {q0},
            x {x},
            y {y},
            z {z}
    {
    }
    Quarternion(const Quarternion& q)
    {
        this->q0 = q.q0;
        this->x = q.x;
        this->y = q.y;
        this->z = q.z;
    }


    Quarternion 
    conjugate()
    {
        return {q0, -x, -y, -z};
    }


    void 
    set_angle(float angle)
    {
        float sin_half = sinf(angle * 0.5f);
        float cos_half = cosf(angle * 0.5f);

        q0  = cos_half;
        x *= sin_half;
        y *= sin_half;
        z *= sin_half;
    }


    m4f4 
    matrix()
    {
        return  {   { 1 - 2*y*y - 2*z*z,    2*x*y - 2*q0*z,     2*x*z + 2*q0*y,     0,      },
                    { 2*x*y + 2*q0*z,       1 - 2*x*x - 2*z*z,  2*y*z - 2*q0*x,     0,      },
                    { 2*x*z - 2*q0*y,       2*y*z + 2*q0*x,     1 - 2*x*x - 2*y*y,  0,      },
                    { 0,                    0,                  0,                  1       }   };
    }

    Quarternion 
    rotate(const Quarternion& q_to_rotate)
    {
        Quarternion q = q_to_rotate;

        return {    q0*q.q0 - x*q.x  - y*q.y  - z*q.z,
                    q0*q.x  + x*q.q0 + y*q.z  - z*q.y,
                    q0*q.y  - x*q.z  + y*q.q0 + z*q.x,
                    q0*q.z  + x*q.y  - y*q.x  + z*q.q0 };
    }


    static Quarternion 
    rotate_quart(Quarternion q_to_rot, Quarternion q_axis, float angle)
    {
        q_axis.set_angle(angle);
        auto q_axis_conj = q_axis.conjugate();

        auto mult1 = q_axis.rotate(q_to_rot);
        auto mult2 = mult1.rotate(q_axis_conj);

        return mult2;
    }


    /** Rotates in place */
    static void 
    rotate_f3(f3& vec_to_rotate, f3 axis, float angle)
    {
        Quarternion rotated_q = rotate_quart({0.0f, vec_to_rotate}, {0.0f, axis}, angle);

        vec_to_rotate = {   rotated_q.x, 
                            rotated_q.y, 
                            rotated_q.z };
    }
};