#include "uic_piano.hh"

#include "piano_app.hh"
#include "ui/ui4/ui_event.hh"

UIEventReturn C4_event_callback(PIANO_UI_EVENT_CALLBACK_PARAMETERS)
{
    switch (ui_event.type)
    {
    case UIEvent::mouse_move:
        {
            
            if(ui_event.target_is_grabbed)
            {
                Print::ln("C4 mouse movement with grab active");
                d2 move_delta = {ui_event.value.mouse_move.dx, ui_event.value.mouse_move.dy};
                node->box.pos.x += move_delta.x;
                node->box.pos.y += move_delta.y;
            }
            else
            {
                Print::ln("C4 mouse move");
            }
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
    case UIEvent::mouse_press:
        {
            Print::ln("C4 mouse press");
            piano_app->phyano.press(NoteName::C4);
            node->visibility.set_color(0xFFAAAAAA);
            return UIEventReturn {UIEventReturn::node_keyboard_grab};
        }
        break;
    case UIEvent::mouse_release:
        {
            Print::ln("C4 mouse release");
            piano_app->phyano.release(NoteName::C4);
            node->visibility.set_color(0xFFCCCCCC);
            return UIEventReturn {UIEventReturn::node_mouse_release};
        }
        break;
    case UIEvent::mouse_scroll:
        {
            Print::ln("C4 mouse scroll");
        }
        break;
    case UIEvent::key_press:
        {
            Print::ln("C4 key press");
        }
        break;
    
    default:
        Print::ln("C4: Unknown UIEvent type");
        break;
    }
    
    return UIEventReturn{};
}



UIEventReturn D4_event_callback(PIANO_UI_EVENT_CALLBACK_PARAMETERS)
{
    switch (ui_event.type)
    {
    case UIEvent::mouse_press:
        {
            piano_app->phyano.press(NoteName::D4);
            node->visibility.set_color(0xFFAAAAAA);
        }
        break;
    case UIEvent::mouse_release:
        {
            node->visibility.set_color(0xFFCCCCCC);
        }
        break;
    
    default:
        break;
    }
    
    return {};
}
UIEventReturn E4_event_callback(PIANO_UI_EVENT_CALLBACK_PARAMETERS)
{
    switch (ui_event.type)
    {
    case UIEvent::mouse_press:
        {
            piano_app->phyano.press(NoteName::E4);
            node->visibility.set_color(0xFFAAAAAA);
        }
        break;
    case UIEvent::mouse_release:
        {
            node->visibility.set_color(0xFFCCCCCC);
        }
        break;
    
    default:
        break;
    }
    
    return {};
}

UIEventReturn F4_event_callback(PIANO_UI_EVENT_CALLBACK_PARAMETERS)
{
    switch (ui_event.type)
    {
    case UIEvent::mouse_press:
        {
            piano_app->phyano.press(NoteName::F4);
            node->visibility.set_color(0xFFAAAAAA);
        }
        break;
    case UIEvent::mouse_release:
        {
            node->visibility.set_color(0xFFCCCCCC);
        }
        break;
    
    default:
        break;
    }
    
    return {};
}


UIEventReturn G4_event_callback(PIANO_UI_EVENT_CALLBACK_PARAMETERS)
{
    switch (ui_event.type)
    {
    case UIEvent::mouse_press:
        {
            piano_app->phyano.press(NoteName::G4);
            node->visibility.set_color(0xFFAAAAAA);
        }
        break;
    case UIEvent::mouse_release:
        {
            node->visibility.set_color(0xFFCCCCCC);
        }
        break;
    
    default:
        break;
    }
    
    return {};
}


UIEventReturn A4_event_callback(PIANO_UI_EVENT_CALLBACK_PARAMETERS)
{
    switch (ui_event.type)
    {
    case UIEvent::mouse_press:
        {
            piano_app->phyano.press(NoteName::A4);
            node->visibility.set_color(0xFFAAAAAA);
        }
        break;
    case UIEvent::mouse_release:
        {
            node->visibility.set_color(0xFFCCCCCC);
        }
        break;
    
    default:
        break;
    }
    
    return {};
}


UIEventReturn B4_event_callback(PIANO_UI_EVENT_CALLBACK_PARAMETERS)
{
    switch (ui_event.type)
    {
    case UIEvent::mouse_press:
        {
            piano_app->phyano.press(NoteName::B4);
            node->visibility.set_color(0xFFAAAAAA);
        }
        break;
    case UIEvent::mouse_release:
        {
            node->visibility.set_color(0xFFCCCCCC);
        }
        break;
    
    default:
        break;
    }
    
    return {};
}

UIEventReturn C5_event_callback(PIANO_UI_EVENT_CALLBACK_PARAMETERS)
{
    switch (ui_event.type)
    {
    case UIEvent::mouse_press:
        {
            piano_app->phyano.press(NoteName::C5);
            node->visibility.set_color(0xFFAAAAAA);
        }
        break;
    case UIEvent::mouse_release:
        {
            node->visibility.set_color(0xFFCCCCCC);
        }
        break;
    
    default:
        break;
    }
    
    return {};
}




UIEventReturn uic_piano_root_callback(UI_CALLBACK_PARAMS(PianoApp))
{
    switch (ui_event.type)
    {
    case UIEvent::key_press:
        {
            NoteName note_name;

            switch (ui_event.value.key_press.key)
            {
                case Keys::A:   note_name = NoteName::C4;   break;
                case Keys::S:   note_name = NoteName::D4;   break;
                case Keys::D:   note_name = NoteName::E4;   break;
                case Keys::F:   note_name = NoteName::F4;   break;
                case Keys::G:   note_name = NoteName::G4;   break;
                case Keys::H:   note_name = NoteName::A4;   break;
                case Keys::J:   note_name = NoteName::B4;   break;
                case Keys::K:   note_name = NoteName::C5;   break;
                
                default:        return {};                     break;
            }

            data->phyano.press(note_name);
        }
        break;
    
    case UIEvent::mouse_press:
        {
            data->piano_ui.ui.current_keyboard_target = node;
            node->visibility.set_color(0xFF556677);
        }
        break;
    
    default:
        break;
    }

    return {};
}



void UIC_piano::init(UI& ui)
{
    d2 root_pos = {50, 250};
    d2 root_size = {400, 200};
    UIBox root_box = {root_pos, root_size};

    root = ui.new_node(&ui.root);
    root->box = root_box;
    root->visibility.set_color(0xFF444444);
    root->event_callback = UI_CALLBACK_CAST uic_piano_root_callback;


    PX32 white_key_color = 0xFFCCCCCC;
    d2 white_key_size = {30, 80};
    // double white_key_x_pos = 60.0;
    double white_key_y_pos = root_pos.y + 10.0;


    C4_node =  ui.new_node(root);
    C4_node->box = UIBox({60.0, white_key_y_pos}, white_key_size);
    C4_node->visibility.set_color(white_key_color);
    C4_node->event_callback = PIANO_UI_EVENT_CALLBACK_CAST C4_event_callback;

    D4_node =  ui.new_node(root);
    D4_node->box = UIBox({95.0, white_key_y_pos}, white_key_size);
    D4_node->visibility.set_color(white_key_color);
    D4_node->event_callback = PIANO_UI_EVENT_CALLBACK_CAST D4_event_callback;

    E4_node =  ui.new_node(root);
    E4_node->box = UIBox({130.0, white_key_y_pos}, white_key_size);
    E4_node->visibility.set_color(white_key_color);
    E4_node->event_callback = PIANO_UI_EVENT_CALLBACK_CAST E4_event_callback;

    F4_node =  ui.new_node(root);
    F4_node->box = UIBox({165.0, white_key_y_pos}, white_key_size);
    F4_node->visibility.set_color(white_key_color);
    F4_node->event_callback = PIANO_UI_EVENT_CALLBACK_CAST F4_event_callback;

    G4_node =  ui.new_node(root);
    G4_node->box = UIBox({200.0, white_key_y_pos}, white_key_size);
    G4_node->visibility.set_color(white_key_color);
    G4_node->event_callback = PIANO_UI_EVENT_CALLBACK_CAST G4_event_callback;

    A4_node =  ui.new_node(root);
    A4_node->box = UIBox({235.0, white_key_y_pos}, white_key_size);
    A4_node->visibility.set_color(white_key_color);
    A4_node->event_callback = PIANO_UI_EVENT_CALLBACK_CAST A4_event_callback;

    B4_node =  ui.new_node(root);
    B4_node->box = UIBox({270.0, white_key_y_pos}, white_key_size);
    B4_node->visibility.set_color(white_key_color);
    B4_node->event_callback = PIANO_UI_EVENT_CALLBACK_CAST B4_event_callback;

    C5_node =  ui.new_node(root);
    C5_node->box = UIBox({305.0, white_key_y_pos}, white_key_size);
    C5_node->visibility.set_color(white_key_color);
    C5_node->event_callback = PIANO_UI_EVENT_CALLBACK_CAST C5_event_callback;
}