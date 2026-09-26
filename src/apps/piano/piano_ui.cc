
#include "piano_ui.hh"

#include "lib/print.hh"

#include "ui/ui4/ui.hh"
#include "apps/piano/piano_app.hh"

#define PIANO_UI_CALLBACK_CAST (void (*)(UINode*, UserInput, void*))    // enable the UI to dispatch any data using void*
#define PIANO_UI_CALLBACK_PARAMETERS UINode* _node, UserInput _user_input,  PianoApp* _piano_app // useful for API changes!

void stop_button_hover(UINode* _node, UserInput _user_input,  PianoApp* piano_app)
{
    _node->visibility.set_color(0xFFAA4433);
    // _node->color = 0xFFBB3333;
}
void stop_button_unhover(UINode* _node, UserInput _user_input,  PianoApp* piano_app)
{
    _node->visibility.set_color(0xFF995544);
    // _node->color = 0xFF994444;
}
void start_button_hover(UINode* _node, UserInput _user_input,  PianoApp* piano_app)
{
    _node->visibility.set_color(0xFF33BB33);
    // _node->color = 0xFF33BB33;
}
void start_button_unhover(UINode* _node, UserInput _user_input,  PianoApp* piano_app)
{
    _node->visibility.set_color(0xFF449944);
    // _node->color = 0xFF449944;
}

void handle_click_print(UINode* _node, UserInput _user_input,  PianoApp* piano_app)
{

    Print::ln("Click handler!");
}

void handle_hover_recolor(UINode* _node, UserInput _user_input,  PianoApp* piano_app)
{

    _node->visibility.set_color(0x12345678);
    // _node->color = 0x12345678;
    // _node->color = 0xFFFFFFFF;
    Print::ln("Hover handler!");
}

void handle_unhover_reset(UINode* _node, UserInput _user_input,  PianoApp* piano_app)
{

    _node->visibility.set_color(0x66666666);
    // _node->color = 0x558855FF;
    // _node->color = 0x66666666;
    Print::ln("Unhover handler!");
}


void play_current_piano_song(UINode* _node, UserInput _user_input,  PianoApp* piano_app)
{

    piano_app->piano_state.song.play(piano_app->piano_state.alsa);

    // Print::ln("Unhover handler!");
}


void stop_current_piano_song(UINode* _node, UserInput _user_input,  PianoApp* piano_app)
{

    piano_app->piano_state.song.stop(piano_app->piano_state.alsa);

    // Print::ln("Unhover handler!");
}

void press_C4(UINode* _node, UserInput _user_input,  PianoApp* _piano_app)
{
    _piano_app->phyano.press(NoteName::C4);
}
void press_D4(UINode* _node, UserInput _user_input,  PianoApp* _piano_app)
{
    _piano_app->phyano.press(NoteName::D4);
}
void press_E4(UINode* _node, UserInput _user_input,  PianoApp* _piano_app)
{
    _piano_app->phyano.press(NoteName::E4);
}
void press_F4(UINode* _node, UserInput _user_input,  PianoApp* _piano_app)
{
    _piano_app->phyano.press(NoteName::F4);
}
void press_G4(UINode* _node, UserInput _user_input,  PianoApp* _piano_app)
{
    _piano_app->phyano.press(NoteName::G4);
}
void press_A4(UINode* _node, UserInput _user_input,  PianoApp* _piano_app)
{
    _piano_app->phyano.press(NoteName::A4);
}
void press_B4(UINode* _node, UserInput _user_input,  PianoApp* _piano_app)
{
    _piano_app->phyano.press(NoteName::B4);
}
void press_C5(UINode* _node, UserInput _user_input,  PianoApp* _piano_app)
{
    _piano_app->phyano.press(NoteName::C5);
}

void phyano_keypress_callback(PIANO_UI_CALLBACK_PARAMETERS)
{
    if(_user_input.is_key_press() && _user_input.event_data.key_press.action == KeyButtonAction::Press)
    {
        NoteName note_name; 

        switch (_user_input.event_data.key_press.key)
        {
            case Keys::A:   note_name = NoteName::C4;   break;
            case Keys::S:   note_name = NoteName::D4;   break;
            case Keys::D:   note_name = NoteName::E4;   break;
            case Keys::F:   note_name = NoteName::F4;   break;
            case Keys::G:   note_name = NoteName::G4;   break;
            case Keys::H:   note_name = NoteName::A4;   break;
            case Keys::J:   note_name = NoteName::B4;   break;
            case Keys::K:   note_name = NoteName::C5;   break;
            
            default:        return;                     break;
        }

        _piano_app->phyano.press(note_name);
    }
}

void close_app(UINode* _node, UserInput _user_input,  void* _data)
{
    PianoApp* piano_app = (PianoApp*) _data;

    piano_app->wayland.close();
}


