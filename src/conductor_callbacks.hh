#ifndef CONDUCTOR_CALLBACKS_HH
#define CONDUCTOR_CALLBACKS_HH

#include "Windowing.hpp"
#include "Input.hpp"

#include "conductor_common.hh"


/** Pointer position change detected in window and called by Input. Position is viewport location and pointer_change is position delta since last call. */
void callback_pointer_position(ViewportCursor pointer_pos, ViewportCursor _pointer_change);
/** Left click detected and called by Input. Position is viewport location at the time of click.  */
void callback_left_down(ViewportCursor _pointer_pos);
/** Left mouse button release detected and called by Input. Position is viewport location at the time of click.  */
void callback_left_release(ViewportCursor _pointer_pos);

/** Middle mouse click detected and called by Input. Position is viewport location at the time of click.  */
void callback_middle_down(ViewportCursor _pointer_pos);
/** Middle mouse release detected and called by Input. Position is viewport location at the time of click.  */
void callback_middle_release(ViewportCursor _pointer_pos);

/** Middle mouse click detected and called by Input. Position is viewport location at the time of click.  */
void callback_mouse_backward(ViewportCursor _pointer_pos);
/** Middle mouse release detected and called by Input. Position is viewport location at the time of click.  */
void callback_mouse_forward(ViewportCursor _pointer_pos);


/**  */
void callback_scroll_y(double y_change);
void callback_key_down(PInput::KeyEvent key_event);
void callback_key_up(PInput::KeyEvent key_event);

/** Conductors callback method for window dimension/scale subscription */
void callback_window_change(PhysWin new_window);

#endif