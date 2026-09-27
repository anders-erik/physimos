#include "uic_piano.hh"

#include "piano_app.hh"

void press_C4_2(PIANO_UI_CALLBACK_PARAMETERS)
{
    piano_app->phyano.press(NoteName::C4);
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




void UIC_piano::init(UI& ui)
{
    root = ui.new_node(&ui.root);
    root->box = UIBox({50, 150}, {300, 100});
    root->visibility.set_color(0xFF444444);

    PX32 white_key_color = 0xFFCCCCCC;
    d2 white_key_size = {30, 80};
    // double white_key_x_pos = 60.0;
    double white_key_y_pos = 160.0;

    C4_node =  ui.new_node(root);
    C4_node->box = UIBox({60.0, white_key_y_pos}, white_key_size);
    C4_node->visibility.set_color(white_key_color);
    C4_node->handle_click = PIANO_UI_CALLBACK_CAST press_C4_2;

    D4_node =  ui.new_node(root);
    D4_node->box = UIBox({95.0, white_key_y_pos}, white_key_size);
    D4_node->visibility.set_color(white_key_color);
    D4_node->handle_click = PIANO_UI_CALLBACK_CAST press_D4_2;

    E4_node =  ui.new_node(root);
    E4_node->box = UIBox({130.0, white_key_y_pos}, white_key_size);
    E4_node->visibility.set_color(white_key_color);
    E4_node->handle_click = PIANO_UI_CALLBACK_CAST press_E4_2;

    F4_node =  ui.new_node(root);
    F4_node->box = UIBox({165.0, white_key_y_pos}, white_key_size);
    F4_node->visibility.set_color(white_key_color);
    F4_node->handle_click = PIANO_UI_CALLBACK_CAST press_F4_2;

    G4_node =  ui.new_node(root);
    G4_node->box = UIBox({200.0, white_key_y_pos}, white_key_size);
    G4_node->visibility.set_color(white_key_color);
    G4_node->handle_click = PIANO_UI_CALLBACK_CAST press_G4_2;

    A4_node =  ui.new_node(root);
    A4_node->box = UIBox({235.0, white_key_y_pos}, white_key_size);
    A4_node->visibility.set_color(white_key_color);
    A4_node->handle_click = PIANO_UI_CALLBACK_CAST press_A4_2;

    B4_node =  ui.new_node(root);
    B4_node->box = UIBox({270.0, white_key_y_pos}, white_key_size);
    B4_node->visibility.set_color(white_key_color);
    B4_node->handle_click = PIANO_UI_CALLBACK_CAST press_B4_2;

    C5_node =  ui.new_node(root);
    C5_node->box = UIBox({305.0, white_key_y_pos}, white_key_size);
    C5_node->visibility.set_color(white_key_color);
    C5_node->handle_click = PIANO_UI_CALLBACK_CAST press_C5_2;
}