#pragma once


#include "math/vecmat.hh"

/** Window content dimensions */
struct PWDims
{
    i2 window_size_i;
    f2 window_size_f;
    f2 content_scale;
    i2 fb_size_i;
    i2 monitor_size_px;
};