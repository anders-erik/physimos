
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


struct ModelColor {
    Transform transform;

    MeshColor mesh;

    void set_base_color(f3 color);
};

struct ModelTexture {
    Transform transform;

    MeshTexture mesh;

};


struct TubeContext {
    float radius;
    int frame_point_count;
    int frame_count;
    float frame_gap;
    int vertex_count_target;
};


void model_add_cube_mesh(MeshColor& mesh);
void model_add_facelet(MeshColor& mesh, f3 color, Axis axis);


void model_center(MeshTexture& mesh);
void model_translate(MeshTexture& mesh, f3 translation);
void model_scale(MeshTexture& mesh, float scale);
/** Only supports x, y, or z axes. */
void model_rotate(MeshTexture& mesh, float angle_rad, f3 axis);

void model_generate_ground(MeshTexture& mesh);
void model_generate_tube(MeshTexture& mesh, TubeContext tube_context);

}