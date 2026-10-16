
#pragma once

struct UINode;
struct UIEvent;
struct UIEventReturn;
struct UI;
struct UserInput;
struct PianoApp;

#define PIANO_UI_EVENT_CALLBACK_CAST (UIEventReturn (*)(UINode*, UIEvent, void*))    // enable the UI to dispatch any data using void*
#define PIANO_UI_EVENT_CALLBACK_PARAMETERS UINode* node, UIEvent ui_event, PianoApp* piano_app // useful for API changes!

#define PIANO_UI_CALLBACK_CAST (void (*)(UINode*, UserInput, void*))    // enable the UI to dispatch any data using void*
#define PIANO_UI_CALLBACK_PARAMETERS UINode* node, UserInput user_input,  PianoApp* piano_app // useful for API changes!

#define UI_CALLBACK_CAST (UIEventReturn (*)(UINode*, UIEvent, void*))    // enable the UI to dispatch any data using void*
#define UI_CALLBACK_PARAMS(x) UINode* node, UIEvent ui_event, x* data // useful for API changes!
