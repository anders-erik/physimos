
#pragma once

#include "lib/print.hh"
#include "lib/arr.hh"

#include "math/vecmat.hh"

// #include "audio/alsa.hh"
// #include "audio/song.hh"
// #include "audio/phyano.hh"

#include "wayland/wayland.hh"

#include "io/input/user_input.hh"
#include "ui/ui4/ui.hh"
#include "rend/sw/swrend.hh"

/** Keeps track of what part of the app is shoudl recieve the input event objects */
struct InputState
{
    enum class Subsystem
    {
        UI,
        None,
    } subsystem = Subsystem::None;

    bool pointer_is_grabbed = false;

    d2 cursor_sane = {0.0, 0.0};
    // d2 cursor_sane = {0.0, 0.0};
    bool left_click_down = false;

    InputState() {}

    InputState::Subsystem get_current_subsystem()
    {
        return subsystem;
    }

    void set_from_events(Arr<UserInput> events)
    {
        for(uint i = 0; i < events.count(); i++)
        {
            set_from_event(events[i]);
        }
    }

    void set_from_event(UserInput _event)
    {
        if(_event.event_type == UserInputType::MouseMove)
        {
            MouseMovement mouse_move_ev = _event.event_data.move;
            cursor_sane = {mouse_move_ev.new_pos.x, mouse_move_ev.new_pos.y};


            Print::buf("Mouse move event: x = ");
            Print::buf(Str::FL(mouse_move_ev.new_pos.x, 4, Str::FloatRep::Fixed));
            Print::buf("  y = ");
            Print::ln(Str::FL(mouse_move_ev.new_pos.y, 4, Str::FloatRep::Fixed));
        }
        if(_event.event_type == UserInputType::MouseClick)
        {
            MouseClick mouse_click_ev = _event.event_data.mouse_click;
            Print::buf("Mouse click event: button = ");
            if(mouse_click_ev.button == MouseClick::Primary)
                Print::ln("Primary");
        }
    }
};


struct PianoApp
{
    Wayland wayland;
    UI ui;
    InputState input_state;
    SWR::Buf renderer; // Main frame buffer provided by window lib
    Clock clock;

    // Alsa alsa;
    // Song song;
    // Phyano phyano;

    PianoApp(i2 dims)
        :   wayland {Wayland{dims}},
            ui { }
    {
        
        renderer.set(   (PX32*)wayland.state.fb.ptr(), 
                        wayland.state.fb.w, 
                        wayland.state.fb.h,
                        PX32F::ARGB,
                        SWR::Buf::Top                  );
        


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
        renderer.bm_paste(white_4x4, {0, 476});


        Bitmap triangle_bmp {40, 40, PX32F::ARGB};
        triangle_bmp.clear_RGBA(0x994444FF);
        renderer.bm_paste(triangle_bmp, {100, 100});
        

        // Populate UI
        ui.add_node( {{300, 300}, {50, 50}} );
        
    }

    void open_window()
    {

        while(wayland.frame_step())
        {

            Arr<UserInput> events = wayland.get_new_input_events();

            for(uint i = 0; i < events.count(); i++)
            {
                input_state.set_from_event(events[i]);

                // Dispatch events to app
                if(input_state.subsystem == InputState::Subsystem::UI)
                {
                    // send event to active listener/reciever in app
                    ui.event(events[i]);
                }
                else
                {
                    // No current input target. Find default target!
                    if(ui.contains_pointer_pos(input_state.cursor_sane))
                    {
                        // state.input_target = AppState::InputTarget::UI;
                        ui.event(events[i]);
                    }
                    else // default target!
                    {
                        ui.reset();
                    }
                }

            }

            render_ui();


            // sleep(1);
            usleep(16000); // ~60fps
            // clock.print_current_m_sec();
        }
    }

    InputState::Subsystem get_current_subsystem_target()
    {
        
    }

    void render_ui()
    {
        LLNode<UINode> *ll_node = ui.nodes.back();

        UINode& ui_node = ll_node->value;
        i2 pos_0_i = {ui_node.box.pos.x, ui_node.box.pos.y};
        i2 pos_1_i = {  ui_node.box.pos.x + ui_node.box.size.x, 
                        ui_node.box.pos.y + ui_node.box.size.y      };
        renderer.draw_rectangle( pos_0_i, pos_1_i, ui_node.color);


        // Bitmap ui_bitmap { 40, 40, PX32F::ARGB };
        // ui_bitmap.clear_RGBA(0x555588FF);
        // wayland.get_state().
    }
};