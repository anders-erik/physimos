
#include "piano_ui.hh"

#include "lib/print.hh"

#include "io/input/input_utils.hh"

#include "ui/ui4/ui.hh"
#include "apps/piano/piano_app.hh"

#include "piano_ui_defs.hh"




void phyano_keypress_callback(PIANO_UI_CALLBACK_PARAMETERS)
{
    if(user_input.is_key_input() && user_input.event_data.key_input.action == KeyButtonAction::Press)
    {
        NoteName note_name; 

        switch (user_input.event_data.key_input.key)
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

        piano_app->phyano.press(note_name);
    }
}

void close_app(UINode* _node, UserInput _user_input,  void* _data)
{
    PianoApp* piano_app = (PianoApp*) _data;

    piano_app->wayland.close();
}


void root_click(PIANO_UI_CALLBACK_PARAMETERS)
{
    piano_app->piano_ui.ui.current_keyboard_target = &piano_app->piano_ui.ui.root;
}


void string_click(PIANO_UI_CALLBACK_PARAMETERS)
{
    piano_app->piano_ui.ui.current_keyboard_target = node;
}
void string_unclick(PIANO_UI_CALLBACK_PARAMETERS)
{
    // piano_app->piano_ui.ui.current_keyboard_target = &piano_app->piano_ui.ui.root;
}

void editable_string_key_press(PIANO_UI_CALLBACK_PARAMETERS)
{
    if(user_input.event_data.key_input.action == KeyButtonAction::Release)
        return;

    Keys key = user_input.event_data.key_input.key;

    switch (key)
    {
        // case Keys::A:
        //     node->set_str(node->str + Str{"a"});
        //     break;
        
        case Keys::Backspace:
            node->set_str(node->str.pop_back());
            break;
        
    
        default:
            // node->set_str(node->str + user_input.event_data.key_press.to_str());
            node->set_str(node->str + InputUtils::key_to_str(key));
            break;
    }
}

#define F64_UI_CALLBACK(y) UI_CALLBACK_PARAMS(y)

/** Typed UiNode.
    The available data is stored in the node data pointer, and no external data is provided, this the parameter is null. */
UIEventReturn f64_ui_callback(UI_CALLBACK_PARAMS(void))
{
    Print::ln("f64 callback!");

    if(!node->is_type(UINode::DataType::f64_ptr))
    {
        Print::ln("Failed '!node->is_type(UINode::DataType::f64)' in f64 ui callback!");
        return {};
    }

    f64* _f64 = node->get_data_f64_ptr();
    

    if(ui_event.type == UIEvent::mouse_press)
    {
        ++(*_f64);
        node->set_str(Str::FL(*_f64, 5, Str::FloatRep::Fixed));
    }

    
    
    return {};
}


UIEventReturn str_ui_callback(UI_CALLBACK_PARAMS(void))
{
    Print::ln("Str callback!");

    if(!node->is_type(UINode::DataType::Str_ptr))
    {
        Print::ln("Failed '!node->is_type(UINode::DataType::Str)' in Str ui callback!");
        return {};
    }

    Str* _str = node->get_data_Str_ptr();
    

    if(ui_event.type == UIEvent::mouse_press)
    {
        _str->append("+");
    }

    // Print::ln(_str);

    node->set_str(*_str);

    return {};
}



UIEventReturn i64_ui_callback(UI_CALLBACK_PARAMS(void))
{
    Print::ln("i64 callback!");

    if(!node->is_type(UINode::DataType::i64))
    {
        Print::ln("Failed '!node->is_type(UINode::DataType::i64)' in i64 ui callback!");
        return {};
    }

    i64 _i64 = node->get_data_i64();
    

    if(ui_event.type == UIEvent::mouse_press)
    {
        // _str->append("+");
        node->data_.I64++;
    }

    // Print::ln(_str);

    node->set_str(Str::SI(node->get_data_i64()));

    return {};
}

