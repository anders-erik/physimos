#pragma once

#include "ui/ui4/ui.hh"

#include "piano_ui_defs.hh"

#include "audio/song.hh"

#define UIC_PIANO_CALLBACK_PARAMETERS (UINode* node, UserInput user_input, UIC_Song* uic_song)

void beat_count_label_scroll(PIANO_UI_CALLBACK_PARAMETERS);
void note_name_label_scroll(PIANO_UI_CALLBACK_PARAMETERS);

void stop_current_piano_song(UINode* _node, UserInput _user_input,  PianoApp* piano_app);
void play_current_piano_song(UINode* _node, UserInput _user_input,  PianoApp* piano_app);
void stop_button_hover(UINode* _node, UserInput _user_input,  PianoApp* piano_app);
void stop_button_unhover(UINode* _node, UserInput _user_input,  PianoApp* piano_app);
void start_button_hover(UINode* _node, UserInput _user_input,  PianoApp* piano_app);
void start_button_unhover(UINode* _node, UserInput _user_input,  PianoApp* piano_app);



struct UIC_Song
{
    Song* song;
    UI* ui;

    UINode* root;
    UINode* play_button;
    UINode* stop_button;

    UINode* beat_count_container;
    UINode* beat_count_label;

    Arr<UINode*> beat_label_array;

    

    UIC_Song()
    {
    }

    void init(UI&_ui, UINode& _parent_node, Song& _song);
    void init(PianoApp& piano_app);
    void update(PianoApp& piano_app);

    void reload_beat_count();
};
