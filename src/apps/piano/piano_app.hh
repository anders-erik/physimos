
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

    // Clock clock;
    PerformanceClock performance_clock;

    AudioPlayer audio_player;
    Phyano phyano;
    Song song;


    PianoApp(i2 dims)
        // :   wayland {Wayland{dims}}
    {
        WaylandConfig wayland_config {
            .width = dims.x,
            .height = dims.y,
            .rendering_type = WaylandRenderingType::SWR,
            .window_title = "Hellooo there!"
        };
        wayland.init(wayland_config);

        audio_player.set_backend_to_alsa();

        // AudioSamplingConfig audio_sampling_config {AudioSamplingConfig::RATE_DURATION, 44100, 2000000};

        // song.bpm = 120.0;
        // song.beat_count = 4;
        song.set_length(120.0, 4.0);
        // song.set_sampling_config(audio_sampling_config);
        // song.notes[0].push_back({ NoteName::C4, NoteType::quarter});
	    // song.notes[1].push_back({ NoteName::D4, NoteType::quarter});
        // song.piano_line.add_note({200, {NoteName::B4, NoteType::quarter}});
        // song.piano_line.add_note({300, {NoteName::C5, NoteType::quarter}});
        LineNote* line_note_1 = song.piano_line.new_note();
        line_note_1->start_index_16th_count = 0;
        line_note_1->note = {NoteName::B4, NoteType::quarter};
        LineNote* line_note_2 = song.piano_line.new_note();
        line_note_2->start_index_16th_count = 4;
        line_note_2->note = {NoteName::C5, NoteType::quarter};

        piano_ui.init(  this, 
                        wayland.get_content_buffer() );
    }

    void open_window()
    {
        while(wayland.frame_step())
        {
            if(wayland.set_size())
            {
                piano_ui.new_frame_buffer(wayland.get_content_buffer());
            }

            Arr<UserInput> events = wayland.get_new_input_events();

            for(uint i = 0; i < events.count(); i++)
            {
                piano_ui.ui.process_user_input(events[i], this);
            }


            // usleep(16000); // ~60fps
            usleep(10000);
            // usleep(1000000); // ~1fps

            piano_ui.render();

            performance_clock.fps_step(false);
        }
    }

};