
#pragma once

#include <queue>

#include "math/vecmat.hh"


#include "auxevent.hh"
#include "coordinate_transform.hh"

// To not require the inclusion of the glfw header
struct GLFWwindow;
struct GLFWcursor;


namespace window {





class Auxwin {

    private:
        f2 current_window_size_f;
        i2 current_window_size_i;

        
        // Internal window closing logic -- two quick escape-presses for now
        Key close_key = Key::Esc; // The key that will trigger auxwin to close twice in quick succession
        double time_of_last_close_key = -1.0; // Time at which the most recent close keystrok was registered. GlfwGetTime()
        double dt_to_close = 0.5; // delta time to which compare successive close-button strokes
        /** Check close condition. If met then the 'close()' method is invoked. */
        void try_close();


        /** Required input for coordinate transformation constants.  */
        PWCoordinatesInput coords_input;
        /** Provides coordinate tranformations of 2D points.  */
        PWCoordinateTransformer coord_transformer;

         /**    Queries glfw for coordinate system values, then reloads the coord object. 
                Depends on current_window_size_f.
         */
        void reload_coordinate_constants_using_glfw();

        void glfw_window_hints();
        void glfw_create_window();
        void opengl_init();

        CursorPosition cursor;

        KeyModifierState modifier_current_state;
        KeyModifiers modifiers_current;

        std::vector<InputEvent> events_input;
        std::vector<InputEvent> events_resize;


        GLFWcursor* default_cursor;
        GLFWcursor* pan_cursor;

    public:
        GLFWwindow *glfw_window;
        bool open = true;


        Auxwin() {};
        Auxwin(f2 new_window_size);

        void init(i2 window_size);
        void init(f2 window_size);
        void init(int width, int height);
        void init();
        

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


        f2 get_monitor_size_px();
        f2 get_monitor_size_mm();
        f2 get_monitor_content_scale();

        /** Create an artificial event. */
        void add_input_event(InputEvent event);
        std::vector<InputEvent> get_events_input();
        std::vector<InputEvent> get_events_window_resize();

        void framebuffer_callback(GLFWwindow* _window, int _width, int _height);
        void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
        void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
        void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
        void key_callback(GLFWwindow *window, int key, int action, int mods);
};



}