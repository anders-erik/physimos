
#include "scene/manager_physics.hh"




IDPhy ManagerPhysics::
new_id()
{
    if(phy_id_count == 0)
        throw "Ran out of new PIDs";
    
    return phy_id_count++;
}


TagPhy ManagerPhysics::
new_tagp()
{
    return {    new_id(),
                TagPhy::Null  };
}



IDPhy ManagerPhysics::push_physics(const Physics & physics)
{
    IDPhy id = new_id();
    data.push_back({id, physics});
    return id;
}

Physics * ManagerPhysics::find_physics(IDPhy physics_id)
{
    for(auto& physics_pair : data)
    {
        if(physics_pair.XX == physics_id)
            return &physics_pair.YY;
    }
    return nullptr;
}



