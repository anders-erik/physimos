
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

UIC_Label_float::UIC_Label_float(std::string label_text, float initial_float, int _max_chars)
    :   Primitive (),
        float_label { Primitive(std::to_string(initial_float)) },
        name_label { Primitive(label_text) },
        max_chars { _max_chars }
{
    // set_value(initial_float);

    set_w("100%");
    set_h("20x");
    set_color({0.0, 0.0, 0.0, 0.0});

    name_label.set_w("20x");
    name_label.set_x("<5x");
    appendChild(&name_label);
    float_label.set_w("30x");
    float_label.set_x("<55x");
    float_label.set_color({0.0, 0.0, 0.0, 0.5});
    appendChild(&float_label);
}

void UIC_Label_float::set_value(float value){
    
    if(number != value){
        number = value;
        std::string _float_str = std::to_string(number);
        float_label.str_setString(_float_str.substr(0, max_chars));
    }

}
void UIC_Label_float::render_component(){
    render();
    name_label.render();
    float_label.render();
}


}