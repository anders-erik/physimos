#ifndef UIC_TRANSFORM_HH
#define UIC_TRANSFORM_HH

#include "transform.hh"

#include "ui/ui_primitive.hh"


namespace UI::component::transform {

class TransformContext;

// Primitive with a WorldObject pointer. 
struct Base : public ::UI::Primitive {
    virtual void reload() {};

    Base() {};
};


struct Container : public UI::component::transform::Base {
    Container(TransformContext* _context);
};


struct XPosition : public UI::component::transform::Base {
    XPosition(TransformContext* _context);
    void reload();
};
struct XPosIncrease : public UI::component::transform::Base {
    XPosIncrease(TransformContext* _context);
    UI::Action click();
};
struct XPosDecrease : public UI::component::transform::Base {
    XPosDecrease(TransformContext* _context);
    UI::Action click();
};




// Full context ui component for Transforms (formerly WorldObject)
class TransformContext {
public:
    // 
    ::Transform* boundObject = nullptr;

    Container* container = nullptr;
    XPosition* xPosition = nullptr;
    XPosIncrease* xPosIncrease = nullptr;
    XPosDecrease* xPosDecrease = nullptr;

    
    void populateContext(::Transform* _transform);
    void newTransform(::Transform* _transform);
    void reloadComponent();

    TransformContext() {};
};



}


#endif