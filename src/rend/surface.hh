#pragma once

#include <variant>

#include "lib/defs.hh"
#include "lib/print.hh"

#include "rend/bitmap.hh"


/** Placeholder for OpenGL-renderable texture object */
struct OpenGLtext {};

typedef std::variant<Bitmap, OpenGLtext> SurfaceData;

struct LineNDC
{
    f3 p_start;
    f3 p_end;
};

class surface
{
    enum Backend {
        EGL,
        Bitmap,
        VkSurfaceKHR,
    };
    Backend backend = EGL;
    SurfaceData data;

public:

    u64 size_byte = 0;
    u32 width = 0;
    u32 height = 0;

    void lineNDC(LineNDC line, u32 color)
    {
        switch (backend)
        {
        case EGL:
            Print::line("Drawing line with EGL backend (Dummy function).");
            break;
        
        default:
            throw std::runtime_error("Drawing line with selected backend not yet implemented.");
            break;
        }
    }
    
};