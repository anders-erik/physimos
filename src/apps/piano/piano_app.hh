
#pragma once

#include "lib/print.hh"
#include "lib/arr.hh"
#include "lib/clock.hh"

#include "math/vecmat.hh"

#include "audio/alsa.hh"
#include "audio/song.hh"
// #include "audio/phyano.hh"

#include "window/wayland1/wayland.hh"

#include "io/input/user_input.hh"
#include "ui/ui4/ui.hh"
#include "rend/sw/swrend.hh"
#include "lib/pixel_buffer.hh"


#include "piano_ui.hh"


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

    // void set_from_events(Arr<UserInput> events)
    // {
    //     for(uint i = 0; i < events.count(); i++)
    //     {
    //         set_from_event(events[i]);
    //     }
    // }

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


struct PianoState
{
    Alsa alsa;
    Song song;
    // Phyano phyano;
};

struct PianoApp
{
    Wayland wayland;
    PianoUI piano_ui;
    InputState input_state;
    Clock clock;

    PianoState piano_state;

    PianoApp(i2 dims)
        :   wayland {Wayland{dims}}
    {

        piano_ui.init(  this, 
                        wayland.get_pixel_buffer()   );
        
        piano_state.song.beat_count = 2;
        piano_state.song.bpm = 120;
        piano_state.song.notes[0].push_back({ NoteName::C4, NoteType::quarter});
	    piano_state.song.notes[1].push_back({ NoteName::D4, NoteType::quarter});
        piano_state.song.generate();
        // piano_state.song.play(piano_state.alsa); // Trigger interactively in ui!
        
    }

    void open_window()
    {

        while(wayland.frame_step())
        {
            Arr<UserInput> events = wayland.get_new_input_events();


            for(uint i = 0; i < events.count(); i++)
            {
                input_state.set_from_event(events[i]);

                piano_ui.ui.process_user_input(events[i], this);
            }


            // render_ui(); // if rendering before the sleep the render looks terrible

            usleep(16000); // ~60fps

            // renderer.clear(0xFF383333);

            
            // renderer.paste_bitmap(black_100x100, {200, 50});

            if(wayland.buffer_is_busy())
                Print::ln("rendering ui while wayland buffer still busy!");
            piano_ui.render();

        }
    }

};