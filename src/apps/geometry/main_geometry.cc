#include <cmath>

#include "lib/print.hh"
#include "lib/clock.hh"

#include "rend/sw/swrend.hh"
#include "opengl/loader.hh"

#include "window/wayland1/wayland.hh"
#include "window/wayland1/wl-defs.hh"

Stopwatch stopwatch;

void callback_done(  void *data,
		    struct wl_callback *wl_callback,
		    uint32_t callback_data);


const struct wl_callback_listener callback_listener = {
    .done = callback_done,
};


void callback_done(  void *data,
		    struct wl_callback *wl_callback,
		    uint32_t callback_data)
{
    stopwatch.stop_and_print_ms();
    Print::ln("Callback done!");

    Wayland* wayland = (Wayland*) data;

    wl_callback_destroy(wl_callback);
    // wl_callback = wl_surface_frame(wayland->surfaces.opengl.wl_surface);
    wl_callback_add_listener(wl_callback, &callback_listener, wayland);

    stopwatch.start();
}



struct OpenGLTriangle
{
    GLuint program;
    GLuint vao = 0;
    GLuint vbo = 0;

    float verts[9] = {
        0.0f,  1.0f, 0.0f, // top center
        -1.0f, -1.0f, 0.0f, // bottom left
        1.0f, -1.0f, 0.0f  // bottom right
    };

};

static OpenGLTriangle opengl_triangle;

namespace OpenGL {

GLuint compile_shader(GLenum type, const char* src)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    return shader;
}



void print_opengl_error()
{
    GLenum error_enum = glGetError();
    if(error_enum == GL_NO_ERROR)
        return;

    Print::buf("OPENGL ERROR: ");
    Str error_str;

    switch (error_enum)
    {
    case GL_INVALID_ENUM: error_str = "GL_INVALID_ENUM"; break;
    case GL_INVALID_VALUE: error_str = "GL_INVALID_VALUE"; break;
    case GL_INVALID_OPERATION: error_str = "GL_INVALID_OPERATION"; break;
    case GL_INVALID_FRAMEBUFFER_OPERATION: error_str = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
    case GL_OUT_OF_MEMORY: error_str = "GL_OUT_OF_MEMORY"; break;
    case GL_STACK_UNDERFLOW: error_str = "GL_STACK_UNDERFLOW"; break;
    case GL_STACK_OVERFLOW: error_str = "GL_STACK_OVERFLOW"; break;
    
    default:
        error_str = "UNKNOWN";
        break;
    }

    Print::ln(error_str);
}

}


static float color_t = 0.0f;
static float color = 0.0f;

void draw()
{

    glUseProgram(opengl_triangle.program);

    // color_t += 0.01f;
    color_t += 0.03f;
    color = sin(color_t);

    // rand clear color 
    // glClearColor((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 1.0f);
    // glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClearColor(color, color, color, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    OpenGL::print_opengl_error();



}



void init_opengl_triangle()
{
    OpenGL::print_opengl_error();

    glViewport(0, 0, 800, 600);

    glGenVertexArrays(1, &opengl_triangle.vao);
    glBindVertexArray(opengl_triangle.vao);
    glEnableVertexAttribArray(0);
    

    glDeleteBuffers(1, &opengl_triangle.vbo);

    if(!glIsBuffer(opengl_triangle.vbo))
        Print::ln("1. vbo not yet a valid buffer");
    glGenBuffers(1, &opengl_triangle.vbo);
    if(!glIsBuffer(opengl_triangle.vbo))
        Print::ln("2. vbo still not a valid buffer");
    glBindBuffer(GL_ARRAY_BUFFER, opengl_triangle.vbo);
    if(glIsBuffer(opengl_triangle.vbo))
        Print::ln("3. vbo is a valid buffer");
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if(glIsBuffer(opengl_triangle.vbo))
        Print::ln("4. vbo is still a valid buffer");
    OpenGL::print_opengl_error();
    glBindBuffer(GL_ARRAY_BUFFER, opengl_triangle.vbo);

    float new_last_vert[3] = {
        1.0f, -0.5f, 0.0f  // new bottom right
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(opengl_triangle.verts), opengl_triangle.verts, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float), sizeof(new_last_vert), new_last_vert);

    

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);


    // create_program
    const char* vert_src =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "    gl_Position = vec4(aPos, 1.0);\n"
        "}";
    const char* frag_src =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "    FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}";

    GLuint vs = OpenGL::compile_shader(GL_VERTEX_SHADER, vert_src);
    GLuint fs = OpenGL::compile_shader(GL_FRAGMENT_SHADER, frag_src);
    opengl_triangle.program = glCreateProgram();
    glAttachShader(opengl_triangle.program, vs);
    glAttachShader(opengl_triangle.program, fs);
    glLinkProgram(opengl_triangle.program);
    glDeleteShader(vs);
    glDeleteShader(fs);
}


int main()
{
    Print::ln("Hello, from Geometry! \n");

    Clock clock;
    PerformanceClock performance_clock;
    uint64_t prev_time = clock.get_unix_epoch_ms();
    

    Wayland wayland;
    // wayland.init({  .width = 800, .height = 600, .rendering_type = WaylandRenderingType::Buffer});
    wayland.init({  .width = 800, 
                    .height = 600, 
                    .rendering_type = WaylandRenderingType::OpenGL, 
                    .window_title = "Main Geometry"                 });


    if(wayland.surfaces.render_type == WaylandRenderingType::OpenGL)
    {
        struct wl_callback *cb = wl_surface_frame(wayland.surfaces.opengl.wl_surface);
        wl_callback_add_listener(cb, &callback_listener, &wayland);
    }

    SWR::Renderer swrenderer;
    // if(wayland.surfaces.render_type == WaylandRenderingType::Buffer)
    // {
    //     swrenderer.set_buffer( wayland.get_content_buffer() );
    //     swrenderer.clear(0xFF555555);
    // }

    init_opengl_triangle();
    draw();
    // wayland.surfaces.opengl.egl.swap_buffers();
    // wayland.surfaces.opengl.egl.swap_buffers();
    

    
    // wayland.surfaces.decorations.surface.commit();
    // wayland.surfaces.decorations.surface.commit();
    // wl_surface_commit(wayland.surfaces.decorations.surface.surface);

    while(wayland.frame_step())
    {
        if(wayland.set_size())
        {
            // glViewport(0, 0, 800, 600);
            // draw();
            // wayland.surfaces.opengl.egl.swap_buffers();

            if(wayland.surfaces.render_type == WaylandRenderingType::OpenGL)
            {
                // draw();
                // wayland.surfaces.commit();
                // wayland.surfaces.opengl.egl.draw();
                // draw();
                // wayland.surfaces.opengl.egl.swap_buffers();
            }
        }
        Arr<UserInput> events = wayland.get_new_input_events();

        for(uint i = 0; i < events.count(); i++)
        {
            events.clear();
        }

        if(wayland.surfaces.render_type == WaylandRenderingType::SWR)
        {
            swrenderer.set_buffer( wayland.get_content_buffer() );
            swrenderer.clear(0xFF654321);
        }
        else if(wayland.surfaces.render_type == WaylandRenderingType::OpenGL)
        {
            // wayland.surfaces.opengl.egl.draw();
            draw();
            wayland.surfaces.opengl.egl.swap_buffers();
        }

        usleep(10000);

        // prev_time = clock.get_unix_epoch_ms();
        // clock.print_current_m_sec();

        // performance_clock.fps_step(true);

        // Print::ln("");
    }

    return 0;
}