
#pragma once

#include "math/vecmat.hh"
#include "model/model.hh"

#include "opengl/program.hh"

#include "scene/mesh.hh"

#include "scene/object_manager.hh"


class ProgramObjectIDs : protected opengl::Program
{

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

public:

    ProgramObjectIDs() : Program("3D/object_ids") {};

    void init();

    void set_camera_view_projection(m4f4 persective_mat, m4f4 view_mat);

    void render(Mesh& mesh, OID id);

    f4 oid_to_vec4(OID oid);
    OID vec4_to_oid(f4 vec4);

};



