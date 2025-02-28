#ifndef CONDUCTOR_INTERNAL_HH
#define CONDUCTOR_INTERNAL_HH

#include "Windowing.hpp"
#include "Input.hpp"

#include "conductor_common.hh"

/** Sets the state variable and reloads dependencies */
void state_main_set(StateMain new_state_main);
/** Returns the state located right of the current state in the topbar menu. Wraps on rightmost state. */
StateMain get_right_state();
/** Returns the state located left of the current state in the topbar menu.  Wraps on leftmost state. */
StateMain get_left_state();

/** Conductor method that will run after known changes, e.g. if viewport_change flag is set in the viewport_context. */
void reload_viewport();

/** Pointer position change detected in window and called by Input. Position is viewport location and pointer_change is position delta since last call. */
void callback_pointer_position(ViewportCursor pointer_pos, ViewportCursor _pointer_change);
/** Left click detected and called by Input. Position is viewport location at the time of click.  */
void callback_left_down(ViewportCursor _pointer_pos);
/** Left mouse button release detected and called by Input. Position is viewport location at the time of click.  */
void callback_left_release(ViewportCursor _pointer_pos);
/**  */
void callback_scroll_y(double y_change);
void callback_key_down(PInput::KeyEvent key_event);
void callback_key_up(PInput::KeyEvent key_event);

/** Conductors callback method for window dimension/scale subscription */
void callback_window_change(PhysWin new_window);

#endif