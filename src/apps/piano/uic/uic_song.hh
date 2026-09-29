#pragma once

#include "ui/ui4/ui.hh"

#include "piano_ui_defs.hh"



void beat_count_label_scroll(PIANO_UI_CALLBACK_PARAMETERS);

void stop_current_piano_song(UINode* _node, UserInput _user_input,  PianoApp* piano_app);
void play_current_piano_song(UINode* _node, UserInput _user_input,  PianoApp* piano_app);
void stop_button_hover(UINode* _node, UserInput _user_input,  PianoApp* piano_app);
void stop_button_unhover(UINode* _node, UserInput _user_input,  PianoApp* piano_app);
void start_button_hover(UINode* _node, UserInput _user_input,  PianoApp* piano_app);
void start_button_unhover(UINode* _node, UserInput _user_input,  PianoApp* piano_app);



struct UIC_Song
{
    UINode* root;

    UINode* play_button;
    UINode* stop_button;
    UINode* beat_count_label;

    Arr<UINode*> beat_label_array;

    uint* beat_count = nullptr;

    UIC_Song()
    {
    }

    void init(PianoApp& piano_app);
    void update(PianoApp& piano_app);
};