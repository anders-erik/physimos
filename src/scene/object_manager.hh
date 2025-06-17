
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


namespace ObjectManager 
{

OID new_oid();
MeshO& push_mesho(Mesh& mesh);
MeshO* get_mesho(OID oid);


}
