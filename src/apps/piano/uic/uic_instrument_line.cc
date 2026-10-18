#include "uic_instrument_line.hh"
#include "io/input/user_input.hh"
#include "piano_app.hh"



UIEventReturn line_note_callback_2(UI_CALLBACK_PARAMS(PianoApp))
{
    Print::ln("line_note_callback!");

    LineNote* line_note = (LineNote*) node->get_data_ptr();

    // Print::ln("Line note callback");
    switch (ui_event.type)
    {
    case UIEvent::mouse_enter:
        // node->visibility.set_color(0xFF665566);
        node->visibility.set_color(0xFF000000);
        break;
    case UIEvent::mouse_exit:
        node->visibility.set_color(0xFF665555);
        break;

    case UIEvent::mouse_press:
        {
        AudioSamplingConfig sampling_config {AudioSamplingConfig::RATE_DURATION, 44100, 500000};
        AudioData16 note_data = PianoWave::generate(sampling_config, NoteFrequencies::to_frequency(line_note->note.name));

        data->audio_player.stop();
        data->audio_player.play(note_data);

        node->box.size.x += 10.0;
        return {UIEventReturn::node_mouse_grab};
        }
        break;
    case UIEvent::mouse_release:
        {
            line_note->note.up_half_note();
            node->box.size.x -= 10.0;
            return {UIEventReturn::node_mouse_release};
        }
        break;

    case UIEvent::mouse_move:
        {
            if(ui_event.target_is_grabbed == true)
            {

                Print::ln("mouse movement with grab active");
                d2 move_delta = {ui_event.value.mouse_move.x, ui_event.value.mouse_move.y};
                node->box.pos.x += move_delta.x;
                
                node->box.pos.y += move_delta.y;
            }
        }
        break;
    
    
    default:
        break;
    }

    return  UIEventReturn{};
}




void UIC_InstrumentLine::init(UIContext<PianoApp, InstrumentLine> _context, UINode& _parent_node)
{
    context = _context;


    UIBox root_box = UIBox({500, 105}, {200, 50});

    uic_root = context.ui->new_node(&_parent_node);
    uic_root->box = root_box;
    uic_root->visibility.set_color(0xFF883333);


    for(int i = 0; i < context.component->line_notes.count(); i++)
    {
        LineNote* line_note = context.component->line_notes[i];

        UINode* new_node = context.ui->new_node(uic_root);

        f64 x_pos = uic_root->box.pos.x + 10.0 + i*40.0;
        f64 y_pos = uic_root->box.pos.y + 1.0;
        f64 note_width = line_note->note.type == NoteType::quarter ? 15.0 : 30.0;
        f64 note_height = 30.0;

        new_node->box = UIBox({x_pos, y_pos}, {note_width, note_height});
        new_node->visibility.set_color(0xFF665555);
        new_node->set_data(UINode::DataType::ptr, line_note);
        new_node->event_callback = UI_CALLBACK_CAST line_note_callback_2;
    }

}

