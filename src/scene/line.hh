#pragma once

#include "math/vecmat.hh"
#include <functional>

#include "scene/manager_3D.hh"

struct Line
{
    f3 p_start;
    f3 p_end;
};

struct SPLine
{
    Line line;
};



// std::function<void()> line_fn = 