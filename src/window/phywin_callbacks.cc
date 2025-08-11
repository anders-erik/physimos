#include "window/phywin_callbacks.hh"


AuxwinCallbacks callbacks;

AuxwinCallbacks get_auxwin_callbacks()
{
    return callbacks;
}
void set_auxwin_callbacks(AuxwinCallbacks new_callbacks)
{
    callbacks = new_callbacks;
}