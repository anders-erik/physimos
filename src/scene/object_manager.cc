
#include "scene/object_manager.hh"
#include "object_manager.hh"






OID ManagerO::
new_oid()
{
    if(oid_index == 0)
        throw std::runtime_error("Ran out of new OIDs");
    
    return oid_index++;
}


TagO ManagerO::
new_tag()
{
    return {    new_oid(),
                TagO::None};
}


TagO ManagerO::
new_tag(TagO::Type type)
{
    return {    new_oid(),
                type};
}


TagO ManagerO::
push_object(Object& object)
{
    return objects.emplace_back(object).tag;
}


Object* ManagerO::
get_object(TagO tag)
{
    if(tag.is_none())
        return nullptr;

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

Object* ManagerO::
get_object(OID oid)
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




TagO ManagerO::
push_squado(SQuadO & squado)
{
    return quados.emplace_back(squado).object.tag;
}


SQuadO* ManagerO::
get_squado(TagO tag)
{
    for(SQuadO& quado : quados)
    {
        if(tag.oid == quado.object.tag.oid)
            return &quado;
    }
    return nullptr;
}


Object ManagerO::
new_object()
{
    TagO tag = new_tag();
    Str name = Str{"object_"} + Str{tag.oid};
    Mesh mesh;
    return Object { tag,
                    name,
                    mesh };
}



