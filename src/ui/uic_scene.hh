#ifndef UI_PSCENE_HH
#define UI_PSCENE_HH

#include "scene/pscene.hh"
#include "scene/pobject.hh"

#include "ui/ui_primitive.hh"


namespace UI::component {

    class PsceneComponent;
    

    struct PsceneBase : public UI::Primitive {
        PsceneBase(){};
        
        virtual void reload() {};
    };


    struct PsceneContainer : public UI::component::PsceneBase {
        PsceneContainer(PsceneComponent* psceneContext);
    };


    struct PsceneNameLabel : public UI::component::PsceneBase {
        // PsceneNameLabel(std::string sceneName);
        PsceneNameLabel(PsceneComponent* psceneContext);
    };


    struct PscenePObjectListObject : public UI::component::PsceneBase {
        // UI action currently depends on this primitive having a bound pObject!
        // TODO : Will eventually be moved into a ui action struct!
        ::PObject* pObject = nullptr;

        PscenePObjectListObject(::PObject* _pObject);
        
        // will reload the pobject container with pobject currently pointed to by this primitive
        ::UI::Action click() {
            return ::UI::Action::LoadPObject;
        };
    };

    struct PscenePObjectList : public UI::component::PsceneBase {        
        PscenePObjectList(PsceneComponent* psceneContext);

        void reloadList();
    };



    // Full context ui component for Pscene
    class PsceneComponent {
    public:
        ::PScene::Scene* boundObject = nullptr;

        PsceneContainer* container = nullptr;
        PsceneNameLabel* nameLabel = nullptr;
        PscenePObjectList* pObjectList = nullptr;

        PsceneComponent() {};
        
        void populateContext(::PScene::Scene* _scene);

        // void reloadComponent();

    };

    extern PsceneComponent* psceneContext;
}


#endif