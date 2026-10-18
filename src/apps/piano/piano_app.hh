
#pragma once

#include "lib/print.hh"
#include "lib/arr.hh"
#include "lib/clock.hh"

#include "math/vecmat.hh"

#include "audio/alsa.hh"
#include "audio/song.hh"
#include "audio/phyano.hh"
#include "audio/audio_player.hh"

#include "window/wayland1/wayland.hh"

#include "io/input/user_input.hh"
#include "ui/ui4/ui.hh"
#include "rend/sw/swrend.hh"
#include "lib/pixel_buffer.hh"


#include "piano_ui.hh"



struct PianoApp
{
    Wayland wayland;
    PianoUI piano_ui;

    Clock clock;

    AudioPlayer audio_player;
    Phyano phyano;
    Song song;


    PianoApp(i2 dims)
        :   wayland {Wayland{dims}}
    {
        audio_player.set_backend_to_alsa();

        song.beat_count = 4;
        song.bpm = 120;
        // song.notes[0].push_back({ NoteName::C4, NoteType::quarter});
	    // song.notes[1].push_back({ NoteName::D4, NoteType::quarter});
        // song.piano_line.add_note({200, {NoteName::B4, NoteType::quarter}});
        // song.piano_line.add_note({300, {NoteName::C5, NoteType::quarter}});
        LineNote* line_note_1 = song.piano_line.new_note();
        line_note_1->start_time_ms = 200;
        line_note_1->note = {NoteName::B4, NoteType::quarter};
        LineNote* line_note_2 = song.piano_line.new_note();
        line_note_2->start_time_ms = 500;
        line_note_2->note = {NoteName::C5, NoteType::quarter};

        // song.generate();
        // piano_state.song.play(piano_state.alsa); // Trigger interactively in ui!

        piano_ui.init(  this, 
                        wayland.get_pixel_buffer()   );

    }

    void open_window()
    {

        while(wayland.frame_step())
        {
            Arr<UserInput> events = wayland.get_new_input_events();

            for(uint i = 0; i < events.count(); i++)
            {
                piano_ui.ui.process_user_input(events[i], this);
            }

            

            // render_ui(); // if rendering before the sleep the render looks terrible

            usleep(16000); // ~60fps

            // renderer.clear(0xFF383333);

            
            // renderer.paste_bitmap(black_100x100, {200, 50});

            piano_ui.update(this);

            if(wayland.buffer_is_busy())
                Print::ln("rendering ui while wayland buffer still busy!");
            piano_ui.render();

        }
    }

};