#include "uic_instrument_line.hh"
#include "io/input/user_input.hh"
#include "piano_app.hh"


UIC_InstrumentLine* uic_instrument_line_ptr = nullptr;

UIEventReturn line_note_callback_2(UI_CALLBACK_PARAMS(PianoApp))
{
    Print::ln("line_note_callback!");

    LineNote* line_note = (LineNote*) node->get_data_ptr();

    switch (ui_event.type)
    {
    case UIEvent::mouse_enter:
        node->visibility.set_color(0xFF000000);
        break;
    case UIEvent::mouse_exit:
        node->visibility.set_color(0xFF665555);
        break;
    case UIEvent::mouse_scroll:
        line_note->note.up_half_note();
        break;

    case UIEvent::mouse_press:
        {
        AudioSamplingConfig sampling_config {AudioSamplingConfig::RATE_DURATION, 44100, 500000};
        AudioData16 note_data = PianoWave::generate(sampling_config, NoteFrequencies::to_frequency(line_note->note.name));

        data->audio_player.stop();
        data->audio_player.play(note_data);

        node->box.size.x += 6.0;
        node->box.pos.x -= 3.0;
        return {UIEventReturn::node_mouse_grab};
        }
        break;
    case UIEvent::mouse_release:
        {
            node->box.size.x -= 6.0;
            node->box.pos.x += 3.0;
            return {UIEventReturn::node_mouse_release};
        }
        break;

    case UIEvent::mouse_move:
        {
            if(ui_event.target_is_grabbed == true)
            {

                Print::ln("mouse movement with grab active");
                d2 move_delta = {ui_event.value.mouse_move.x, ui_event.value.mouse_move.y};
                
                // f64 x_pos = 0.0;
                
                double x_min = node->parent->box.pos.x;
                double x_max = node->parent->box.pos.x + node->parent->box.size.x - 0.5 * node->box.size.x;

                node->box.pos.x += move_delta.x;

                if(node->box.pos.x < x_min)
                    node->box.pos.x = x_min;
                if(node->box.pos.x > x_max)
                    node->box.pos.x = x_max;
                
                f64 fraction_x_pos = (node->box.pos.x - x_min) / (x_max - x_min);
                f64 song_length_ms = 2000;
                f64 start_time_ms = song_length_ms * fraction_x_pos;
                line_note->start_time_ms = (i32) start_time_ms;
                // node->box.pos.y += move_delta.y;
            }
        }
        break;
    
    
    default:
        break;
    }

    return  UIEventReturn{};
}

UIEventReturn create_new_line_note_callback(UI_CALLBACK_PARAMS(PianoApp))
{
    switch (ui_event.type)
    {
    case UIEvent::mouse_press:
        {
        LineNote* new_line_note = uic_instrument_line_ptr->instrument_line->new_note();
        new_line_note->start_time_ms = 1000;
        new_line_note->note = Note{NoteName::C4, NoteType::quarter};

        uic_instrument_line_ptr->add_line_note_node(new_line_note);
        }
        break;
    
    default:
        break;
    }

    return {};
}




void UIC_InstrumentLine::init(UIAppContext<PianoApp> _context, InstrumentLine* _instrument_line, UINode& _parent_node)
{
    uic_instrument_line_ptr = this;
    context = _context;
    instrument_line = _instrument_line;


    UIBox root_box = UIBox( {_parent_node.box.pos.x + 5.0, _parent_node.box.pos.y + 20.0}, 
                            {_parent_node.box.size.x - 10.0, 120.0});

    uic_root = context.ui->new_node(&_parent_node);
    uic_root->box = root_box;
    uic_root->visibility.set_color(0xFF883333);


    create_new_line_note = context.ui->new_node(uic_root);
    create_new_line_note->box = UIBox{  {uic_root->box.pos.x + uic_root->box.size.x - 40.0, uic_root->box.pos.y + uic_root->box.size.y - 40.0},
                                        {30.0, 30.0}};
    create_new_line_note->visibility.set_color(0xFF111111);
    create_new_line_note->event_callback = UI_CALLBACK_CAST create_new_line_note_callback;

    for(int i = 0; i < instrument_line->line_notes.count(); i++)
    {
        LineNote* line_note = instrument_line->line_notes[i];
        add_line_note_node(line_note);

        // UINode* new_node = context.ui->new_node(uic_root);
        // f64 start_time_fraction = ((f64)line_note->start_time_ms) / 2000.0;
        // f64 x_pos = uic_root->box.pos.x + 10.0 + start_time_fraction * root_box.size.x;

        // f64 y_pos = uic_root->box.pos.y + 1.0;
        // switch (line_note->note.name)
        // {
        //     case NoteName::C4: y_pos+=  5.0; break;
        //     case NoteName::D4: y_pos+= 10.0; break;
        //     case NoteName::E4: y_pos+= 15.0; break;
        //     case NoteName::F4: y_pos+= 20.0; break;
        //     case NoteName::G4: y_pos+= 25.0; break;
        //     case NoteName::A4: y_pos+= 30.0; break;
        //     case NoteName::B4: y_pos+= 35.0; break;
        //     case NoteName::C5: y_pos+= 40.0; break;
        
        //     default: break;
        // }
        // // f64 note_width = line_note->note.type == NoteType::quarter ? 15.0 : 30.0;
        // f64 note_width = 30.0;
        // f64 note_height = 10.0;

        // new_node->box = UIBox({x_pos, y_pos}, {note_width, note_height});
        // new_node->visibility.set_color(0xFF665555);
        // new_node->set_data(UINode::DataType::ptr, line_note);
        // new_node->event_callback = UI_CALLBACK_CAST line_note_callback_2;
    }

}

void UIC_InstrumentLine::add_line_note_node(LineNote* _line_note)
{
    UINode* new_node = context.ui->new_node(uic_root);
    f64 start_time_fraction = ((f64)_line_note->start_time_ms) / 2000.0;
    f64 x_pos = uic_root->box.pos.x + 10.0 + start_time_fraction * uic_root->box.size.x;

    f64 y_pos = uic_root->box.pos.y + 1.0;
    switch (_line_note->note.name)
    {
        case NoteName::C4: y_pos+=  5.0; break;
        case NoteName::D4: y_pos+= 10.0; break;
        case NoteName::E4: y_pos+= 15.0; break;
        case NoteName::F4: y_pos+= 20.0; break;
        case NoteName::G4: y_pos+= 25.0; break;
        case NoteName::A4: y_pos+= 30.0; break;
        case NoteName::B4: y_pos+= 35.0; break;
        case NoteName::C5: y_pos+= 40.0; break;
    
        default: break;
    }
    // f64 note_width = line_note->note.type == NoteType::quarter ? 15.0 : 30.0;
    f64 note_width = 30.0;
    f64 note_height = 10.0;

    new_node->box = UIBox({x_pos, y_pos}, {note_width, note_height});
    new_node->visibility.set_color(0xFF665555);
    new_node->set_data(UINode::DataType::ptr, _line_note);
    new_node->event_callback = UI_CALLBACK_CAST line_note_callback_2;
}

