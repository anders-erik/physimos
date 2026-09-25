
#include "piano_ui.hh"

#include "lib/print.hh"

#include "ui/ui4/ui.hh"
#include "apps/piano/piano_app.hh"

#define _CAST_PIANO_UI_ (void (*)(UINode*, void*))

void stop_button_hover(UINode* _node, PianoApp* piano_app)
{
    _node->visibility.set_color(0xFFBB3333);
    // _node->color = 0xFFBB3333;
}
void stop_button_unhover(UINode* _node, PianoApp* piano_app)
{
    _node->visibility.set_color(0xFF994444);
    // _node->color = 0xFF994444;
}
void start_button_hover(UINode* _node, PianoApp* piano_app)
{
    _node->visibility.set_color(0xFF33BB33);
    // _node->color = 0xFF33BB33;
}
void start_button_unhover(UINode* _node, PianoApp* piano_app)
{
    _node->visibility.set_color(0xFF449944);
    // _node->color = 0xFF449944;
}

void handle_click_print(UINode* _node, void* _data)
{
    PianoApp* piano_app = (PianoApp*) _data;

    Print::ln("Click handler!");
}

void handle_hover_recolor(UINode* _node, void* _data)
{
    PianoApp* piano_app = (PianoApp*) _data;

    _node->visibility.set_color(0x12345678);
    // _node->color = 0x12345678;
    // _node->color = 0xFFFFFFFF;
    Print::ln("Hover handler!");
}

void handle_unhover_reset(UINode* _node, void* _data)
{

    _node->visibility.set_color(0x66666666);
    // _node->color = 0x558855FF;
    // _node->color = 0x66666666;
    Print::ln("Unhover handler!");
}


void play_current_piano_song(UINode* _node, void* _data)
{
    PianoApp* piano_app = (PianoApp*) _data;

    piano_app->piano_state.song.play(piano_app->piano_state.alsa);

    // Print::ln("Unhover handler!");
}


void stop_current_piano_song(UINode* _node, void* _data)
{
    PianoApp* piano_app = (PianoApp*) _data;

    piano_app->piano_state.song.stop(piano_app->piano_state.alsa);

    // Print::ln("Unhover handler!");
}




