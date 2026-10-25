#include <cmath>

#include "uic_instrument_line.hh"
#include "io/input/user_input.hh"
#include "piano_app.hh"

// Provides UIC context for components until I've figured out a better solution
// Similar to the Lambda Capture Clause, but I manually place the function (lambda) and a pointer (the captured reference) in the same translation uint
UIC_InstrumentLine* uic_instrument_line_ptr = nullptr;



UIEventReturn uic_instrument_line_root_callback(UI_CALLBACK_PARAMS(PianoApp))
{
    switch (ui_event.type)
    {
    case UIEvent::mouse_scroll:
        {
            UIC_Song uic_song = data->piano_ui.uic_song;

            if(ui_event.key_mods.is_none())
            {
                if(ui_event.value.scroll.is_up())
                {
                    AudioSamplingConfig current_sampling_config = data->song.sampling_config;
                    AudioSamplingConfig new_sampling_config {AudioSamplingConfig::RATE_DURATION, 44100, current_sampling_config.duration_us + 100000};
                    uic_song.song->set_sampling_config(new_sampling_config);
                    uic_song.song_length_label->set_str(Str::UI(new_sampling_config.duration_us));
                }
                else if(ui_event.value.scroll.is_down())
                {
                    AudioSamplingConfig current_sampling_config = data->song.sampling_config;
                    AudioSamplingConfig new_sampling_config {AudioSamplingConfig::RATE_DURATION, 44100, current_sampling_config.duration_us - 100000};
                    uic_song.song->set_sampling_config(new_sampling_config);
                    uic_song.song_length_label->set_str(Str::UI(new_sampling_config.duration_us));
                }
            }
            else if(ui_event.key_mods.shift)
            {
                if(ui_event.value.scroll.is_up())
                    ++uic_instrument_line_ptr->start_jiffy_index;
                else
                    --uic_instrument_line_ptr->start_jiffy_index;
            }

            uic_song.uic_instrument_line.reload_background();
            uic_song.uic_instrument_line.reload_notes();
        }
        break;
    
    default:
        break;
    }

    return {};
}

