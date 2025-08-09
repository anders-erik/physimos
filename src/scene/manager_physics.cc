
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



IDPhy ManagerPhysics::push_physics(Physics & physics)
{
    IDPhy id = new_id();

    physics.id = id;

    if(physics.static_flag)
        data_stat.push_back({id, physics});
    else
        data_dyn.push_back({id, physics});

    return id;
}

Physics * ManagerPhysics::find_physics(IDPhy physics_id)
{
    if(physics_id == 0)
        return nullptr;

    for(auto& physics_pair : data_dyn)
    {
        if(physics_pair.XX == physics_id)
            return &physics_pair.YY;
    }

    for(auto& physics_pair : data_stat)
    {
        if(physics_pair.XX == physics_id)
            return &physics_pair.YY;
    }
    return nullptr;
}



