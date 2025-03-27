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



#endif