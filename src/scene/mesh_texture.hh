
#pragma once

#include <vector>

#include "math/vecmat.hh"


typedef f2 VertexTexture;


struct MeshTexture
{
    std::vector<VertexTexture> verts;

    MeshTexture() {};

    /** Returnes the total byte size of the vertices. [verts.size() * sizeof(Vertex)] */
    unsigned int vert_size_bytes();

    void create_cube();
    void create_quad();
};


