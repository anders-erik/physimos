#ifndef UIC_DRAW_TRANSFORM_HH
#define UIC_DRAW_TRANSFORM_HH


#include "ui/ui_primitive.hh"

#include "ui/ui_globals.hh"

#include "draw/draw.hh"


namespace UI::component {


struct UIC_Draw_Transform_MainViewPos : public ::UI::Primitive {

    draw::TransformContext transform_context_cache;

    Primitive main_view_x;
    Primitive main_view_y;

    void update_component();
    void render_component();

    UIC_Draw_Transform_MainViewPos();
};


struct UIC_Draw_Transform: public ::UI::Primitive {

    draw::TransformContext& boundObject;
    

    Primitive title;

    UIC_Draw_Transform_MainViewPos   main_view_pos;

    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
    UIC_Draw_Transform();
};



}


#endif