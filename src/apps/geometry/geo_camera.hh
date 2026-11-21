#pragma once

#include "math/vecmat.hh"
#include "math/const.hh"
#include "math/transform.hh"
#include "math/quarternion.hh"

struct FiniteProjection
{
    float AR = 1.5f;
    float fov = 0.785f;
    float zn = 0.1f;
    float zf = 100.0f;
};
// constexpr Finite    default_finite()    { return {1.5f, 0.785f, 0.1f, 100.0f};  }

struct GeoCamera
{
    m4f4 view;
    m4f4 projection;

    FiniteProjection fp;

    void init()
    {
        view = m4f4::I();
        projection = m4f4::I();

        float ar = fp.AR;
        float tan_half_fov = tanf(fp.fov / 2.0f);
        float n = fp.zn;
        float f = fp.zf;

        projection.x.x = 1 / (ar * tan_half_fov);
        projection.y.y = 1 / tan_half_fov;
        projection.z.z = - (f + n) / (f - n);
        projection.w.w = 0.0f;

        projection.z.w = - 2.0f * f * n / (f - n);
        projection.w.z = - 1.0f;
    }

    void set_pos(f3 pos)
    {
        view.x.w = -pos.x;
        view.y.w = -pos.y;
        view.z.w = -pos.z;
    }
    f3 get_pos()
    {
        return {-view.x.w, -view.y.w, -view.z.w};
    }
    f3 move(f3 pos_delta)
    {
        f3 new_pos = pos_delta + get_pos();
        set_pos(new_pos);
        return new_pos;
    }

};