UIEventReturn line_note_callback_2(UI_CALLBACK_PARAMS(PianoApp))
{
    Print::ln("line_note_callback!");

    LineNote* line_note = (LineNote*) node->get_data_ptr();

    switch (ui_event.type)
    {
    case UIEvent::mouse_enter:
        node->visibility.set_color(0xFF222222);
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

        node->visibility.set_color(0xFF334433);
        // node->box.size.x += 6.0;
        // node->box.pos.x -= 3.0;
        return {UIEventReturn::node_mouse_grab};
        }
        break;
    case UIEvent::mouse_release:
        {
            
            node->visibility.set_color(0xFF222222);
            // node->box.size.x -= 6.0;
            // node->box.pos.x += 3.0;
            return {UIEventReturn::node_mouse_release};
        }
        break;

    case UIEvent::mouse_move:
        {
            if(ui_event.target_is_grabbed == true)
            {

                Print::ln("mouse movement with grab active");
                d2 move_delta = {ui_event.value.mouse_move.dx, ui_event.value.mouse_move.dy};
                
                

                // X
                f64 x_min = node->parent->box.pos.x;
                f64 x_max = node->parent->box.pos.x + node->parent->box.size.x ;

                // f64 mouse_location_line_x = (ui_event.value.mouse_move.x - x_min) - node->box.size.x * 0.5;
                f64 mouse_location_line_x = (ui_event.value.mouse_move.x - x_min);

                Print::buf("mouse line location x: ");
                Print::buf(Str::SI(mouse_location_line_x));
                Print::buf("\n");

                if(mouse_location_line_x < 0.0)
                    mouse_location_line_x = 0.0;
                if(mouse_location_line_x > node->parent->box.size.x)
                    mouse_location_line_x = node->parent->box.size.x;
                
                Print::buf("mouse line location x: ");
                Print::buf(Str::SI(mouse_location_line_x));
                Print::buf("\n");


                f64 pixels_per_note_jiffy = uic_instrument_line_ptr->get_pixels_per_note_jiffy();
                i32 mouse_jiffy_index = (i32)(mouse_location_line_x / pixels_per_note_jiffy);

                Print::buf("Jiffy_index: ");
                Print::buf(Str::SI(mouse_jiffy_index));
                Print::buf("\n");

                i32 current_jiffy_index = line_note->start_index_16th_count;

                if(current_jiffy_index != mouse_jiffy_index)
                {
                    line_note->start_index_16th_count = mouse_jiffy_index;
                    node->box.pos.x = x_min + mouse_jiffy_index * pixels_per_note_jiffy;
                }

                if(false)
                {

                    // Print::buf("Time step index: ");
                    // Print::buf(Str::SI(time_step_x_index));
                    // Print::buf("\n");



                    f64 s_per_sixteenth = NoteTypeDuration::get_duration_s(NoteType::sixteenth, 120);
                    i32 ms_per_sixteenth = (i32) (s_per_sixteenth * 1000.0);
                    f64 song_length_ms = 2000.0;
                    

                    // double x_min = node->parent->box.pos.x;
                    // double x_max = node->parent->box.pos.x + node->parent->box.size.x - 0.5 * node->box.size.x;
                
                    f64 pixels_per_ms = (x_max - x_min) / song_length_ms;
                    f64 pixels_per_sixteenth = pixels_per_ms * (f64)ms_per_sixteenth;

                    i32 pixel_x_index = (i32)(ui_event.value.mouse_move.x - x_min);
                    i32 time_step_x_index = (i32)(pixel_x_index / (i32)pixels_per_sixteenth);

                    

                    // node->box.pos.x =  x_min + (f64)time_step_x_index * pixels_per_sixteenth;
                    node->box.pos.x = uic_instrument_line_ptr->get_x_pos_from_line_note_index(line_note->start_index_16th_count);
                    // node->box.pos.x += move_delta.x;

                    if(node->box.pos.x < x_min)
                        node->box.pos.x = x_min;
                    if(node->box.pos.x > x_max)
                        node->box.pos.x = x_max;
                    
                    f64 fraction_x_pos = (node->box.pos.x - x_min) / (x_max - x_min);
                    f64 start_time_ms = song_length_ms * fraction_x_pos;
                    line_note->set_start_time_ms((i32) start_time_ms);
                    // Round to nearest sixteenth note
                    
                    i32 rounded_start_time = (i32)line_note->get_start_time_ms() - (i32)line_note->get_start_time_ms() % ms_per_sixteenth;
                    line_note->set_start_time_ms( (f64)rounded_start_time);
                    
                    // line_note->start_time_ms -= line_note->start_time_ms % 50;
                }


                // Y
                double y_min = node->parent->box.pos.y;
                double y_max = node->parent->box.pos.y + node->parent->box.size.y - uic_instrument_line_ptr->get_note_height();
                
                if(node->box.pos.y < y_min)
                    node->box.pos.y = y_min;
                if(node->box.pos.y > y_max)
                    node->box.pos.y = y_max-2.0;

                NoteName previous_note_name = line_note->note.name;

                NoteName new_note_name = uic_instrument_line_ptr->get_note_from_y_pos(ui_event.value.mouse_move.y);

                switch (new_note_name)
                {
                case NoteName::C4: node->box.pos.y = y_min + node->parent->box.size.y*0.000; break;
                case NoteName::D4: node->box.pos.y = y_min + node->parent->box.size.y*0.125; break;
                case NoteName::E4: node->box.pos.y = y_min + node->parent->box.size.y*0.250; break;
                case NoteName::F4: node->box.pos.y = y_min + node->parent->box.size.y*0.375; break;
                case NoteName::G4: node->box.pos.y = y_min + node->parent->box.size.y*0.500; break;
                case NoteName::A4: node->box.pos.y = y_min + node->parent->box.size.y*0.625; break;
                case NoteName::B4: node->box.pos.y = y_min + node->parent->box.size.y*0.750; break;
                case NoteName::C5: node->box.pos.y = y_min + node->parent->box.size.y*0.875; break;

                default:
                    break;
                }

                // Play note if it changed
                if(previous_note_name != new_note_name)
                {
                    if(new_note_name == NoteName::None)
                        return {};

                    line_note->note.name = new_note_name;

                    AudioSamplingConfig sampling_config {AudioSamplingConfig::RATE_DURATION, 44100, 500000};
                    AudioData16 note_data = PianoWave::generate(sampling_config, NoteFrequencies::to_frequency(line_note->note.name));

                    data->audio_player.stop();
                    data->audio_player.play(note_data);
                }
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
        new_line_note->set_start_time_ms(1000);
        new_line_note->note = Note{NoteName::C4, NoteType::quarter};

        uic_instrument_line_ptr->add_line_note_node(new_line_note);
        }
        break;
    
    default:
        break;
    }

    return {};
}


f64 UIC_InstrumentLine::get_pixels_per_note_jiffy()
{
    f64 jiffy_s = NoteTypeDuration::get_note_jiffy(context.app->song.bpm);
    
    f64 width_component = uic_root->box.size.x;
    f64 displayed_duration_s = context.app->song.sampling_config.get_duration_s();

    f64 pixels_per_second = width_component / displayed_duration_s;
    f64 pixels_per_jiffy = pixels_per_second * jiffy_s;
    f64 pixels_per_jiffy_floor = floor(pixels_per_jiffy);

    return pixels_per_jiffy_floor;
}

