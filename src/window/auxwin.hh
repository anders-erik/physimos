
#pragma once

#include <queue>

#include "math/vecmat.hh"


#include "auxevent.hh"

// To not require the inclusion of the glfw header
struct GLFWwindow;
struct GLFWcursor;


namespace window {


struct PWCoordinatesInput{
    f2 window_size_sc;
    f2 content_scale;
    f2 monitor_size_mm; 
    f2 monitor_size_px;
};

// Coordinate Transformation Matrices -- All values are relative to window origins!
struct PWCoordinateTransformer {

    /** All available Physimos window units. */
    enum class PWUnit {
        SC,     /** (virtual) Sceen Coordinates  */
        PX,     /** Real Pixels */
        N,      /** Normalized */
        MM,     /** Millimeters */
    };
    /** All but the glfw-input system share an origin and have parallel basis axes: bottom left of window; y is up.  */
    enum class PWCoordinateSystems {
        INPUT,          /** GLFW input coordinate in content area coordinate. PWUnit : SC */
        SANE,           /** PWUnit : SC */
        PIXELS,         /** PWUnit : PX */
        NORMALIZED,     /** PWUnit : N */
        MILLIMETERS,    /** PWUnit : MM */
    };

    void set_constants(PWCoordinatesInput input){
        win_sc.x = (float) input.window_size_sc.x;
        win_sc.y = (float) input.window_size_sc.y;

        cs = input.content_scale;
        mon_px = input.monitor_size_px;
        mon_mm = input.monitor_size_mm;

        dm_dp.x = mon_mm.x / mon_px.x;
        dm_dp.y = mon_mm.y / mon_px.y;

        M_i_s.set_to_identity();
        M_i_s.y.y = -1.0f;
        M_i_s.y.z = win_sc.y;

        M_s_n.set_to_identity();
        M_s_n.x.x = 1 / win_sc.x;
        M_s_n.y.y = 1 / win_sc.y;

        M_s_p.set_to_identity();
        M_s_p.x.x = cs.x; // TODO: make sure its not 1 / cs
        M_s_p.y.y = cs.y; // TODO: make sure its not 1 / cs
        
        M_p_m.set_to_identity();
        M_p_m.x.x = dm_dp.x;
        M_p_m.y.y = dm_dp.y;
    }

    /** Input to sane */
    f2 i_s(f2 i){
        return M_i_s.mult_vec(i);
    }
     /** Sane to normalized */
    f2 s_n(f2 s){
        return M_s_n.mult_vec(s);
    }
     /** Sane to pixels */
    f2 s_p(f2 s){
        return M_s_p.mult_vec(s);
    }
    /** Sane to millimeters */
    f2 s_m(f2 s){
        return M_p_m.mult_vec(s_p(s));
    }

private:

    m3f3 M_i_s;   /** From glfw input to sane : ( (1 0 0) (0 -1 win_h) (0 0 1) ) */
    m2f2 M_s_n;   /** From sane to Normalized : ( (1/win_w 0) (0 1/win_h) ) */
    m2f2 M_s_p;   /** From sane to pixels : ( (CSx 0) (0 CSy) ) */
    m2f2 M_p_m;   /** From pixels to Millimeters : ( (dp_dm_x 0) (0 dp_dm_y) ) */

    f2 cs;      /** Content scale */
    f2 win_sc;  /** Window size, screen coordinated */
    f2 mon_px;  /** Monitor size, pixels */
    f2 mon_mm;  /** Monitor size, millimeters */
    f2 dm_dp;   /** pixels per millimeters : wm_px / wm_mm */

};



class Auxwin {

    private:
        f2 current_window_size_f;
        i2 current_window_size_i;
        Key close_key = Key::Esc; // The key that will trigger auxwin to close.

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

        std::queue<InputEvent> input_events;

        GLFWcursor* default_cursor;
        GLFWcursor* pan_cursor;

    public:
        GLFWwindow *glfw_window;
        bool open = true;


        Auxwin() {};

        void init(i2 window_size);
        void init(f2 window_size);
        void init(int width, int height);
        void init();
        
        void make_current();
        
        // new frame GL-calls & retrieves all new events
        std::vector<InputEvent> new_frame();
        // end frame + check if open
        bool                    end_frame();  

        bool is_open();
        // Will trigger the window to close on next end of frame call.
        void close();
        void destroy();

        std::queue<InputEvent> get_input_events();
        InputEvent get_input_event();
        /** Create an artificial event. */
        void add_input_event(InputEvent event);


        void framebuffer_callback(GLFWwindow* _window, int _width, int _height);
        void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
        void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
        void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
        void key_callback(GLFWwindow *window, int key, int action, int mods);
};



}