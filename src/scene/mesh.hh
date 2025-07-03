
#pragma once

#include <variant>
#include <vector>

#include "lib/str.hh"

#include "math/transform.hh"
#include "math/polynomial.hh"

#include "opengl/texture.hh"


struct TubeContext
{
    int     frame_count;        // number of circles along its long axis
    int     circle_point_count; // points per 'circle'
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
    uint color = 0xffffffff;

    Mesh() = default;

    /** Returns the total byte size of the vertices. [verts.size() * sizeof(Vertex)] */
    unsigned int vert_size_bytes();

    void clear();

    void center();
    void scale(float factor);
    void scale(f3 scale);
    void scale_z(float factor);
    void move_z(float factor);
    void move(const f3& delta);

    void sheet();
    void sheet(SheetContext context);
    void quad();
    void cube();

    void tube(TubeContext t_context);
    void tube_poly_z(TubeContext t_context, Polynomial<float> polynomial);
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

