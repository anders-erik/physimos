#ifndef CONDUCTOR_INTERNAL_HH
#define CONDUCTOR_INTERNAL_HH

#include "Windowing.hpp"
#include "Input.hpp"

#include "conductor_common.hh"

void state_main_set(StateMain new_state_main);
void state_main_switch_right();
void state_main_switch_left();

/** Pointer position change detected in window and called by Input. Position is viewport location and pointer_change is position delta since last call. */
void callback_pointer_position(PInput::PointerPosition pointer_pos, PInput::PointerChange _pointer_change);
/** Left click detected and called by Input. Position is viewport location at the time of click.  */
void callback_left_down(PInput::PointerPosition _pointer_pos);
void callback_left_release(PInput::PointerPosition _pointer_pos);
void callback_scroll_y(double y_change);
void callback_key_down(PInput::KeyEvent key_event);
void callback_key_up(PInput::KeyEvent key_event);

/** Conductors callback method for window dimension/scale subscription */
void callback_window_change(PhysWin new_window);

#endif