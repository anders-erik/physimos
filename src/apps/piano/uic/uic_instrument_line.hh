#pragma once

#include "ui/ui4/ui.hh"
#include "ui/ui4/ui_defs.hh"

#include "audio/song.hh"

#include "piano_ui_defs.hh"

#define CONTEXT_ UIContext<PianoApp, InstrumentLine>

UIEventReturn line_note_callback_2(UI_CALLBACK_PARAMS(PianoApp));


struct UIC_InstrumentLine
{
    UIContext<PianoApp, InstrumentLine> context;
    InstrumentLine* instrument_line;
    UINode* uic_root = nullptr;

    UIC_InstrumentLine()
    {
    }

    void init(UIContext<PianoApp, InstrumentLine> _context, UINode& _parent_node);
};
