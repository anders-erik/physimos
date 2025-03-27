#ifndef UIC_BUTTON_HH
#define UIC_BUTTON_HH

#include <string>
#include "ui/ui_globals.hh"
#include "ui/ui_primitive.hh"

namespace UI::component {

struct UIC_Button : public ::UI::Primitive {
    /** Plain callback with no arguments no return value */
    // void (*callback_click)(void) = nullptr;
    // void set_click_callback(void (*new_callback_click)());


    CAction action = CAction::None;
    void set_click_action(CAction _action);

    /** Returns set CAction. */
    UiResult click();

    UIC_Button() ;
};

struct UIC_Button_Label : public UIC_Button {
    Primitive string_primitive;

    UIC_Button_Label(std::string label_str);
};


}


#endif