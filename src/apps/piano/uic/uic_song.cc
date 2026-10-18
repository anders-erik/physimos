#include "uic_song.hh"
#include "io/input/user_input.hh"
#include "piano_app.hh"

void beat_count_label_scroll(PIANO_UI_CALLBACK_PARAMETERS)
{
    // Print::ln("Handling scroll");
    if(user_input.event_type == UserInputType::ScrollInput)
    {
        uint new_beat_count = piano_app->song.beat_count;

        if(user_input.event_data.scroll_input.scroll_direction == ScrollDirection::Up)
        {
            new_beat_count++;
            if(new_beat_count > 4)
                new_beat_count = 4;
        }
        else
        {
            new_beat_count--;
            if(new_beat_count < 1)
                new_beat_count = 1;
        }

        // Regenerate song with the appropriate beats count value
        piano_app->song.set_beat_count(new_beat_count);
        for(uint i = 0; i < new_beat_count; i++)
        {
            piano_app->song.notes[i].push_back({ NoteName::C4, NoteType::quarter}); // always a C4 on first beat
        }
        piano_app->song.generate();

        // TODO: resolve adding and removing the UINodes from the beat count editor
        // piano_app->piano_ui.uic_song.reload_beat_count();

        Print::ln("Beat count reloaded");
    }
}


// void note_name_label_scroll(PIANO_UI_CALLBACK_PARAMETERS)
// {
//     // TODO: I ned to be able to track the index of the target UINode in order to edit the corresponding note in thac backend
    
//     if(user_input.event_data.scroll_input.scroll_direction == ScrollDirection::Up)
//         piano_app->song.notes[0][0].up_half_note();
//     // else
        

//     piano_app->piano_ui.uic_song.reload_beat_count();
// }


UIEventReturn play_button_callback(UI_CALLBACK_PARAMS(PianoApp))
{
    switch (ui_event.type)
    {
    case UIEvent::mouse_enter:
        {
            node->visibility.set_color(0xFF33BB33);
        }
        break;
    case UIEvent::mouse_exit:
        {
            node->visibility.set_color(0xFF449944);
        }
        break;
    case UIEvent::mouse_press:
        {
            data->song.generate();
            data->audio_player.play(data->song.song_data);
        }
        break;
    
    default:
        break;
    }

    return {};
}




UIEventReturn stop_button_callback(UI_CALLBACK_PARAMS(PianoApp))
{
    switch (ui_event.type)
    {
    case UIEvent::mouse_enter:
        {
            node->visibility.set_color(0xFFAA4433);
        }
        break;
    case UIEvent::mouse_exit:
        {
            node->visibility.set_color(0xFF995544);
        }
        break;
    case UIEvent::mouse_press:
        {
            data->audio_player.stop();
        }
        break;
    
    default:
        break;
    }

    return {};
}



UIEventReturn line_note_callback(PIANO_UI_EVENT_CALLBACK_PARAMETERS)
{
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
            node->box.size.x += 10.0;
            return {UIEventReturn::node_mouse_grab};
        }
        break;
    case UIEvent::mouse_release:
        {
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

void UIC_Song::init(UI&_ui, UINode& _parent_node, Song& _song)
{

}


void UIC_Song::init(PianoApp& piano_app)
{
    ui = &piano_app.piano_ui.ui;
    song = &piano_app.song;

    UIBox root_box = UIBox({500, 100}, {300, 250});

    root = ui->new_node(&ui->root);
    root->box = root_box;
    root->visibility.set_color(0xFF444444);


    uic_instrument_line.init({ui, &piano_app}, &song->piano_line, *root);


    play_button =  ui->new_node(root);
    play_button->box = UIBox({root_box.pos.x + 100.0, root_box.pos.y + 190.0}, {50, 50});
    // play_button->color = 0xFF449944;
    play_button->visibility.set_color(0xFF449944);
    play_button->event_callback = UI_CALLBACK_CAST play_button_callback;

    stop_button = ui->new_node(root);
    stop_button->box = UIBox({root_box.pos.x + 160.0, root_box.pos.y + 190.0}, {50, 50});
    stop_button->visibility.set_color(0xFF995544);
    stop_button->event_callback = UI_CALLBACK_CAST stop_button_callback;



    // beat_count_label = ui->new_node(root);
    // beat_count_label->box.pos = {root_box.pos.x + 0.0, root_box.pos.y + 130.0};
    // // beat_count_label->box = UIBox({450, 220}, {80, 30});
    // beat_count_label->set_str(Str::UI(song->beat_count));
    // // beat_count_label->handle_scroll = PIANO_UI_CALLBACK_CAST beat_count_label_scroll;


    // beat_count_container = ui->new_node(root);
    // beat_count_container->box = UIBox({root_box.pos.x + 20.0, root_box.pos.y + 120.0}, {180, 40});
    // beat_count_container->visibility.set_color(0xFF333333);

    // reload_beat_count();


    // continuous_piano_line_container  = ui->new_node(root);
    // continuous_piano_line_container->box = UIBox({root_box.pos.x + 20.0, root_box.pos.y + 70.0}, {180, 40});
    // continuous_piano_line_container->visibility.set_color(0xFF333344);

    // for(u32 i = 0; i < song->piano_line.line_notes.count(); i++)
    // {
    //     LineNote* line_note = song->piano_line.line_notes[i];

    //     UINode* new_node = ui->new_node(continuous_piano_line_container);

    //     f64 x_pos = continuous_piano_line_container->box.pos.x + 10.0 + i*40.0;
    //     f64 y_pos = continuous_piano_line_container->box.pos.y + 10.0;
    //     f64 note_width = line_note->note.type == NoteType::quarter ? 15.0 : 30.0;
    //     f64 note_height = 15.0;

    //     new_node->box = UIBox({x_pos, y_pos}, {note_width, note_height});
    //     new_node->visibility.set_color(0xFF665555);
    //     new_node->event_callback = PIANO_UI_EVENT_CALLBACK_CAST line_note_callback;

    //     // beat_label_array.push_back(new_node);
    // }

}

// void UIC_Song::update(PianoApp& piano_app)
// {
//     beat_count_label->set_str(Str::UI(piano_app.song.beat_count));
// }


// void UIC_Song::reload_piano_line()
// {
//     Print::ln("Reloading piano line in t he song uic");
// }


// void UIC_Song::reload_beat_count()
// {
//     ui->delete_node_descendants(beat_count_container);
//     beat_label_array.clear();

//     for(uint i = 0; i < song->beat_count; i++)
//     {
//         UINode* new_node = ui->new_node(beat_count_container);

//         double x_pos = (double) (535 + i*40);
//         new_node->box = UIBox({x_pos, beat_count_container->box.pos.y + 10.0},{5, 5});
//         new_node->set_str(NoteSerializer::note_name_to_str(song->notes[i][0].name));
//         // new_node->handle_scroll = PIANO_UI_CALLBACK_CAST note_name_label_scroll;

//         beat_label_array.push_back(new_node);
//     }
// }
