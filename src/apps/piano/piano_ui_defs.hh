
#pragma once

struct UINode;
struct UI;
struct UserInput;
struct PianoApp;

#define PIANO_UI_CALLBACK_CAST (void (*)(UINode*, UserInput, void*))    // enable the UI to dispatch any data using void*
#define PIANO_UI_CALLBACK_PARAMETERS UINode* node, UserInput user_input,  PianoApp* piano_app // useful for API changes!