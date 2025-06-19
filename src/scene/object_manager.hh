
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


// Scene texture
struct SQuadTexture 
{
    enum Type {
        Scene2D,
    };
    unsigned int texture_id = 0;
};

struct QuadO 
{
    Object object;
    Mesh mesh;

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
