#ifndef CONDUCTOR_VIEWPORT_HH
#define CONDUCTOR_VIEWPORT_HH

#include "Types.hpp"
#include "Windowing.hpp"

#include "conductor_common.hh"



typedef enum class ViewportViews {
    Topbar,
    MainView,
    LeftPanel,
    RightPanel,
    Workbench,
} ViewportViews;

typedef struct Size_2D {
    uint w;
    uint h;
} Size2D;

typedef struct AABB_2D {
    int x;
    int y;
    int w;
    int h;

    bool contains(double x, double y);
} AABB_2D;

typedef struct ViewportSize {
    Size2D raw;
    Size2D logical;
    Size2D scale;
} ViewportSize;

typedef struct ViewportCursor {
    double x;
    double y;
} ViewportCursor;


typedef struct ViewportViewVisibility {
    bool topbar = true;
    bool main_view = true;
    bool left_panel = true;
    bool right_panel = true;
    bool workbench = true;
} ViewportViewVisibility;

typedef struct ViewportViewAABB {
    AABB_2D topbar;
    AABB_2D main_view;
    AABB_2D left_panel;
    AABB_2D right_panel;
    AABB_2D workbench;
} ViewportViewAABB;

typedef struct ViewportViewSizeAccumulator {
    int topbar      = 0;
    int main_view   = 0;
    int left_panel  = 0;
    int right_panel = 0;
    int workbench   = 0;
} ViewportViewSizeAccumulator;


typedef struct ViewportContext {
    ViewportViewAABB            aabbs;
    ViewportSize                size;
    ViewportViewVisibility      visibility;
    ViewportViewSizeAccumulator accumulator;
    /** Location of the cursor in the viewport, measure in logical pixels from the bottom left. */
    ViewportCursor              cursor_real;
    /** Location of the cursor in the main_view, measured in logical pixels from the bottom left of main_view. */
    ViewportCursor              cursor_main_view;

    void set_cursor(double x_real, double y_real);
} ViewportContext;



#endif