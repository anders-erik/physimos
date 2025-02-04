#ifndef UI_WORLDOBJECT_HH
#define UI_WORLDOBJECT_HH

#include "WorldObject.hpp"

#include "ui/ui_primitive.hh"


namespace UI::WorldObject {

    Primitive* newComponent(::WorldObject* _worldObject);



    struct Container : public UI::Primitive {
        Container(::WorldObject* _worldObject);
        ::WorldObject* worldObject = nullptr;
    };


    struct NameLabel : public UI::Primitive {
        NameLabel(::WorldObject* _worldObject);
        ::WorldObject* worldObject = nullptr;
    };


    struct XPosition : public UI::Primitive {
        XPosition(::WorldObject* _worldObject);
        ::WorldObject* worldObject = nullptr;
    };
    struct XPosIncrease : public UI::Primitive {
        XPosIncrease(::WorldObject* _worldObject);
        ::WorldObject* worldObject = nullptr;
        void click();
    };
    struct XPosDecrease : public UI::Primitive {
        XPosDecrease(::WorldObject* _worldObject);
        ::WorldObject* worldObject = nullptr;
        void click();
    };


    struct YPosition : public UI::Primitive {
        YPosition(::WorldObject* _worldObject);
        ::WorldObject* worldObject = nullptr;
    };
    struct YPosIncrease : public UI::Primitive {
        YPosIncrease(::WorldObject* _worldObject);
        ::WorldObject* worldObject = nullptr;
        void click();
    };
    struct YPosDecrease : public UI::Primitive {
        YPosDecrease(::WorldObject* _worldObject);
        ::WorldObject* worldObject = nullptr;
        void click();
    };


    struct ZPosition : public UI::Primitive {
        ZPosition(::WorldObject* _worldObject);
        ::WorldObject* worldObject = nullptr;
    };
    struct ZPosIncrease : public UI::Primitive {
        ZPosIncrease(::WorldObject* _worldObject);
        ::WorldObject* worldObject = nullptr;
        void click();
    };
    struct ZPosDecrease : public UI::Primitive {
        ZPosDecrease(::WorldObject* _worldObject);
        ::WorldObject* worldObject = nullptr;
        void click();
    };



    // Setting to public to be able to use primitive base pointer without cast
    class ToggleWireframe : public UI::Primitive {
    public:
        ToggleWireframe(::WorldObject* _worldObject);
        ::WorldObject* worldObject = nullptr;
        void click();
    };

}


#endif