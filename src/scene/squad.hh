#pragma once

#include "vecmat.hh"

#include "scene/sid.hh"

#include "scene/object.hh"


// Scene texture - keeps refences to texture type for querying
struct SQuad 
{
    enum Type {
        _Scene2D,
    } type = _Scene2D;
    unsigned int texture_id = 0; // LEGACY

    SID sid = 0;

    bool is_scene2D() { return type == _Scene2D; }

};


struct SQuadO 
{
    Object object;
    SQuad squad;

    SQuadO(Object object, SQuad squad)
        :   object  {object},
            squad   {squad}
    {
    }
};


// struct SQuadTransform
// {
// private:
//     float AR = 1.0f;
//     f3 size = {1.0f, 1.0f, 0.0f};
// public:
//     f3 rot;
//     f3 pos;

//     const f3& get_size() const { return size;};
//     void set_AR(float new_AR)
//     {
//         AR = new_AR;
//         set_width(size.x);
//     }
//     void set_width(float new_width) 
//     {
//         size.x = new_width;
//         size.y = new_width / AR;
//         size.z = 0.0f;
//     };

//     const m4f4 get_matrix()
//     {
//         m4f4 matrix;
//         return matrix;
//     }
// };