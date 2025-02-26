
#ifndef UI_HPP
#define UI_HPP


#include "Windowing.hpp"
#include "Input.hpp"

#include "conductor_action.hh"

#include "ui/ui_globals.hh"

namespace UI {

    /** Structure that defines the main grid of the UI.
     *  Default values as defined in this struct will be loaded at program start.  
     */
    typedef struct Grid {
        size_t topbar_h_px = 30;
        size_t left_panel_w_pct = 20;
        size_t right_panel_w_pct = 20;
        size_t workbench_h_pct = 25;
        bool left_panel_visible = true;
        bool right_panel_visible = true;
        bool workbench_visible = true;
    } Grid;


    // PUBLIC 

    void init();
    void update();

    /** Queries all UI component and returns the matched primitive with the smallest z-value. */
    UiResult try_find_target(double x, double y);
    /** Returns match primitive from UI editors/list. */
    UiResult try_find_target_old(double x, double y);

    /** The only way to change the layout of the main UI grid. Provides all dimensions and placing logic of the grid. */
    void set_ui_grid(Grid _new_grid);

    /** Set new window dimension/scale for ui.  */
    void update_window(PhysWin physimos_window);
}



#endif