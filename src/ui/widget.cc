
#pragma once

#include "lib/pair.hh"

#include "rend/pui/renderer_base.hh"

#include "ui/string.hh"

#include "scene/manager.hh"
#include "scene2D/scene2D.hh"
#include "scene2D/quadS2D.hh"

#include "widget.hh"


namespace UI {


bool Widget::has_cursor(f2 cursor_sane)
{
    if(!frame.contains_point(cursor_sane))
        return false;

    this->cursor_sane = cursor_sane;

    return true;
}




}