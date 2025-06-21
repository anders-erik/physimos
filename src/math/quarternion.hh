#pragma once

#include <cmath>

#include "math/vecmat.hh"
#include "math/transform.hh"

struct Quarternion
{
    float w = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Quarternion() = default;
    Quarternion(float w, const f3 pure_q)
        :   w {w},
            x {pure_q.x},
            y {pure_q.y},
            z {pure_q.z}
    {
    }
    Quarternion(float w, float x, float y, float z)
        :   w {w},
            x {x},
            y {y},
            z {z}
    {
    }
    Quarternion(const Quarternion& q)
    {
        this->w = q.w;
        this->x = q.x;
        this->y = q.y;
        this->z = q.z;
    }

    Quarternion conjugate()
    {
        return {w, -x, -y, -z};
    }

    void set_angle(float angle)
    {
        float sin_half = sinf(angle * 0.5f);
        float cos_half = cosf(angle * 0.5f);

        w  = cos_half;
        x *= sin_half;
        y *= sin_half;
        z *= sin_half;
    }

    Quarternion rotate(const Quarternion& q_to_rotate)
    {
        Quarternion q = q_to_rotate;

        return {    w*q.w - x*q.x - y*q.y - z*q.z,
                    w*q.x + x*q.w + y*q.z - z*q.y,
                    w*q.y - x*q.z + y*q.w + z*q.x,
                    w*q.z + x*q.y - y*q.x + z*q.w };
    }


    static Quarternion rotate_quart(Quarternion q_to_rot, Quarternion q_axis, float angle)
    {
        q_axis.set_angle(angle);
        auto q_axis_conj = q_axis.conjugate();

        auto mult1 = q_axis.rotate(q_to_rot);
        auto mult2 = mult1.rotate(q_axis_conj);

        return mult2;
    }

    /** Rotates in place */
    static void rotate_f3(f3& vec_to_rotate, f3 axis, float angle)
    {
        Quarternion rotated_q = rotate_quart({0.0f, vec_to_rotate}, {0.0f, axis}, angle);

        vec_to_rotate = {   rotated_q.x, 
                            rotated_q.y, 
                            rotated_q.z };
    }
};