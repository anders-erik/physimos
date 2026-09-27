#pragma once

#include "ui/ui4/ui.hh"

#include "piano_ui_defs.hh"



void press_C4_2(PIANO_UI_CALLBACK_PARAMETERS);
void press_D4_2(PIANO_UI_CALLBACK_PARAMETERS);
void press_E4_2(PIANO_UI_CALLBACK_PARAMETERS);
void press_F4_2(PIANO_UI_CALLBACK_PARAMETERS);
void press_G4_2(PIANO_UI_CALLBACK_PARAMETERS);
void press_A4_2(PIANO_UI_CALLBACK_PARAMETERS);
void press_B4_2(PIANO_UI_CALLBACK_PARAMETERS);
void press_C5_2(PIANO_UI_CALLBACK_PARAMETERS);


struct UIC_piano
{
    UINode* root;

    UINode* C4_node;
    UINode* D4_node;
    UINode* E4_node;
    UINode* F4_node;
    UINode* G4_node;
    UINode* A4_node;
    UINode* B4_node;
    UINode* C5_node;

    UIC_piano()
    {
    }

    void init(UI& ui);
};