#ifndef CONDUCTOR_HH
#define CONDUCTOR_HH

#include "Windowing.hpp"

#include "conductor_common.hh"


/** Inits all modules necessary for regular physimos start. */
int conductor_rouse();
/** Physimos main loop. */
void conductor_main();

/** Physimos actions. */
void conductor_perform_action(CAction action);


#endif