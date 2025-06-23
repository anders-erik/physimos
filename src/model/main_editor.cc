#include <iostream>

#include "window/auxwin.hh"
#include "scene/scene.hh"

int main(){

    window::Auxwin auxwin;
    auxwin.init(800, 600);

    std::cout << "Hello from editor!" << std::endl;


    SceneModelEditor scene;
    
    while(auxwin.end_frame()){
        // auxwin.new_frame();
        auxwin.new_frame();

        auto resizes = auxwin.get_events_window_resize();
        // for(auto& resize : resizes)
            // scene.handle_window_resize(resize);

        std::vector<window::InputEvent> events = auxwin.get_events_input();
        for(window::InputEvent& event : events){
            // window::InputEvent event = events.back();
            
            // if(event.type == window::EventType::Keystroke && event.key_stroke.key == window::Key::Esc)
            //     auxwin.close();

            scene.handle_input(event);
            
            // events.pop_back();
        }

        scene.update();
        scene.render();
        // auxwin.end_frame();
    }


    return 0;
}