f64 UIC_InstrumentLine::get_x_pos_from_line_note_index(i32 line_note_index)
{
    return uic_root->box.pos.x + get_pixels_per_note_jiffy() * (f64) line_note_index;
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
    uic_root->event_callback = UI_CALLBACK_CAST uic_instrument_line_root_callback;
    uic_root->uic = this;
    
    
    Bitmap* root_bitmap = uic_root->set_bitmap();
    reload_background();
    // PixelBuffer pix_buf {root_bitmap->get_data_mut(), root_box.size.x, root_box.size.y, PX32F::ARGB, PixelBuffer::Bottom};
    // SWR::Renderer renderer {pix_buf};
    // renderer.clear(0xFF000000);
    
    // for(i32 i = 0; i < context.app->song.beat_count*4; i++)
    // {
    //     renderer.draw_line({i*pixels_per_jiffy, 0},{i*pixels_per_jiffy, (i32)uic_root->box.size.y}, 0xFF666666);
    // }
    // for(i32 i = 0; i < context.app->song.beat_count+1; i++)
    // {
    //     renderer.draw_line({i*pixels_per_beat, 0},{i*pixels_per_beat, (i32)uic_root->box.size.y}, 0xFFFFFFFF);
    // }
    // renderer.draw_line({(int)(root_box.size.x-10.0), 0},{(int)(root_box.size.x-10.0), 50}, 0xFFFFFFFF);
    



    create_new_line_note = context.ui->new_node(uic_root);
    create_new_line_note->box = UIBox{  {uic_root->box.pos.x + uic_root->box.size.x - 40.0, uic_root->box.pos.y + uic_root->box.size.y - 40.0},
                                        {30.0, 30.0}};
    create_new_line_note->visibility.set_color(0xFF111111);
    create_new_line_note->event_callback = UI_CALLBACK_CAST create_new_line_note_callback;
    create_new_line_note->uic = this;

    for(int i = 0; i < instrument_line->line_notes.count(); i++)
    {
        LineNote* line_note = instrument_line->line_notes[i];
        add_line_note_node(line_note);
    }

}


void UIC_InstrumentLine::reload_notes()
{
    for(uint i = 0; i < line_note_nodes.count(); i++)
        context.ui->allocator.delete_node(line_note_nodes[i]);
    line_note_nodes.clear();   

    for(int i = 0; i < instrument_line->line_notes.count(); i++)
    {
        LineNote* line_note = instrument_line->line_notes[i];
        add_line_note_node(line_note);
    }
}

void UIC_InstrumentLine::reload_background()
{
    i32 pixels_per_jiffy = (i32) get_pixels_per_note_jiffy();
    i32 pixels_per_beat = 4 * pixels_per_jiffy;

    Bitmap* root_bitmap = uic_root->visibility.value.bitmap;
    PixelBuffer pix_buf {root_bitmap->get_data_mut(), uic_root->box.size.x, uic_root->box.size.y, PX32F::ARGB, PixelBuffer::Bottom};
    SWR::Renderer renderer {pix_buf};
    renderer.clear(0xFF000000);
    
    for(i32 i = 0; i < context.app->song.beat_count*4; i++)
    {
        i32 x_pos = (i+start_jiffy_index)*pixels_per_jiffy;
        renderer.draw_line({x_pos, 0}, {x_pos, (i32)uic_root->box.size.y}, 0xFF666666);
    }
    for(i32 i = 0; i < context.app->song.beat_count+1; i++)
    {
        renderer.draw_line({i*pixels_per_beat, 0},{i*pixels_per_beat, (i32)uic_root->box.size.y}, 0xFFFFFFFF);
    }
}

void UIC_InstrumentLine::add_line_note_node(LineNote* _line_note)
{
    UINode* new_node = context.ui->new_node(uic_root);
    line_note_nodes.push_back(new_node);

    f64 line_duration_ms = instrument_line->line_audio_data.duration_s_double() * 1000.0;

    f64 start_time_fraction = ((f64)_line_note->get_start_time_ms()) / line_duration_ms;

    // f64 x_pos = uic_root->box.pos.x + start_time_fraction * uic_root->box.size.x;
    f64 x_pos = get_x_pos_from_line_note_index(_line_note->start_index_16th_count);
    

    f64 note_duration_ms = NoteTypeDuration::get_duration_ms(_line_note->note.type, context.app->song.bpm);
    f64 note_duration_fraction_of_line = note_duration_ms / line_duration_ms;
    f64 note_width = note_duration_fraction_of_line * uic_root->box.size.x;

    new_node->box = UIBox({x_pos, get_y_pos_from_note_name(_line_note->note.name)}, {note_width, get_note_height()});
    new_node->visibility.set_color(0xFF665555);
    new_node->set_data(UINode::DataType::ptr, _line_note);
    new_node->event_callback = UI_CALLBACK_CAST line_note_callback_2;
    new_node->uic = this;
}
