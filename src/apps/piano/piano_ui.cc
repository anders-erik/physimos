
#include "piano_ui.hh"

#include "lib/print.hh"

#include "ui/ui4/ui.hh"
#include "apps/piano/piano_app.hh"

#define _CAST_PIANO_UI_ (void (*)(UINode*, void*))

void stop_button_hover(UINode* _node, PianoApp* piano_app)
{
    _node->color = 0xFFBB3333;
}
void stop_button_unhover(UINode* _node, PianoApp* piano_app)
{
    _node->color = 0xFF994444;
}
void start_button_hover(UINode* _node, PianoApp* piano_app)
{
    _node->color = 0xFF33BB33;
}
void start_button_unhover(UINode* _node, PianoApp* piano_app)
{
    _node->color = 0xFF449944;
}

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



PianoUI::PianoUI()
{
    ui.root.box = Box({300, 300}, {100, 100});

    UINode* play_button =  ui.new_node(&ui.root);
    play_button->box = Box({320, 320}, {25, 25});
    play_button->color = 0xFF449944;

    play_button->handle_click = play_current_piano_song;
    play_button->handle_hover = _CAST_PIANO_UI_ start_button_hover;
    play_button->handle_unhover = _CAST_PIANO_UI_ start_button_unhover;

    UINode* stop_button =  ui.new_node(&ui.root);
    stop_button->box = Box({360, 320}, {25, 25});
    stop_button->color = 0xFF994444;
    stop_button->handle_hover = _CAST_PIANO_UI_ stop_button_hover;
    stop_button->handle_unhover = _CAST_PIANO_UI_ stop_button_unhover;
    

    // root.children.push_back(play_button);

    // UINode* stop_button = allocator.new_node();
    // root.children.push_back(stop_button);

    // UINode* export_button = allocator.new_node();
    // root.child = export_button;
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