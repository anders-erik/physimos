#ifndef UIC_LABEL_HH
#define UIC_LABEL_HH

#include <string>
#include "ui/ui_globals.hh"
#include "ui/ui_primitive.hh"

namespace UI::component {

struct UIC_Label : public ::UI::Primitive {
    
    Primitive string_primitive;

    void render_component();

    UIC_Label(std::string initial_str);
};


struct UIC_Label_float : public Primitive {

    Primitive float_label;
    float number = 0.0f;
    int max_chars = 10;

    Primitive name_label;
    std::string name; // displayed name of float


    UIC_Label_float(std::string label_text, float initial_float, int _max_chars);

    void set_value(float value);
    void render_component();
};



}


#endif