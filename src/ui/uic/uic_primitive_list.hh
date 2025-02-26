#ifndef UIC_PRIMITIVE_LIST_HH
#define UIC_PRIMITIVE_LIST_HH

#include <vector>
#include <list>

#include "ui/ui_globals.hh"
#include "ui/ui_primitive.hh"

#include "uic/uic_primitive_color.hh"
#include "uic/uic_primitive_list.hh"


namespace UI::component {

// extern ::UI::component::UIC_PrimitiveColor color_editor_1;

/** Returns a */
void store(Primitive *_parent, ::UI::Primitive& _primitive);

struct UIC_PrimitiveList : public ::UI::Primitive {

    ::UI::Primitive& boundObject; // Primitive to edit

    Primitive title;

    int list_item_stride_px = 40;
    
    /** 
     *  Owns the components.
     *  In order to prevent a change in the loaction of the objects, thus breaking the internal pointer structure of the components, a linked list is used. Lists never invalidate or move the stored object unless explicitly told to do so. 
     *  Obviously this solution is slow at a large number of components, but does not pose a realistic threat here. The static nature of the list is very much important. 
     */
    std::list<::UI::component::UIC_PrimitiveColor> listObjects;
    
    // std::vector<::UI::component::UIC_PrimitiveColor> listObjects;

    // std::list<::UI::component::UIC_PrimitiveColor*> listObjectPtnrs;

    UiResult scroll(double y_change);
    
    void new_list_object();
    void del_list_object();

    UiResult try_find_target_component(double x, double y);
    void render_component();
    
    UIC_PrimitiveList(::UI::Primitive& _primitive);
};



}


#endif