void PianoUI::init(PianoApp* _piano_app, PixelBuffer _pixel_buffer)
{
    PianoApp& app = *_piano_app;
    renderer.set_buffer(_pixel_buffer);

    // ui.root.box = Box({300, 300}, {100, 100});
    ui.root.box = UIBox({0, 0}, {_pixel_buffer.w, _pixel_buffer.h});
    ui.root.visibility.set_color(0xFF222222);
    ui.root.handle_key_press = PIANO_UI_CALLBACK_CAST phyano_keypress_callback;
    // ui.root.handle_click = handle_click_print;
    // ui.root.handle_click = play_current_piano_song;
    // ui.root.handle_hover = handle_hover_recolor;
    // ui.root.handle_unhover = handle_unhover_reset;

    

    UINode* play_button =  ui.new_node(&ui.root);
    play_button->box = UIBox({360, 320}, {50, 50});
    // play_button->color = 0xFF449944;
    play_button->visibility.set_color(0xFF449944);
    play_button->handle_click = PIANO_UI_CALLBACK_CAST play_current_piano_song;
    play_button->handle_hover = PIANO_UI_CALLBACK_CAST start_button_hover;
    play_button->handle_unhover = PIANO_UI_CALLBACK_CAST start_button_unhover;

    UINode* stop_button =  ui.new_node(&ui.root);
    stop_button->box = UIBox({410, 320}, {50, 50});
    // stop_button->color = 0xFF994444;
    stop_button->visibility.set_color(0xFF995544);
    stop_button->handle_click = PIANO_UI_CALLBACK_CAST stop_current_piano_song;
    stop_button->handle_hover = PIANO_UI_CALLBACK_CAST stop_button_hover;
    stop_button->handle_unhover = PIANO_UI_CALLBACK_CAST stop_button_unhover;
    

    UINode* third_button =  ui.new_node(&ui.root);
    // third_button->box = UIBox({ui.root.box.size.x-50, ui.root.box.size.y-50}, {35, 35}); // ORIGINAL
    // third_button->box = UIBox({ui.root.box.size.x-50, ui.root.box.size.y-20}, {35, 35}); // OK
    // third_button->box = UIBox({ui.root.box.size.x-50, -20.0}, {35, 35}); // OK
    third_button->box = UIBox({ui.root.box.size.x-10, ui.root.box.size.y-50}, {35, 35}); // OK
    // third_button->box = UIBox({-10.0, ui.root.box.size.y-50}, {35, 35}); // BROKEN: tries to write to too large y-values
    // third_button->visibility.set_color(0xFF333366);
    third_button->visibility.set_bitmap(third_button->box.size.x, third_button->box.size.y);
    third_button->handle_click = close_app;

    // UINode* string =  ui.new_node(&ui.root);
    // string->box = UIBox({ui.root.box.size.x-50, ui.root.box.size.y-50}, {35, 35});
    // string->visibility.set_color(0xFF333366);
    // string->handle_click = close_app;

    // root.children.push_back(play_button);

    // UINode* stop_button = allocator.new_node();
    // root.children.push_back(stop_button);

    // UINode* export_button = allocator.new_node();
    // root.child = export_button;



    // renderer.clear(0x00663333);

    
    UINode* phyano_node =  ui.new_node(&ui.root);
    phyano_node->box = UIBox({50, 300}, {300, 100});
    phyano_node->visibility.set_color(0xFF444444);

    PX32 white_key_color = 0xFFCCCCCC;
    d2 white_key_size = {30, 80};
    // double white_key_x_pos = 60.0;
    double white_key_y_pos = 310.0;

    UINode* C4_node =  ui.new_node(phyano_node);
    C4_node->box = UIBox({60.0, white_key_y_pos}, white_key_size);
    C4_node->visibility.set_color(white_key_color);
    C4_node->handle_click = PIANO_UI_CALLBACK_CAST press_C4;

    UINode* D4_node =  ui.new_node(phyano_node);
    D4_node->box = UIBox({95.0, white_key_y_pos}, white_key_size);
    D4_node->visibility.set_color(white_key_color);
    D4_node->handle_click = PIANO_UI_CALLBACK_CAST press_D4;

    UINode* E4_node =  ui.new_node(phyano_node);
    E4_node->box = UIBox({130.0, white_key_y_pos}, white_key_size);
    E4_node->visibility.set_color(white_key_color);
    E4_node->handle_click = PIANO_UI_CALLBACK_CAST press_E4;

    UINode* F4_node =  ui.new_node(phyano_node);
    F4_node->box = UIBox({165.0, white_key_y_pos}, white_key_size);
    F4_node->visibility.set_color(white_key_color);
    F4_node->handle_click = PIANO_UI_CALLBACK_CAST press_F4;

    UINode* G4_node =  ui.new_node(phyano_node);
    G4_node->box = UIBox({200.0, white_key_y_pos}, white_key_size);
    G4_node->visibility.set_color(white_key_color);
    G4_node->handle_click = PIANO_UI_CALLBACK_CAST press_G4;

    UINode* A4_node =  ui.new_node(phyano_node);
    A4_node->box = UIBox({235.0, white_key_y_pos}, white_key_size);
    A4_node->visibility.set_color(white_key_color);
    A4_node->handle_click = PIANO_UI_CALLBACK_CAST press_A4;

    UINode* B4_node =  ui.new_node(phyano_node);
    B4_node->box = UIBox({270.0, white_key_y_pos}, white_key_size);
    B4_node->visibility.set_color(white_key_color);
    B4_node->handle_click = PIANO_UI_CALLBACK_CAST press_B4;

    UINode* C5_node =  ui.new_node(phyano_node);
    C5_node->box = UIBox({305.0, white_key_y_pos}, white_key_size);
    C5_node->visibility.set_color(white_key_color);
    C5_node->handle_click = PIANO_UI_CALLBACK_CAST press_C5;
}




