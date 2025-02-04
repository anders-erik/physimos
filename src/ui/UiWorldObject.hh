#ifndef UI_WORLDOBJECT_HH
#define UI_WORLDOBJECT_HH

#include "WorldObject.hpp"

#include "ui/ui_primitive.hh"


namespace UI::WorldObject {

    Primitive* newComponent(::WorldObject* _worldObject);

    // Primitive with a WorldObject pointer. 
    struct Base : public UI::Primitive {
        ::WorldObject* worldObject = nullptr;

        Base(){};
    };


    struct Container : public UI::WorldObject::Base {
        Container(::WorldObject* _worldObject);
    };


    struct NameLabel : public UI::WorldObject::Base {
        NameLabel(::WorldObject* _worldObject);
    };


    struct XPosition : public UI::WorldObject::Base {
        XPosition(::WorldObject* _worldObject);
    };
    struct XPosIncrease : public UI::WorldObject::Base {
        XPosIncrease(::WorldObject* _worldObject);
        UI::Action click();
    };
    struct XPosDecrease : public UI::WorldObject::Base {
        XPosDecrease(::WorldObject* _worldObject);
        UI::Action click();
    };


    struct YPosition : public UI::WorldObject::Base {
        YPosition(::WorldObject* _worldObject);
    };
    struct YPosIncrease : public UI::WorldObject::Base {
        YPosIncrease(::WorldObject* _worldObject);
        UI::Action click();
    };
    struct YPosDecrease : public UI::WorldObject::Base {
        YPosDecrease(::WorldObject* _worldObject);
        UI::Action click();
    };


    struct ZPosition : public UI::WorldObject::Base {
        ZPosition(::WorldObject* _worldObject);
    };
    struct ZPosIncrease : public UI::WorldObject::Base {
        ZPosIncrease(::WorldObject* _worldObject);
        UI::Action click();
    };
    struct ZPosDecrease : public UI::WorldObject::Base {
        ZPosDecrease(::WorldObject* _worldObject);
        UI::Action click();
    };



    // Setting to public to be able to use primitive base pointer without cast
    class ToggleWireframe : public UI::WorldObject::Base {
    public:
        ToggleWireframe(::WorldObject* _worldObject);
        UI::Action click();
    };

}


#endif