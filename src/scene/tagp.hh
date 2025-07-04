
#pragma once

/** Property ID */
typedef unsigned int    PID;

struct TagP
{
    enum Type
    {
        Null,
        Lamp,
        Camera,
    };

    PID  pid    = 0;
    Type type   = Null;

    TagP() = default;
    TagP(PID pid, Type type)
        : pid   {pid}
        , type  {type}
    {}


    bool is_lamp() { return type == Lamp;}
    bool is_camera() { return type == Camera;}
};
