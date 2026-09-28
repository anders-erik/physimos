#include "uic_beat_count_editor.hh"
#include "io/input/user_input.hh"
#include "piano_app.hh"

void beat_count_label_scroll(PIANO_UI_CALLBACK_PARAMETERS)
{
    // Print::ln("Handling scroll");
    if(user_input.event_type == UserInputType::ScrollInput)
    {
        if(user_input.event_data.scroll_input.scroll_direction == ScrollDirection::Up)
            piano_app->piano_state.song.beat_count++;
        else
            piano_app->piano_state.song.beat_count--;
    }
}



void UIC_BeatCountEditor::init(PianoApp& piano_app)
{
    root = piano_app.piano_ui.ui.new_node(&piano_app.piano_ui.ui.root);
    root->box = UIBox({500, 150}, {80, 30});
    root->visibility.set_color(0xFF444444);


    beat_count_label = piano_app.piano_ui.ui.new_node(root);
    // beat_count_label->box.pos = {100, 100};
    beat_count_label->box = UIBox({500, 150}, {80, 30});
    beat_count_label->set_str(Str::UI(piano_app.piano_state.song.beat_count));
    // beat_count_label->set_str("asdf");
    beat_count_label->handle_scroll = PIANO_UI_CALLBACK_CAST beat_count_label_scroll;
}


void UIC_BeatCountEditor::update(PianoApp& piano_app)
{
    beat_count_label->set_str(Str::UI(piano_app.piano_state.song.beat_count));
}


