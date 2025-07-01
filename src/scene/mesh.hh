
#pragma once

#include <vector>

#include "lib/str.hh"

#include "math/transform.hh"

#include "opengl/texture.hh"


struct TubeContext
{
    float   radius;
    int     frame_point_count;
    int     frame_count;
    float   frame_gap;
    bool    closed;
    f2      domain;
};

struct SheetContext
{
    float   width;
    int     width_count;
};


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

    Mesh() = default;

    /** Returnes the total byte size of the vertices. [verts.size() * sizeof(Vertex)] */
    unsigned int vert_size_bytes();

    void clear();

    void sheet();
    void sheet(SheetContext context);
    void tube();
    void quad();
    void cube();

    void center();
    void scale(float factor);
    void move(const f3& delta);
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

