#ifndef CONDUCTOR_VIEWPORT_HH
#define CONDUCTOR_VIEWPORT_HH

#include "Types.hpp"
#include "Windowing.hpp"

#include "conductor_common.hh"

#define VIEWPORT_WIDTH_INIT             1200
#define VIEWPORT_HEIGHT_INIT            800

#define topbar_default_visibilty        true
#define left_panel_default_visibilty    true
#define workbench_default_visibilty     true
#define right_panel_default_visibilty   true

#define topbar_height                   30
#define left_panel_default_width        300
#define workbench_default_height        100
#define right_panel_default_width       300



typedef enum class ViewportViews {
    Topbar,
    MainView,
    LeftPanel,
    RightPanel,
    Workbench,
} ViewportViews;


typedef struct ViewportSize {
    Size_UINT_2D raw;
    Size_UINT_2D logical;
    float xscale;
    float yscale;
} ViewportSize;

typedef struct ViewportCursor {
    double x;
    double y;
} ViewportCursor;


typedef struct ViewportViewVisibility {
    bool topbar         = topbar_default_visibilty;
    bool left_panel     = left_panel_default_visibilty;
    bool right_panel    = right_panel_default_visibilty;
    bool workbench      = workbench_default_visibilty;
} ViewportViewVisibility;

typedef struct ViewportViewSizes {
    AABB_INT_2D main_view;
    AABB_INT_2D workbench;
    int topbar_h        = topbar_height;
    int left_panel_w;
    int right_panel_w;
} ViewportViewSizes;

typedef struct ViewportViewResizeAccumulator {
    int left_panel  = 0;
    int right_panel = 0;
    int workbench   = 0;
} ViewportViewResizeAccumulator;


typedef struct ViewportContext {
    ViewportContext() {};

    ViewportSize        size;

    ViewportViewSizes               view_sizes;
    ViewportViewVisibility          visibility;
    ViewportViewResizeAccumulator   accumulator;

    /** Location of the cursor in the viewport, measure in logical pixels from the bottom left. */
    ViewportCursor              cursor_real;
    /** Location of the cursor in the main_view, measured in logical pixels from the bottom left of main_view. */
    ViewportCursor              cursor_main_view;


    void set_workbench_h(int h_px);
    void toggle_workbench();

    void set_left_panel_w(int w_px);
    void set_right_panel_w(int w_px);
    void toggle_left_panel();
    void toggle_right_panel();

    void update_heights();
    void update_widths();


    void set_cursor(double x_real, double y_real);
} ViewportContext;



#endif