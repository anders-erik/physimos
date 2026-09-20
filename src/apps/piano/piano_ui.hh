
#pragma once

#include "lib/arr.hh"

#include "io/input/user_input.hh"
#include "ui/ui4/ui.hh"


struct UI;
struct PianoApp;


class PianoUI
{

public:
    UI ui;
    

    PianoUI() {}

    void init(PianoApp* _piano_app);
    void handle_events(Arr<UserInput>& _events);

    void process_user_input(UserInput _user_input, PianoApp* _piano_app);

    bool contains_pointer_pos(d2 _pointer_pos)
    {
        return (ui.get_element_at_pos(_pointer_pos) == nullptr) ? false : true;
    }
    // bool has_current_target() { return (current_target == nullptr) ? false : true; }

    void set_no_active_events()
    {
        ui.root.undim();
    }
};
