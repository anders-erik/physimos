
#include "scene/manager_property.hh"
#include "manager_property.hh"



PID ManagerProperty::
new_pid()
{
    if(pid_count == 0)
        throw "Ran out of new PIDs";
    
    return pid_count++;
}


TagP ManagerProperty::
new_tagp()
{
    return {    new_pid(),
                TagP::Null  };
}


TagP ManagerProperty::
new_tagp(TagP::Type type)
{
    return {    new_pid(),
                type        };
}


TagP ManagerProperty::
push_lamp(const Lamp& lamp)
{
    TagP tagp {new_pid(), TagP::Lamp};
    lamps.push_back({tagp, lamp});
    return tagp;
}


Lamp* ManagerProperty::find_lamp(TagP lamp_tagp)
{
    for(auto& lamp_pair : lamps)
    {
        if(lamp_tagp.pid == lamp_pair.XX.pid)
            return &lamp_pair.YY;
    }
    return nullptr;
}

TagP ManagerProperty::push_camera(const CameraFree& camera)
{
    TagP tagp {new_pid(), TagP::Camera};
    cameras.push_back({tagp, camera});
    return tagp;
}

CameraFree* ManagerProperty::find_camera(TagP camera_tagp)
{
    for(auto& cam_pair : cameras)
    {
        if(camera_tagp.pid == cam_pair.XX.pid)
            return &cam_pair.YY;
    }
    return nullptr;
}

TagP ManagerProperty::push_physics(const Physics & physics)
{
    TagP new_tag = {new_pid(), TagP::Physics};
    physicss.push_back({new_tag, physics});
    return new_tag;
}

Physics * ManagerProperty::find_physics(TagP physics_tagp)
{
    for(auto& physics_pair : physicss)
    {
        if(physics_pair.XX.pid == physics_tagp.pid)
            return &physics_pair.YY;
    }
    return nullptr;
}



