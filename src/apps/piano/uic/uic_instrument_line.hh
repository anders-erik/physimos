#pragma once

#include "ui/ui4/ui.hh"
#include "ui/ui4/ui_defs.hh"

#include "audio/song.hh"

#include "piano_ui_defs.hh"

#define CONTEXT_ UIContext<PianoApp, InstrumentLine>

UIEventReturn line_note_callback_2(UI_CALLBACK_PARAMS(PianoApp));


struct UIC_InstrumentLine
{
    UIAppContext<PianoApp> context;
    InstrumentLine* instrument_line;

    UINode* uic_root = nullptr;
    UINode* create_new_line_note = nullptr;

    UIC_InstrumentLine()
    {
    }

    void init(UIAppContext<PianoApp> _context, InstrumentLine* _instrument_line, UINode& _parent_node);

    void add_line_note_node(LineNote* _line_note);

    f64 ms_per_x_pixel()
    {   
        // TODO: make sure that the song length is set properly
    }
};
