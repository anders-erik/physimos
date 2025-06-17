
#pragma once

#include <vector>

#include "lib/str.hh"

#include "math/transform.hh"

#include "opengl/texture.hh"

typedef unsigned int ObjID;

struct TriangleFaceIndeces {
    int v0;
    int v1;
    int v2;
};



struct Mesh
{
    std::vector<f3> verts;
    std::vector<TriangleFaceIndeces> faces;
};


struct Model
{
    ObjID id;
    Str name;

    Mesh mesh;
    
    f3 pos;
    f3 rot;

    unsigned int color;
};

