#include "uic_piano.hh"

#include "piano_app.hh"
#include "ui/ui4/ui_event.hh"

void press_C4_2(PIANO_UI_CALLBACK_PARAMETERS)
{
    piano_app->phyano.press(NoteName::C4);
    node->visibility.set_color(0xFFAAAAAA);
}
void release_C4_2(PIANO_UI_CALLBACK_PARAMETERS)
{
    piano_app->phyano.release(NoteName::C4);
    node->visibility.set_color(0xFFCCCCCC);
}
void C4_event_callback(PIANO_UI_EVENT_CALLBACK_PARAMETERS)
{
    switch (ui_event.type)
    {
    case UIEvent::mouse_move:
        {
            Print::ln("C4 mouse move");
        }
        break;
    case UIEvent::mouse_enter:
        {
            Print::ln("C4 enter");
        }
        break;
    case UIEvent::mouse_exit:
        {
            Print::ln("C4 exit");
        }
        break;
    
    default:
        Print::ln("C4: Unknown UIEvent type");
        break;
    }
    
}
void press_D4_2(PIANO_UI_CALLBACK_PARAMETERS)
{
    piano_app->phyano.press(NoteName::D4);
}
void press_E4_2(PIANO_UI_CALLBACK_PARAMETERS)
{
    piano_app->phyano.press(NoteName::E4);
}
void press_F4_2(PIANO_UI_CALLBACK_PARAMETERS)
{
    piano_app->phyano.press(NoteName::F4);
}
void press_G4_2(PIANO_UI_CALLBACK_PARAMETERS)
{
    piano_app->phyano.press(NoteName::G4);
}
void press_A4_2(PIANO_UI_CALLBACK_PARAMETERS)
{
    piano_app->phyano.press(NoteName::A4);
}
void press_B4_2(PIANO_UI_CALLBACK_PARAMETERS)
{
    piano_app->phyano.press(NoteName::B4);
}
void press_C5_2(PIANO_UI_CALLBACK_PARAMETERS)
{
    piano_app->phyano.press(NoteName::C5);
}



void piano_root_click(PIANO_UI_CALLBACK_PARAMETERS)
{
    piano_app->piano_ui.ui.current_keyboard_target = node;
    node->visibility.set_color(0xFF556677);
}

// void piano_root_click(PIANO_UI_CALLBACK_PARAMETERS)
// {
//     piano_app->piano_ui.ui.current_keyboard_target = node;
//     node->visibility.set_color(0xFF556677);
// }


void root_keypress_callback(PIANO_UI_CALLBACK_PARAMETERS)
{
    if(user_input.is_key_input() && user_input.event_data.key_input.action == KeyButtonAction::Press)
    {
        NoteName note_name; 

        switch (user_input.event_data.key_input.key)
        {
            case Keys::A:   note_name = NoteName::C4;   break;
            case Keys::S:   note_name = NoteName::D4;   break;
            case Keys::D:   note_name = NoteName::E4;   break;
            case Keys::F:   note_name = NoteName::F4;   break;
            case Keys::G:   note_name = NoteName::G4;   break;
            case Keys::H:   note_name = NoteName::A4;   break;
            case Keys::J:   note_name = NoteName::B4;   break;
            case Keys::K:   note_name = NoteName::C5;   break;
            
            default:        return;                     break;
        }

        piano_app->phyano.press(note_name);
    }
}


void UIC_piano::init(UI& ui)
{
    d2 root_pos = {50, 250};
    d2 root_size = {400, 200};
    UIBox root_box = {root_pos, root_size};

    root = ui.new_node(&ui.root);
    root->box = root_box;
    root->visibility.set_color(0xFF444444);
    root->handle_mouse_click = PIANO_UI_CALLBACK_CAST piano_root_click;
    root->handle_key_press = PIANO_UI_CALLBACK_CAST root_keypress_callback;

    PX32 white_key_color = 0xFFCCCCCC;
    d2 white_key_size = {30, 80};
    // double white_key_x_pos = 60.0;
    double white_key_y_pos = root_pos.y + 10.0;

    C4_node =  ui.new_node(root);
    C4_node->box = UIBox({60.0, white_key_y_pos}, white_key_size);
    C4_node->visibility.set_color(white_key_color);
    C4_node->handle_mouse_click = PIANO_UI_CALLBACK_CAST press_C4_2;
    C4_node->handle_mouse_release = PIANO_UI_CALLBACK_CAST release_C4_2;
    C4_node->event_callback = PIANO_UI_EVENT_CALLBACK_CAST C4_event_callback;

    D4_node =  ui.new_node(root);
    D4_node->box = UIBox({95.0, white_key_y_pos}, white_key_size);
    D4_node->visibility.set_color(white_key_color);
    D4_node->handle_mouse_click = PIANO_UI_CALLBACK_CAST press_D4_2;

    E4_node =  ui.new_node(root);
    E4_node->box = UIBox({130.0, white_key_y_pos}, white_key_size);
    E4_node->visibility.set_color(white_key_color);
    E4_node->handle_mouse_click = PIANO_UI_CALLBACK_CAST press_E4_2;

    F4_node =  ui.new_node(root);
    F4_node->box = UIBox({165.0, white_key_y_pos}, white_key_size);
    F4_node->visibility.set_color(white_key_color);
    F4_node->handle_mouse_click = PIANO_UI_CALLBACK_CAST press_F4_2;

    G4_node =  ui.new_node(root);
    G4_node->box = UIBox({200.0, white_key_y_pos}, white_key_size);
    G4_node->visibility.set_color(white_key_color);
    G4_node->handle_mouse_click = PIANO_UI_CALLBACK_CAST press_G4_2;

    A4_node =  ui.new_node(root);
    A4_node->box = UIBox({235.0, white_key_y_pos}, white_key_size);
    A4_node->visibility.set_color(white_key_color);
    A4_node->handle_mouse_click = PIANO_UI_CALLBACK_CAST press_A4_2;

    B4_node =  ui.new_node(root);
    B4_node->box = UIBox({270.0, white_key_y_pos}, white_key_size);
    B4_node->visibility.set_color(white_key_color);
    B4_node->handle_mouse_click = PIANO_UI_CALLBACK_CAST press_B4_2;

    C5_node =  ui.new_node(root);
    C5_node->box = UIBox({305.0, white_key_y_pos}, white_key_size);
    C5_node->visibility.set_color(white_key_color);
    C5_node->handle_mouse_click = PIANO_UI_CALLBACK_CAST press_C5_2;
}