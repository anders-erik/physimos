#pragma once

struct GLFWcursor;


struct Cursor
{
    enum State 
    {
        NORMAL,
        INVERTED,
        HAND,
        CAPTURE,
    } state = NORMAL;
 
};

