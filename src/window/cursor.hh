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

    struct Image
    {
        GLFWcursor* arrow       = nullptr;  // Default
        GLFWcursor* cross_hair    = nullptr;  // 
        GLFWcursor* hand        = nullptr;  // 
        GLFWcursor* none        = nullptr;  // Capturing
    } glfw_images;
 
};