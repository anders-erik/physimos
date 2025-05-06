#ifndef UIC_LABEL_HH
#define UIC_LABEL_HH

#include <string>
#include "ui/ui_globals.hh"
#include "ui/ui_primitive.hh"
#include "math/vecmat.hh"

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


struct UIC_Label_f2 : public Primitive {

    Primitive float_label_x;
    Primitive float_label_y;
    f2 numbers;
    int max_chars = 10;

    Primitive name_label;
    std::string name; // displayed name of float


    UIC_Label_f2(std::string label_text, f2 initial_f2, int _max_chars);

    void set_value(f2 values);
    void render_component();
};



}


#endif