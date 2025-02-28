#ifndef CONDUCTOR_VIEWPORT_HH
#define CONDUCTOR_VIEWPORT_HH

#include "Types.hpp"
#include "Windowing.hpp"
#include "window_cursors.hh"

#include "conductor_common.hh"

#define VIEWPORT_WIDTH_INIT             1200
#define VIEWPORT_HEIGHT_INIT            800

#define topbar_default_visibilty        true
#define left_panel_default_visibilty    true
#define workbench_default_visibilty     true
#define right_panel_default_visibilty   true

#define topbar_height                   30
#define left_panel_default_width        300
#define workbench_default_height        300
#define right_panel_default_width       300


/** The 5 available view regions of the viewport. */
typedef enum class ViewportViews {
    Topbar,
    MainView,
    LeftPanel,
    RightPanel,
    Workbench,
} ViewportViews;

/** The window size of the primary window.
 *  Raw value is in physical pixel unit.
 *  Logical size is the physical pixel count divided by scale.
 */
typedef struct ViewportSize {
    Size_UINT_2D raw;
    Size_UINT_2D logical;
    float xscale;
    float yscale;
} ViewportSize;

/** Visilibility of the 4 viewport view whole visilibility is toggleable. (Main view is always visible?) */
typedef struct ViewportViewVisibility {
    bool topbar         = topbar_default_visibilty;
    bool left_panel     = left_panel_default_visibilty;
    bool right_panel    = right_panel_default_visibilty;
    bool workbench      = workbench_default_visibilty;
} ViewportViewVisibility;

/** Stores the minimal information to perfectly specify the ui layout of the viewport views. 
 *  (workbench does track a superflous y-value, but is placed in a AABB for convencience. )
 */
typedef struct ViewportViewSizes {
    AABB_INT_2D main_view;
    AABB_INT_2D workbench;
    int topbar_h        = topbar_height;
    int left_panel_w;
    int right_panel_w;
} ViewportViewSizes;

/** To prevent view resizing on every cursor movement we accumulate the cursor movement and only trigger resize when the change exceeds a specificed threshold value. */
typedef struct ViewportViewResizeAccumulator {
    double left_panel  = 0;
    double right_panel = 0;
    double workbench   = 0;
} ViewportViewResizeAccumulator;


/**
 *  The current state of the viewport.  
 *  Speicifed the current window/viewport size info, viewport views placement, and cursor locations.
 *  Any change to a viewport view dimension or location are set and calculated here, then the resulting information is passed to the depending physimos modules. 
 */
typedef struct ViewportContext {
    ViewportContext() {};

    /** Flag to trigger reload of dependants on viewport sizes. */
    bool viewport_changed = true;

    /** The window size of the primary window.
     *  Raw value is in physical pixel unit.
     *  Logical size is the physical pixel count divided by scale.
     */
    PhysWin        size;

    /** Stores the minimal information to perfectly specify the ui layout of the viewport views. 
     *  (workbench does track a superflous y-value, but is placed in a AABB for convencience. )
     */
    ViewportViewSizes               view_sizes;
    /** Visilibility of the 4 viewport view whole visilibility is toggleable. (Main view is always visible?) */
    ViewportViewVisibility          visibility;
    /** To prevent view resizing on every cursor movement we accumulate the cursor movement and only trigger resize when the change exceeds a specificed threshold value. */
    ViewportViewResizeAccumulator   accumulator;

    /** Location of the cursor in the viewport, measure in logical pixels from the bottom left. */
    ViewportCursor              cursor_real;
    /** Location of the cursor in the main_view, measured in logical pixels from the bottom left of main_view. */
    ViewportCursor              cursor_main_view;


    /** Sets workbench height and calculates main view. Sets viewport change flag. */
    void set_workbench_h(int h_px);
    /** Sets left panel width and updates depending views accordingly. Sets viewport change flag. */
    void set_left_panel_w(int w_px);
    /** Sets right panel width and updates depending views accordingly. Sets viewport change flag. */
    void set_right_panel_w(int w_px);

    /** Toggles visibility, reload heights, and sets viewport change flag. */
    void toggle_workbench();
    /** Toggles visibility, reload widths, and sets viewport change flag. */
    void toggle_left_panel();
    /** Toggles visibility, reload widths, and sets viewport change flag. */
    void toggle_right_panel();

    /** Calculates the height */
    void update_heights();
    void update_widths();

    /** Absolute value at which resizing is triggered */
    double accumulate_threshold = 5.0;
    /** Add a dy to the workbench accumulator and compare against accumulate_threshold. Updates real size and set viewport change flag if threshold met.  */
    void accumulate_workbench(double dy);
    /** Add a dy to the left panel accumulator and compare against accumulate_threshold. Updates real size and set viewport change flag if threshold met.  */
    void accumulate_left_panel(double dx);
    /** Add a dy to the right panel accumulator and compare against accumulate_threshold. Updates real size and set viewport change flag if threshold met.  */
    void accumulate_right_panel(double dx);


    void set_cursor(double x_real, double y_real);
} ViewportContext;



#endif