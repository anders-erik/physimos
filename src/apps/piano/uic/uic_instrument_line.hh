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
    Arr<UINode*> line_note_nodes;

    // f64 y_accumulator = 0.0; // an accumulator for snapping the line notes between different values in the UI

    UIC_InstrumentLine()
    {
    }

    void init(UIAppContext<PianoApp> _context, InstrumentLine* _instrument_line, UINode& _parent_node);
    void reload_notes();
    void reload_background();

    void add_line_note_node(LineNote* _line_note);

    f64 get_pixels_per_note_jiffy(); // rounded down to integer value
    f64 get_x_pos_from_line_note_index(i32 line_note_index);

    double get_note_height()
    {
        return uic_root->box.size.y / 8;
    }



    NoteName get_note_from_y_pos(f64 y_pos)
    {
        double y_min = uic_root->box.pos.y;
        double y_max = uic_root->box.pos.y + uic_root->box.size.y - get_note_height();
        double y_span = y_max - y_min;

        if(y_pos > y_min + y_span*0.875)
            return NoteName::C5;
        else if(y_pos > y_min + y_span*0.750)
            return NoteName::B4;
        else if(y_pos > y_min + y_span*0.625)
            return NoteName::A4;
        else if(y_pos > y_min + y_span*0.500)
            return NoteName::G4;
        else if(y_pos > y_min + y_span*0.375)
            return NoteName::F4;
        else if(y_pos > y_min + y_span*0.250)
            return NoteName::E4;
        else if(y_pos > y_min + y_span*0.125)
            return NoteName::D4;
        else if(y_pos > y_min)
            return NoteName::C4;
        else
            return NoteName::None;
    }

    f64 get_y_pos_from_note_name(NoteName note_name)
    {
        double y_min = uic_root->box.pos.y;
        double y_max = uic_root->box.pos.y + uic_root->box.size.y - get_note_height();

        switch (note_name)
        {
        case NoteName::C4:  return y_min + uic_root->box.size.y*0.000;    break;
        case NoteName::D4:  return y_min + uic_root->box.size.y*0.125;    break;
        case NoteName::E4:  return y_min + uic_root->box.size.y*0.250;    break;
        case NoteName::F4:  return y_min + uic_root->box.size.y*0.375;    break;
        case NoteName::G4:  return y_min + uic_root->box.size.y*0.500;    break;
        case NoteName::A4:  return y_min + uic_root->box.size.y*0.625;    break;
        case NoteName::B4:  return y_min + uic_root->box.size.y*0.750;    break;
        case NoteName::C5:  return y_min + uic_root->box.size.y*0.875;    break;
        
        default:            return 0.0;                     break;
        }

    }

    f64 ms_per_x_pixel()
    {   
        // TODO: make sure that the song length is set properly
    }
};
