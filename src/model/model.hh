
#pragma once

#include <vector>
#include "math/transform.hh"


namespace xpeditor {



struct TriangleFaceIndeces {
    int v0;
    int v1;
    int v2;
};

typedef struct VertexBase {
    f3 pos;
    f3 normal;

    VertexBase(f3 _pos, f3 _normal) : pos {_pos}, normal {_normal} {};
    VertexBase(f3 _pos) : pos {_pos}, normal {f3()} {};
    VertexBase() : pos {f3()}, normal {f3()} {};
} VertexBase;


typedef struct VertexColor {
    f3 pos;
    f3 color;
    f3 normal;

    VertexColor(f3 pos) : pos {pos}, color {f3()}, normal {f3()} {};
    VertexColor() : pos {f3()}, color {f3()}, normal {f3()} {};
} VertexColor;

typedef struct VertexTexture {
    f3 pos;
    f3 normal;
    f2 tex;

    VertexTexture(f3 pos) : pos {pos}, normal {f3()}, tex { f2()} {};
    VertexTexture() : pos {f3()}, normal {f3()}, tex { f2()} {};
} VertexTexture;


typedef struct MeshColor {
    std::vector<VertexColor> vertices;
    std::vector<TriangleFaceIndeces> faces;
} MeshColor;

typedef struct MeshTexture {
    std::vector<VertexTexture> vertices;
    std::vector<TriangleFaceIndeces> faces;
} MeshTexture;


struct Model {
    Transform transform;

    MeshColor mesh;

    void set_transform_matrix();
    void set_transform_matrix_anim();
    void set_base_color(f3 color);
};




void model_add_cube_mesh(MeshColor& mesh);
void model_add_facelet(MeshColor& mesh, f3 color, Axis axis);



}