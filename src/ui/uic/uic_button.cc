
#include "ui/uic/uic_button.hh"

namespace UI::component {

UIC_Button::UIC_Button() {
    

    set_w("20x");
    set_h("20x");
    set_color({0.0, 0.0, 1.0, 1.0});
}

void UIC_Button::set_click_callback(void (*new_callback_click)()){
    callback_click = new_callback_click;
}


UiResult UIC_Button::click() {
    callback_click();
    return UiResult(true, CAction::None, this);
}

}