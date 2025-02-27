
#include "conductor_viewport.hh"


bool AABB_INT_2D::contains(double _x, double _y){
    if((int) _x < x || (int) _x > (x + w))
        return false;
    if((int) _y < y || (int) _y > (y + h))
        return false;

    return true;
}




void ViewportContext::set_workbench_h(int h_px){
    view_sizes.workbench.h = h_px;

    view_sizes.main_view.h = size.logical.h - view_sizes.topbar_h - view_sizes.workbench.h;
    view_sizes.main_view.y = view_sizes.workbench.h;
}
void ViewportContext::toggle_workbench(){
    visibility.workbench = visibility.workbench ? false : true;

    update_heights();
}


void ViewportContext::set_left_panel_w(int w_px){
    view_sizes.left_panel_w = w_px;

    view_sizes.main_view.w = size.logical.w - view_sizes.left_panel_w - view_sizes.right_panel_w;
    view_sizes.workbench.w = size.logical.w - view_sizes.left_panel_w - view_sizes.right_panel_w;

    view_sizes.main_view.x = view_sizes.left_panel_w;
    view_sizes.workbench.x = view_sizes.left_panel_w;
}
void ViewportContext::set_right_panel_w(int w_px){
    view_sizes.right_panel_w = w_px;
    
    view_sizes.main_view.w = size.logical.w - view_sizes.left_panel_w - view_sizes.right_panel_w;
    view_sizes.workbench.w = size.logical.w - view_sizes.left_panel_w - view_sizes.right_panel_w;

    // view_sizes.main_view.x = view_sizes.left_panel_w;
    // view_sizes.workbench.x = view_sizes.left_panel_w;

}

void ViewportContext::toggle_left_panel(){
    visibility.left_panel = visibility.left_panel ? false : true;

    update_widths();
}

void ViewportContext::toggle_right_panel(){
    visibility.right_panel = visibility.right_panel ? false : true;

    update_widths();
}

void ViewportContext::update_heights(){
    if(visibility.workbench){
        view_sizes.main_view.h = size.logical.h - view_sizes.workbench.h - view_sizes.topbar_h;
        view_sizes.main_view.y = view_sizes.workbench.h;
    }
    else{
        view_sizes.main_view.h = size.logical.h - view_sizes.topbar_h;
        view_sizes.main_view.y = 0;
    }
}

void ViewportContext::update_widths(){

    // 4 cases
    if      (visibility.left_panel && visibility.right_panel){
        view_sizes.main_view.w = size.logical.w - view_sizes.left_panel_w - view_sizes.right_panel_w;
        view_sizes.main_view.x = view_sizes.left_panel_w;

        view_sizes.workbench.w = size.logical.w - view_sizes.left_panel_w - view_sizes.right_panel_w;
        view_sizes.workbench.x = view_sizes.left_panel_w;
    }
    else if (visibility.left_panel && !visibility.right_panel){
        view_sizes.main_view.w = size.logical.w - view_sizes.left_panel_w;
        view_sizes.main_view.x = view_sizes.left_panel_w;

        view_sizes.workbench.w = size.logical.w - view_sizes.left_panel_w;
        view_sizes.workbench.x = view_sizes.left_panel_w;
    }
    else if (!visibility.left_panel && visibility.right_panel){
        view_sizes.main_view.w = size.logical.w - view_sizes.right_panel_w;
        view_sizes.main_view.x = 0;

        view_sizes.workbench.w = size.logical.w - view_sizes.right_panel_w;
        view_sizes.workbench.x = 0;
    }
    else    {
        view_sizes.main_view.w = size.logical.w;
        view_sizes.main_view.x = 0;

        view_sizes.workbench.w = size.logical.w;
        view_sizes.workbench.x = 0;
    }
}


void ViewportContext::set_cursor(double x_real, double y_real){

    cursor_real.x = x_real;
    cursor_real.y = y_real;

    if(view_sizes.main_view.contains(x_real, y_real)){
        cursor_main_view.x = x_real - view_sizes.main_view.x;
        cursor_main_view.y = y_real - view_sizes.main_view.y;
    }
    else{
        cursor_main_view.x = -2222;
        cursor_main_view.y = -2222;
    }

}