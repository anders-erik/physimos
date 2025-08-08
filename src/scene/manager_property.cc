
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

TagP ManagerProperty::push_camera(const CameraView& camera)
{
    TagP tagp {new_pid(), TagP::Camera};
    cameras.push_back({tagp, camera});
    return tagp;
}

CameraView* ManagerProperty::find_camera(TagP camera_tagp)
{
    for(auto& cam_pair : cameras)
    {
        if(camera_tagp.pid == cam_pair.XX.pid)
            return &cam_pair.YY;
    }
    return nullptr;
}



