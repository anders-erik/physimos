
#include "piano_ui.hh"

#include "lib/print.hh"

#include "ui/ui4/ui.hh"
#include "apps/piano/piano_app.hh"

#define _CAST_PIANO_UI_ (void (*)(UINode*, void*))

void stop_button_hover(UINode* _node, PianoApp* piano_app)
{
    _node->visibility.set_color(0xFFAA4433);
    // _node->color = 0xFFBB3333;
}
void stop_button_unhover(UINode* _node, PianoApp* piano_app)
{
    _node->visibility.set_color(0xFF995544);
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

void close_app(UINode* _node, void* _data)
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

    // ui.root.handle_click = handle_click_print;
    // ui.root.handle_click = play_current_piano_song;
    // ui.root.handle_hover = handle_hover_recolor;
    // ui.root.handle_unhover = handle_unhover_reset;

    

    UINode* play_button =  ui.new_node(&ui.root);
    play_button->box = UIBox({320, 320}, {50, 50});
    // play_button->color = 0xFF449944;
    play_button->visibility.set_color(0xFF449944);
    play_button->handle_click = play_current_piano_song;
    play_button->handle_hover = _CAST_PIANO_UI_ start_button_hover;
    play_button->handle_unhover = _CAST_PIANO_UI_ start_button_unhover;

    UINode* stop_button =  ui.new_node(&ui.root);
    stop_button->box = UIBox({390, 320}, {50, 50});
    // stop_button->color = 0xFF994444;
    stop_button->visibility.set_color(0xFF995544);
    stop_button->handle_click = stop_current_piano_song;
    stop_button->handle_hover = _CAST_PIANO_UI_ stop_button_hover;
    stop_button->handle_unhover = _CAST_PIANO_UI_ stop_button_unhover;
    

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

    
}




void PianoUI::render_ui_node(UINode* _node)
{
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
            i2 parent_pos = {_node->parent->box.pos.x, _node->parent->box.pos.y};
            i2 parent_size = {_node->parent->box.size.x, _node->parent->box.size.y};
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

}


void PianoUI::render()
{
    render_ui_node(&ui.root);

    // Currently onle one level
    for(uint i = 0; i < ui.root.children.count(); i++)
    {
        render_ui_node(ui.root.children[i]);
    }


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