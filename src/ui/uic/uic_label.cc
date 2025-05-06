
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
        max_chars { _max_chars },
        name_label { Primitive(label_text) }
{
    // set_value(initial_float);

    set_w("100%");
    set_h("20x");
    set_color({0.0, 0.0, 0.0, 0.1});

    name_label.set_w("40%");
    name_label.set_x("<10%");
    appendChild(&name_label);
    float_label.set_w("50%");
    float_label.set_x("<50%");
    float_label.set_color({0.0, 0.0, 0.0, 0.4});
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




UIC_Label_f2::UIC_Label_f2(std::string label_text, f2 initial_f2, int _max_chars)
    :   Primitive (),
        float_label_x { Primitive(std::to_string(initial_f2.x)) },
        float_label_y { Primitive(std::to_string(initial_f2.y)) },
        max_chars { _max_chars },
        name_label { Primitive(label_text) }
{
    // set_value(initial_float);

    set_w("100%");
    set_h("20x");
    set_color({0.0, 0.0, 0.0, 0.1});

    name_label.set_w("40%");
    name_label.set_x("<5%");
    appendChild(&name_label);
    float_label_x.set_w("25%");
    float_label_x.set_x("<50%");
    float_label_x.set_color({0.0, 0.0, 0.0, 0.4});
    appendChild(&float_label_x);
    float_label_y.set_w("25%");
    float_label_y.set_x("<75%");
    float_label_y.set_color({0.0, 0.0, 0.0, 0.4});
    appendChild(&float_label_y);
}

void UIC_Label_f2::set_value(f2 values){
    
    if(numbers.x != values.x || numbers.y != values.y){
        numbers = values;
        std::string _float_str_x = std::to_string(numbers.x);
        std::string _float_str_y = std::to_string(numbers.y);

        float_label_x.str_setString(_float_str_x.substr(0, max_chars));
        float_label_y.str_setString(_float_str_y.substr(0, max_chars));
    }

}
void UIC_Label_f2::render_component(){
    render();
    name_label.render();
    float_label_x.render();
    float_label_y.render();
}

}