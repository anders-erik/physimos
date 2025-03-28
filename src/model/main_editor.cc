#include <iostream>

#include "window/auxwin.hh"

#include "scene.hh"

int main(){

    window::Auxwin auxwin(800, 600);

    std::cout << "Hello from editor!" << std::endl;

    xpeditor::scene_init();
    
    while(auxwin.is_open()){
        auxwin.new_frame();

        window::InputState input_state = auxwin.get_input_state();
        if(input_state.esc)
            auxwin.close();


        xpeditor::scene_handle_input(input_state);
        xpeditor::scene_update();
        xpeditor::scene_render();

        auxwin.end_frame();
    }


    return 0;
}