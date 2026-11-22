#pragma once

#include "math/vecmat.hh"


/** 
    Three projection types: finite perspective, infinite perspective, and Othographic.
    Matrices are equal to GLTF specs. */
struct Projection
{
    struct Finite
    {
        float AR;
        float fov;
        float zn;
        float zf;
    };
    struct Infinite
    {
        float AR;
        float fov;
        float zn;
    };
    struct Ortho
    {
        float xmax;
        float ymax;
        float zn;
        float zf;
    };

    constexpr Finite    default_finite()    { return {1.5f, 0.785f, 0.1f, 100.0f};  }
    constexpr Infinite  default_infinite()  { return {1.5f, 0.785f, 0.1f};          }
    constexpr Ortho     default_ortho()     { return {30.0f, 20.0f, 0.1f, 100.0f};  }


    enum class Type
    {
        Finite,
        Infinite,
        Ortho,
    };
    union Value
    {
        Finite      finite;
        Infinite    infinite;
        Ortho       ortho;
    };

    void set_viewport(int new_width, int new_height)
    {
        float ar = (float) new_width / (float) new_height;

        if(type == Type::Finite)
            value.finite.AR = ar;
        else if(type == Type::Infinite)
            value.infinite.AR = ar;
        else if(type == Type::Ortho)
            value.ortho.xmax = value.ortho.ymax * ar;
        else 
            throw "Updated camera viewport without a matching type.";
    }

    float AR()
    {
        if(type == Type::Finite)
            return value.finite.AR;
        else if(type == Type::Infinite)
            return value.infinite.AR;
        else if(type == Type::Ortho)
            return value.ortho.xmax / value.ortho.ymax;
        else 
            throw "Get camera AR without a matching type.";
    }

    bool is_perspective() { return type == Type::Finite || type == Type::Infinite; }

    void set_finite(const Finite& finite)
    {
        value.finite = finite;
        type = Type::Finite;
    }
    void set_finite()
    {
        value.finite = default_finite();
        type = Type::Finite;
    }
    void set_infinite(const Infinite& infinite)
    {
        value.infinite = infinite;
        type = Type::Infinite;
    }
    void set_infinite()
    {
        value.infinite = default_infinite();
        type = Type::Infinite;
    }
    void set_ortho(const Ortho& ortho)
    {
        value.ortho = ortho;
        type = Type::Ortho;
    }
    void set_ortho()
    {
        value.ortho = default_ortho();
        type = Type::Ortho;
    }

    m4f4 matrix()
    {
        m4f4 mat;
        mat.set_to_I();

        if(type == Type::Finite)
        {
            float ar = value.finite.AR;
            float tan_half_fov = tanf(value.finite.fov / 2.0f);
            float n = value.finite.zn;
            float f = value.finite.zf;

            mat.x.x = 1 / (ar * tan_half_fov);
            mat.y.y = 1 / tan_half_fov;
            mat.z.z = - (f + n) / (f - n);
            mat.w.w = 0.0f;

            mat.z.w = - 2.0f * f * n / (f - n);
            mat.w.z = - 1.0f;
        }
        else if(type == Type::Infinite)
        {
            float ar = value.finite.AR;
            float tan_half_fov = tanf(value.finite.fov / 2.0f);
            float n = value.infinite.zn;

            mat.x.x = 1 / (ar * tan_half_fov);
            mat.y.y = 1 / tan_half_fov;
            mat.z.z = - 1;
            mat.w.w = 0.0f;

            mat.z.w = - 2.0f * n;
            mat.w.z = - 1.0f;
        }
        else if(type == Type::Ortho)
        {
            float r = value.ortho.xmax;
            float t = value.ortho.ymax;
            float n = value.ortho.zn;
            float f = value.ortho.zf;

            mat.x.x = 1 / r;
            mat.y.y = 1 / t;
            mat.z.z = 2 / (n-f);
            mat.w.w = 1.0f;

            mat.z.w = (f+n) / (n-f);
        }
        else 
        {
            throw "Updated camera projection matrix without a matching type.";
        }

        return mat;
    }

    Value   value;
    Type    type;
};