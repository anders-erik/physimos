
#include "ui/uic/uic_label.hh"

namespace UI::component {

UIC_Label::UIC_Label(std::string initial_str) 
    :   string_primitive { Primitive(initial_str) }
{

    set_w("20x");
    set_h("20x");
    set_color({0.0, 0.0, 0.0, 0.0});

    appendChild(&string_primitive);

}

void UIC_Label::render_component(){
    render();
    string_primitive.render();
}

UIC_Label_float::UIC_Label_float(float initial_float, int _max_chars)
    :   UIC_Label(std::to_string(initial_float)),
        max_chars { _max_chars }
{
    set_value(initial_float);
}

void UIC_Label_float::set_value(float value){
    
    if(number != value){
        number = value;
        std::string _float_str = std::to_string(number);
        string_primitive.str_setString(_float_str.substr(0, max_chars));
    }

}


}