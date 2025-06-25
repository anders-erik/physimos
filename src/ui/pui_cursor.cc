
// #include "ui/pui.hh"
#include "pui_cursor.hh"

namespace UI {


bool PUICursor::
is_targeting_widget()
{
    if(is_grabbing_widget())
        return true;
    if(is_hovering_widget())
        return true;
    return false;
}

bool PUICursor::
is_targeted_widget(Widget* widget)
{
    if(is_grabbing(widget))
        return true;
    if(is_hovering(widget))
        return true;
    return false;
}

void PUICursor::grab(Widget * widget)
{
    grabbed_widget = widget;
}

void PUICursor::hover(Widget * widget)
{
    hovered_widget = widget;
}

bool PUICursor::
is_hovering_widget()
{
    return hovered_widget == nullptr ? false : true;
}

bool PUICursor::
is_grabbing_widget()
{
    return grabbed_widget == nullptr ? false : true;
}

bool PUICursor::
is_grabbing(Widget* widget)
{
    return grabbed_widget == widget ? true : false;
}

bool PUICursor::is_hovering(Widget * widget)
{
    return hovered_widget == widget ? true : false;
}

void PUICursor::clear_hover()
{
    hovered_widget = nullptr;
}

void PUICursor::clear_grabbed_widget()
{
    grabbed_widget = nullptr;
}

void PUICursor::clear_all()
{
    clear_hover();
    clear_grabbed_widget();

    // TODO: clear grab in widget update if no grab reset event since last update!
}

void PUICursor::handle_event_result(InputResponse event_result, Widget* widget)
{
    if(event_result.is_mouse_grab())
    {
        grab(widget);
    }
    else 
    {
        clear_grabbed_widget();
    }
}





bool PUICursor::is_targeted_by_cursor()
{
    if(is_targeting_widget())
        return true;

    return false;
}

bool PUICursor::is_grabbing_cursor()
{
    if(is_grabbing_widget())
        return true;

    return false;
}







}

