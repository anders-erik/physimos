
#ifndef UI_HPP
#define UI_HPP


#include "Windowing.hpp"
#include "Input.hpp"



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
        bool workbench_visible = false;
    } Grid;


    // PUBLIC 

    void init();
    void update();

    /** The only way to change the layout of the main UI grid. Provides all dimensions and placing logic of the grid. */
    void set_ui_grid(Grid _new_grid);

    // INTERNAL CALLBACK METHODS

    /** Pointer position change detected in window and called by Input. Position is viewport location and pointer_change is position delta since last call. */
    void callback_pointer_position(PInput::PointerPosition pointer_pos, PInput::PointerChange _pointer_change);
    /** Left click detected and called by Input. Position is viewport location at the time of click.  */
    void callback_left_down(PInput::PointerPosition _pointer_pos);
    void callback_left_release(PInput::PointerPosition _pointer_pos);
    void callback_scroll_y(double y_change);

    /** Set new window dimension/scale for ui.  */
    void update_window(PhysWin physimos_window);
}



#endif