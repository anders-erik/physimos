
#pragma once

#include "lib/str.hh"


enum class ScrollDirection
{
    Up,
    Down
};


struct ScrollInput
{
    ScrollDirection scroll_direction;

    ScrollInput(ScrollDirection _scroll_direction)
        :   scroll_direction {_scroll_direction}
    {
    }
};


