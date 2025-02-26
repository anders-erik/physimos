#ifndef CONDUCTOR_HH
#define CONDUCTOR_HH

#include "Windowing.hpp"

#include "conductor_action.hh"

typedef enum class StateMain {
    Scene3D,
    Canvas,
    UIEditor,
} StateMain;


/** Inits all modules necessary for regular physimos start. */
int conductor_rouse();
/** Physimos main loop. */
void conductor_main();

/** Physimos actions. */
void conductor_perform_action(CAction action);


/** Conductors callback method for window dimension/scale subscription */
void callback_window_change(PhysWin physimos_window);

#endif