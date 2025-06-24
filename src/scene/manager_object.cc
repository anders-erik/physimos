
#include "scene/manager_object.hh"






OID ManagerObject::
new_oid()
{
    if(oid_index == 0)
        throw std::runtime_error("Ran out of new OIDs");
    
    return oid_index++;
}


TagO ManagerObject::
new_tag()
{
    return {    new_oid(),
                TagO::Null  };
}


TagO ManagerObject::
new_tag(TagO::Type type)
{
    return {    new_oid(),
                type        };
}


TagO ManagerObject::
push_object(Object& object)
{
    return objects.emplace_back(object).tag;
}


Object* ManagerObject::
get_object(OID oid)
{
    if(oid == 0)
        return nullptr;

    for(Object& object : objects)
    {
        if(object.tag.oid == oid)
            return &object;
    }

    return nullptr;
}

Object* ManagerObject::
get_object(TagO tag)
{
    if(tag.is_none())
        return nullptr;

    return get_object(tag.oid);
}




// TagO ManagerObject::
// push_squado(QuadO & quado)
// {
//     return quados.emplace_back(quado).base.tag;
// }


// QuadO* ManagerObject::
// get_squado(TagO tag)
// {
//     for(QuadO& quado : quados)
//     {
//         if(tag.oid == quado.object.tag.oid)
//             return &quado;
//     }
//     return nullptr;
// }


Object& ManagerObject::
new_object()
{
    TagO tag = new_tag();
    Str name = Str{"object_"} + Str{tag.oid};
    Mesh mesh;
    return objects.emplace_back(    tag,
                                    name,
                                    mesh );
}



