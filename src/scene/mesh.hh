
#pragma once

#include <variant>
#include <vector>

#include "lib/str.hh"

#include "math/transform.hh"
#include "math/polynomial.hh"

#include "opengl/texture.hh"


struct TubeContext
{
    int     circle_point_count; // points per 'circle'
    int     frame_count;        // number of circles along its long axis
    bool    closed = false;     // will close ends of tube if true
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

    MeshContext context;

    Mesh() = default;

    /** Returnes the total byte size of the vertices. [verts.size() * sizeof(Vertex)] */
    unsigned int vert_size_bytes();

    void clear();

    void center();
    void scale(float factor);
    void scale_z(float factor);
    void move_z(float factor);
    void poly_z(Polynomial<float> polynomial);
    void move(const f3& delta);

    void sheet();
    void sheet(SheetContext context);
    void quad();
    void cube();


    void tube(TubeContext t_context);
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

