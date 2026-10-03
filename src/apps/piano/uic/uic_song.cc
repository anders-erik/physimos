#include "uic_song.hh"
#include "io/input/user_input.hh"
#include "piano_app.hh"

void beat_count_label_scroll(PIANO_UI_CALLBACK_PARAMETERS)
{
    // Print::ln("Handling scroll");
    if(user_input.event_type == UserInputType::ScrollInput)
    {
        uint new_beat_count = piano_app->piano_state.song.beat_count;

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
        piano_app->piano_state.song.set_beat_count(new_beat_count);
        for(uint i = 0; i < new_beat_count; i++)
        {
            piano_app->piano_state.song.notes[i].push_back({ NoteName::C4, NoteType::quarter}); // always a C4 on first beat
        }
        piano_app->piano_state.song.generate();

        // TODO: resolve adding and removing the UINodes from the beat count editor
        piano_app->piano_ui.uic_song.reload_beat_count();

        Print::ln("Beat count reloaded");
    }
}


void note_name_label_scroll(PIANO_UI_CALLBACK_PARAMETERS)
{
    // TODO: I ned to be able to track the index of the target UINode in order to edit the corresponding note in thac backend
    
    if(user_input.event_data.scroll_input.scroll_direction == ScrollDirection::Up)
        piano_app->piano_state.song.notes[0][0].up_half_note();
    // else
        

    piano_app->piano_ui.uic_song.reload_beat_count();
}


void stop_button_hover(UINode* _node, UserInput _user_input,  PianoApp* piano_app)
{
    _node->visibility.set_color(0xFFAA4433);
    // _node->color = 0xFFBB3333;
}
void stop_button_unhover(UINode* _node, UserInput _user_input,  PianoApp* piano_app)
{
    _node->visibility.set_color(0xFF995544);
    // _node->color = 0xFF994444;
}
void start_button_hover(UINode* _node, UserInput _user_input,  PianoApp* piano_app)
{
    _node->visibility.set_color(0xFF33BB33);
    // _node->color = 0xFF33BB33;
}
void start_button_unhover(UINode* _node, UserInput _user_input,  PianoApp* piano_app)
{
    _node->visibility.set_color(0xFF449944);
    // _node->color = 0xFF449944;
}



void play_current_piano_song(UINode* _node, UserInput _user_input,  PianoApp* piano_app)
{
    piano_app->piano_state.song.generate();
    piano_app->piano_state.song.play(piano_app->piano_state.alsa);

    // Print::ln("Unhover handler!");
}


void stop_current_piano_song(UINode* _node, UserInput _user_input,  PianoApp* piano_app)
{

    piano_app->piano_state.song.stop(piano_app->piano_state.alsa);

    // Print::ln("Unhover handler!");
}


void UIC_Song::init(UI&_ui, UINode& _parent_node, Song& _song)
{

}


void UIC_Song::init(PianoApp& piano_app)
{
    ui = &piano_app.piano_ui.ui;
    song = &piano_app.piano_state.song;

    root = ui->new_node(&ui->root);
    root->box = UIBox({400, 100}, {200, 250});
    root->visibility.set_color(0xFF444444);


    play_button =  ui->new_node(root);
    play_button->box = UIBox({470, 290}, {50, 50});
    // play_button->color = 0xFF449944;
    play_button->visibility.set_color(0xFF449944);
    play_button->handle_click = PIANO_UI_CALLBACK_CAST play_current_piano_song;
    play_button->handle_hover = PIANO_UI_CALLBACK_CAST start_button_hover;
    play_button->handle_unhover = PIANO_UI_CALLBACK_CAST start_button_unhover;

    stop_button = ui->new_node(root);
    stop_button->box = UIBox({520, 290}, {50, 50});
    stop_button->visibility.set_color(0xFF995544);
    stop_button->handle_click = PIANO_UI_CALLBACK_CAST stop_current_piano_song;
    stop_button->handle_hover = PIANO_UI_CALLBACK_CAST stop_button_hover;
    stop_button->handle_unhover = PIANO_UI_CALLBACK_CAST stop_button_unhover;



    beat_count_label = ui->new_node(root);
    beat_count_label->box.pos = {450, 220};
    // beat_count_label->box = UIBox({450, 220}, {80, 30});
    beat_count_label->set_str(Str::UI(song->beat_count));
    beat_count_label->handle_scroll = PIANO_UI_CALLBACK_CAST beat_count_label_scroll;


    beat_count_container = ui->new_node(root);
    beat_count_container->box = UIBox({405, 105}, {180, 100});
    beat_count_container->visibility.set_color(0xFF333333);

    reload_beat_count();
}


void UIC_Song::update(PianoApp& piano_app)
{
    beat_count_label->set_str(Str::UI(piano_app.piano_state.song.beat_count));
}



void UIC_Song::reload_beat_count()
{
    ui->delete_node_descendants(beat_count_container);
    beat_label_array.clear();

    for(uint i = 0; i < song->beat_count; i++)
    {
        UINode* new_node = ui->new_node(beat_count_container);

        double x_pos = (double) (405 + i*40);
        new_node->box = UIBox({x_pos, 105.0},{5, 5});
        new_node->set_str(NoteSerializer::note_name_to_str(song->notes[i][0].name));
        new_node->handle_scroll = PIANO_UI_CALLBACK_CAST note_name_label_scroll;

        beat_label_array.push_back(new_node);
    }
}
