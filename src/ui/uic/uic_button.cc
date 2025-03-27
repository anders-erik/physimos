
#include "ui/uic/uic_button.hh"

namespace UI::component {

UIC_Button::UIC_Button() {
    set_w("20x");
    set_h("20x");
    set_color({0.0, 0.0, 1.0, 1.0});
}

// void UIC_Button::set_click_callback(void (*new_callback_click)()){
//     callback_click = new_callback_click;
// }
void UIC_Button::set_click_action(CAction _action){
    action = _action;
}


UiResult UIC_Button::click() {
    // callback_click();
    return UiResult(true, action, this);
}


UIC_Button_Label::UIC_Button_Label(std::string label_str)
    :   string_primitive { Primitive(label_str) }
{
    appendChild(&string_primitive);
}

}