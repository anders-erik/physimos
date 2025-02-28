
#ifndef UI_HPP
#define UI_HPP


#include "Windowing.hpp"
#include "Input.hpp"

#include "conductor_common.hh"
#include "conductor_viewport.hh"

#include "ui/ui_globals.hh"

namespace UI {


    // PUBLIC 

    void init();
    void update();


    void state_main_set(StateMain new_state_main);

    /** Queries all UI component and returns the matched primitive with the smallest z-value. */
    UiResult try_find_target(double x, double y);

    /** The only way to change the layout of the main UI grid. Provides all dimensions and placing logic of the grid. */
    void set_ui_views(ViewportViewSizes view_sizes, ViewportViewVisibility visibility);
    /** NOTE: LEGACY 
     * The only way to change the layout of the main UI grid. Provides all dimensions and placing logic of the grid. */
    // void set_ui_grid(Grid _new_grid);

    /** Set new window dimension/scale for ui.  */
    void update_window(PhysWin physimos_window);
}



#endif