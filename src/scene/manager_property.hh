
#pragma once


#include <vector>

#include "lib/str.hh"


#include "scene/camera.hh"
#include "scene/lamp.hh"

#include "scene/tagp.hh"

#include "lib/pair.hh"

struct ManagerProperty
{
    PID pid_count = 1;

    std::vector<Pair<TagP, Lamp>> lamps;
    std::vector<Pair<TagP, CameraFree>>cameras;

    PID new_pid();
    TagP new_tagp();
    TagP new_tagp(TagP::Type type);

    TagP push_lamp(const Lamp& lamp);
    Lamp* find_lamp(TagP lamp_tagp);

    TagP push_camera(const CameraFree& camera);
    CameraFree* find_camera(TagP camera_tagp);

};




