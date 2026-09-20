
#include "piano_ui.hh"

#include "lib/print.hh"

#include "ui/ui4/ui.hh"
#include "apps/piano/piano_app.hh"



void handle_click_print(UINode* _node, void* _data)
{
    PianoApp* piano_app = (PianoApp*) _data;

    Print::ln("Click handler!");
}

void handle_hover_recolor(UINode* _node, void* _data)
{
    PianoApp* piano_app = (PianoApp*) _data;

    _node->color = 0x12345678;
    // _node->color = 0xFFFFFFFF;
    Print::ln("Hover handler!");
}

void handle_unhover_reset(UINode* _node, void* _data)
{

    // _node->color = 0x558855FF;
    _node->color = 0x66666666;
    Print::ln("Unhover handler!");
}


void play_current_piano_song(UINode* _node, void* _data)
{
    PianoApp* piano_app = (PianoApp*) _data;

    piano_app->piano_state.song.play(piano_app->piano_state.alsa);

    // Print::ln("Unhover handler!");
}



void PianoUI::init(PianoApp* _piano_app)
{
    PianoApp& app = *_piano_app;

    // ui.root.handle_click = handle_click_print;
    ui.root.handle_click = play_current_piano_song;
    ui.root.handle_hover = handle_hover_recolor;
    ui.root.handle_unhover = handle_unhover_reset;
}


void PianoUI::handle_events(Arr<UserInput>& _events)
{
    
}


void PianoUI::process_user_input(UserInput _user_input, PianoApp* _piano_app)
{
    Print::ln("User input recieved for processing!");

    d2 current_cursor_pos = _piano_app->input_state.cursor_sane;

    UINode* node_pointed_at = ui.get_element_at_pos(current_cursor_pos);
    if(node_pointed_at == nullptr)
    {
        return;
    }


    Print::ln("Returned valid ui node for user input processing!");



    if(_user_input.is_mouse_click())
    {
        if(node_pointed_at->handle_click != nullptr)
            node_pointed_at->handle_click(node_pointed_at, (PianoApp*)_piano_app);
    }
    else if(_user_input.is_mouse_move())
    {
        if(node_pointed_at->handle_hover != nullptr)
            node_pointed_at->handle_hover(node_pointed_at, (PianoApp*)_piano_app);
    }
    else
    {
        Print::ln("Recieved unsupported user input type.");
    }


}