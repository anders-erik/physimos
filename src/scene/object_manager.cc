
#include "scene/object_manager.hh"
#include "object_manager.hh"





namespace ObjectManager
{

static OID oid_index = 1;

static std::vector<Object> objects;
static std::vector<SQuadO> quados;

OID new_oid()
{
    if(oid_index == 0)
        throw std::runtime_error("Ran out of new OIDs");
    
    return oid_index++;
}


TagO new_tag()
{
    return {    new_oid(),
                TagO::None};
}

TagO new_tag(TagO::Type type)
{
    return {    new_oid(),
                type};
}

TagO push_object(Object& object)
{
    return objects.emplace_back(object).tag;
}

Object* get_object(TagO tag)
{
    if(tag.is_base())
    {
        for(Object& object : objects)
        {
            if(object.tag.oid == tag.oid)
                return &object;
        }
    }

    if(tag.is_quad())
    {
        for(SQuadO& quado :  quados)
        {
            if(quado.object.tag.oid == tag.oid)
                return &quado.object;
        }
    }

    return nullptr;
}

Object* get_object(OID oid)
{
    if(oid == 0)
        return nullptr;

    for(Object& object : objects)
    {
        if(object.tag.oid == oid)
            return &object;
    }

    for(SQuadO& quado :  quados)
    {
        if(quado.object.tag.oid == oid)
            return &quado.object;
    }

    return nullptr;
}




TagO
push_squado(SQuadO & squado)
{
    return quados.emplace_back(squado).object.tag;
}


SQuadO* 
get_squado(TagO tag)
{
    for(SQuadO& quado : quados)
    {
        if(tag.oid == quado.object.tag.oid)
            return &quado;
    }
    return nullptr;
}


Object new_object()
{
    TagO tag = new_tag();
    Str name = Str{"object_"} + Str{tag.oid};
    Mesh mesh;
    return Object { tag,
                    name,
                    mesh };
}


}

