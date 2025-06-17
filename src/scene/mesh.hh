
#pragma once

#include <vector>

#include "lib/str.hh"

#include "math/transform.hh"

#include "opengl/texture.hh"


struct TriangleFaceIndeces {
    int v0;
    int v1;
    int v2;
};

typedef f3 Vertex;

struct Mesh
{
    std::vector<Vertex> verts;
    std::vector<TriangleFaceIndeces> faces;

    Mesh() {};

    void create_cube();
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

