
#pragma once

#include <vector>

#include "lib/str.hh"

#include "math/transform.hh"

#include "opengl/texture.hh"

#include "scene/mesh.hh"
#include "scene/scene_base.hh"


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

    bool operator!=(const Object& rhs) { return !(*this == rhs); }
    bool operator==(const Object& rhs)
    {
        return  id      == rhs.id      &&
                name    == rhs.name    &&
                type    == rhs.type;
    }

    bool is_quad() {return type == Quad ? true : false;}
    bool is_mesh() {return type == Mesh ? true : false;}

};


struct MeshO
{
    Object object;
    Mesh mesh;

    MeshO(Mesh& mesh);
};


// Scene texture - keeps refences to texture type for querying
struct SQuadTexture 
{
    enum Type {
        Scene2D,
    } type = Scene2D;
    unsigned int texture_id = 0; // LEGACY

    SID sid = 0;

    bool is_scene2D() { return type == Scene2D; }

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
