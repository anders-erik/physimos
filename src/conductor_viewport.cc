
#include "conductor_viewport.hh"


bool AABB_2D::contains(double _x, double _y){
    if((int) _x < x || (int) _x > (x + w))
        return false;
    if((int) _y < y || (int) _y > (y + h))
        return false;

    return true;
}

void ViewportContext::set_cursor(double x_real, double y_real){

    cursor_real.x = x_real;
    cursor_real.y = y_real;

    if(aabbs.main_view.contains(x_real, y_real)){
        cursor_main_view.x = x_real - aabbs.main_view.x;
        cursor_main_view.y = y_real - aabbs.main_view.y;
    }
    else{
        cursor_main_view.x = -2222;
        cursor_main_view.y = -2222;
    }

}