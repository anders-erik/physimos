#ifndef UIC_DRAW_TRANSFORM_HH
#define UIC_DRAW_TRANSFORM_HH


#include "ui/ui_primitive.hh"
#include "ui/ui_globals.hh"
#include "ui/uic/uic_label.hh"

#include "draw/draw.hh"


namespace UI::component {


struct UIC_Draw_Transform : public ::UI::Primitive {

    draw::TransformContext& boundObject;
    draw::TransformContext transform_context_cache;

    Primitive title;

    // Primitive zoom;
    UIC_Label_float zoom;
    Primitive pan_x;
    Primitive pan_y;
    Primitive texture_x;
    Primitive texture_y;
    Primitive texture_px_x;
    Primitive texture_px_y;
    Primitive main_view_x;
    Primitive main_view_y;

    void update_component();
    void render_component();

    UIC_Draw_Transform();
};


}


#endif