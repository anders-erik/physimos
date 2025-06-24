
#pragma once

#include <vector>
#include "math/transform.hh"

#include "opengl/texture.hh"


namespace model {



struct TriangleFaceIndeces {
    int v0;
    int v1;
    int v2;
};


struct Vertex 
{
    f3 pos;

    Vertex() {};
    Vertex(f3 pos) : pos {pos} {};
};

struct VertexColor {
    f3 pos;
    f3 color;
    f3 normal;

    VertexColor(f3 pos) : pos {pos} {};
    VertexColor() {};
};

struct VertexT {
    f3 pos;
    f3 normal;
    f2 tex;

    VertexT() {};
    VertexT(f3 pos) : pos {pos} {};
};


struct MeshColor 
{
    std::vector<VertexColor> vertices;
    std::vector<TriangleFaceIndeces> faces;
};


struct MeshT 
{
    std::vector<VertexT> vertices;
    std::vector<TriangleFaceIndeces> faces;
};


struct ModelColor 
{
    MeshColor mesh;
    Transform transform;

    void set_base_color(f3 color);
};


struct ModelT 
{
    MeshT mesh;
    Transform transform;

    opengl::Textures texture;
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


void model_center(MeshT& mesh);
void model_translate(MeshT& mesh, f3 translation);
void model_scale(MeshT& mesh, float scale);
/** Only supports x, y, or z axes. */
void model_rotate(MeshT& mesh, float angle_rad, f3 axis);

void model_generate_ground(MeshT& mesh);
void model_generate_tube(MeshT& mesh, TubeContext tube_context);

}