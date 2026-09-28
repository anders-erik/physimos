#pragma once

#include "ui/ui4/ui.hh"

#include "piano_ui_defs.hh"



void handle_scroll(PIANO_UI_CALLBACK_PARAMETERS);


struct UIC_BeatCountEditor
{
    UINode* root;
    UINode* beat_count_label;

    uint* beat_count = nullptr;

    UIC_BeatCountEditor()
    {
    }

    void init(PianoApp& piano_app);

    void update(PianoApp& piano_app);
};