void PianoUI::init(PianoApp* _piano_app, PixelBuffer _pixel_buffer)
{
    PianoApp& app = *_piano_app;
    renderer.set_buffer(_pixel_buffer);

    // ui.root.handle_click = handle_click_print;
    ui.root.handle_click = play_current_piano_song;
    ui.root.handle_hover = handle_hover_recolor;
    ui.root.handle_unhover = handle_unhover_reset;


    UINode* play_button =  ui.new_node(&ui.root);
    play_button->box = Box({320, 320}, {25, 25});
    // play_button->color = 0xFF449944;
    play_button->visibility.set_color(0xFF449944);
    

    play_button->handle_click = play_current_piano_song;
    play_button->handle_hover = _CAST_PIANO_UI_ start_button_hover;
    play_button->handle_unhover = _CAST_PIANO_UI_ start_button_unhover;

    UINode* stop_button =  ui.new_node(&ui.root);
    stop_button->box = Box({360, 320}, {25, 25});
    // stop_button->color = 0xFF994444;
    stop_button->visibility.set_color(0xFF994444);
    stop_button->handle_click = stop_current_piano_song;
    stop_button->handle_hover = _CAST_PIANO_UI_ stop_button_hover;
    stop_button->handle_unhover = _CAST_PIANO_UI_ stop_button_unhover;
    

    // root.children.push_back(play_button);

    // UINode* stop_button = allocator.new_node();
    // root.children.push_back(stop_button);

    // UINode* export_button = allocator.new_node();
    // root.child = export_button;



    renderer.clear(0x00663333);

    renderer.draw_point({10, 10}, 0x12345678);
    renderer.draw_point({20, 10}, 0x12345678);
    renderer.draw_point({30, 10}, 0x12345678);
    renderer.draw_point({11, 10}, 0x00FFFFFF);
    renderer.draw_point({12, 10}, 0x00FFFFFF);
    renderer.draw_point({13, 10}, 0x00FFFFFF);

    renderer.draw_line({30, 30}, {50, 170}, 0x00FFFFFF);
    renderer.draw_line({34, 30}, {54, 170}, 0xFFFFFF00);
    renderer.draw_line({38, 30}, {58, 170}, PX::RGBA_to_ARGB(0xFFFFFF00));

    renderer.draw_rectangle({300, 100}, {340, 120}, PX::RGBA_to_ARGB(0x88f888800));

    renderer.draw_triangle_no_fill({200, 30}, {250, 80}, {220, 120}, 0x00FFFFFF);

    renderer.draw_triangle({200, 30}, {250, 80}, {220, 120}, 0x00FFFFFF);

    Bitmap white_4x4 {4, 4, PX32F::ARGB};
    // white_4x4.clear(0x00FFFFFF); // XRGB format for wayland compatibility
    // white_4x4.clear(0xFFFFFF00); //
    white_4x4.clear_RGBA(0xFFFFFF00); // automatically converts the pixel to match underlying format
    // white_4x4.set_format(PX32F::ARGB);
    renderer.paste_bitmap(white_4x4, {0, 476});


    Bitmap triangle_bmp {40, 40, PX32F::ARGB};
    triangle_bmp.clear_RGBA(0x994444FF);
    renderer.paste_bitmap(triangle_bmp, {100, 100});
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



void PianoUI::render_ui_node(UINode* _node)
{
    i2 pos_0_i = {(int)_node->box.pos.x, (int)_node->box.pos.y};

    i2 pos_1_i = { _node->box.pos.x + _node->box.size.x, 
                _node->box.pos.y + _node->box.size.y      };

    // renderer.draw_rectangle( pos_0_i, pos_1_i, _node->color);
    switch(_node->visibility.type)
    {
        case UINodeVisibility::COLOR:   
            renderer.draw_rectangle( pos_0_i, pos_1_i, _node->visibility.value.color);
            break;

        case UINodeVisibility::BITMAP:
            // renderer.draw_rectangle( pos_0_i, pos_1_i, _node->visibility.value.color);
            renderer.paste_bitmap(*(_node->visibility.value.bitmap), pos_0_i);
            break;
        
        case UINodeVisibility::NONE:   
            
            break;

        default:
            break;
    }

}


void PianoUI::render()
{

    Bitmap black_100x100 {100, 100, PX32F::ARGB, 0xFF000000};
    Bitmap white_50x50 {50, 50, PX32F::ARGB, 0xFFFFFFFF};
    Bitmap red_30x30  {30, 30, PX32F::ARGB, 0xFFFF0000};
    Bitmap green_30x30  {30, 30, PX32F::ARGB, 0xFF00FF00};
    Bitmap blue_30x30  {30, 30, PX32F::ARGB, 0xFF0000FF};

    renderer.paste_bitmap(red_30x30, {1, 1});
    renderer.paste_bitmap(green_30x30, {31, 31});
    // renderer.paste_bitmap(blue_30x30, {-15, -15}); //

    // Render the first node with bitmap-visibility
    UIString ui_string { "Physimos!", {400, 75} };
    // ui_string.visibility.set_bitmap();
    ui_string.visibility.value.bitmap->set_format(PX32F::ARGB);
    render_ui_node(&ui_string);
    

    i2 pos_0_i;
    i2 pos_1_i;
    // LLNode<UINode> *ll_node = ui.nodes.back();
    // UINode& ui_node = ll_node->value;

    render_ui_node(&ui.root);

    render_ui_node(ui.root.children[0]);
    render_ui_node(ui.root.children[1]);

    // UINode& ui_node = piano_ui.ui.root;

    // pos_0_i = {(int)ui_node.box.pos.x, (int)ui_node.box.pos.y};

    // pos_1_i = { ui_node.box.pos.x + ui_node.box.size.x, 
    //             ui_node.box.pos.y + ui_node.box.size.y      };

    // renderer.draw_rectangle( pos_0_i, pos_1_i, ui_node.color);


    // UINode* stop_btn = piano_ui.ui.root.children[0];

    // pos_0_i = {stop_btn->box.pos.x, stop_btn->box.pos.y};

    // pos_1_i = { stop_btn->box.pos.x + stop_btn->box.size.x, 
    //             stop_btn->box.pos.y + stop_btn->box.size.y      };

    // renderer.draw_rectangle( pos_0_i, pos_1_i, stop_btn->color);


    // Bitmap ui_bitmap { 40, 40, PX32F::ARGB };
    // ui_bitmap.clear_RGBA(0x555588FF);
    // wayland.get_state().
}