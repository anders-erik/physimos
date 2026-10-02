#pragma once

#include "ui/ui4/ui.hh"
// #include "ui/ui4/ui_defs.hh"
// #include input

#include "io/input/user_input.hh"
#include "piano_ui_defs.hh"



// template <typename T>
struct Waveform;
struct UIC_Waveform;

#define UIC_WAVEFORM_CALLBACK_PARAMETERS (UINode* node, UserInput user_input, UIC_Waveform* uic_waveform)

void click_callback_waveform(UINode* node, UserInput user_input, UIC_Waveform* uic_waveform);





// template <typename T>
struct UIC_Waveform
{
    Waveform* value;

    UINode* uic_root;
    
// template <typename T>
    UIC_Waveform(Waveform* _value)
    {
        value = _value;
    }

    void init(Waveform* _value);
    void update(Waveform* _value);

};
