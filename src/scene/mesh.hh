
#pragma once

#include <variant>
#include <vector>

#include "lib/str.hh"

#include "math/transform.hh"
#include "math/polynomial.hh"
#include "math/aabb.hh"

#include "opengl/texture.hh"
#include "opengl/texture_unit.hh"
#include "opengl/color.hh"


struct TubeContext
{
    int     frame_count;        // number of circles along its long axis
    int     circle_point_count; // points per 'circle'
};

struct CircleContext
{
    uint CC;   // Circle count
    uint PPC;  // Points per circle
};

struct SheetContext
{
    float   width;
    int     width_count;
};

using MeshContext = std::variant<   TubeContext, 
                                    SheetContext    >;


struct TriangleFaceIndeces 
{
    int v0;
    int v1;
    int v2;
};

typedef f3 Vertex;


struct Mesh
{
    std::vector<Vertex> verts;
    std::vector<TriangleFaceIndeces> faces;

    std::vector<f3> normals;
    std::vector<f3> colors;

    uint color_mesh = 0xffffffff;
    Colorf3 default_color = {1.0f, 1.0f, 1.0f};

    Mesh() = default;

    /** Returns the total byte size of the vertices. [verts.size() * sizeof(Vertex)] */
    unsigned int vert_size_bytes();

    void clear();

    void merge(const Mesh& mesh_2);

    f3 get_center();

    void center();
    void scale(float factor);
    void scale(f3 scale);
    void scale_z(float factor);
    void move_z(float delta);
    Mesh& move(const f3& delta);
    void poly_r(Polynomial<float> poly);
    void color(ColorInt color_i);

    void sheet();
    void sheet(SheetContext context);
    void quad();
    void cube();
    void aabb(AABBf ab);

    void circle(CircleContext c_ctx);
    void circle_poly_r(CircleContext c_ctx, Polynomial<float> poly);

    void tube(TubeContext t_context);
    void tube_poly_z(TubeContext t_context, Polynomial<float> polynomial);
    void tube_color(TubeContext t_context, ColorInt base_color, ColorInt top_color);
};



struct Model
{
    Mesh mesh;
    std::vector<f3> normals;
    std::vector<f2> text_coords;

    // Texture texture;
    
    f3 pos;
    f3 rot;

    unsigned int color = 0x000000ff; // base color if no texture is set
};

