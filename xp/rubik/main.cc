
#include "window.hh"
#include "gpu.hh"
#include "fs.hh"
#include "scene.hh"
#include "rubik.hh"

using namespace xprubik;

Cube _cube;

int main(){

    bool fs_ok = fs_init();
    if(!fs_ok) exit(1);

    bool window_ok = window_init();
    if(!window_ok) return 1;

    bool gpu_ok = gpu_init();
    if(!gpu_ok) return 1;

    bool scene_ok = scene_init(_cube);
    if(!scene_ok) return 1;

    
    // gpu_use_program(Shader::Model);

    
    

    while (!window_should_close())
    {

        window_new_frame();

        InputState input_state = window_get_input();
        if(input_state.esc)
            window_set_close();

        // gpu_draw_triangle_moodel_shader();


        _cube.handle_input(input_state);
        _cube.update_cubies();


        scene_handle_input(input_state);
        scene_update(_cube);
        scene_render(_cube);

        window_end_of_frame();
    }
    


}

