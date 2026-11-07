
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

struct PerformanceClock : public Clock
{
    enum Type
    {
        FPS,
        CommitBuffer,
        RenderCycle,
    };

    i64 fps_time_start_um = 0;
    i64 fps_time_end_um = 0;
    i64 commit_buffer_time_start_us = 0;

    PerformanceClock()
    {
        fps_start();
    }


    void start_commit_buffer_time()
    {
        commit_buffer_time_start_us = get_unix_epoch_us();
    }

    f32 get_commit_buffer_time_ms()
    {
        i64 commit_buffer_time_now_us = get_unix_epoch_us();
        i64 commit_buffer_time_delta_um = commit_buffer_time_now_us - commit_buffer_time_start_us;
        f32 commit_buffer_time_delta_ms = ((f32)commit_buffer_time_delta_um) * 0.001f;
        return commit_buffer_time_delta_ms;
    }

    void print_commit_buffer_time()
    {
        Print::ln(Str::FL(get_commit_buffer_time_ms(), 2, Str::FloatRep::Fixed));
    }


    void fps_start()
    {
        fps_time_end_um = 0;
        fps_time_start_um = get_unix_epoch_us();
    }
    void fps_end()
    {
        if(fps_time_start_um == 0)
        {
            Print::ln("WARNING: trying to end FPS-clock without setting a start fps time. ");
        }

        fps_time_end_um = get_unix_epoch_us();
    }
    f32 fps_get()
    {
        if(fps_time_start_um == 0)
            fps_end();

        i64 delta_time_us = fps_time_end_um - fps_time_start_um;

        f32 delta_time_ms = ((f32) delta_time_us) * 0.001f;

        return 1000.0f / delta_time_ms;
    }
    void fps_print()
    {
        Print::buf("FPS: ");
        Print::buf(Str::FL(fps_get(), 2, Str::FloatRep::Fixed));
        Print::buf("\n");
    }

    /** Pass 'true' as a parameter to print the value. This value defaults to false; */
    void fps_step(bool print = false)
    {
        fps_end();
        if(print)
            fps_print();
        fps_start();
    }

};

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
        :   wayland {Wayland{dims}}
    {
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
                        wayland.get_pixel_buffer() );
    }

    void open_window()
    {
        while(wayland.frame_step())
        {
            if(wayland.set_size())
            {
                piano_ui.new_frame_buffer(wayland.get_pixel_buffer());
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