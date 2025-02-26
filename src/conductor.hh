#ifndef CONDUCTOR_HH
#define CONDUCTOR_HH

#include "Windowing.hpp"

/** Inits all modules necessary for regular physimos start. */
int conductor_rouse();
/** Physimos main loop. */
void conductor_main();


/** Conductors callback method for window dimension/scale subscription */
void callback_window_change(PhysWin physimos_window);

#endif