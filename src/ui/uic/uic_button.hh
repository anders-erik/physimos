#ifndef UIC_BUTTON_HH
#define UIC_BUTTON_HH

#include "ui/ui_globals.hh"
#include "ui/ui_primitive.hh"

namespace UI::component {

struct UIC_Button : public ::UI::Primitive {
    /** Plain callback with no arguments no return value */
    void (*callback_click)(void) = nullptr;

    void set_click_callback(void (*new_callback_click)());

    /** Calls the currently set callback function */
    UiResult click();

    // UIC_Button(void (*new_callback_click)() ) ;
    UIC_Button( ) ;
};


}


#endif