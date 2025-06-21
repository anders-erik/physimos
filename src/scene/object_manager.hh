
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
struct SQuad 
{
    enum Type {
        Scene2D,
    } type = Scene2D;
    unsigned int texture_id = 0; // LEGACY

    SID sid = 0;

    bool is_scene2D() { return type == Scene2D; }

};

struct SQuadTransform
{
private:
    float AR = 1.0f;
    f3 size = {1.0f, 1.0f, 0.0f};
public:
    f3 rot;
    f3 pos;

    const f3& get_size() const { return size;};
    void set_AR(float new_AR)
    {
        AR = new_AR;
        set_width(size.x);
    }
    void set_width(float new_width) 
    {
        size.x = new_width;
        size.y = new_width / AR;
        size.z = 0.0f;
    };

    const m4f4& get_matrix()
    {
        m4f4 matrix;
        Transform::translate(matrix, pos);
        // matrix.scale(size);
        return matrix;
    }
};

struct SQuadO 
{
    Object object;
    Mesh mesh;

    SQuad texture;

    SQuadO(SQuad& scene_quad_texture);
};


namespace ObjectManager 
{

OID new_oid();

Object get_object(OID oid);

MeshO& push_mesho(Mesh& mesh);
MeshO* get_mesho(OID oid);

SQuadO& push_quado(SQuad& s_texture);
SQuadO* get_quado(OID oid);

}