void PianoUI::init(PianoApp* _piano_app, PixelBuffer _pixel_buffer)
{
    PianoApp& app = *_piano_app;
    renderer.set_buffer(_pixel_buffer);

    // ui.root.box = Box({300, 300}, {100, 100});
    ui.root.box = UIBox({0, 0}, {_pixel_buffer.w, _pixel_buffer.h});
    ui.root.visibility.set_color(0xFF222222);
    // ui.root.handle_key_press = PIANO_UI_CALLBACK_CAST phyano_keypress_callback;
    // ui.root.handle_click = handle_click_print;
    ui.root.handle_mouse_click = PIANO_UI_CALLBACK_CAST root_click;
    // ui.root.handle_click = play_current_piano_song;
    // ui.root.handle_hover = handle_hover_recolor;
    // ui.root.handle_unhover = handle_unhover_reset;

    
    UINode* node_f64 = ui.new_node(&ui.root);
    node_f64->box = UIBox{{40, 500}, {100, 20}};
    // node_f64->data = &bound_f64;
    node_f64->set_data<f64*>(UINode::DataType::f64_ptr, &bound_f64);
    node_f64->event_callback = UI_CALLBACK_CAST f64_ui_callback;

    UINode* node_f64_A = ui.new_node(&ui.root);
    node_f64_A->box = UIBox{{40, 550}, {100, 20}};
    node_f64_A->set_data<f64*>(UINode::DataType::f64_ptr, &bound_f64_A);
    node_f64_A->event_callback = UI_CALLBACK_CAST f64_ui_callback;

    UINode* node_f64_B = ui.new_node(&ui.root);
    node_f64_B->box = UIBox{{40, 600}, {100, 20}};
    node_f64_B->set_data<f64*>(UINode::DataType::f64_ptr, &bound_f64_B);
    node_f64_B->event_callback = UI_CALLBACK_CAST f64_ui_callback;

    UINode* node_bound_str = ui.new_node(&ui.root);
    node_bound_str->box = UIBox{{40, 650}, {100, 20}};
    node_bound_str->set_data<Str*>(UINode::DataType::Str_ptr, &bound_str);
    node_bound_str->event_callback = UI_CALLBACK_CAST str_ui_callback;

    UINode* node_owned_i64 = ui.new_node(&ui.root);
    node_owned_i64->box = UIBox{{40, 700}, {100, 20}};
    node_owned_i64->set_data<i64>(UINode::DataType::i64, 0);
    node_owned_i64->event_callback = UI_CALLBACK_CAST i64_ui_callback;

    

    UINode* third_button =  ui.new_node(&ui.root);
    // third_button->box = UIBox({ui.root.box.size.x-50, ui.root.box.size.y-50}, {35, 35}); // ORIGINAL
    // third_button->box = UIBox({ui.root.box.size.x-50, ui.root.box.size.y-20}, {35, 35}); // OK
    // third_button->box = UIBox({ui.root.box.size.x-50, -20.0}, {35, 35}); // OK
    third_button->box = UIBox({ui.root.box.size.x-25, ui.root.box.size.y-50}, {35, 35}); // OK
    // third_button->box = UIBox({-10.0, ui.root.box.size.y-50}, {35, 35}); // BROKEN: tries to write to too large y-values
    // third_button->visibility.set_color(0xFF333366);
    third_button->visibility.set_bitmap(third_button->box.size.x, third_button->box.size.y);
    third_button->handle_mouse_click = close_app;



    UINode* string_node = ui.new_node(&ui.root);
    // ui_string.visibility.set_bitmap();
    string_node->box.pos = {400, 50};
    string_node->set_str("Physimos!");
    string_node->handle_mouse_click = PIANO_UI_CALLBACK_CAST string_click;
    string_node->handle_mouse_release = PIANO_UI_CALLBACK_CAST string_unclick;
    string_node->handle_key_press = PIANO_UI_CALLBACK_CAST editable_string_key_press;
    // ui_string.visibility.value.bitmap->set_format(PX32F::ARGB);
    // render_ui_node(&ui_string);


    // Add add and remove UInodes for debugging purposes - 2026-10-01
    // UINode* node_parent = ui.new_node(&ui.root);
    // for(uint i = 0; i < 5; i++)
    // {
    //     UINode* node = ui.new_node(node_parent);
    //     node->box.pos = {50.0 + 20*i, 300.0};
    //     node->box.size = {15.0, 15.0};
    //     node->set_color(0xFF663366);
    // }
    // ui.delete_node_descendants(node_parent);



    uic_piano.init(ui);
    uic_song.init(*_piano_app);
}

void PianoUI::update(PianoApp* _piano_app)
{
    uic_song.update(*_piano_app);
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
    render_ui_node(uic_piano.root);
    render_ui_node(uic_song.root);
    // render_ui_node(uic_beat_count_editor.root);

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