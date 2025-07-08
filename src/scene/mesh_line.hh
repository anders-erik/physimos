
#pragma once

#include <variant>
#include <vector>

#include "lib/str.hh"

#include <cmath>
#include "math/const.hh"
#include "math/vecmat.hh"
#include "math/transform.hh"
#include "math/polynomial.hh"
#include "math/aabb.hh"

#include "opengl/texture.hh"
#include "opengl/texture_unit.hh"
#include "opengl/color.hh"



struct Line
{
    f3 a;
    f3 b;

    Line() = default;
    Line(f3 a, f3 b)
        : a {a}
        , b {b}
    {}
};



struct MeshLine
{
    std::vector<Line> lines;
    
    Colorf3 default_color = {1.0f, 1.0f, 1.0f};

    MeshLine() = default;

    /** Returns the total byte size of the vertices. [verts.size() * sizeof(Vertex)] */
    unsigned int vert_size_bytes();

    void color(ColorInt color_i);

    void append(const MeshLine& mesh_to_append);

    void rotate_x(float angle);
    void rotate_y(float angle);
    void rotate_z(float angle);

    void sheet();
    void quad();
    void cube_centered();
    void cube_origin_aligned();

    void aabb(AABBf aabb);
    void bounding_sphere(float radius);

    void circle(float radius, uint pts);
};


