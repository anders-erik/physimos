
#pragma once

#include <vector>

#include "lib/str.hh"

#include "math/transform.hh"

#include "opengl/texture.hh"

#include "scene/mesh.hh"


typedef unsigned int OID;





struct Object 
{
    enum Type {
        Mesh,
        Quad,
        None,
    };

    OID id = 0;
    Str name;
    Type type;
};


struct MeshO
{
    Object object;
    Mesh mesh;

    MeshO(Mesh& mesh);
};

/** Scene transform */
struct STransform 
{
    f3 rot;
    f3 scale;
    f3 pos;

};

// Scene texture
struct SQuadTexture 
{
    unsigned int texture_id = 0;
    std::vector<f2> text_coords; // 4 verts in quad mesh

    SQuadTexture();
};

struct QuadO 
{
    Object object;
    Mesh mesh;
    STransform transform;
    SQuadTexture texture;

    QuadO(SQuadTexture& scene_quad_texture);
};


namespace ObjectManager 
{

OID new_oid();

Object get_object(OID oid);

MeshO& push_mesho(Mesh& mesh);
MeshO* get_mesho(OID oid);

QuadO& push_quado(SQuadTexture& s_texture);
QuadO* get_quado(OID oid);

}
