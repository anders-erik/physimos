#pragma once

#include "lib/print.hh"

#include <wayland-client.h>
#include "wayland-cursor.h"
#include "xdg_wl_extension/xdg-shell-client-protocol.h"

struct Wayland;
struct WL_State;
struct WL_Core;
struct WL_Input;
struct WL_XDG;
struct WL_FrameBuffer;



enum class WLDecorationArea
{
    CloseButton,
    MinimizeButton,
    MaximizeButton,
    TopLeft,
    Topbar,
    EdgeTop,
    EdgeTopLeft,
    EdgeTopRight,
    EdgeRight,
    EdgeLeft,
    EdgeBottom,
    EdgeBottomLeft,
    EdgeBottomRight,
    None,
};



inline void print_area_name(WLDecorationArea _area)
{
    switch (_area)
    {
    case WLDecorationArea::EdgeLeft:
            Print::ln("EdgeLeft");
        break;
    case WLDecorationArea::EdgeRight:
            Print::ln("EdgeRight");
        break;
    case WLDecorationArea::EdgeTopLeft:
            Print::ln("EdgeTopLeft");
        break;
    case WLDecorationArea::EdgeTop:
            Print::ln("EdgeTop");
        break;
    case WLDecorationArea::EdgeTopRight:
            Print::ln("EdgeTopRight");
        break;
    case WLDecorationArea::EdgeBottom:
            Print::ln("EdgeBottom");
        break;
    case WLDecorationArea::EdgeBottomRight:
            Print::ln("EdgeBottomRight");
        break;
    case WLDecorationArea::EdgeBottomLeft:
            Print::ln("EdgeBottomLeft");
        break;
    case WLDecorationArea::Topbar:
            Print::ln("Topbar");
        break;
    
    default:
            Print::ln("None");
        break;
    }
}