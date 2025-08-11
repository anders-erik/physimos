#pragma once

#include <GLFW/glfw3.h>

#include "lib/print.hh"

#include "math/vecmat.hh"

// #include "window/auxwin.hh"
#include "window/phywin_types.hh"
#include "window/cursor.hh"


struct PWGLFWCursorImages
{
    GLFWcursor* arrow       = nullptr;  // Default
    GLFWcursor* cross_hair  = nullptr;  // 
    GLFWcursor* hand        = nullptr;  // 
    GLFWcursor* none        = nullptr;  // Capturing
};



struct PWGLFW
{
    GLFWwindow* window = nullptr;

    PWGLFWCursorImages cursor_images;

    PWDims dims;

    void init(i2 content_size)
    {

        glfwInit();
        window_hints();


        create_window({content_size.x, content_size.y});
        create_cursors();
    }

    void set_cursor_state(Cursor::State new_state)
    {
        switch (new_state)
        {
        case Cursor::NORMAL:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursor(window, cursor_images.arrow);
            break;

        case Cursor::INVERTED:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursor(window, cursor_images.cross_hair);
            break;

        case Cursor::HAND:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursor(window, cursor_images.hand);
            break;

        case Cursor::CAPTURE:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursor(window, cursor_images.none);
            // center_cursor_pos();
            // center_real_cursor();
            // events_input.clear();
            break;
        }
    }

    void update_dims_with_fb(i2 new_fb_size_i)
    {
        dims.fb_size_i = new_fb_size_i;
        dims.content_scale = get_content_scale();
        dims.window_size_f = dims.fb_size_i.to_f2() / dims.content_scale;
        dims.window_size_i = dims.window_size_f.to_i2();
    }

    // Retrieves content scale from GLFW, stores it in content_scale, and returns it.
    f2 get_content_scale()
    {
        GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
        glfwGetMonitorContentScale( primary_monitor, 
                                    &dims.content_scale.x,
                                    &dims.content_scale.y   );
        return dims.content_scale;
    }

    i2 get_monitor_size_px()
    {
        GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primary_monitor);
        return i2{ mode->width, mode->height };
    }

    i2 get_monitor_size_mm()
    {
        GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();

        int width_mm, height_mm;
        glfwGetMonitorPhysicalSize(primary_monitor, &width_mm, &height_mm);

        return i2{ (float)width_mm, (float)height_mm };
    }

    void make_current()
    {
        glfwMakeContextCurrent(window);
    }

    void set_window_viewport()
    {
        glViewport( 0,
                    0, 
                    dims.fb_size_i.x, 
                    dims.fb_size_i.y );
    }


    void close()
    {
        glfwSetWindowShouldClose(window, true);
    }
    bool is_open()
    {
        return !glfwWindowShouldClose(window);
    }

    void end_frame()
    {
        glfwSwapBuffers(window);
	    glfwPollEvents();
    }

    void destroy()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

private:

    void window_hints()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_SAMPLES, 8);

        // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    }

    void create_window(i2 window_size_i)
    {
        window = glfwCreateWindow(  window_size_i.x, 
                                    window_size_i.y, 
                                    "Auxwin", 
                                    NULL, 
                                    NULL                );

        dims.window_size_i = window_size_i;
        dims.window_size_f = window_size_i.to_f2();
        dims.content_scale = get_content_scale();
        dims.fb_size_i = (dims.window_size_f * dims.content_scale).to_i2();

        // GLFWwindow *window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            Print::line("Failed to create GLFW window");
            glfwTerminate();
            exit(1);
        }



        glfwSetFramebufferSizeCallback( window, 
                                        glfw_framebuffer_callback      );

        glfwSetMouseButtonCallback(     window, 
                                        glfw_mouse_button_callback     );

        glfwSetCursorPosCallback(       window, 
                                        glfw_cursor_position_callback  );

        glfwSetScrollCallback(          window, 
                                        glfw_scroll_callback           );

        glfwSetKeyCallback(             window, 
                                        glfw_key_callback              );


        // TODO: THIS SHOULD NOT BE FORCED WHEN CREATING A NEW WINDOW!
        glfwMakeContextCurrent(window);
    }

    void create_cursors()
    {
        cursor_images.arrow        = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        cursor_images.cross_hair   = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
        cursor_images.hand         = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
        cursor_images.none         = glfwCreateStandardCursor(GLFW_CURSOR_HIDDEN);
    }

    /*
    GLFW C-Style callbacks as indirection layer
    */
    static void glfw_framebuffer_callback(GLFWwindow* _window, int _width, int _height);
    static void glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    static void glfw_cursor_position_callback(GLFWwindow *window, double xpos, double ypos);

    static void glfw_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
    static void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

};