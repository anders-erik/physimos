
#pragma once

#include <queue>
#include <functional>

#include "lib/time.hh"
#include "math/vecmat.hh"

#include "opengl/opengl.hh"

#include "coordinate_transform.hh"

#include "auxevent.hh"
#include "window/cursor.hh"

#if PHY_NATWIN == 1

#else
#include "window/glfw.hh"
#endif


using AuxwinProcAdrFn = void* (*)(const char* procname);


namespace window
{


class Auxwin 
{
    i2 window_fb_size;

    opengl::OpenGL gl;

    /** Check keystroke close conditions. If match, close flag is set. */
    void try_close_keystroke(KeyStrokeEvent keystroke, KeyModifiers modifiers);


    /** Required input for coordinate transformation constants.  */
    PWCoordinatesInput coords_input;
    /** Provides coordinate tranformations of 2D points.  */
    PWCoordinateTransformer coord_transformer;

        /**    Queries glfw for coordinate system values, then reloads the coord object. 
            Depends on current_window_size_f.
        */
    void reload_coordinate_constants();

    void glfw_window_hints();
    void glfw_create_window(i2 window_size_i);

    CursorPosition cursor_pos;

    KeyModifiers modifiers_current;

    std::vector<InputEvent> events_input;
    std::vector<WindowResizeEvent> events_resize;

    Cursor cursor;

public:
    // AuxwinCallbacks callbacks;

    Timer timer;
    static const uchar dt_count = 10;
    float dt_s_last_10[dt_count] {0.016f};
    void print_fps()
    {
        float avg = 0.0f;
        for(int i=0; i<dt_count; i++)
            avg += dt_s_last_10[i];
        avg /= (float) dt_count;

        float avg_fps = 1.0f / avg;
        
        // print("avg: ");
        // Print::ln(Str::FL(avg, 4, Str::FloatRep::Fixed));
        print("FPS: ");
        Print::ln(Str::FL(avg_fps, 4, Str::FloatRep::Fixed));
    }

    GLFWwindow *glfw_window;
    bool open = true;


    Auxwin() = default;

    void init(int width, int height);
    
    AuxwinProcAdrFn get_auxwin_proc_adr_fn();

    /** Activate the associated glfw window */
    void make_current();
    
    void bind_window_framebuffer();
    
    /** Clear main framebuffer. */
    void new_frame();
    /** Swap buffers, poll events, and check if should close. */
    bool end_frame();  

    bool is_open();
    // Will trigger the window to close on next end of frame call.
    void close();
    void destroy();

    void set_cursor_state(Cursor::State new_state);
    CursorPosition get_cursor_pos() const;
    f2 get_center_pos();

    i2 get_window_fb_size() const;
    f2 get_window_fb_size_float() const;

    f2 get_monitor_size_px();
    f2 get_monitor_size_mm();
    f2 get_monitor_content_scale();

    /** Create an artificial event. */
    void add_input_event(InputEvent event);
    std::vector<InputEvent> get_events_input();
    std::vector<WindowResizeEvent> get_events_window_resize();

    void framebuffer_callback(int _width, int _height);
    void mouse_button_callback(int button, int action, int mods);
    void cursor_position_callback(double xpos, double ypos);
    void scroll_callback(double xoffset, double yoffset);
    void key_callback(int key, int action, int mods);


private:

#if PHY_NATWIN == 1

#else
    PWGLFW pwglfw;
#endif

};


}



static Auxwin* current_auxwin = nullptr;
Auxwin* get_current_auxwin();

