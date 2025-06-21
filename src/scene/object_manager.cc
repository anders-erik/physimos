
#include "object_manager.hh"



MeshO::MeshO(Mesh& mesh)
{
    object.id = ObjectManager::new_oid();
    object.type = Object::Mesh;
    object.name = Str("mesh_") + Str::to_str_int(object.id);

    this->mesh = mesh;
}



namespace ObjectManager
{

static OID oid_index = 1;

static std::vector<MeshO> meshos;
static std::vector<SQuadO> quados;

OID new_oid()
{
    if(oid_index == 0)
        throw std::runtime_error("Ran out of new OIDs");
    
    return oid_index++;
}

Object get_object(OID oid)
{
    for(MeshO& mesho : meshos)
    {
        if(mesho.object.id == oid)
            return mesho.object;
    }

    for(SQuadO& quado :  quados)
    {
        if(quado.object.id == oid)
            return quado.object;
    }

    return Object();
}

MeshO& push_mesho(Mesh& mesh)
{
    return meshos.emplace_back(mesh);
}

MeshO* get_mesho(OID oid)
{
    for(MeshO& mesho : meshos)
    {
        if(mesho.object.id == oid)
            return &mesho;
    }

    return nullptr;
}



SQuadO& 
push_quado(SQuad & s_q_texture)
{
    return quados.emplace_back(s_q_texture);
}


SQuadO* 
get_quado(OID oid)
{
    for(SQuadO& quado : quados)
    {
        if(oid == quado.object.id)
            return &quado;
    }
    return nullptr;
}





}

SQuadO::SQuadO(SQuad& scene_quad_texture)
{
    object.id = ObjectManager::new_oid();
    object.type = Object::Quad;
    object.name = Str("quad_") + Str::to_str_int(object.id);

    mesh.create_quad();

    texture = scene_quad_texture;
}

