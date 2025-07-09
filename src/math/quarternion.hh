#pragma once

#include <iostream>

#include <cmath>

#include "math/const.hh"
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

    Quarternion operator*(const Quarternion& q)
    {
        return Quarternion{  q0*q.q0 - x*q.x  - y*q.y - z*q.z
                            ,q0*q.x  + x*q.q0 + y*q.z - z*q.y
                            ,q0*q.y  + y*q.q0 + z*q.x - x*q.z
                            ,q0*q.z  + z*q.q0 + x*q.y - y*q.x   };
    }

    Quarternion& normalize()
    {
        float norm = sqrtf(q0*q0 + x*x + y*y + z*z);
        q0 /= norm;
        x  /= norm;
        y  /= norm;
        z  /= norm;
        return *this;
    }

    void reset()
    {
        q0 = 1.0f;
        x  = 0.0f;
        y  = 0.0f;
        z  = 0.0f;
    }

    float mult_conjugate()
    {
        return ((*this) * this->conjugate()).q0;
    }


    Quarternion 
    conjugate()
    {
        return {q0, -x, -y, -z};
    }

    Quarternion 
    inverse()
    {
        Quarternion ret_q = {q0, -x, -y, -z};
        float norm_squared = ret_q.mult_conjugate();
        ret_q.q0 /= norm_squared;
        ret_q.x  /= norm_squared;
        ret_q.y  /= norm_squared;
        ret_q.x  /= norm_squared;
        return ret_q;
    }
    

    f3 
    pure()
    {
        return {x, y, z};
    }


    void 
    set_angle(float angle)
    {
        float sin_half = sinf(angle * 0.5f);
        float cos_half = cosf(angle * 0.5f);

        q0  = cos_half;
        x  *= sin_half;
        y  *= sin_half;
        z  *= sin_half;
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

    Quarternion& 
    rotate(f3 rot_axis, float angle)
    {
        Quarternion q_axis = {0.0f, rot_axis};
        q_axis.set_angle(angle);

        *this = q_axis * (*this);
        return this->normalize();
    }


    static Quarternion 
    rotate_quart(Quarternion q_to_rot, Quarternion q_axis, float angle)
    {
        q_axis.set_angle(angle);
        auto q_axis_conj = q_axis.conjugate();

        auto mult1 = q_axis * q_to_rot;
        auto mult2 = mult1.rotate(q_axis_conj);

        // return mult1.normalize();
        return mult2.normalize();
    }


    /** Rotates in place */
    static void 
    rotate_f3(f3& vec_to_rotate, f3 axis, float angle)
    {
        Quarternion rotated_q = rotate_quart({0.0f, vec_to_rotate.unit()}, {0.0f, axis.unit()}, angle);

        float vec_norm = vec_to_rotate.norm();

        vec_to_rotate = {   rotated_q.x * vec_norm, 
                            rotated_q.y * vec_norm, 
                            rotated_q.z * vec_norm };
        // vec_to_rotate.unit();
    }

    /** Rotate vector */
    static f3 
    rotate(const f3& vec_to_rotate, f3 axis, float angle)
    {
        Quarternion rotated_q = rotate_quart(   {0.0f, vec_to_rotate.unit()},
                                                {0.0f, axis.unit()}, 
                                                angle                         );

        float vec_norm = vec_to_rotate.norm();

        return {    rotated_q.x * vec_norm, 
                    rotated_q.y * vec_norm, 
                    rotated_q.z * vec_norm };
    }


    /** Rotate vector */
    static f3 
    reflect(const f3& vec_to_reflect, const f3& normal)
    {
        Quarternion rotated_q = rotate_quart(   {0.0f, -vec_to_reflect.unit()},
                                                {0.0f, normal.unit()}, 
                                                PIf                         );

        float vec_norm = vec_to_reflect.norm();

        return {    rotated_q.x * vec_norm, 
                    rotated_q.y * vec_norm, 
                    rotated_q.z * vec_norm };
    }

    /** Rotates in place */
    static f3 
    rotate_f3(Quarternion q, f3 vec_to_rotate)
    {
        Quarternion q_to_rot = {0.0f, vec_to_rotate};

        Quarternion rotated_q = q * q_to_rot * q.conjugate();

        return  {   rotated_q.x, 
                    rotated_q.y, 
                    rotated_q.z };
    }


    void print()
    {
        std::cout << "q0 = " << q0 << " x = " << x << " y = " << y << " z = " << z << std::endl;
        
    }
};