void PianoUI::render_ui_node(UINode* _node)
{
    i2 parent_pos = {0, 0};
    i2 parent_size = {0, 0};

    if(_node->parent != nullptr)
    {
        parent_pos = {_node->parent->box.pos.x, _node->parent->box.pos.y};
        parent_size = {_node->parent->box.size.x, _node->parent->box.size.y};
    }

    i2 pos_0_i = {(int)_node->box.pos.x, (int)_node->box.pos.y};
    i2 pos_1_i = {  _node->box.pos.x + _node->box.size.x, 
                    _node->box.pos.y + _node->box.size.y      };

    // renderer.draw_rectangle( pos_0_i, pos_1_i, _node->color);
    switch(_node->visibility.type)
    {
        case UINodeVisibility::COLOR:   
            renderer.draw_rectangle( pos_0_i, pos_1_i, _node->visibility.value.color);
            break;

        case UINodeVisibility::BITMAP:
            {
            // renderer.draw_rectangle( pos_0_i, pos_1_i, _node->visibility.value.color);
            // renderer.paste_bitmap(*(_node->visibility.value.bitmap), pos_0_i);

            // Currently all bitmap rendering are being drawn using the parent size as the mask
            renderer.paste_bitmap_with_mask(    *(_node->visibility.value.bitmap), 
                                                pos_0_i,
                                                parent_pos,
                                                parent_size                         );
            }
            break;
        
        case UINodeVisibility::NONE:   
            
            break;

        default:
            break;
    }


    for(uint i = 0; i < _node->children.count(); i++)
    {
        render_ui_node(_node->children[i]);
    }

}


void PianoUI::render()
{
    render_ui_node(&ui.root);

    // Currently onle one level
    // for(uint i = 0; i < ui.root.children.count(); i++)
    // {
    //     render_ui_node(ui.root.children[i]);
    // }


    if(false)
    {
        // Render the first node with bitmap-visibility
        UIString ui_string { "Physimos!", {400, 75} };
        // ui_string.visibility.set_bitmap();
        ui_string.visibility.value.bitmap->set_format(PX32F::ARGB);
        render_ui_node(&ui_string);
    }
    
    // render_ui_node(ui.root.children[1]);
    // render_ui_node(ui.root.children[2]);


    Bitmap black_100x100 {100, 100, PX32F::ARGB, 0xFF000000};
    Bitmap white_50x50 {50, 50, PX32F::ARGB, 0xFFFFFFFF};
    Bitmap red_30x30  {30, 30, PX32F::ARGB, 0xFFFF0000};
    Bitmap green_30x30  {30, 30, PX32F::ARGB, 0xFF00FF00};
    Bitmap blue_30x30  {30, 30, PX32F::ARGB, 0xFF0000FF};

    renderer.paste_bitmap(red_30x30, {1, 1});
    renderer.paste_bitmap(green_30x30, {31, 31});
    // renderer.paste_bitmap(blue_30x30, {-15, -15}); //


    renderer.draw_point({10, 10}, 0x12345678);
    renderer.draw_point({20, 10}, 0x12345678);
    renderer.draw_point({30, 10}, 0x12345678);
    renderer.draw_point({11, 10}, 0x00FFFFFF);
    renderer.draw_point({12, 10}, 0x00FFFFFF);
    renderer.draw_point({13, 10}, 0x00FFFFFF);

    renderer.draw_line({30, 30}, {50, 170}, 0x00FFFFFF);
    renderer.draw_line({34, 30}, {54, 170}, 0xFFFFFF00);
    renderer.draw_line({38, 30}, {58, 170}, PX::RGBA_to_ARGB(0xFFFFFF00));

    renderer.draw_rectangle({300, 100}, {340, 120}, PX::RGBA_to_ARGB(0x88888800));

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